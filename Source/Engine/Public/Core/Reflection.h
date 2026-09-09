#pragma once
#include "Array.h"
#include "Name.h"
#include "Serializer.h"

struct FType;
struct FProperty;
struct FTypeBuilder;

#define _DECLARE_TYPE(Type, Kind)                                         \
  template<>                                                              \
  struct TStaticType<Type> : std::true_type {                             \
    static FType* GetType() {                                             \
      static FType myType = FTypeOf::MakeType<Type>(#Type, Kind);         \
      static bool bReflected = FTypeReflectHelper::Reflect<Type>(myType); \
      return &myType;                                                     \
    }                                                                     \
  };

#define DECLARE_STRUCT(Type) \
  struct Type;               \
  _DECLARE_TYPE(Type, ETypeKind::Struct)

#define DECLARE_COMPONENT(Type) \
  struct Type;                  \
  _DECLARE_TYPE(Type, ETypeKind::Component)

#define DECLARE_PRIMITIVE(Type) _DECLARE_TYPE(Type, ETypeKind::Primitive)
#define REFLECT(Name)           static void Reflect(FTypeBuilder& Name)

template<typename T>
struct TStaticType : std::false_type {};

template<typename, typename = void>
struct THasReflect : std::false_type {};
template<typename T>
struct THasReflect<T, std::void_t<decltype(T::Reflect(std::declval<FTypeBuilder&>()))>> : std::true_type {};

enum class ETypeKind { Primitive, Struct, Component };

struct FProperty {
  FName name{};
  FType* type{nullptr};

  void* (*GetPtr)(void* Obj){nullptr};
  void (*Get)(void* Obj, void* Value){nullptr};
  void (*Set)(void* Obj, const void* Value){nullptr};

  bool bSerialize{false};
  bool bReadOnly{false};
};

struct FType {
  FName name{};
  uint64 size{0};
  uint64 align{0};
  ETypeKind kind{ETypeKind::Struct};
  uint64 id{0};
  TArray<FProperty> props;
  void (*OnReflect)(FTypeBuilder&);
  void (*OnSerialize)(FArchive&, void*);
  bool bReflected{false};

  FProperty* FindProperty(FName Name) {
    for(auto& prop : props) {
      if(prop.name == Name) {
        return &prop;
      }
    }
    return nullptr;
  }
};

struct FTypeOf {
  template<typename T>
  static FType MakeType(cstring TypeName, ETypeKind Kind) {
    FType type = {};
    type.name = FName(TypeName);
    type.size = sizeof(T);
    type.align = alignof(T);
    type.kind = Kind;
    if constexpr(TSerializer<T>::Supported) {
      type.OnSerialize = [](FArchive& Ar, void* Obj) {  //
        TSerializer<T>::Serialize(Ar, *static_cast<T*>(Obj));
      };
    }
    if constexpr(THasReflect<T>::value) {
      type.OnReflect = &T::Reflect;
    }
    return type;
  }

  static void RegisterComponents(const FName& ModuleName, TArray<FType*>& List) {
    //AllModules.Add(FModuleEntry{ModuleName, &List});
  }

  static void UnregisterComponents(const FName& ModuleName) {
    for(uint32 c = 0; c < AllModules.Count(); c++) {
      auto& entry = AllModules.Get(c);
      if(entry.name == ModuleName) {
        AllModules.RemoveSwap(c);
        return;
      }
    }
  }

  static FType* FindByName(const FName& Name) {
    for(auto& entry : AllModules) {
      for(auto& type : *entry.components) {
        if(type->name == Name) {
          return type;
        }
      }
    }
    return nullptr;
  }

  struct FModuleEntry {
    FName name{};
    TArray<FType*>* components{nullptr};
  };
  static inline TArray<FModuleEntry> AllModules{};
};

template<auto TMember>
struct TFieldAccessor;

template<typename TStruct, typename T, T TStruct::* Member>
struct TFieldAccessor<Member> {
  using StructType = TStruct;
  using FieldType = T;

  static void* GetPtr(void* Obj) {
    TStruct* Object = static_cast<TStruct*>(Obj);
    return &(Object->*Member);
  }

  static void Get(void* Obj, void* Value) {
    TStruct* object = static_cast<TStruct*>(Obj);
    T* outValue = static_cast<T*>(Value);
    *outValue = object->*Member;
  }

  static void Set(void* Obj, const void* Value) {
    TStruct* object = static_cast<TStruct*>(Obj);
    const T* inValue = static_cast<const T*>(Value);
    object->*Member = *inValue;
  }
};

struct FTypeBuilder {
  template<auto TMember>
  FTypeBuilder& Property(cstring Name) {
    using TAccessor = TFieldAccessor<TMember>;
    currentProp = owner.props.Count();
    FProperty prop{};
    prop.name = Name;
    prop.GetPtr = &TAccessor::GetPtr;
    prop.Get = &TAccessor::Get;
    prop.Set = &TAccessor::Set;
    prop.type = TStaticType<typename TAccessor::FieldType>::GetType();
    owner.props.Add(prop);
    return *this;
  }

  FTypeBuilder& Serialize() {
    GetCurrentProperty().bSerialize = true;
    return *this;
  }

  FTypeBuilder& ReadOnly() {
    GetCurrentProperty().bReadOnly = true;
    return *this;
  }

private:
  friend struct FTypeReflectHelper;
  FTypeBuilder(FType& Owner) : owner(Owner) {}
  FProperty& GetCurrentProperty() {
    return owner.props.Get(currentProp);
  }
  FType& owner;
  int32 currentProp{-1};
};

struct FTypeReflectHelper {
  template<typename T>
  static bool Reflect(FType& Type) {
    if(!Type.bReflected) {
      Type.bReflected = true;
      if constexpr(THasReflect<T>::value) {
        FTypeBuilder builder{Type};
        T::Reflect(builder);
        return true;
      }
    }
    return false;
  }
};

#define X_LIST(X) \
  X(bool)         \
  X(int8)         \
  X(int16)        \
  X(int32)        \
  X(int64)        \
  X(uint8)        \
  X(uint16)       \
  X(uint32)       \
  X(uint64)       \
  X(cstring)      \
  X(float)        \
  X(double)

X_LIST(DECLARE_PRIMITIVE)
#undef X_LIST

DECLARE_STRUCT(FName);

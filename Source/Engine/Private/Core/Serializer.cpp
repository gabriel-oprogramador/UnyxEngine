#include "Core/Serializer.h"
#include "Core/Name.h"
#include "Math/Math.h"

#define TYPE_SERIALIZE_IMPL(Type) void TSerializer<Type>::Serialize(FArchive& Ar, Type& Value)

TYPE_SERIALIZE_IMPL(bool) {
  Ar.WriteFormat("%s", Value ? "true" : "false");
}

TYPE_SERIALIZE_IMPL(int8) {
  Ar.WriteFormat("%hhd", Value);
}

TYPE_SERIALIZE_IMPL(int16) {
  Ar.WriteFormat("%hd", Value);
}

TYPE_SERIALIZE_IMPL(int32) {
  Ar.WriteFormat("%d", Value);
}

TYPE_SERIALIZE_IMPL(int64) {
  Ar.WriteFormat("%lld", Value);
}

TYPE_SERIALIZE_IMPL(uint8) {
  Ar.WriteFormat("%hhu", Value);
}

TYPE_SERIALIZE_IMPL(uint16) {
  Ar.WriteFormat("%hu", Value);
}

TYPE_SERIALIZE_IMPL(uint32) {
  Ar.WriteFormat("%u", Value);
}

TYPE_SERIALIZE_IMPL(uint64) {
  Ar.WriteFormat("%llu", Value);
}

TYPE_SERIALIZE_IMPL(cstring) {
  Ar.WriteFormat("%s", Value);
}

TYPE_SERIALIZE_IMPL(float) {
  Ar.WriteFormat("%f", Value);
}

TYPE_SERIALIZE_IMPL(double) {
  Ar.WriteFormat("%lf", Value);
}

TYPE_SERIALIZE_IMPL(FName) {
  Ar.WriteFormat("FName{%s}", Value.ToStr());
}

//TYPE_SERIALIZE_IMPL(FVector3) {
//Ar.WriteFormat("FVector3{%f, %f, %f}", Value.x, Value.y, Value.z);
//}

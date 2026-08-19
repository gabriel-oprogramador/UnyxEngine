#include "Core/Unicode.h"
#include "Mojibake/mojibake.h"
#include <cstring>

static bool InternalNormalize(cstring Str, uint64 Length, mjb_normalization Normalization, FUnicode::FResult& Out) {
  if(!Str) {
    return false;
  }
  mjb_result result{};
  if(mjb_normalize(Str, Length, MJB_ENC_UTF_8, Normalization, MJB_ENC_UTF_8, &result) != MJB_STATUS_OK) {
    return false;
  }
  Out.data = result.output;
  Out.size = result.output_size;
  Out.transformed = result.transformed;
  return true;
}

static bool InternalIsNormalized(cstring Str, uint64 Length, mjb_normalization Normalization) {
  if(!Str) {
    return false;
  }
  mjb_quick_check_result Result{};
  if(mjb_normalization_quick_check(Str, Length, MJB_ENC_UTF_8, Normalization, &Result) != MJB_STATUS_OK) {
    return false;
  }
  return Result == MJB_QC_YES;
}

static bool InternalCase(cstring Str, uint64 Length, mjb_map_case_type Case, FUnicode::FResult& Out) {
  mjb_result result{};
  mjb_status status = mjb_map_case(Str, Length, MJB_ENC_UTF_8, Case, MJB_ENC_UTF_8, &result);
  if(status != MJB_STATUS_OK) {
    return false;
  }
  Out.data = result.output;
  Out.size = result.output_size;
  Out.transformed = result.transformed;
  return true;
}

bool FUnicode::IsASCII(cstring Str, uint64 Length) {
  return Str && mjb_is_ascii(Str, Length);
}

bool FUnicode::IsUTF8(cstring Str, uint64 Length) {
  return Str && mjb_is_utf8(Str, Length);
}

uint64 FUnicode::ByteLength(cstring Str) {
  if(!Str) {
    return 0;
  }
  return std::strlen(Str);
}

uint64 FUnicode::CodePointCount(cstring Str, uint64 Length) {
  if(!Str) {
    return 0;
  }
  return static_cast<uint64>(mjb_count_codepoints(Str, Length, MJB_ENC_UTF_8));
}

bool FUnicode::IsNormalizedNFC(cstring Str, uint64 Length) {
  return InternalIsNormalized(Str, Length, MJB_NORMALIZATION_NFC);
}

bool FUnicode::IsNormalizedNFD(cstring Str, uint64 Length) {
  return InternalIsNormalized(Str, Length, MJB_NORMALIZATION_NFD);
}

bool FUnicode::IsNormalizedNFKC(cstring Str, uint64 Length) {
  return InternalIsNormalized(Str, Length, MJB_NORMALIZATION_NFKC);
}

bool FUnicode::IsNormalizedNFKD(cstring Str, uint64 Length) {
  return InternalIsNormalized(Str, Length, MJB_NORMALIZATION_NFKD);
}

bool FUnicode::NormalizeNFC(cstring Str, uint64 Length, FResult& Out) {
  return InternalNormalize(Str, Length, MJB_NORMALIZATION_NFC, Out);
}

bool FUnicode::NormalizeNFD(cstring Str, uint64 Length, FResult& Out) {
  return InternalNormalize(Str, Length, MJB_NORMALIZATION_NFD, Out);
}

bool FUnicode::NormalizeNFKC(cstring Str, uint64 Length, FResult& Out) {
  return InternalNormalize(Str, Length, MJB_NORMALIZATION_NFKC, Out);
}

bool FUnicode::NormalizeNFKD(cstring Str, uint64 Length, FResult& Out) {
  return InternalNormalize(Str, Length, MJB_NORMALIZATION_NFKD, Out);
}

bool FUnicode::ToLower(cstring Str, uint64 Length, FResult& Out) {
  return InternalCase(Str, Length, MJB_CASE_LOWER, Out);
}

bool FUnicode::ToUpper(cstring Str, uint64 Length, FResult& Out) {
  return InternalCase(Str, Length, MJB_CASE_UPPER, Out);
}

bool FUnicode::ToTitle(cstring Str, uint64 Length, FResult& Out) {
  return InternalCase(Str, Length, MJB_CASE_TITLE, Out);
}

bool FUnicode::CaseFold(cstring Str, uint64 Length, FResult& Out) {
  return InternalCase(Str, Length, MJB_CASE_CASEFOLD, Out);
}

bool FUnicode::NFKCCaseFold(cstring Str, uint64 Length, FResult& Out) {
  mjb_result result;
  mjb_status status = mjb_nfkc_casefold(Str, Length, MJB_ENC_UTF_8, MJB_ENC_UTF_8, &result);
  if(status != MJB_STATUS_OK) {
    return false;
  }
  Out.data = result.output;
  Out.size = result.output_size;
  Out.transformed = result.transformed;
  return true;
}

void FUnicode::Free(FResult& Result) {
  if(Result.data) {
    mjb_result result{};
    result.output = static_cast<char*>(Result.data);
    result.output_size = Result.size;
    result.transformed = Result.transformed;
    mjb_result_free(&result);
    Result.data = nullptr;
    Result.size = 0;
    Result.transformed = false;
  }
}

bool FUnicode::IsGraphic(FCodePoint CodePoint) {
  return mjb_codepoint_is_graphic(CodePoint);
}

bool FUnicode::IsLetter(FCodePoint CodePoint) {
  mjb_character info{};
  if(mjb_codepoint_info(CodePoint, &info) != MJB_STATUS_OK) {
    return false;
  }
  switch((uint32)info.category) {
    case MJB_CATEGORY_LU:
    case MJB_CATEGORY_LL:
    case MJB_CATEGORY_LT:
    case MJB_CATEGORY_LM:
    case MJB_CATEGORY_LO: return true;
  }
  return false;
}

bool FUnicode::IsNumber(FCodePoint CP) {
  mjb_character info{};
  if(mjb_codepoint_info(CP, &info) != MJB_STATUS_OK) {
    return false;
  }
  switch((uint32)info.category) {
    case MJB_CATEGORY_ND:
    case MJB_CATEGORY_NL:
    case MJB_CATEGORY_NO: return true;
  }
  return false;
}

bool FUnicode::IsWhitespace(FCodePoint CodePoint) {
  return mjb_codepoint_is_pattern_white_space(CodePoint);
}

bool FUnicode::Decode(cstring Str, uint64 Length, FCodePoint& OutCodePoint, uint64& OutBytes) {
  if(!Str || Length == 0) {
    return false;
  }
  const uint8_t* Data = reinterpret_cast<const uint8_t*>(Str);
  const uint8_t First = Data[0];

  // ASCII: U+0000 - U+007F
  if(First <= 0x7F) {
    OutCodePoint = First;
    OutBytes = 1;
    return true;
  }

  // 2 bytes: U+0080 - U+07FF
  if((First & 0xE0) == 0xC0) {
    if(Length < 2) {
      return false;
    }
    const uint8_t B1 = Data[1];
    if((B1 & 0xC0) != 0x80) {
      return false;
    }
    FCodePoint CP = ((First & 0x1F) << 6) | (B1 & 0x3F);
    // Overlong encoding
    if(CP < 0x80) {
      return false;
    }
    OutCodePoint = CP;
    OutBytes = 2;
    return true;
  }

  // 3 bytes: U+0800 - U+FFFF
  if((First & 0xF0) == 0xE0) {
    if(Length < 3) {
      return false;
    }
    const uint8_t B1 = Data[1];
    const uint8_t B2 = Data[2];
    if((B1 & 0xC0) != 0x80 || (B2 & 0xC0) != 0x80) {
      return false;
    }
    FCodePoint CP = ((First & 0x0F) << 12) | ((B1 & 0x3F) << 6) | (B2 & 0x3F);
    // Overlong
    if(CP < 0x800) {
      return false;
    }
    // UTF-16 surrogate range is invalid UTF-8
    if(CP >= 0xD800 && CP <= 0xDFFF) {
      return false;
    }
    OutCodePoint = CP;
    OutBytes = 3;
    return true;
  }

  // 4 bytes: U+10000 - U+10FFFF
  if((First & 0xF8) == 0xF0) {
    if(Length < 4) {
      return false;
    }
    const uint8_t B1 = Data[1];
    const uint8_t B2 = Data[2];
    const uint8_t B3 = Data[3];
    if((B1 & 0xC0) != 0x80 || (B2 & 0xC0) != 0x80 || (B3 & 0xC0) != 0x80) {
      return false;
    }
    FCodePoint CP = ((First & 0x07) << 18) | ((B1 & 0x3F) << 12) | ((B2 & 0x3F) << 6) | (B3 & 0x3F);
    // Overlong
    if(CP < 0x10000) {
      return false;
    }
    // Unicode ends at U+10FFFF
    if(CP > 0x10FFFF) {
      return false;
    }
    OutCodePoint = CP;
    OutBytes = 4;
    return true;
  }

  return false;
}

void FUnicode::TestUnicode() {
  cstring ch = "é";
  FCodePoint cp;
  uint64 bytes;
  uint32 length = FUnicode::ByteLength(ch);
  FUnicode::Decode(ch, length, cp, bytes);

  cstring category = "NoCategory";
  if(FUnicode::IsNumber(cp)) {
    category = "NUMBER";
  } else if(FUnicode::IsLetter(cp)) {
    category = "LETTER";
  } else if(FUnicode::IsWhitespace(cp)) {
    category = "SPACE";
  }

  UE_ALERT("Unicode Str:%s, Codepoint:%u, Size:%llu, Category:%s", ch, cp, bytes, category);

  cstring text = "pLAYER, Meu nome é Gabriel o Programador seu dor de cabeça!";
  uint64 textLength = FUnicode::ByteLength(text);

  FUnicode::FResult result{};
  UE_ALERT("Before:%s", text);

  FUnicode::ToTitle(text, textLength, result);
  UE_ALERT("Title:%s", result.data);
  FUnicode::Free(result);

  FUnicode::ToUpper(text, textLength, result);
  UE_ALERT("Upper:%s", result.data);
  FUnicode::Free(result);

  FUnicode::ToLower(text, textLength, result);
  UE_ALERT("Lower:%s", result.data);
  FUnicode::Free(result);

  FUnicode::CaseFold(text, textLength, result);
  UE_ALERT("Fold:%s", result.data);
  FUnicode::Free(result);

  FUnicode::NFKCCaseFold(text, textLength, result);
  UE_ALERT("NFKC:%s", result.data);
  FUnicode::Free(result);
}

#include <stdio.h>
#include <string.h>
#include <string>
#include "internal/lang_script.h"
#include "public/compact_lang_det.h"
#include "public/encodings.h"
using namespace CLD2;

typedef struct {
  const char *name;
  const char *code;
  bool reliable;
  //ReturnChunk *re;
} RESULT;

// Conveys the same information as ReturnChunk, but is more FFI-friendly (for linking to Ruby).
typedef struct {
  const char *langcode;
  const char *content;
} ReturnChunk;

extern "C" {
  ReturnChunk convertToReturnChunk(const char* src, ResultChunk* resultchunk) {
    ReturnChunk rc;
    rc.langcode = LanguageCode(static_cast<Language>(resultchunk->lang1));
    rc.content = std::string(src, resultchunk->offset, resultchunk->bytes).c_str(); 
    return rc;
  }

  RESULT detectLanguageThunkInt(const char * src, bool is_plain_text) {
    const int flags = 0;  // no flags
    const char* tld_hint = NULL;
    const int encoding_hint = UNKNOWN_ENCODING;
    const Language language_hint = UNKNOWN_LANGUAGE;
    const CLDHints cldhints = {NULL, tld_hint, encoding_hint, language_hint};  
    Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    ResultChunkVector* resultchunkvector;
    int text_bytes;
    bool is_reliable;
    Language lang;

    lang = ExtDetectLanguageSummary(src,
                          strlen(src),
                          is_plain_text,
                          &cldhints,
                          flags,
                          language3,
                          percent3,
                          normalized_score3,
                          resultchunkvector,
                          &text_bytes,
                          &is_reliable);

    RESULT res;
    res.name = LanguageName(lang);
    res.code = LanguageCode(lang);
    res.reliable = is_reliable;
    return res;
  }
}

int main(int argc, char **argv) {
}
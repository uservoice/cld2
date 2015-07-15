#include <stdio.h>
#include <string.h>
#include "internal/lang_script.h"
#include "public/compact_lang_det.h"
#include "public/encodings.h"
#include "internal/"

using namespace CLD2;

typedef struct {
  const char *name;
  const char *code;

  bool reliable;
} RESULT;

typedef struct {

} 

extern "C" {
  RESULT detectLanguageThunkInt(const char * src, bool is_plain_text) {
    const int flags = 0;  // no flags
    const char* tld_hint = NULL;
    const int encoding_hint = UNKNOWN_ENCODING;
    const Language language_hint = UNKNOWN_LANGUAGE;
    const CLDHints cldhints = {tld_hint, encoding_hint, language_hint};  
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
                          &resultchunkvector,
                          &text_bytes,
                          &is_reliable);

    RESULT res;
    res.name = LanguageName(lang);
    res.code = LanguageCode(lang);
    res.reliable = is_reliable;
    return res;
  }
}

  for (int i = 0; i < static_cast<int>(resultchunkvector->size()); ++i) {
    ResultChunk* rc = &(*resultchunkvector)[i];
    Language lang1 = static_cast<Language>(rc->lang1);
    string this_chunk = string(src, rc->offset, rc->bytes);
    fprintf(f, "[%d]{%d %d %s} ", i, rc->offset, rc->bytes, LanguageCode(lang1));
  }

int main(int argc, char **argv) {
}
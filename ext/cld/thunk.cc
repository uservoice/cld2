#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "internal/lang_script.h"
#include "public/compact_lang_det.h"
#include "public/encodings.h"
using namespace CLD2;

typedef struct {
  const char *code;
  int percent;
  double score;
} LanguageResult;

// Conveys the same information as CLD::ResultChunk, but contains language code 
// instead of CLD's internal language representation.
typedef struct {
  int offset;
  uint16 bytes;
  const char *langcode;
} ReturnChunk;

typedef struct {
  const char *name;
  const char *code;
  bool reliable;
  LanguageResult *langresults;
  int num_chunks;
  ReturnChunk *returnchunksptr;
} RESULT;

extern "C" {

  RESULT detectLanguageThunkInt(const char * src, bool is_plain_text) {
    const int flags = 0;  // no flags
    const char* tld_hint = NULL;
    const int encoding_hint = UNKNOWN_ENCODING;
    const Language language_hint = UNKNOWN_LANGUAGE;
    const CLDHints cldhints = {NULL, tld_hint, encoding_hint, language_hint};  
    Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    ResultChunkVector resultchunkvector;
    int text_bytes;
    bool is_reliable;    
    Language lang;

    lang = ExtDetectLanguageSummary(
                          src,
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

    // Construct language results to return
    LanguageResult *langresults = new LanguageResult [3];
    for (int i = 0; i < 3; i++) {
      langresults[i].code = LanguageCode(language3[i]);
      langresults[i].percent = percent3[i];
      langresults[i].score = normalized_score3[i];
    }

    // Constructs individual chunk results to return
    int num_chunks = static_cast<int>(resultchunkvector.size());
    ReturnChunk *returnchunkptr = new ReturnChunk [num_chunks];
    for (int i = 0; i < num_chunks; i++) {
      ResultChunk rc = resultchunkvector[i];
      returnchunkptr[i].offset = rc.offset;
      returnchunkptr[i].bytes = rc.bytes;
      returnchunkptr[i].langcode = LanguageCode(static_cast<Language>(rc.lang1));
    }

    RESULT res;
    res.name = LanguageName(lang);
    res.code = LanguageCode(lang);
    res.reliable = is_reliable;
    res.langresults = langresults;
    res.num_chunks = num_chunks;
    res.returnchunksptr = returnchunkptr;
    return res;
  }
}

int main(int argc, char **argv) {
}

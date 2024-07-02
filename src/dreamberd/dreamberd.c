#include "dreamberd.h"
#include "lexer.h"

#include "nob.h"

bool dbRunProgram(char* text) {
    Tokens tokens = dbLexer(text);
    dbLexerLogTokens(NOB_INFO, tokens);
    return true;
}

#include "dreamberd.h"
#include "lexer.h"

#include "nob.h"

bool dbRunProgram(char* text) {
    Tokens tokens = dbLexerMakeTokens(text);
    dbLexerLogTokens(NOB_INFO, tokens);
    return true;
}

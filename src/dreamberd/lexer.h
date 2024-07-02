#pragma once
#include <stdbool.h>
#include "nob.h"

typedef enum {
    TOKEN_NONE,
    TOKEN_IDENTIFIER,
    TOKEN_QUOTE,
    TOKEN_WHITESPACE,
    TOKEN_BACKSLASH,
    TOKEN_EOL,
    TOKEN_NEWLINE,
} TokenType;

typedef struct {
    TokenType type;
    // The value of this token
    Nob_String_Builder strValue;
    // Some additional information (e.g. amount of quotes)
    int intValue;
} Token;

typedef struct {
    Token* items;
    size_t count;
    size_t capacity;
} Tokens;

Tokens dbLexerMakeTokens(char* text);
void dbLexerLogTokens(Nob_Log_Level level, Tokens tokens);

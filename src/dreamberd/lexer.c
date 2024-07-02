#include "lexer.h"

Tokens dbLexerMakeTokens(char* text) {
    Tokens tokens = {0};
    Token currentToken = {0};

    #define newToken() do {                         \
        if (currentToken.type != TOKEN_NONE)        \
            nob_da_append(&tokens, currentToken);   \
        currentToken = (Token){0};                  \
    } while (0)
    #define discardToken() currentToken = (Token){0}

    char* c = text;
    while (*c != 0) {
        // Always add an identifier if a backslash was before this character, except with newlines
        if (*c != '\n' && currentToken.type == TOKEN_BACKSLASH) {
            discardToken();
            currentToken.type = TOKEN_IDENTIFIER;
            nob_da_append(&currentToken.strValue, *c);
            c++;
            continue;
        }

        switch (*c) {
            case '\\': {
                newToken();
                currentToken.type = TOKEN_BACKSLASH;
                nob_da_append(&currentToken.strValue, *c);
                c++;
            } break;
            case ' ': case '(': case ')': {
                newToken();
                currentToken.type = TOKEN_WHITESPACE;
                nob_da_append(&currentToken.strValue, ' ');
                c++;
            } break;
            case '!': case '?': {
                newToken();
                currentToken.type = TOKEN_EOL;
                nob_da_append(&currentToken.strValue, *c);
                c++;
            } break;
            case '\'': case '"': {
                newToken();
                currentToken.type = TOKEN_QUOTE;
                while (*c != 0 && (*c == '\'' || *c == '"')) {
                    nob_da_append(&currentToken.strValue, *c);
                    currentToken.intValue += *c == '"' ? 2 : 1;
                    c++;
                }
            } break;
            case '\n': {
                // Ignore the newline if a backslash was found before
                if (currentToken.type == TOKEN_BACKSLASH) {
                    discardToken();
                    c++;
                    break;
                }
                newToken();
                currentToken.type = TOKEN_NEWLINE;
                nob_da_append(&currentToken.strValue, *c);
                c++;
            } break;
            default: {
                if (currentToken.type != TOKEN_IDENTIFIER)
                    newToken();

                currentToken.type = TOKEN_IDENTIFIER;
                nob_da_append(&currentToken.strValue, *c);
                c++;
            } break;
        }
    }
    newToken();
    #undef newToken
    #undef discardToken

    return tokens;
}

static const char* dbLexerTokenNames[] = {
    [TOKEN_NONE] = "TOKEN_NONE",
    [TOKEN_IDENTIFIER] = "TOKEN_IDENTIFIER",
    [TOKEN_QUOTE] = "TOKEN_QUOTE",
    [TOKEN_WHITESPACE] = "TOKEN_WHITESPACE",
    [TOKEN_BACKSLASH] = "TOKEN_BACKSLASH",
    [TOKEN_EOL] = "TOKEN_EOL",
    [TOKEN_NEWLINE] = "TOKEN_NEWLINE",
};

void dbLexerLogTokens(Nob_Log_Level level, Tokens tokens) {
    for (size_t i = 0; i < tokens.count; ++i) {
        Token token = tokens.items[i];
        const char* tokenName = dbLexerTokenNames[token.type];
        nob_log(level, "Token{type: %s, strValue: `%.*s`, intValue: %d}", tokenName, token.strValue.count, token.strValue.items, token.intValue);
    }
}

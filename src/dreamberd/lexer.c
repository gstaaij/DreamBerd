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
    #define parseBackslash() if (currentToken.type == TOKEN_BACKSLASH) {    \
        discardToken();                                                     \
        currentToken.type = TOKEN_IDENTIFIER;                               \
        nob_da_append(&currentToken.strValue, *c);                          \
        c++;                                                                \
        break;                                                              \
    }

    char* c = text;
    while (*c != 0) {
        switch (*c) {
            case '\\': {
                parseBackslash();
                newToken();
                currentToken.type = TOKEN_BACKSLASH;
                nob_da_append(&currentToken.strValue, *c);
                c++;
            } break;
            case ' ': case '(': case ')': {
                parseBackslash();
                newToken();
                currentToken.type = TOKEN_WHITESPACE;
                nob_da_append(&currentToken.strValue, *c);
                c++;
            } break;
            case '!': case '?': {
                parseBackslash();
                newToken();
                currentToken.type = TOKEN_EOL;
                nob_da_append(&currentToken.strValue, *c);
                c++;
            } break;
            case '\'': case '"': {
                parseBackslash();
                newToken();
                currentToken.type = TOKEN_QUOTE;
                nob_da_append(&currentToken.strValue, *c);
                currentToken.intValue += *c == '"' ? 2 : 1;
                c++;
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
            case '\r': {
                // Skip the CR, the end of line sequence should always contain an LF as well.
                c++;
            } break;
            default: {
                char escapedChar = *c;
                // If a backslash was before this, escape the character accordinly
                if (currentToken.type == TOKEN_BACKSLASH) {
                    discardToken();
                    switch (*c) {
                        case 'a': {
                            escapedChar = '\a';
                        } break;
                        case 'b': {
                            escapedChar = '\b';
                        } break;
                        case 'f': {
                            escapedChar = '\f';
                        } break;
                        case 'n': {
                            escapedChar = '\n';
                        } break;
                        case 'r': {
                            escapedChar = '\r';
                        } break;
                        case 't': {
                            escapedChar = '\t';
                        } break;
                        case 'v': {
                            escapedChar = '\v';
                        } break;
                        // TODO: implement \nnn, \xhh, \uhhhh and \Uhhhhhhhh
                        default: {
                            escapedChar = *c;
                        } break;
                    }
                }

                // If the current token is an identifier, we can just add to the current token,
                // otherwise, we have to start a new token
                if (currentToken.type != TOKEN_IDENTIFIER)
                    newToken();

                currentToken.type = TOKEN_IDENTIFIER;
                nob_da_append(&currentToken.strValue, escapedChar);
                c++;
            } break;
        }
    }
    newToken();
    currentToken.type = TOKEN_EOF;
    newToken();
    #undef newToken
    #undef discardToken

    return tokens;
}

// Makes the string tokens and combines adjacent identifier tokens
Tokens dbLexerMakeStrings(Tokens tokens) {
    Tokens newTokens = {0};
    for (size_t i = 0; i < tokens.count; ++i) {
        Token token = tokens.items[i];
        // If this token is a quote, check for strings
        if (token.type == TOKEN_QUOTE) {
        #ifdef DEBUG
            nob_log(NOB_INFO, "----------------------------------------");
        #endif // DEBUG

            size_t quoteTokenAmount = 0;
            size_t j = i;
            // Count the amount of quote tokens
            for (; j < tokens.count && tokens.items[j].type == TOKEN_QUOTE; ++j) {
                quoteTokenAmount++;
            }
            size_t initialQuoteTokenAmount = quoteTokenAmount;

            bool found = false;
            size_t endIndex = 0;
            // The first loop
            for (;;) {
                found = false;
                // The second loop: go over all tokens until the end of the line to check for matching quotes
                for (size_t k = j; k + quoteTokenAmount < tokens.count; ++k) {
                    Token token = tokens.items[k];
                    if (token.type == TOKEN_QUOTE) {
                        // If this token is a quote, start checking for matching quotes
                        // The third loop
                        for (size_t quoteIndex = 0; quoteIndex < quoteTokenAmount; ++quoteIndex) {
                            // The current quote
                            Token rightToken = tokens.items[k + quoteIndex];
                            // The corresponding quote at the beginning of the string
                            Token leftToken = tokens.items[i + (quoteTokenAmount - quoteIndex - 1)];
                            if (
                                rightToken.type == TOKEN_QUOTE &&
                                leftToken.intValue == rightToken.intValue
                            ) {
                            #ifdef DEBUG
                                nob_log(NOB_INFO, "Found a valid quote! (quoteTokenAmount = %zu)", quoteTokenAmount);
                            #endif // DEBUG

                                // The quotes match
                                found = true;
                            } else {
                            #ifdef DEBUG
                                nob_log(NOB_INFO, "Not a quote or invalid quote (quoteTokenAmount = %zu)", quoteTokenAmount);
                            #endif // DEBUG

                                // The quotes don't match, break out of the third loop
                                found = false;
                                break;
                            }
                        }
                        // If we found matching quotes, great! We can break out of the second loop
                        if (found) {
                            endIndex = k;
                            break;
                        }
                    } else if (token.type == TOKEN_NEWLINE || token.type == TOKEN_EOF) {
                        // If we run out of places to check for matching quotes, because we reached the end of the line or the end of the file,
                        // we haven't found anything and we can break out of the second loop
                        found = false;
                        break;
                    }
                }
                if (found) {
                    // If we found something, we can skip the quotes and add everything within the found quotes to the new token list,
                    // and break out of the first loop
                    Token newToken = {0};
                    newToken.type = TOKEN_STRING;
                    for (i = j; i < endIndex; ++i) {
                        Token oldToken = tokens.items[i];
                        nob_sb_append_buf(&newToken.strValue, oldToken.strValue.items, oldToken.strValue.count);
                    }
                    nob_da_append(&newTokens, newToken);

                #ifdef DEBUG
                    nob_log(NOB_INFO, "Found string `%.*s`! (quoteTokenAmount = %zu)", newToken.strValue.count, newToken.strValue.items, quoteTokenAmount);
                #endif // DEBUG

                    // And skip all of the tokens for this string.
                    i = endIndex + quoteTokenAmount - 1;

                    break;
                } else {
                #ifdef DEBUG
                    nob_log(NOB_INFO, "Didn't find quote... (quoteTokenAmount = %zu -> %zu)", quoteTokenAmount, quoteTokenAmount - 1);
                #endif // DEBUG

                    // We didn't find matching quotes, try it with less quotes
                    quoteTokenAmount--;
                    j--;
                    if (quoteTokenAmount == 0 || j <= i) {
                    #ifdef DEBUG
                        nob_log(NOB_INFO, "Couldn't find any matching closing quotes.");
                    #endif // DEBUG

                        // If we ran out of quotes to check, so just add the quote tokens to the new tokens,
                        // and break out of the first loop
                        found = false;
                        for (j = i; j < i + initialQuoteTokenAmount; ++j) {
                            nob_da_append(&newTokens, tokens.items[j]);
                        }
                        // Also don't forget to skip the quotes we just added
                        i += initialQuoteTokenAmount - 1;
                        break;
                    }
                }
            }
            // And we're finally done checking for strings...
        } else if (token.type == TOKEN_IDENTIFIER) {
            // Because of backslashes, two or more identifiers can be created next to each other
            // We don't want this, so we'll glue them together
            Token newToken = {0};
            newToken.type = TOKEN_IDENTIFIER;
            for (; i < tokens.count && tokens.items[i].type == TOKEN_IDENTIFIER; ++i) {
                Token oldToken = tokens.items[i];
                nob_sb_append_buf(&newToken.strValue, oldToken.strValue.items, oldToken.strValue.count);
            }
            nob_da_append(&newTokens, newToken);
            // i overshoots by 1, so we need to decrease it again
            i--;
        } else {
            // If this token isn't a quote and it's not an identifier, we can just add the token
            nob_da_append(&newTokens, token);
        }
    }
    return newTokens;
}

Tokens dbLexer(char* text) {
    Tokens tokens = dbLexerMakeTokens(text);
    Tokens tokensWithStrings = dbLexerMakeStrings(tokens);
    nob_da_free(tokens);
    return tokensWithStrings;
}

static const char* dbLexerTokenNames[] = {
    [TOKEN_NONE] = "TOKEN_NONE",
    [TOKEN_IDENTIFIER] = "TOKEN_IDENTIFIER",
    [TOKEN_QUOTE] = "TOKEN_QUOTE",
    [TOKEN_STRING] = "TOKEN_STRING",
    [TOKEN_WHITESPACE] = "TOKEN_WHITESPACE",
    [TOKEN_BACKSLASH] = "TOKEN_BACKSLASH",
    [TOKEN_EOL] = "TOKEN_EOL",
    [TOKEN_EOF] = "TOKEN_EOF",
    [TOKEN_NEWLINE] = "TOKEN_NEWLINE",
};

void dbLexerLogTokens(Nob_Log_Level level, Tokens tokens) {
    for (size_t i = 0; i < tokens.count; ++i) {
        Token token = tokens.items[i];
        const char* tokenName = dbLexerTokenNames[token.type];
        nob_log(level, "Token{type: %s, strValue: `%.*s`, intValue: %d}", tokenName, token.strValue.count, token.strValue.items, token.intValue);
    }
}

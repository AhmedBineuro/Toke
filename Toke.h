#ifndef TOKE_H
#define TOKE_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/**
 * @author Ahmed Abdalla
 * @date 27/08/2025
 * @brief A simple tokenizer API to tokenize a file into custom tokens (Ignoring white space and new lines)
 *
 */

typedef struct
{
    char *str;
    size_t size;
    size_t capacity;
    bool free;
} T_String;

typedef struct
{
    T_String type; // Can be a custom type but everything else will be a null token
    T_String token;
    int at;
    size_t lineNumber; // Which line was it read from in the file
} Token;

typedef bool (*ft_ptr)(T_String);

// A format based token
typedef struct
{
    T_String name;
    ft_ptr formatTest;
} FormatToken;

typedef struct
{
    FormatToken *array;
    size_t size;
    size_t capacity;
} FormatTokenArray;
typedef struct
{
    Token *array;
    size_t size;
    size_t capacity;
} TokenArray;

typedef struct
{
    TokenArray reservedTokens;
    TokenArray tokens;
    FormatTokenArray postprocTokens;
} Context;

// Creates a fresh instance of a context
Context *CreateConext();
// Includes text based token
void IncludeToken(Context *CTX, char *name, char *token);
// Includes format based token
void IncludeFormatToken(Context *CTX, char *name, ft_ptr formatValidator);
TokenArray *TokenizeFile(Context *CTX, char *path);
void FreeContext(Context *CTX);

// T_String functions
T_String CreateString(size_t size);
// Adds a character to the end of the string
void AddChar(T_String *str, char c);
// Sets the string to the value passed
T_String GetString(char *str);
void SetString(T_String *str, char *value);
void PrintString(const T_String *str);
bool IsEqual(T_String a, T_String b);

// Format tests
bool IsFloat(T_String str);
bool IsInteger(T_String str);
bool IsNumber(T_String str);
bool IsHex(T_String str);
bool IsAlphabetic(T_String str);
bool IsAlphaNumeric(T_String str);

// Add token into token array,
void AddToken(TokenArray *ta, Token token);
// Add token type into token type array
void PrintToken(const Token *t);

// Add a format based token
void AddFormatToken(FormatTokenArray *ta, FormatToken token);
void FitFormatTokenArray(FormatTokenArray *ta);

// Shrinks string to size
void FitString(T_String *str);
// Shrinks token array to size
void FitTokenArray(TokenArray *ta);

void PrintTokenArray(const TokenArray *ta);
/**
 * @brief Function that gets the type match of a character
 * @returns TokenType with type (NULLTOK by default)
 */
Token GetTokenMatch(TokenArray *ta, T_String str);

/**
 * @brief Function that gets the type match of a character based on the formatting
 * @returns TokenType with type (NULLTOK by default)
 */
FormatToken GetFormatTokenMatch(FormatTokenArray *ta, T_String str);

/**
 * @brief Function to tokenize file
 * @returns TokenArray filled with tokens
 * */
TokenArray Tokenize(FILE *f, TokenArray *reservedTokens);

// Frees memory taken by the token array
void FreeTokenArray(TokenArray *ta);
// Tests if the name passed matches any of the token type names in the array
bool HasType(TokenArray *ta, char *typeName);
bool HasToken(TokenArray *ta, Token tt);
bool HasTokenText(TokenArray *ta, T_String str);
////////////////START OF IMPLEMENTATION//////////////////////

Context *CreateConext()
{
    Context *CTX = (Context *)malloc(sizeof(Context));
    CTX->tokens.size = 0;
    CTX->reservedTokens.size = 0;
    CTX->postprocTokens.size = 0;
    return CTX;
}

void IncludeToken(Context *CTX, char *name, char *token)
{
    if (CTX == NULL)
    {
        printf("Context not initialized, use CreateConext function before adding tokens\n");
        return;
    }
    Token t;
    SetString(&t.type, name);
    SetString(&t.token, token);
    AddToken(&CTX->reservedTokens, t);
}
void IncludeFormatToken(Context *CTX, char *name, ft_ptr formatValidator)
{
    if (CTX == NULL)
    {
        printf("Context not initialized, use CreateConext function before adding tokens\n");
        return;
    }
    FormatToken ft;
    SetString(&ft.name, name);
    ft.formatTest = formatValidator;
    AddFormatToken(&CTX->postprocTokens, ft);
}

TokenArray *TokenizeFile(Context *CTX, char *path)
{
    if (CTX == NULL)
    {
        printf("Context not initialized, use CreateConext function before tokenizing\n");
        return NULL;
    }
    FILE *f = fopen(path, "r");
    CTX->tokens = Tokenize(f, &CTX->reservedTokens);
    // Perform post processing typing
    for (int i = 0; i < CTX->tokens.size; i++)
    {
        // Only change the null tokens since everything else has been typed
        if (strcmp(CTX->tokens.array[i].type.str, "NULLTOK") != 0)
            continue;
        FormatToken type = GetFormatTokenMatch(&CTX->postprocTokens, CTX->tokens.array[i].token);
        if (strcmp(type.name.str, "NULLTOK") != 0)
            CTX->tokens.array[i].type = type.name;
    }
    if (f != NULL)
        fclose(f);
    return &CTX->tokens;
}

void FreeContext(Context *CTX)
{
    if (CTX == NULL)
        return;
    FreeTokenArray(&CTX->tokens);
    free(CTX);
}

T_String CreateString(size_t size)
{
    T_String str;
    str.size = size;
    str.capacity = size;
    if (size == 0)
    {
        str.str = NULL;
        str.free = false;
    }
    else
    {
        str.str = (char *)malloc(sizeof(char) * size);
        str.free = true;
    }
    return str;
}
void AddChar(T_String *str, char c)
{
    bool add = true;
    if (str->size == 0)
    {
        str->size = 0;
        str->capacity = 2;
        str->str = (char *)malloc((sizeof(char) * str->capacity));
    }
    else if (!str->free)
    {
        T_String s = CreateString(0);
        for (int i = 0; str->str[i] != '\0'; i++)
        {
            AddChar(&s, str->str[i]);
        }
        AddChar(&s, c);
        str->str = s.str;
        str->size = s.size;
        str->capacity = s.capacity;
        add = false;
    }
    if (str->size + 1 >= str->capacity)
    {
        str->capacity *= 2;
        str->str = (char *)realloc(str->str, sizeof(char) * str->capacity);
    }
    str->free = true;
    if (add)
    {
        str->str[str->size] = c;
        str->str[str->size + 1] = '\0';
        str->size += 1;
    }
}
void SetString(T_String *str, char *value)
{
    if (value == NULL)
        return;
    str->free = false;
    int size = 0;
    while (value[size] != '\0')
        size += 1;
    str->str = value;
    str->size = size;
    str->capacity = size + 1;
}
T_String GetString(char *str)
{
    T_String out;
    SetString(&out, "");
    if (str == NULL)
        return out;
    SetString(&out, str);
    return out;
}
void PrintString(const T_String *str)
{
    printf("%s", str->str);
}
void FitString(T_String *str)
{
    if (str->size + 1 != str->capacity)
    {
        str->capacity = str->size + 1;
        str->str = (char *)realloc(str->str, (sizeof(char) * str->capacity));
    }
}

void AddToken(TokenArray *ta, Token token)
{
    if (ta->size == 0)
    {
        ta->capacity = 2;
        ta->array = (Token *)malloc((sizeof(Token) * ta->capacity));
    }
    if (ta->size == ta->capacity)
    {
        ta->capacity *= 2;
        ta->array = (Token *)realloc(ta->array, (sizeof(Token) * ta->capacity));
    }
    ta->array[ta->size] = token;
    ta->size += 1;
}
void AddFormatToken(FormatTokenArray *ta, FormatToken token)
{
    if (ta->size == 0)
    {
        ta->capacity = 2;
        ta->array = (FormatToken *)malloc((sizeof(FormatToken) * ta->capacity));
    }
    if (ta->size == ta->capacity)
    {
        ta->capacity *= 2;
        ta->array = (FormatToken *)realloc(ta->array, (sizeof(FormatToken) * ta->capacity));
    }
    ta->array[ta->size] = token;
    ta->size += 1;
}

inline void FitFormatTokenArray(FormatTokenArray *ta)
{
    if (ta->size != ta->capacity)
    {
        ta->capacity = ta->size;
        ta->array = (FormatToken *)realloc(ta->array, (sizeof(FormatToken) * ta->capacity));
    }
}

void PrintToken(const Token *t)
{
    printf("\n[Type]:");
    PrintString(&t->type);
    printf("\t[Line]:%ld\t[At]:%d\t[Text]:", t->lineNumber, t->at);
    PrintString(&t->token);
}
void FitTokenArray(TokenArray *ta)
{
    if (ta->size != ta->capacity)
    {
        ta->capacity = ta->size;
        ta->array = (Token *)realloc(ta->array, (sizeof(Token) * ta->capacity));
    }
}
void PrintTokenArray(const TokenArray *ta)
{
    for (int i = 0; i < ta->size; i++)
    {
        PrintToken(&ta->array[i]);
        printf("\n\n");
    }
}
bool IsReservedToken(TokenArray *ta, T_String c)
{
    for (int i = 0; i < ta->size; i++)
    {
        if (((ta->array[i].token.size == 0) && (strcmp(ta->array[i].type.str, "NULLTOK") != 0)) || IsEqual(ta->array[i].token, c))
        {
            return true;
        }
    }
}
Token GetTokenMatch(TokenArray *ta, T_String str)
{
    for (int i = 0; i < ta->size; i++)
    {
        if (IsEqual(ta->array[i].token, str))
        {

            return ta->array[i];
        }
    }
    Token t;
    SetString(&t.type, "NULLTOK");
    t.token = str;
    t.lineNumber = -1;
    return t;
}

inline FormatToken GetFormatTokenMatch(FormatTokenArray *ta, T_String str)
{
    FormatToken t;
    bool found = false;
    for (int i = 0; i < ta->size; i++)
    {
        if (ta->array[i].formatTest != NULL && ta->array[i].formatTest(str))
        {
            t.name = ta->array[i].name;
            found = true;
            break;
        }
    }
    if (!found)
        SetString(&t.name, "NULLTOK");
    return t;
}

TokenArray Tokenize(FILE *f, TokenArray *reservedTokens)
{
    int line = 1;
    int at = 1;
    TokenArray ta;
    ta.size = 0;
    ta.capacity = 0;
    if (f == NULL)
        return ta;
    // Add base null token token
    if (!HasType(reservedTokens, "NULLTOK"))
    {
        Token t;
        SetString(&t.type, "NULLTOK");
        SetString(&t.token, "\0");
        t.lineNumber = -1;
        t.at = -1;
        AddToken(reservedTokens, t);
    }
    char c;
    while ((c = fgetc(f)) != EOF)
    {
        Token t;
        SetString(&t.token, "");
        if (c == '\n')
        {
            line += 1;
            at = 1;
            continue;
        }
        // If it is white space skip to the next character
        if (c == ' ')
        {
            at++;
            continue;
        }

        T_String tok;
        tok.size = 0;
        AddChar(&tok, c);
        t = GetTokenMatch(reservedTokens, tok);
        t.at = at;
        if (strcmp(t.type.str, "NULLTOK") != 0)
        {
            t.token.size = 0;
            AddChar(&t.token, c);
        }
        else
        {
            bool endReserved = false, whiteSpace = false; // Used to indicate if the end of the current token is a reserved character
            T_String s;
            s.size = 0;
            Token endToken; // Is used in case the string ends with a reserved token
            int newAt = at;
            SetString(&t.token, "");
            while (c != EOF)
            {
                SetString(&s, "");
                AddChar(&s, c);
                if (c == ' ')
                {
                    whiteSpace = true;
                    break;
                }
                if (c == '\n')
                {
                    line++;
                    newAt = 0;
                    break;
                }
                // Check if the last fetched character is a reserved token and if so break to not add it to the string
                endToken = GetTokenMatch(reservedTokens, s);
                if (strcmp(endToken.type.str, "NULLTOK") != 0)
                {
                    endReserved = true;
                    endToken.at = newAt;
                    break;
                }
                AddChar(&t.token, c);
                c = fgetc(f);
                newAt++;
            }
            Token tempType = GetTokenMatch(reservedTokens, t.token);
            if (strcmp(tempType.type.str, "NULLTOK") != 0)
            {
                t = tempType;
            }
            if (endReserved)
            {
                t.lineNumber = line;
                t.at = at;
                AddToken(&ta, t);
                t = endToken;
            }
            if (endReserved || whiteSpace)
                at = newAt;
        }
        t.lineNumber = line;
        AddToken(&ta, t);
        at++;
    }
    return ta;
}
void FreeTokenArray(TokenArray *ta)
{
    for (int i = 0; i < ta->size; i++)
    {
        if (ta->array[i].token.free)
            free(ta->array[i].token.str);
        if (ta->array[i].type.free)
            free(ta->array[i].type.str);
    }
    if (ta->size != 0)
        free(ta->array);
}

bool HasType(TokenArray *ta, char *typeName)
{
    for (int i = 0; i < ta->size; i++)
    {
        if (strcmp(ta->array[i].type.str, typeName))
            return true;
    }
    return false;
}
inline bool HasToken(TokenArray *ta, Token t)
{
    for (int i = 0; i < ta->size; i++)
    {
        if (strcmp(ta->array[i].type.str, t.type.str) == 0 &&
            IsEqual(ta->array[i].token, t.token))
            return true;
    }
    return false;
}
inline bool HasTokenText(TokenArray *ta, T_String str)
{
    for (int i = 0; i < ta->size; i++)
    {
        if (strcmp(ta->array[i].token.str, str.str) == 0)
            return true;
    }
    return false;
}
bool IsEqual(T_String a, T_String b)
{
    if (a.size != b.size)
        return false;
    for (int i = 0; i < a.size; i++)
        if (a.str[i] != b.str[i])
            return false;
    return true;
}
inline bool IsNumber(T_String str)
{
    return IsInteger(str) || IsFloat(str);
}
inline bool IsInteger(T_String str)
{
    for (int i = 0; i < str.size; i++)
    {
        if (str.str[i] < '0' || str.str[i] > '9')
            return false;
    }
    return true && (str.size > 0);
}
inline bool IsFloat(T_String str)
{
    bool decimalFlag = false;
    for (int i = 0; i < str.size; i++)
    {
        if (str.str[i] == '.')
        {
            // If we already encountered a decimal point, then having another one invalidates the number
            if (decimalFlag)
                return false;
            decimalFlag = true;
        }
        else if (str.str[i] < '0' || str.str[i] > '9')
            return false;
    }
    return true && (str.size > 0);
}
inline bool IsHex(T_String str)
{

    bool xFlag = false;
    for (int i = 0; i < str.size; i++)
    {
        if (str.str[i] == 'x' || str.str[i] == 'X')
        {
            // If we already encountered an x,
            // then having another one invalidates the hex value
            // Also confirm that the x occures either in the beginning or after a 0 only
            if (xFlag || (i != 0 && (i == 1 && str.str[0] != '0')))
                return false;
            xFlag = true;
        }
        else if (
            // Not in the numerical range
            (str.str[i] < '0' || (str.str[i] > '9')) &&
            // Not in the allowed uppercase alphabetical range
            (str.str[i] < 'A' || (str.str[i] > 'F') &&
                                     // Not in the allowed lowercase alphabetical range
                                     (str.str[i] < 'a' || (str.str[i] > 'f'))))
            return false;
    }

    return true && (str.size > 0);
}
inline bool IsAlphabetic(T_String str)
{

    for (int i = 0; i < str.size; i++)
    {
        if ((str.str[i] < 'A' || str.str[i] > 'Z') && (str.str[i] < 'a' || str.str[i] > 'z'))
            return false;
    }
    return true && (str.size > 0);
}
inline bool IsAlphaNumeric(T_String str)
{
    for (int i = 0; i < str.size; i++)
    {
        if ((str.str[i] < '0' || str.str[i] > '9') &&
            (str.str[i] < 'A' || str.str[i] > 'Z') &&
            (str.str[i] < 'a' || str.str[i] > 'z'))
            return false;
    }
    return true && (str.size > 0);
}
#endif
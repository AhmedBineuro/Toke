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
    T_String name;
    char token;
} TokenType;

typedef struct
{
    TokenType *array;
    size_t size;
    size_t capacity;
} TokenTypeArray;

typedef struct
{
    TokenType type; // Can be a custom type but everything else will be either Identifier,Space, or New line
    T_String str;
    size_t lineNumber; // Which line was it read from in the file
} Token;

typedef struct
{
    Token *array;
    size_t size;
    size_t capacity;
} TokenArray;

// T_String functions
//Adds a character to the end of the string
void AddChar(T_String *str, char c);
//Sets the string to the value passed
T_String GetString(const char* str);
void SetString(T_String *str, char *value);
void PrintString(const T_String *str);

// Add token into token array,
void AddToken(TokenArray *ta, Token token);
//Add token type into token type array
void AddTokenType(TokenTypeArray *ta, TokenType token);
void PrintToken(const Token *t);
void PrintTokenType(const TokenType *t);

//Shrinks string to size
void FitString(T_String *str);
//Shrinks token array to size
void FitTokenArray(TokenArray *ta);
//Shrinks token type array to size
void FitTokenTypeArray(TokenTypeArray *ta);

void PrintTokenArray(const TokenArray *ta);
//Tests if the character is reserved and not an identifier
bool IsReservedToken(TokenTypeArray *ta, char c);
/**
 * @brief Function that gets the type match of a character
 * @returns TokenType with type (Identifier by default)
 */
TokenType GetTypeMatch(TokenTypeArray*ta,char c);
Token GetTokenMatch(TokenArray*ta,T_String str);
/**
 * @brief Function to tokenize file
 * @returns TokenArray filled with tokens
 * */
TokenArray Tokenize(FILE *f, TokenTypeArray *tokenTypes);

//Frees memory taken by the token array
void FreeTokenArray(TokenArray* ta);
//Frees memory taken by the token type array
void FreeTokenTypeArray(TokenTypeArray* ta);

bool HasType(TokenTypeArray*ta,TokenType tt);
//Tests if the name passed matches any of the token type names in the array
bool HasTypeName(TokenTypeArray *ta, T_String str);
bool HasToken(TokenArray *ta, Token tt);
bool HasTokenText(TokenArray *ta, T_String str); 
////////////////START OF IMPLEMENTATION//////////////////////


void AddChar(T_String *str, char c)
{
    str->free=true;
    if(str->size==0){
        str->capacity = 2;
        str->str = (char *)malloc((sizeof(char) * str->capacity));
    }
    if (str->size + 1 == str->capacity)
    {
        str->capacity *= 2;
        str->str = (char *)realloc(str->str, sizeof(char) * str->capacity);
    }
    str->str[str->size] = c;
    str->str[str->size + 1] = '\0';
    str->size += 1;
}
void SetString(T_String *str, char *value)
{
    if(value==NULL)
        return;
    str->free=false;
    int size = 0;
    while (value[size] != '\0')
        size += 1;
    str->str = value;
    str->size = size;
    str->capacity = size + 1;
}
T_String GetString(const char* str){
    T_String out;
    SetString(&out,"");
    if(str==NULL)
        return out;
    SetString(&out,str);
    return out;
}
void PrintString(const T_String *str)
{
    printf("%s",str->str);
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
    if(ta->size==0){
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
void AddTokenType(TokenTypeArray *ta, TokenType tokentype)
{
    if(ta->size==0){
        ta->capacity = 2;
        ta->array = (TokenType *)malloc((sizeof(TokenType) * ta->capacity));
    }
    if (ta->size == ta->capacity)
    {
        ta->capacity *= 2;
        ta->array = (TokenType *)realloc(ta->array, (sizeof(TokenType) * ta->capacity));
    }
    ta->array[ta->size] = tokentype;
    ta->size += 1;
}

void PrintTokenType(const TokenType *t)
{
    printf("[Type]:");
    PrintString(&t->name);
    printf("\t[Character]:%c", t->token);
}
void PrintToken(const Token *t)
{
    PrintTokenType(&t->type);
    printf("\n[Line]:%ld\t[Text]:",t->lineNumber);
    PrintString(&t->str);
}
void FitTokenTypeArray(TokenTypeArray *ta)
{
    if (ta->size != ta->capacity)
    {
        ta->capacity = ta->size;
        ta->array = (TokenType *)realloc(ta->array, (sizeof(TokenType) * ta->capacity));
    }
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
bool IsReservedToken(TokenTypeArray *ta, char c)
{
    for (int i = 0; i < ta->size; i++)
    {
        if (ta->array[i].token == c && (strcmp(ta->array[i].name.str, "Identifier") != 0))
        {
            return true;
        }
    }
    return false;
}
TokenType GetTypeMatch(TokenTypeArray*ta,char c){
    for (int i = 0; i < ta->size; i++)
    {
        if (ta->array[i].token == c && (strcmp(ta->array[i].name.str, "Identifier") != 0))
        {
            return ta->array[i];
        }
    }
    TokenType tt;
    SetString(&tt.name,"Identifier");
    tt.token='\0';
    return tt;
}
Token GetTokenMatch(TokenArray*ta,T_String str){
        for (int i = 0; i < ta->size; i++)
    {
        if (strcmp(ta->array[i].str.str,str.str)==0)
        {
            return ta->array[i];
        }
    }
    Token t;
    t.str=str;
    TokenType tt;
    SetString(&tt.name,"Identifier");
    tt.token='\0';
    t.type=tt;
    t.lineNumber=0;
    return t;
}

TokenArray Tokenize(FILE *f, TokenTypeArray *tokentypes)
{
    int line = 1;
    TokenArray ta;
    ta.size = 0;
    ta.capacity = 0;
    if (f == NULL)
        return ta;

    TokenType tt;
    SetString(&tt.name,"Identifier");
    if(!HasType(tokentypes,tt))
        AddTokenType(tokentypes, tt);
    FitTokenTypeArray(tokentypes);
    char c;
    while ((c = fgetc(f)) != EOF)
    {
        Token t;
        SetString(&t.str, "");
        if (c == '\n')
        {
            line += 1;
            continue;
        }
        if(c==' ') continue;
        t.type=GetTypeMatch(tokentypes,c);
        if (strcmp(t.type.name.str,"Identifier")!=0)
        {
            AddChar(&t.str, c);
        }
        else
        {
            bool endReserved=false;//Used to indicate if the end of the current Identifier is a reserved character
            while (c!= EOF && c!= ' ' && c != '\n')
            {
                if(c == '\n')
                {
                    line++;
                    break;
                }
                if(IsReservedToken(tokentypes, c))
                {
                    endReserved=true;
                    break;
                }
                AddChar(&t.str, c);
                c = fgetc(f);
            }
            if(endReserved){
                t.lineNumber = line;
                AddToken(&ta, t);
                char* tempStr=(char*) malloc(2*sizeof(char));
                tempStr[0]=c;
                tempStr[1]='\0';
                SetString(&t.str,tempStr);
                t.type=GetTypeMatch(tokentypes,c);
            }  
        }
        t.lineNumber = line;
        AddToken(&ta, t);
    }
    return ta;
}
void FreeTokenArray(TokenArray* ta){
    for(int i=0;i<ta->size;i++){
        if(ta->array[i].str.free)
            free(ta->array[i].str.str);
        if(ta->array[i].type.name.free)
            free(ta->array[i].type.name.str);
    }
    if(ta->size!=0)
        free(ta->array);
}  
void FreeTokenTypeArray(TokenTypeArray* ta){
    for(int i=0;i<ta->size;i++){
        if(ta->array[i].name.free)
            free(ta->array[i].name.str);
    }
    if(ta->size!=0)
        free(ta->array);
}
inline bool HasType(TokenTypeArray *ta, TokenType tt)
{
    for(int i=0;i<ta->size;i++){
        if (strcmp(ta->array[i].name.str,tt.name.str)==0&&ta->array[i].token==tt.token)
            return true;
    }
    return false;
}
inline bool HasTypeName(TokenTypeArray *ta, T_String str)
{
    for(int i=0;i<ta->size;i++){
        if (strcmp(ta->array[i].name.str,str.str)==0)
            return true;
    }
    return false;
}
inline bool HasToken(TokenArray *ta, Token tt)
{
    for(int i=0;i<ta->size;i++){
        if (strcmp(ta->array[i].str.str,tt.str.str)==0 &&
        strcmp(ta->array[i].type.name.str,tt.type.name.str)==0 &&
        ta->array[i].type.token==tt.type.token)
            return true;
    }
    return false;
}
inline bool HasTokenText(TokenArray *ta, T_String str)
{
    for(int i=0;i<ta->size;i++){
        if (strcmp(ta->array[i].str.str,str.str)==0)
            return true;
    }
    return false;
}
#endif
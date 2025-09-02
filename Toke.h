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
    T_String type; // Can be a custom type but everything else will be either Identifier,Space, or New line
    T_String token;
    size_t lineNumber; // Which line was it read from in the file
} Token;

typedef struct
{
    Token *array;
    size_t size;
    size_t capacity;
} TokenArray;

typedef struct{
    TokenArray reservedTokens;
    TokenArray tokens;
}Context;



Context* CreateConext();
void IncludeToken(Context* CTX,char* name,char* token);
TokenArray* TokenizeFile(Context* CTX,char* path);
void FreeContext(Context* CTX);


// T_String functions
//Adds a character to the end of the string
void AddChar(T_String *str, char c);
//Sets the string to the value passed
T_String GetString(char* str);
void SetString(T_String *str, char *value);
void PrintString(const T_String *str);
bool IsEqual(T_String a,T_String b); 

// Add token into token array,
void AddToken(TokenArray *ta, Token token);
//Add token type into token type array
void PrintToken(const Token *t);

//Shrinks string to size
void FitString(T_String *str);
//Shrinks token array to size
void FitTokenArray(TokenArray *ta);

void PrintTokenArray(const TokenArray *ta);
/**
 * @brief Function that gets the type match of a character
 * @returns TokenType with type (Identifier by default)
 */
Token GetTokenMatch(TokenArray*ta,T_String str);
/**
 * @brief Function to tokenize file
 * @returns TokenArray filled with tokens
 * */
TokenArray Tokenize(FILE *f, TokenArray *reservedTokens);

//Frees memory taken by the token array
void FreeTokenArray(TokenArray* ta);
//Tests if the name passed matches any of the token type names in the array
bool HasType(TokenArray *ta, char* typeName);
bool HasToken(TokenArray *ta, Token tt);
bool HasTokenText(TokenArray *ta, T_String str);
////////////////START OF IMPLEMENTATION//////////////////////

Context* CreateConext(){
    Context* CTX=(Context*)malloc(sizeof(Context));
    CTX->tokens.size=0;
    CTX->reservedTokens.size=0;
    return CTX;
}

void IncludeToken(Context* CTX,char *name, char *token)
{
    if(CTX==NULL)
    {
        printf("Context not initialized, use CreateConext function before adding tokens\n");
        return;
    }
    Token t;
    SetString(&t.type,name);
    SetString(&t.token,token);
    AddToken(&CTX->reservedTokens,t);
}

TokenArray* TokenizeFile(Context* CTX,char* path)
{
    if(CTX==NULL)
    {
        printf("Context not initialized, use CreateConext function before tokenizing\n");
        return NULL;
    }
    FILE* f=fopen(path,"r");
    CTX->tokens=Tokenize(f,&CTX->reservedTokens);
    if(f!=NULL)
        fclose(f);
    return &CTX->tokens;
}

void FreeContext(Context* CTX)
{
    if(CTX==NULL)
        return;
    FreeTokenArray(&CTX->tokens);
    free(CTX);
}

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
T_String GetString(char* str){
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

void PrintToken(const Token *t)
{
    printf("\n[Type]:");
    PrintString(&t->type);
    printf("\t[Line]:%ld\t[Text]:",t->lineNumber);
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
        if (IsEqual(ta->array[i].token,c) && (strcmp(ta->array[i].type.str, "Identifier") != 0))
        {
            return true;
        }
    }
    return false;
}
Token GetTypeMatch(TokenArray*ta,T_String c){
    for (int i = 0; i < ta->size; i++)
    {
        if ((strcmp(ta->array[i].type.str, "Identifier") != 0)&&IsEqual(ta->array[i].token,c))
        {
            return ta->array[i];
        }
    }
    Token tt;
    SetString(&tt.type,"Identifier");
    SetString(&tt.token,c.str);
    return tt;
}
Token GetTokenMatch(TokenArray*ta,T_String str){
        for (int i = 0; i < ta->size; i++)
    {
        if (strcmp(ta->array[i].token.str,str.str)==0)
        {
            return ta->array[i];
        }
    }
    Token t;
    t.token=str;
    SetString(&t.type,"Identifier");
    t.lineNumber=0;
    return t;
}

TokenArray Tokenize(FILE *f, TokenArray *reservedTokens)
{
    int line = 1;
    TokenArray ta;
    ta.size = 0;
    ta.capacity = 0;
    if (f == NULL)
        return ta;
    if(!HasType(reservedTokens,"Identifier"))
    {
        Token t;
        SetString(&t.type,"Identifier");
        SetString(&t.token,"\0");
        t.lineNumber=-1;
        AddToken(reservedTokens, t);
    }
    FitTokenArray(reservedTokens);
    char c;
    while ((c = fgetc(f)) != EOF)
    {
        Token t;
        SetString(&t.token, "");
        if (c == '\n')
        {
            line += 1;
            continue;
        }
        if(c==' ') continue;
        T_String tok;
        tok.size=0;
        AddChar(&tok,c);
        t=GetTypeMatch(reservedTokens,tok);
        if (strcmp(t.type.str,"Identifier")!=0)
        {
            t.token.size=0;
            AddChar(&t.token, c);
        }
        else
        {
            bool endReserved=false;//Used to indicate if the end of the current Identifier is a reserved character
            T_String s;
            s.size=0;
            Token endToken; //Is used in case the string ends with a reserved token
            SetString(&t.token,"");
            while (c!= EOF && c!= ' ' && c != '\n')
            {
                SetString(&s,"");
                AddChar(&s,c);
                if(c == '\n')
                {
                    line++;
                    break;
                }
                endToken=GetTypeMatch(reservedTokens, s);
                if(strcmp(endToken.type.str,"Identifier")!=0)
                {
                    endReserved=true;
                    break;
                }
                AddChar(&t.token, c);
                c = fgetc(f);
            }
            Token tempType = GetTypeMatch(reservedTokens,t.token);
            if(strcmp(tempType.type.str,"Identifier")!=0){
                t=tempType;
            }
            if(endReserved){
                t.lineNumber = line;
                AddToken(&ta, t);
                t=endToken;
            }  
        }
        t.lineNumber = line;
        AddToken(&ta, t);
    }
    return ta;
}
void FreeTokenArray(TokenArray* ta){
    for(int i=0;i<ta->size;i++){
        if(ta->array[i].token.free)
            free(ta->array[i].token.str);
        if(ta->array[i].type.free)
            free(ta->array[i].type.str);
    }
    if(ta->size!=0)
        free(ta->array);
}  

bool HasType(TokenArray *ta, char* typeName)
{
    for(int i=0;i<ta->size;i++){
        if (strcmp(ta->array[i].type.str,typeName))
            return true;
    }
    return false;
}
inline bool HasToken(TokenArray *ta, Token t)
{
    for(int i=0;i<ta->size;i++){
        if (strcmp(ta->array[i].type.str,t.type.str)==0 &&
            IsEqual(ta->array[i].token,t.token))
            return true;
    }
    return false;
}
inline bool HasTokenText(TokenArray *ta, T_String str)
{
    for(int i=0;i<ta->size;i++){
        if (strcmp(ta->array[i].token.str,str.str)==0)
            return true;
    }
    return false;
}
bool IsEqual(T_String a,T_String b){
    if(a.size!=b.size)
        return false;
    for(int i=0;i<a.size;i++)
        if(a.str[i]!=b.str[i])
            return false;
    return true;
}
#endif
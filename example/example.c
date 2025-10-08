#include "../Toke.h"

/**
 * @author Ahmed Abdalla
 * @brief This is an example trying to tokenize an HTML file using Toke
 */

// Custom validation function
bool IsGoober(T_String str);

int main(){
    Context* CTX=CreateConext(CTX);
    
    //Adding the tokens
    IncludeToken(CTX,"OPEN TAG","<");
    IncludeToken(CTX,"CLOSE TAG",">");
    IncludeToken(CTX,"SLASH","/");
    IncludeToken(CTX,"ASSIGNMENT","=");
    IncludeToken(CTX,"QUOTATION","\"");
    IncludeToken(CTX,"SEMICOLON",";");
    IncludeToken(CTX,"FLOAT","\0");
    //String matching example
    IncludeToken(CTX,"HEADER 1","h1");
    
    //Format matching example (Using built in IsFloat)
    IncludeFormatToken(CTX,"FLOAT",IsFloat);
    //Format matching example (Using built custom IsGoober)
    IncludeFormatToken(CTX,"GOOBER",IsGoober);
    
    //Getting the token array
    TokenArray* ta=TokenizeFile(CTX,"./index.html");
    PrintTokenArray(ta);

    // You can also get the results from the CTX variable
    printf("\n----------------------\nHello from context\n----------------------\n");
    PrintTokenArray(&CTX->tokens);

    FreeContext(CTX);
    return 0;
}
bool IsGoober(T_String str){
    return(strcmp(str.str,"goober")==0);
}
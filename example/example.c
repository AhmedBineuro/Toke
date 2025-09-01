#include "../Toke.h"

/**
 * @author Ahmed Abdalla
 * @brief This is an example trying to tokenize an HTML file using Toke
 */

int main(){
    Context* CTX=CreateConext(CTX);
    
    //Adding the tokens
    IncludeToken(CTX,"OPEN TAG","<");
    IncludeToken(CTX,"CLOSE TAG",">");
    IncludeToken(CTX,"SLASH","/");
    IncludeToken(CTX,"ASSIGNMENT","=");
    IncludeToken(CTX,"QUOTATION","\"");
    IncludeToken(CTX,"SEMICOLON",";");
    //String matching example
    IncludeToken(CTX,"HEADER 1","h1");
    
    //Getting the token array
    TokenArray* ta=TokenizeFile(CTX,"./index.html");
    PrintTokenArray(ta);

    // You can also get the results from the CTX variable
    printf("\n----------------------\nHello from context\n----------------------\n");
    PrintTokenArray(&CTX->tokens);

    FreeContext(CTX);
    return 0;
}
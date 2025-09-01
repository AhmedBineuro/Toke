#include "../Toke.h"

/**
 * @author Ahmed Abdalla
 * @brief This is an example trying to tokenize an HTML file using Toke
 */

int main(){

    InitContext();
    
    //Adding the tokens
    IncludeToken("OPEN TAG","<");
    IncludeToken("CLOSE TAG",">");
    IncludeToken("SLASH","/");
    IncludeToken("ASSIGNMENT","=");
    IncludeToken("QUOTATION","\"");
    IncludeToken("SEMICOLON",";");
    
    //Getting the token array
    TokenArray* ta=TokenizeFile("./index.html");
    PrintTokenArray(ta);

    // You can also get the results from the CTX variable
    printf("\n----------------------\nHello from context\n----------------------\n");
    PrintTokenArray(&CTX->tokens);

    FreeContext();
    return 0;
}
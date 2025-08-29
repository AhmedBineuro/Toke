#include "../Toke.h"

/**
 * @author Ahmed Abdalla
 * @brief This is an example trying to tokenize an HTML file using Toke
 */

int main(){
    //The token type array will hold the custom characters we are trying to tokenize 
    TokenTypeArray tta;
    TokenType tt;
    //Setting the size to 0 is important for proper array size expansion and to prevent undefined behaviour
    tta.size=0;
    
    //Here are the three operations to add a custom character to tokenize
    SetString(&tt.name,"OPEN TAG");
    tt.token='<'; //This assignment sets the character the Toke will compare against 
    AddTokenType(&tta,tt); //Safe add function
    
    SetString(&tt.name,"CLOSE TAG");
    tt.token='>';
    AddTokenType(&tta,tt);
    
    SetString(&tt.name,"SLASH");
    tt.token='/';
    AddTokenType(&tta,tt);
    
    SetString(&tt.name,"QUOTATION");
    tt.token='\"';
    AddTokenType(&tta,tt);
    
    SetString(&tt.name,"ASSIGNMENT");
    tt.token='=';
    AddTokenType(&tta,tt);
    
    SetString(&tt.name,"COLON");
    tt.token=':';
    AddTokenType(&tta,tt);
    
    SetString(&tt.name,"SEMICOLON");
    tt.token=';';
    AddTokenType(&tta,tt);
    
    
    FILE* f=fopen("index.html","r");

    //Pass the file pointer and a pointer to the token type array
    TokenArray ta=Tokenize(f,&tta);
    
    //This will print all the tokens and their types
    PrintTokenArray(&ta);
    
    // Calling these functions is essential for preventing memory leaks
    FreeTokenArray(&ta);
    FreeTokenTypeArray(&tta);
    
    if(f!=NULL)
        fclose(f);
    return 0;
}
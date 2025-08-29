# Toke
<img style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width:50%;
           height:auto;
           align-self=center;
           backdrop-filter:invert(100%);
           " 
src="./TokeImg.png" alt="Toke Icon" />

## What is Toke?

Toke is a small and simple tokenizer for text parsing purposes. The entire tokenizer is in the header file that you can easily include in your project.

## What can you do with Toke?

With Toke, you can tokenize special characters by adding them to a `TokenTypeArray` to specify their name and character. Toke will automatically add a `TokenType` called `Identifier` with a token of `'\0'`  that will encompass any other remaining text or characters. Toke automatically consumes any whitespace and new line characters but it will keep count of which token belonged to which line.

## How to use Toke?

Toke can be used with 5 simple steps:
1. Create a `TokenTypeArray` and set it's size to zero.
```C
TokenTypeArray tta;
tta.size=0;
```
**Setting the size to 0 is essential in preventing undefined behaviour when it comes to sizing up the array**

2.  Define the token types using a `TokenType` variable
```C
    TokenType tt;
    SetString(&tt.name,"OPEN TAG"); //Set the name of the token
    tt.token='<'; //This assignment sets the character the Toke will compare against 
```

3. Add the token to your `TokenTypeArray`
```C
    //The token type is passed by value so you can reuse the TokenType variable
    AddTokenType(&tta,tt); //Safe add function
```
4. Open your file and pass it to the `Tokenize` function along with the `TokenTypeArray` you created
```C
    FILE* f=fopen("index.html","r");
    
    //Pass the file pointer and a pointer to the token type array
    //The function return a token array with all the elements tokens in it!
    TokenArray ta=Tokenize(f,&tta);
```
5. Free the memory you used after you're done using the tokens and token types!
```
    FreeTokenArray(&ta);
    FreeTokenTypeArray(&tta);
```
**Check the example in **`example/example.c`** where I try to parse an html file for a more "real life" application**
# <em style="color:purple;">Toke</em>
<img style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width:50%;
           height:auto;
           align-self=center;
           backdrop-filter:invert(100%);
           " 
src="./TokeImg.png" alt="Toke Icon" />

## [Toke Introduction Video](https://www.youtube.com/watch?v=hjOIkNJAsTw&ab_channel=DynamicBineuro)

## What is <em style="color:purple;">Toke</em>?

Toke is a small and simple tokenizer for text parsing purposes. The entire tokenizer is in the header file that you can easily include in your project.

## What can you do with <em style="color:purple;">Toke</em>?

With Toke, you can tokenize special characters and strings by adding them to context using the `IncludeToken` and the `IncludeFormatToken` functions to specify their name and character/string *(more on FormatTokens in [What's new with toke?](#whats-new-with-toke))*. Toke will automatically add a `TokenType` called `NULLTOK` with a token of `'\0'`  that will encompass any other remaining text or characters. Then, Toke will automatically consume any whitespace and new line characters but it will keep count of which token belonged to which line.

## <em style="color:rgb(0, 176, 123);">What's new with Toke?</em>

- Format based tokens!
  - With format based tokens you can do a post processing typing operation to all remaining `NULLTOK` tokens
    - This means that the format matching happens **AFTER** parsing the whole file
  - To add a format based token, you have to provide the format validation function which takes in one `T_string` variable and returns a boolean value
- Format validation functions
  - Along with the format based tokens, there are a couple of pre-implemented validation functions like `IsInteger`, `IsHex`, `IsFloat`, and `IsAlphabetic`
  - These function can be used or viewed to understand how the format validation works or used directly for your custom tokens

## How to use Toke?

Toke can be used with 4 simple steps:
1. Create the context
```C
Context* CTX=CreateContext();
```
2.  Include your tokens
```C
    IncludeToken(CTX,"OPEN TAG","<");
    IncludeToken(CTX,"CLOSE TAG",">");
    IncludeToken(CTX,"SLASH","/");
    IncludeToken(CTX,"ASSIGNMENT","=");
    IncludeToken(CTX,"QUOTATION","\"");
    IncludeToken(CTX,"SEMICOLON",";");
    //String match example
    IncludeToken(CTX,"HEADER 1","h1");
```
3. Include any format based tokens
```C
  // IsHex is a function with the following signature
  // bool IsHex(T_string str)
  IncludeFormatToken(CTX,"Hex",IsHex)
```
4. Tokenize the file!
```C
    TokenArray* ta=TokenizeFile(CTX,"./index.html");
```
1. Free the context once you're done!
```C
    FreeContext(CTX);
```
**Check the example in **`example/example.c`** where I try to parse an html file for a more "real life" application**

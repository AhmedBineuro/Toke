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

With Toke, you can tokenize special characters and strings by adding them to context using the`IncludeToken` function to specify their name and character/string. Toke will automatically add a `TokenType` called `Identifier` with a token of `'\0'`  that will encompass any other remaining text or characters. Toke automatically consumes any whitespace and new line characters but it will keep count of which token belonged to which line.

## <em style="color:rgb(0, 176, 123);">What's new with Toke?</em>

A new and simplified context-based API containing only 4 functions! With this new API you won't have to worry about setting array sizes or remembering to initialize and free specific variables.  

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

3. Tokenize the file!
```C
    TokenArray* ta=TokenizeFile(CTX,"./index.html");
```
4. Free the context once you're done!
```C
    FreeContext(CTX);
```
**Check the example in **`example/example.c`** where I try to parse an html file for a more "real life" application**
          
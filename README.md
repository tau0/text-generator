# Text generator based on a Markov's chains.
This project shows you ways to generate text using simple Markov's chains.

## Features and limitations.
- Processes different one byte encodings correctly.
- Multibyte encodings aren't supported right now, because of a poor c++ support of a multibytes encodings.
- Support saving and loading a trained model to and from a binary file in google protocol buffers format.

## Texts desription.
There are some sample texts in the texts directory of this project which can be used as training sets for this text generator.
War and peace book is in Russian and saved using CP1251 encoding. Everything else is in English (ascii). 

## Some notes.
Often in this program you need to pass some text to the stdin of the program. Usually there is UTF8 default encoding of your terminal, but this program doesn't support multibyte encodings, and if you want to pass some text to the stdin in some other encoding better to use posix pipes from a coorectly saved file.

## Quick start.
- TBD: here will be description of script which should show you a capabilities of this program.
- Have a look at the generator/README.md for hints of using this program.




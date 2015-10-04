# Text generator based on a Markov's chains.
This project shows you ways to generate text using simple Markov's chains.

## Features and limitations.
- Processes different one byte encodings correctly.
- Multibyte encodings aren't supported right now, because of a poor c++ support of multibytes encodings.
- Support saving and loading a trained model to and from a binary file in google protocol buffers format.

## Some notes about texts.
There are some sample texts in the texts directory of this project which can be used as training sets for this text generator.
War and peace book is in Russian and saved using CP1251 encoding. Everything else is in English (ascii). 

## Some notes.
Often in this program you need to pass some text to the stdin of the program. Usually there is an UTF8 default encoding of your terminal, but this program doesn't support multibyte encodings, and if you want to pass some text to the stdin in some other encoding (such as CP1251) better to use posix pipes from a correctly saved file.

## Quick start.
After compilation place the binary file to the generators folder and try:
```$  ./download_and_train.sh```
it will download alice in wonderland and train markov chain of second order. 

You can have a look on the model:
```./generator/text_generator show ./model.protobin```

And generate some text:

    $ ./generator/text_generator generate 20 ./model.protobin
    there was
    there was a narrow escape said alice as she could remember them all ornamented with hearts next came an angry voice_the rabbits_pat 
- Don't forget to press ctrl+d in the end of an input.
- Have a look at the generator/README.md for hints of using this program.
- For additional parametrs you can take a look inside download_and_train.sh.



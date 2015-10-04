# Text generator.
This text generator supports three modes:
- Fit mode - in this mode program builds a Markov chain based on an input text and save it to a file.
- Show mode - in this mode program shows you a chain to the stdout.
- Generate mode - in this mode program takes a beginning of a text and try to continue it.

## Compilation prerequisites.
- Google Protocol buffers should be installed on your system if you want to compile this text generator.
- GNU make utility

## Compilation process.
You can just type

    $ make
in your terminal.

## Fitting model.
Some notes:
- Unfortunately now only 1 byte encodings are supported.
- You need to give a train text to the stdin of the program.

### Arguments.
```text_generator fit <markov chain order> <locale> <file where to save model>```
- You can check available on your system locales using ```locale -a```

### Example.
```$ cat ../texts/war_and_peace_russian_cp1251.txt | ./text_generator fit 2 ru_RU.CP1251 model.protobin```
- the train_text.txt saved using ru_RU.CP1251 locale.
- model.protobin the desired markov chain saved in protobuf binary format.
- 2 is an order of the desired markov chain.

## Generating text.
- Important! The text should be given to a stdin in the same encoding as the training text.
- Model should be valid markov chain in protobuf format.

### Arguments description
```./text_generator generate <number of words to generate> <path to a model protobuf file>```

### Example:
```$ cat ./input.txt | ./text_generator generate 100 model.protobin```
- input.txt is in the same encoding as the training text.
- 100 is the number of words to append to the the text.
- model.protobin is a markov chain model.

## Showing model.
You can have a look on a generated model using this command.
```./text_generator generate <path to model>```

### Example:
$ ./text_generator show model.protobin > model_human_representation.txt

### Notes
Your terminal could use utf8 encoding and you will see only garbage. You should open an output in some text editor with properly setted encoding. 






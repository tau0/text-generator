Compilation prerequisites:
You need installed protobuf library for compilation this project.

Compilation:
make

Fit the model:
- Unfortunately now only 1 byte encodings are supported.
- You need to give train text to the stdin of the program.
This is general description of the fields.
text_generator fit <markov chain order> <locale> <file to save model>

Example:
$ cat ./train_text.txt | ./text_generator fit 2 ru_RU.CP1251 model.protobin

Generate text:
- The text should be given to stdin in the same encoding as the training text.
After generate keyword should be path to the file with description of the model.
Model should be encoded as model.proto protobuf.

./text_generator generate <number of words to gen> <path to file>

Example:
$ cat ./prefix_text.txt | ./text_generator generate 100 model.protobin

Show model:
./text_generator generate <path to file>
Example:
$ cat ./prefix_text.txt | ./text_generator show model.protobin






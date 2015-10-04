all:
	cd ./src/protos && protoc model.proto --cpp_out cpp && cd ..
	g++ ./src/text_generator.cpp ./src/main.cpp ./src/words_compressor.cpp ./src/protos/cpp/model.pb.cc -Wall -std=c++11 -O2 -lprotobuf  -o text_generator

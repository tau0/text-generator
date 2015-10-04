all:
	cd ./protos && protoc model.proto --cpp_out cpp && cd ..
	g++ text_generator.cpp main.cpp words_compressor.cpp protos/cpp/model.pb.cc -Wall -std=c++11 -O2 -lprotobuf 

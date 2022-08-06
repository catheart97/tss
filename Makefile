#!make

.PHONY: all tl run ls clean

all:
	@mkdir -p build
	@clang++-8 -I include src/main.cpp -o build/tss -lpthread

tl:
	@mkdir -p build
	@clang++-8 -I include -I /usr/include/python3.6m src/main.cpp -o build/tss -lpthread -D THREAD_LOCAL

ls:
	@mkdir -p build
	@clang++-8 -I include -I /usr/include/python3.6m src/main.cpp -o build/tss -lpthread -D LOCAL_STORAGE

run:
	@./build/tss

clean:
	# @rm -rf build 
	@rm data/vectors1_out.txt
	@rm data/vectors2_out.txt
	@rm data/vectors3_out.txt
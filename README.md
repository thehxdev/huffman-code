# Huffman Code
This is an implementation of [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding) algorithm in C.

## Get the source code
This project depends on [sth](https://github.com/thehxdev/sth) library which is a base layer or my C projects.
So you must clone repository with submodules.
```bash
git clone --recurse-submodules --remote-submodules \
    --shallow-submodules --depth=1 https://github.com/thehxdev/huffman-code
```

## Build
Build the project with `cmake`.
```bash
mkdir -p build
cmake -B build -S .
cmake --build build
```
This will build the example executable that builds a Huffman tree and prints it.

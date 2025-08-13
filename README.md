# Huffman Code
This is an implementation of [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)
algorithm in C.

## Build
Build the project with just a C compiler.
```bash
cc -O3 -o hc build.c
```
Then to generate a huffman-tree from a file:
```bash
cd build
./hc '<file-path>'
```

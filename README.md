This is a cache simulator written in C. It takes in a memory trace file and prints the number of cache hits/misses, as well as memory reads/writes. It uses FIFO to update the Cache.

Please use the Makefile to compile the program.

To run the program, please use the following format: 

"./first cache-size block-size cache-policy associativity trace-file"

where:
A) cache-size =  total size of the cache in bytes. This number should be a power of 2.
B) block-size =  power of 2 integer that specifies the size of the cache block in bytes.
C) cache-policy =  please enter "fifo"
D) associativity = please enter either:
direct - to simulate a direct mapped cache.
assoc- to simulate a fully associative cache.
assoc:n - sto imulate an n way associative cache. n will be a power of 2.
E) trace-file is the name of the trace file.

Example: ./first 32 4 fifo assoc:2 trace2.txt

Two sample trace files have been provided in this repository.
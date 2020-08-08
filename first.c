#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MEMORYSIZE 48

//global values that keep track of cache and memory info
int cacheHit = 0;
int cacheMiss = 0;
int memoryRead = 0;
int memoryWrite = 0;

//will perform the read operation
void read(char* address) {

	printf("%s\n", address);

}

//will perform the write operation
void write(char* address) {
	
	printf("%s\n", address);

}

//if not found in read or write, will add to cache
void addToCache() {

}

//looks for memory address within the cache
void searchInCache() {

}

int main(int argc, char **argv)
{

	//check for invalid argument number
	if (argc != 6) {
		printf("\n Incorrect number of arguments\n");


	//check if inputs are in valid format
	//} else if ( argv[1] != || argv[2] || argv[3] || arg[v4] || argv[5]){
		
	} else {

	//get all the values of the inputs
	int cacheSize = atoi(argv[1]);
	int blockSize = atoi(argv[2]);
	char* cachePolicy = argv[3];
	char* associativity = argv[4];
	char* data = argv[5];

	//calculate the number of bits for offset
	int offsetBits = log(blockSize) / log(2);
	int numSets = 0;
	int setBits = 0;
	int numLines = 0;
	int tagBits = 0;

//check associativity type to get cache size
	if (strcmp(associativity, "direct") == 0){

		numSets = cacheSize / blockSize;
		setBits = log(numSets) / log(2);
		numLines = numSets;

	} else if (strcmp(associativity, "assoc") == 0) {

		numSets = 1;
		setBits = 0;
		numLines = cacheSize / blockSize;

	} else {

		//gets the number of ways the set is associative
		char* num = strtok(associativity, ":");
		num = strtok(NULL, "");
		int numWays = atoi(num);

		printf( "%d\n",numWays);

		numLines = cacheSize / blockSize;
		numSets = numLines / numWays;

	}

    tagBits = MEMORYSIZE - offsetBits - setBits;
	

    //open the file
    FILE *file = fopen(data, "r");

    //will be updated as we move along the file
    char accessType[10] = "";
    char memoryAddress[15] = "";

    //read the file for its contents
    while (fscanf(file, "%s%s\n", accessType, memoryAddress) != EOF) {

		//makes sure the end of the file has not been reached yet
		if (strcmp(accessType, "#eof") != 0){
        
        //check if it is a read or write operation
		if (strcmp(accessType, "R") == 0) {

			read(memoryAddress);

		} else {

		    write(memoryAddress);

		}

		//checks the cache policy
		if (strcmp(cachePolicy, "fifo")) {

		}

		}

    }

	//print out output
	printf("%s%d\n", "Memory reads: ", memoryRead);
	printf("%s%d\n", "Memory writes: ", memoryWrite);
	printf("%s%d\n", "Cache hits: ", cacheHit);
	printf("%s%d\n", "Cache misses: ", cacheMiss);



    

    //close the file
    fclose(file);


	}
	

	//return to make sure main went smoothly
	return 0;
}
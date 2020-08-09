#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

//we are given that it is a 48 bit address
#define MEMORYSIZE 48

//will represent a specific line in the cache
typedef struct cacheline
{
	//will represent the valid bit and the tag bits within a cache line
	unsigned short int validBit;
	unsigned long int tagBits;
	//dont need to keep track of when this line was added to the set
	//the higher the number, the later it was added
	int iteration;

} CacheLine;

//global values that keep track of cache and memory info
int cacheHit = 0;
int cacheMiss = 0;
int memoryRead = 0;
int memoryWrite = 0;

//will perform the read operation
void read(char *address, CacheLine **Cache)
{
	//printf("%s\n", address);
}

//will perform the write operation
void write(char *address, CacheLine **Cache)
{
	//printf("%s\n", address);
}

//if not found in read or write, will add to cache
void addToCache(CacheLine **Cache)
{
}

//looks for memory address within the cache
void searchInCache(CacheLine **Cache)
{
}

//initializes the valid bit within the cache as 0
void initializeCache(CacheLine **Cache, int x, int y)
{

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			(Cache[i][j]).validBit = 0;
		}
	}
}

//prints the valid bits within the Cache (used for testing)
void printCache(CacheLine **Cache, int x, int y)
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			printf("%d\t", (Cache[i][j]).validBit);
		}
		printf("\n");
	}
}

//returns 1 if not power of 2 otherwise returns 0
int notPowerOf2(int num) {

	//iterate till you get 1 or 0
	while (num > 1) {
		num = num % 2;
	}
	
	return num;

}

//prints out the output
void printOutput()
{
	//print out output
	printf("%s%d\n", "Memory reads: ", memoryRead);
	printf("%s%d\n", "Memory writes: ", memoryWrite);
	printf("%s%d\n", "Cache hits: ", cacheHit);
	printf("%s%d\n", "Cache misses: ", cacheMiss);
}

int main(int argc, char **argv)
{

	//check for invalid argument number
	if (argc != 6)
	{
		printf("Incorrect number of arguments\n");
	}
	//check if inputs are in valid format
	else if (notPowerOf2(atoi(argv[1]))|| notPowerOf2(atoi(argv[2]))) {

		printf("Error: Inputs must be powers of 2\n");
	}
	else if ((strcmp(argv[3], "fifo") != 0 && (strcmp(argv[3], "lru") != 0)))
	{
		printf("Error: type must be lru or fifo\n");
	}
	else if ((strcmp(argv[4], "direct") != 0) && (strcmp(argv[4], "assoc") != 0)
		&& (strstr(argv[4], "assoc:") == NULL))
	{
		printf("Error: associativity must be direct, assoc, or assoc:n\n");
	}
	else
	{

		//get all the values of the inputs
		int cacheSize = atoi(argv[1]);
		int blockSize = atoi(argv[2]);
		char *cachePolicy = argv[3];
		char *associativity = argv[4];
		char *data = argv[5];

		//calculate the number of bits for offset
		int offsetBits = log(blockSize) / log(2);
		int numSets = 0;
		int setBits = 0;
		int numLines = 0;
		int tagBits = 0;

		//check associativity type to get the proper information about the cache size
		if (strcmp(associativity, "direct") == 0)
		{

			numSets = cacheSize / blockSize;
			setBits = log(numSets) / log(2);
			numLines = numSets;
		}
		else if (strcmp(associativity, "assoc") == 0)
		{

			numSets = 1;
			setBits = 0;
			numLines = cacheSize / blockSize;
		}
		else
		{

			//gets the number of ways the set is associative
			char *num = strtok(associativity, ":");
			num = strtok(NULL, "");
			int numWays = atoi(num);

			numLines = cacheSize / blockSize;
			numSets = numLines / numWays;
			setBits = log(numSets) / log(2);
		}

		tagBits = MEMORYSIZE - offsetBits - setBits;
		/*
		printf("%s%d\n", "offsetBits: ", offsetBits);
		printf("%s%d\n", "numSets: ", numSets);
		printf("%s%d\n", "setBits: ", setBits);
		printf("%s%d\n", "numLines: ", numLines);
		printf("%s%d\n", "tag bits: ", tagBits);
		*/

		//CREATE CACHE (2-D array with sets and lines as rows/columns)
		CacheLine **Cache = (CacheLine **)malloc((sizeof(CacheLine *)) * numSets);
		//allocate space on Cache
		for (int i = 0; i < numSets; i++)
		{
			Cache[i] = (CacheLine *)malloc(sizeof(CacheLine) * (numLines / numSets));
		}

		//initializes the cache (makes all valid bits = 0)
		initializeCache(Cache, numSets, numLines / numSets);

		//printCache(Cache, numSets, numLines / numSets);

		//open the file
		FILE *file = fopen(data, "r");

		//will be updated as we move along the file
		char accessType[10] = "";
		char memoryAddress[15] = "";

		//read the file for its contents
		while (fscanf(file, "%s%s\n", accessType, memoryAddress) != EOF)
		{

			//makes sure the end of the file has not been reached yet
			if (strcmp(accessType, "#eof") != 0)
			{
				//only runs in cases of fifo
				if (strcmp(cachePolicy, "fifo") == 0)
				{
					//check if it is a read or write operation
					if (strcmp(accessType, "R") == 0)
					{

						read(memoryAddress, Cache);
					}
					else
					{

						write(memoryAddress, Cache);
					}
				}
			}
		}

		printOutput();

		//FREE CACHE From Heap
		for (int i = 0; i < numSets; i++)
		{
			free(Cache[i]);
		}
		free(Cache);

		//close the file
		fclose(file);
	}

	//return to make sure main went smoothly
	return 0;
}
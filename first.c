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
	int validBit;
	int bitsOfTag;
	//dont need to keep track of when this line was added to the set
	//the higher the number, the later it was added
	int iteration;

} CacheLine;

//global values that keep track of cache and memory info
int cacheHit = 0;
int cacheMiss = 0;
int memoryRead = 0;
int memoryWrite = 0;

//Information about the cache
int offsetBits = 0;
int numSets = 0;
int setBits = 0;
int numLines = 0;
int tagBits = 0;
int linesPerSet = 0;
//will help keep track of FIFO
int operationNumber = 0;

//will perform the operation needed
void performOperation(char *address, char instruction, CacheLine **Cache)
{
	//initialize string to empty
	char addressInBinary[52] = "";

	char *middleAdd = strtok(address, "x");
	middleAdd = strtok(NULL, "");

	char newAddress[25] = "";

	//append leading zeros
	for (int i = strlen(middleAdd); i < (MEMORYSIZE / 4); i++)
	{
		strcat(newAddress, "0");
	}

	//add the final address
	strcat(newAddress, middleAdd);

	//looping through each digit in hex
	int addressLength = strlen(newAddress);

	for (int i = 0; i < addressLength; i++)
	{

		//converts hexadecimal to decimal
		if (newAddress[i] == '0')
		{
			strcat(addressInBinary, "0000");
		}
		else if (newAddress[i] == '1')
		{
			strcat(addressInBinary, "0001");
		}
		else if (newAddress[i] == '2')
		{
			strcat(addressInBinary, "0010");
		}
		else if (newAddress[i] == '3')
		{
			strcat(addressInBinary, "0011");
		}
		else if (newAddress[i] == '4')
		{
			strcat(addressInBinary, "0100");
		}
		else if (newAddress[i] == '5')
		{
			strcat(addressInBinary, "0101");
		}
		else if (newAddress[i] == '6')
		{
			strcat(addressInBinary, "0110");
		}
		else if (newAddress[i] == '7')
		{
			strcat(addressInBinary, "0111");
		}
		else if (newAddress[i] == '8')
		{
			strcat(addressInBinary, "1000");
		}
		else if (newAddress[i] == '9')
		{

			strcat(addressInBinary, "1001");
		}
		else if (newAddress[i] == 'a' || newAddress[i] == 'A')
		{
			strcat(addressInBinary, "1010");
		}
		else if (newAddress[i] == 'b' || newAddress[i] == 'B')
		{
			strcat(addressInBinary, "1011");
		}
		else if (newAddress[i] == 'c' || newAddress[i] == 'C')
		{
			strcat(addressInBinary, "1100");
		}
		else if (newAddress[i] == 'd' || newAddress[i] == 'D')
		{
			strcat(addressInBinary, "1101");
		}
		else if (newAddress[i] == 'e' || newAddress[i] == 'E')
		{
			strcat(addressInBinary, "1110");
		}
		else if (newAddress[i] == 'f' || newAddress[i] == 'F')
		{
			strcat(addressInBinary, "1111");
		}
		else
		{
			printf("error in input");
		}
	}

	char tag[50] = "";
	char setIndex[50] = "";

	//find the tag bits
	for (int i = 0; i < tagBits; i++)
	{
		strncat(tag, &(addressInBinary[i]), 1);
	}
	//find the set bits
	for (int i = tagBits; i < (tagBits + setBits); i++)
	{
		strncat(setIndex, &(addressInBinary[i]), 1);
	}

	int tagInt = 0;
	int setInt = 0;

	//convert binary to decimal
	for (int i = 0; i < strlen(tag); i++)
	{
		int num = tag[i] == '1';
		tagInt += (pow(2, (strlen(tag) - 1) - i)) * num;
	}
	for (int i = 0; i < strlen(setIndex); i++)
	{
		int num = setIndex[i] == '1';
		setInt += (pow(2, (strlen(setIndex) - 1) - i)) * num;
	}

	//printf("%d\t%d\t%d\n", tagInt, setInt, operationNumber);

	//if read operation
	if (instruction == 'R')
	{
		// Iterate through Cache[setInt][i] to find the value
		for (int i = 0; i < linesPerSet; i++)
		{
			//printf("%d\t",(Cache[setInt][i]).validBit);
			//if valid bit is on and found in Cache
			if (((Cache[setInt][i]).validBit == 1) && ((Cache[setInt][i]).bitsOfTag == tagInt))
			{
				cacheHit++;
				//increment the operation number
				operationNumber++;
				return;
			}
		}

		//if not found in cache, then add to cache

		//first check if there is space on cache
		for (int i = 0; i < linesPerSet; i++)
		{
			//check if any valid bit is off
			if ((Cache[setInt][i]).validBit == 0)
			{
				cacheMiss++;
				memoryRead++;
				(Cache[setInt][i]).validBit = 1;
				(Cache[setInt][i]).bitsOfTag = tagInt;
				(Cache[setInt][i]).iteration = operationNumber;
				operationNumber++;
				return;
			}
		}

		//if there is not space on cache, it means we need to evict
		//first check if there is space on cache
		int oldestOnCache = operationNumber + 1;
		int indexOfOldest = -1;
		for (int i = 0; i < linesPerSet; i++)
		{
			//if operation occurred earlier
			if ((Cache[setInt][i]).iteration < oldestOnCache)
			{
				//printf("here\n");
				oldestOnCache = (Cache[setInt][i]).iteration;
				indexOfOldest = i;
			}
		}

		if (indexOfOldest != -1)
		{
			//after finding the oldest, replace with new value
			(Cache[setInt][indexOfOldest]).bitsOfTag = tagInt;
			(Cache[setInt][indexOfOldest]).iteration = operationNumber;
			cacheMiss++;
			memoryRead++;
		}
		operationNumber++;
		return;

	} //if write operation
	else
	{
		// Iterate through Cache[setInt][i] to find the value
		for (int i = 0; i < linesPerSet; i++)
		{

			//if valid bit is on and found in Cache
			if (((Cache[setInt][i]).validBit == 1) && ((Cache[setInt][i]).bitsOfTag == tagInt))
			{
				cacheHit++;
				memoryWrite++;
				operationNumber++;
				return;
			}
		}

		//if cache miss

		//first check if there is space on cache
		for (int i = 0; i < linesPerSet; i++)
		{
			//check if valid bit is off
			if ((Cache[setInt][i]).validBit == 0)
			{
				cacheMiss++;
				memoryRead++;
				memoryWrite++;
				(Cache[setInt][i]).validBit = 1;
				(Cache[setInt][i]).bitsOfTag = tagInt;
				(Cache[setInt][i]).iteration = operationNumber;
				operationNumber++;
				return;
			}
		}

		//if there is not space on cache, it means we need to evict
		//first check if there is space on cache
		int oldestOnCache = operationNumber + 1;
		int indexOfOldest = -1;
		for (int i = 0; i < linesPerSet; i++)
		{
			//if operation occurred earlier
			if ((Cache[setInt][i]).iteration < oldestOnCache)
			{
				oldestOnCache = (Cache[setInt][i]).iteration;
				indexOfOldest = i;
			}
		}

		if (indexOfOldest != -1)
		{
			//after finding the oldest, replace with new value
			(Cache[setInt][indexOfOldest]).bitsOfTag = tagInt;
			(Cache[setInt][indexOfOldest]).iteration = operationNumber;
			cacheMiss++;
			memoryRead++;
			memoryWrite++;
		}
		operationNumber++;
		return;
	}
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
			(Cache[i][j]).bitsOfTag = 0;
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
			printf("%s%d%s%d\t", "Valid Bit: ", (Cache[i][j]).validBit, "Tag bits: ", (Cache[i][j]).bitsOfTag);
		}
		printf("\n");
	}
	printf("\n");

}

//returns 1 if not power of 2 otherwise returns 0
int notPowerOf2(int num)
{

	//edge case when equal input is 2^0
	if (num == 1) {
		return 0;
	}

	//iterate till you get 1 or 0
	while (num > 1)
	{
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
	else if (notPowerOf2(atoi(argv[1])) || notPowerOf2(atoi(argv[2])))
	{

		printf("Error: Inputs must be powers of 2\n");
	}
	else if ((strcmp(argv[3], "fifo") != 0 && (strcmp(argv[3], "lru") != 0)))
	{
		printf("Error: type must be lru or fifo\n");
	}
	else if ((strcmp(argv[4], "direct") != 0) && (strcmp(argv[4], "assoc") != 0) && (strstr(argv[4], "assoc:") == NULL))
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
		offsetBits = log(blockSize) / log(2);

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
		linesPerSet = numLines / numSets;
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
			Cache[i] = (CacheLine *)malloc(sizeof(CacheLine) * linesPerSet);
		}

		//initializes the cache (makes all valid bits = 0)
		initializeCache(Cache, numSets, linesPerSet);

		//printCache(Cache, numSets, linesPerSet);

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
						performOperation(memoryAddress, 'R', Cache);

					}
					else
					{

						performOperation(memoryAddress, 'W', Cache);

					}
				}
			}
		}

		//printCache(Cache, numSets, linesPerSet);

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
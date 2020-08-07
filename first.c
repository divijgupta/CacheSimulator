#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

//ADD COMPILATION FLAGS
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
	//open the file
	FILE *file = fopen(data, "r");

	//will be updated as we move along the file
	char* accessType = "";
	char* memoryAddress;
	//read the file for its contents
	while (strcmp(accessType, "#eof") != 0) {

		//read the first value
		fscanf(file, "%s", accessType);

		//check if the value is #eof
		if (strcmp(accessType, "#eof") != 0) {
			break;
		}

		//read the second value
		fscanf(file, "%s\n", memoryAddress);

		printf("%s\t",accessType);
		printf("%s\n",memoryAddress);

	}


	

	//close the file
	fclose(file);



	}
	

	//return to make sure main went smoothly
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 255
#define DEFAULT_FILENAME "data1.txt"

/*
Extra credit Lab
Due 12/11/2015 11:59pm

Complete the below functions printRowSums and printColSums.

The program reads in integers from a provided data file 
and stores the ints in a 2D array. The program prints 
the # of rows and columns of the 2D array and then calls 
printRowSums and printColSums. 

printRowSums: iterates through the 2D array row-by-row 
and prints the sum of each row

printColSums: iterates through the 2D array column-by-column 
and prints the sum of each column

Note: this program accepts a data file name as a command line 
argument. If no file name is given on the command line, it will
use the default file name "data1.txt"

Note 2: gcc may need to be told explicitly compile using the 
c99 standard. if so, use -std=c99
For example:
gcc -Wall -std=c99 ec_lab.c 

Below is example output of the program processing data1.txt

# rows: 4
# cols: 4
Sum of rows:
	Row 0 sum is 10
	Row 1 sum is 26
	Row 2 sum is 42
	Row 3 sum is 58
Sum of columns:
	Column 0 sum is 28
	Column 1 sum is 32
	Column 2 sum is 36
	Column 3 sum is 40
*/

//prototypes (you don't need to look at nor understand these)
void removeNewLineFromString(char str[]);
int readFileData(int rows, int cols, int arr[][cols], FILE * f);
int readFileHeader(int * rows, int * cols, FILE * f);
int readFileData(int rows, int cols, int arr[][cols], FILE * f);

//complete this function
void printRowSums(int rows, int cols, int arr[][cols]) {
	printf("Sum of rows:\n");
	int i, j;
	int sum = 0;
	for(i = 0; i < rows; i++) {
		for(j = 0; j < cols; j++) {
		sum += arr[i][j];
	}
	printf("Row %d sum is %d\n", i, sum);
	sum = 0;
	}
}

//complete this function
void printColSums(int rows, int cols, int arr[][cols]) {
	printf("Sum of columns:\n");
	int i, j;
	int sum = 0;
	for(i = 0; i < cols; i++) {
		for(j = 0; j < rows; j++) {
		sum += arr[j][i];
	}
	printf("Column %d sum is %d\n", i, sum);
	sum = 0;
	}
}

//*** main ***
int main(int argc, char * argv[]) {
	//2nd arg is optional data file name
	//if missing then use DEFAULT_FILENAME
	FILE * file = NULL;
	if(argc < 2) {
		file = fopen(DEFAULT_FILENAME, "r");
	} else {
		file = fopen(argv[1], "r");
	}
	//open the data file (make sure to print an error message and exit if file not found)
	//always check for NULL 
	if(file == NULL) {
		printf("Error: fopen returned NULL!\n");
		exit(EXIT_FAILURE);
	}
	
	int numRows, numCols;
	//read the # of rows and cols from file
	if(readFileHeader(&numRows, &numCols, file) == 0) {
		printf("Unable to read data from file!\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	
	//create enough space for the data
	int data[numRows][numCols];
	if(readFileData(numRows, numCols, data, file) == 0) {
		printf("Unable to read data from file!\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	
	printf("# rows: %d\n# cols: %d\n", numRows, numCols);

	//print sum of rows
	printRowSums(numRows, numCols, data);
	
	//print sum of columns
	printColSums(numRows, numCols, data);
	
	//always call fclose with every fopen!
	fclose(file);
}

//returns 0 if error
//else returns 1
int readFileData(int rows, int cols, int arr[][cols], FILE * f) {
	//need space for newline and null char
	char * buffer = calloc(BUFFER_SIZE + 2, sizeof(char));
	
	if(buffer == NULL) {
		printf("Could not allocate buffer in readFileData!\n");
		return 0;
	}
	
	int numElements = rows * cols;
	int i = 0, numConversions;
	//read in enough data to fill the array
	while(!feof(f)) {
		//read line
		if(fgets(buffer, BUFFER_SIZE + 2, f) != NULL) {
			//ignore any line that starts with #
			if(buffer[0] == '#') {
				continue;
			}
			
			//convert as many ints from buffer as possible
			int offset = 0;
			char * temp = buffer;
			do {
				numConversions = sscanf(temp, "%d%n", &arr[i / cols][i % cols], &offset);
				if(numConversions > -1) {
					temp += offset;
					i++;
				}
			} while(i < numElements && numConversions == 1);
			
		}
	}
	free(buffer);
	//if we did not read enough ints from file then abort the whole thing
	if(i < numElements - 1)
		return 0;
	return 1;
}

//returns 0 if error
//else returns 1
int readFileHeader(int * rows, int * cols, FILE * f) {
	//need space for newline and null char
	char buffer[BUFFER_SIZE + 2] = "";

	int numConversions;
	
	//init rows and cols
	*rows = *cols = 0;
	
	//read until we find a line that is not a comment (starts with #)
	//and then get # rows and # cols
	while(!feof(f)) {
		//read line
		if(fgets(buffer, BUFFER_SIZE + 2, f) != NULL) {
			//ignore any line that starts with #
			if(buffer[0] == '#') {
				continue;
			}

			removeNewLineFromString(buffer);

			//first non-comment is # rows and # cols
			numConversions = sscanf(buffer, "%d %d", rows, cols);
			if(numConversions != 2)
				return 0;
			
			//done with this loop
			break;
		}
	}
	
	//abort if file does not have good row and col meta data
	if(*rows == 0 || *cols == 0)
		return 0;
	
	return 1;
}

//removeNewLineFromString : scan through string and set \n char to \0
//parameters: char [] str : the string to have newline set to null char
//returns : void
void removeNewLineFromString(char str[]) {
	//iterate through str until we find \0 or \n
	int i = 0;
	while(str[i] != '\0') {
		//replace \n with \0
		if(str[i] == '\n') {
			str[i] = '\0';
			return;
		}
		i++;
	}
}

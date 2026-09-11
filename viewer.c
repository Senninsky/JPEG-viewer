#include <stdio.h>
#include <stdlib.h>
#include "viewer.h"
#include "matrix.h"

int main(int argc, char* argv[]) {

    	if (argc != 2) {
        	printf("Error: Not the correct amount of arguments\n");
        	return -1;
    	}

    	unsigned char* buffer = openFile(argv[1]);
	if (buffer == NULL) {
		printf("The opening of file: '%s' failed", argv[1]);
		return -1;
	}

	long fileSize = getFileSize(argv[1]);

	// Visual test
	int amountToPrint = 10;
	printf("Printing %i first bytes:\n", amountToPrint);
	for (int i = 0; i < amountToPrint; i++) {
		printf("%02x\n", (unsigned char)buffer[i]);
	}
	printf("\n");

	// Checking JPG format
	if ((buffer[0] != 0xff) || (buffer[1] != 0xd8)) {
		printf("File format is probably not a correct JPG");
		return -1;
	}

	// Checking for markers
	printf("All found markers:\n");
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] != 0x00)) {
			printf("%02x %02x\n", buffer[i], buffer[i+1]);
		}
	}
	printf("\n");

	// Get pointer to char*'s where all DQT's lie
	DQT65** DQTs = getDQTs(buffer, fileSize);

	if (DQTs == NULL) {
		printf("Getting the DQTs failed!");
		return -1;
	}

	// Giving up the allocated buffer-space again
    	free(buffer);

    	return 0;
}

unsigned char* openFile(const char* relativePath) {
    	// Create filehandle
    	FILE* file = fopen(relativePath, "rb");
	if (file == NULL) {
		return NULL;
	}
	
	// Find file length
	fseek(file, 0, SEEK_END);
    	long size = ftell(file);
	rewind(file);

    	// Saving file content in buffer
    	unsigned char* buffer = malloc(size * sizeof(char));

	// Putting filecontent in buffer location
	fread(buffer, sizeof(char), size, file);

    	// Close file handle
    	fclose(file);

    	return buffer;
}

long getFileSize(const char* relativePath) {
	// Create filehandle
    	FILE* file = fopen(relativePath, "rb");
	
	// Find file length
	fseek(file, 0, SEEK_END);
    	long size = ftell(file);
	rewind(file);

	// Close file handle
	fclose(file);

	return size;
}

void zigzagToSquare(Matrix* array, Matrix* square) {
	if ((array->width != (square->width)*(square->width)) || (array->height != 1) || (square->width != square->height) || (array == NULL) || (square == NULL)) {
		return;
	}
	
	int index = 0;
	for (int diagonal = 0; diagonal < (square->width)+(square->height) - 1; diagonal++) {
		// Find row range
		int firstRow = (diagonal < (square->width)) ? 0 : diagonal - square->width + 1;
		int lastRow = (diagonal < (square->width)) ? diagonal : square->height - 1;
		
		// Go through diagonals
		for (int j = firstRow; j <= lastRow; j++) {
			int row;
			
			
			if (diagonal % 2 == 0) {
				row = lastRow - (j - firstRow); // Move upwards
			} else {
				row = j; // Move downwards
			}
			
			int column = diagonal - row;

			// Now we have (row, column) where we need to put array[i] at the place square[row, column]
			insertMatrixValue(square, row, column, array->values[index]);
			index++;
		}
	}
}

DQT65** getDQTs(unsigned char* buffer, long fileSize) {
	int amount = 0;

	printf("Checking lengths of 'ff db' markers (DQTs)\n");
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xdb)) {
			size_t length = ((size_t)buffer[i+2]<<8) + buffer[i+3];
			printf("Length: %d\n", length);
			if (length != 67) {
				return NULL;
			}
			amount++;
		}
	}

	// Allocate the amount of size of the typedef'ed stuct
	printf("Amount of struct* to place in DQTs buffer: %i\n", amount);
	DQT65** DQTs = malloc(amount * sizeof(DQT65*));

	// Fill in  those structs
	int index = 0;
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xdb)) {
			DQT65* dqt65 = malloc(sizeof(DQT65));
			dqt65->precision = buffer[i+4] >> 4;
			dqt65->tableID = buffer[i+4] & 0x0F;
			
			Matrix* charArray = initMatrix(64, 1, &(buffer[i+4+1]));
			printMatrix(charArray);

			Matrix* dqtValues = initSparseMatrix(8, 8);
			
			// Put in the values via zigzag pattern (first bissectrice/start: (0,0), (0,1), (1,0), (2,0), (1,1), ...)
			zigzagToSquare(charArray, dqtValues);
			printf("Filled in square matrix via zigzag\n");
			printMatrix(dqtValues);
			dqt65->values = dqtValues;
			
			// Don't need to free charArray->values because they are contained in the malloced file-bytes
			free(charArray);

			DQTs[index] = dqt65;
			index++;
		}
	}

	return DQTs;
}
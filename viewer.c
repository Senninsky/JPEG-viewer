#include <stdio.h>
#include <stdlib.h>
#include "viewer.h"
#include "matrix.h"
#include "dqt.h"
#include "sof.h"

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

	// Get pointer to DQT65* array
	int DQTamount;
	DQT65** DQTs = getDQTs(buffer, fileSize, &DQTamount);
	printf("Amount of DQTs to free later: %i\n\n", 	DQTamount);
	if (DQTs == NULL) {
		printf("Getting the DQTs failed!");
		return -1;
	}

	// Get pointer to SOF0 struct
	SOF* sof = getSOF(buffer, fileSize);
	if (sof == NULL) {
		printf("Getting the SOF failed!");
		return -1;
	}

	// Giving up the allocated memory-space again
    	free(buffer);

	for (int i = 0; i < DQTamount; i++) {
		freeMatrix(DQTs[i]->values);
		free(DQTs[i]);
	}
	free(DQTs);

	for (int i = 0; i < sof->N; i++) {
		free(sof->components[i]);
		printf("Freed sof component");
	}
	free(sof);
	
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
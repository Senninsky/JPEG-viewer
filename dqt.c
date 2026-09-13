#include "dqt.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "viewer.h"

DQT65** getDQTs(unsigned char* buffer, long fileSize) {
	int amount = 0;

	printf("Checking lengths of 'ff db' markers (DQTs)\n");
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xdb)) {
			size_t length = ((size_t)buffer[i+2]<<8) + buffer[i+3];
			printf("Length: %d\n", length);
			// 2 Bytes length + x*(65 bytes payload = 1 byte (2 time 4 bits) + 64 bytes values)
			if (((length-2) % 65) != 0) {
				return NULL;
			}
			amount += floor((double)(length-2) / (double)65.0f);
		}
	}

	// Allocate the amount of size of the typedef'ed stuct
	printf("Amount of struct* to place in DQTs buffer: %i\n", amount);
	if (amount == 0) {
		return NULL;
	}
	DQT65** DQTs = malloc(amount * sizeof(DQT65*));

	// Fill in  those structs
	int index = 0;
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xdb)) {
			for (int j = 0; j < amount; j++) {
				DQT65* dqt65 = malloc(sizeof(DQT65));
				// Cutting up the information byte
				dqt65->precision = buffer[(i+(j*65))+4] >> 4;
				dqt65->tableID = buffer[(i+(j*65))+4] & 0x0F;
			
				Matrix* charArray = initMatrix(64, 1, &(buffer[(i+(j*65))+4+1]));
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
	}
	printf("\n");

	return DQTs;
}
#include "sos.h"
#include <stdio.h>
#include <stdlib.h>

SOS** getSOSs(unsigned char* buffer, long fileSize, int* amount_) {
	int amount = 0;
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xda)) {
			amount++;
		}
	}
	
	printf("This amount of SOSs found: %d", amount);
	*amount_ = amount;
	SOS** SOSs = malloc(amount * sizeof(SOS*));
	
	int index = 0;
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xda)) {
			SOS* sos = malloc(sizeof(SOS));
			sos->length = ((int)buffer[i+2]<<8) + buffer[i+3];
			printf("SOS length: %d\n", sos->length);
			SOSs[index] = sos;
			index++;
		}
	}
	printf("\n");
	
	return SOSs;
}
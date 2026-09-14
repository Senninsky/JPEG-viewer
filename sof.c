#include "sof.h"
#include <stdio.h>
#include <stdlib.h>

SOF* getSOF(unsigned char* buffer, long fileSize) {
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && ((buffer[i+1] == 0xc0) || (buffer[i+1] == 0xc2))) {
			printf("SOF found! (You should see this message only once)\n");
			SOF* sof = malloc(sizeof(SOF));
			size_t length = ((size_t)buffer[i+2]<<8) + buffer[i+3];
			sof->precision = buffer[i+4];
			sof->imageHeight = ((int)buffer[i+5]<<8) + buffer[i+6];
			printf("Image Height found in SOF = %i\n", sof->imageHeight);
			sof->imageWidth = ((int)buffer[i+7]<<8) + buffer[i+8];
			printf("Image width found in SOF = %i\n", sof->imageWidth);
			sof->N = (int)buffer[i+9];
			printf("Amount of SOF-components to be added: %i\n", sof->N);
			
			sof->components = malloc(sof->N * sizeof(Component*));
			for (int j = 0; j < sof->N; j++) {
				Component* comp = malloc(sizeof(Component));
				comp->ID = buffer[i+10 + 3*j];
				comp->samplingFactor = buffer[i+10 + 3*j + 1]; // upper 4 bits horizontal - lower 4 bits vertical
				comp->quantTableID = buffer[i+10 + 3*j + 2];
				sof->components[j] = comp;
			}
			
			printf("\n");
			
			return sof;
		}
	}
	
	return NULL;
}
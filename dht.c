#include <stdio.h>
#include "dht.h"

DHT** getDHTs(unsigned char* buffer, long fileSize, int* amount_) {
	// TODO: first get the amount, then malloc, then fill in (see DQT)
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xc4)) {
			
		}
	}
	return NULL;
}
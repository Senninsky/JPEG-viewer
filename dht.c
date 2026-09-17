#include <stdio.h>
#include <stdlib.h>
#include "dht.h"

DHT** getDHTs(unsigned char* buffer, long fileSize, int* amount_) {
	// TODO: first get the amount, then malloc, then fill in (see DQT)
	int amount = 0;
	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xc4)) {
			amount++;
		}
	}
	*amount_ = amount;

	DHT** DHTs = malloc(amount * sizeof(DHT*));
	
	int index = 0;

	for (int i = 0; i < fileSize; i++) {
		if ((buffer[i] == 0xff) && (buffer[i+1] == 0xc4)) {
			DHT* dht = malloc(sizeof(DHT));
			dht->length = ((size_t)buffer[i+2]<<8) + buffer[i+3];
			printf("DHT lengths: %d\n", dht->length);

			dht->tableInfo = buffer[i+4];

			dht->Li = &buffer[i+5]; // always 16 bytes long
			int sum = 0;
			for (int j = 0; j < 16; j++) {
				sum += dht->Li[j];
			}
			printf("The sum of amount of huffman code symbols of every length is: %d\n", sum);
			int checkSum = (int)(dht->length - 2 - 1 - 16);
			printf("And the checksum (length - 2 - 1 - 16) is: %i\n", checkSum);
			if (sum != checkSum) {
				printf("The checksum of this DHT was not correct\n");
				return NULL;
			}

			dht->huffmanCodeSymbols = &buffer[i+21]; // Amount = sum(Li)

			DHTs[index] = dht;
			index++;
		}
	}
	printf("\n");
	
	return DHTs;
}
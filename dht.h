#ifndef DHT_H
#define DHT_H

typedef struct {
	int length; // 2 length bytes are included in length
	unsigned char tableInfo; // Upper 4 bits: Tc (table class - 0->DC-table ; 1->AC-table) - lower 4 bits: Th (table identifier)
	unsigned char* Li; // exactly 16 bytes where Li tells you how many Huffman codes exist having a particular length
	unsigned char* huffmanCodeSymbols; // There are the amount of bytes equal to the sum of Li (sum(Li) = length - 2 - 1 - 16)
} DHT;

DHT** getDHTs(unsigned char* buffer, long fileSize, int* amount);

#endif
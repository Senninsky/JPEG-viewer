#ifndef DHT_H
#define DHT_H

typedef struct {
	int length; // 2 length bytes are included in length
	unsigned char tableInfo; // Upper 4 bits: Tc (table class - 0->DC-table ; 1->AC-table) - lower 4 bits: Th (table identifier) 
	unsigned char* tableData;
} DHT;

DHT** getDHTs(unsigned char* buffer, long fileSize, int* amount);

#endif
#ifndef DQT_H
#define DQT_H

#include "matrix.h"

typedef struct {
	unsigned int precision; // Upper 4 bits - 0 = 8-bit values; 1 = 16-bit values;
	unsigned int tableID; // Lower 4 bits - 0-3 (SOF specifies which table each component uses)
	Matrix* values; // 64 values from an 8x8 matrix, stored in zigzag order
} DQT65;

DQT65** getDQTs(unsigned char* buffer, long fileSize);

#endif
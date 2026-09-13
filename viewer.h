#ifndef VIEWER_H
#define VIEWER_H

#include "matrix.h"

typedef struct {
	unsigned int precision; // Upper 4 bits - 0 = 8-bit values; 1 = 16-bit values;
	unsigned int tableID; // Lower 4 bits - 0-3 (SOF specifies which table each component uses)
	Matrix* values; // 64 values from an 8x8 matrix, stored in zigzag order
} DQT65;

typedef struct {
	unsigned int precision; // 1 byte
	unsigned int imageHeight; // In pixels - 2 bytes (Rarely, height is zero and supplied later through DNL)
	unsigned int imageWidth; // In pixels - 2 bytes
	unsigned int N; // Number of component descriptions following (3 bytes per component)
	Component** components; // Three bytes per component - 3N bytes (1: ComponentID (lable used by SOS), 2: Sampling factors (upper four bits = horizontal H, lower four = vertical V), 3: Quantization table ID (selects a table defined by DQT)
} SOF0;

unsigned char* openFile(const char* relativePath);

long getFileSize(const char* relativePath);

DQT65** getDQTs(unsigned char* buffer, long fileSize);

void zigzagToSquare(Matrix* array, Matrix* square);

SOF0* getSOF0(unsigned char* buffer, long fileSize);

#endif
#ifndef SOF_H
#define SOF_H

typedef struct {
	unsigned char ID;
	unsigned char samplingFactor; // Upper 4 bit: horizontal - lower 4 bits: vertical
	unsigned char quantTableID;
} Component;

typedef struct {
	unsigned int precision; // 1 byte
	unsigned int imageHeight; // In pixels - 2 bytes (Rarely, height is zero and supplied later through DNL)
	unsigned int imageWidth; // In pixels - 2 bytes
	unsigned int N; // Number of component descriptions following (3 bytes per component) - 1 byte
	Component* components; // Three bytes per component - 3N bytes (1: ComponentID (lable used by SOS), 2: Sampling factors (upper four bits = horizontal H, lower four = vertical V), 3: Quantization table ID (selects a table defined by DQT)
} SOF;

SOF* getSOF(unsigned char* buffer, long bufferSize);

#endif
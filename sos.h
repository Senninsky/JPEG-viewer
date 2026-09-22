#ifndef SOS_H
#define SOS_H

typedef struct {
	int length;
	unsigned char Ns; // Tells how many image components participate in this scan: e.g. : 3 = Y, Cb, Cr
	// Now there are 2 bytes per component: Cs | Td|Ta
	unsigned char Cs;
	unsigned char TdTa;
	
} SOS;

SOS** getSOSs(unsigned char* buffer, long fileSize, int* amount_);

#endif
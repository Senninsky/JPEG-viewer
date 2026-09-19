#ifndef SOS_H
#define SOS_H

typedef struct {
	int length;
} SOS;

SOS** getSOSs(unsigned char* buffer, long fileSize, int* amount_);

#endif
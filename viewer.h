#ifndef VIEWER_H
#define VIEWER_H

#include "matrix.h"

unsigned char* openFile(const char* relativePath);

long getFileSize(const char* relativePath);

void zigzagToSquare(Matrix* array, Matrix* square);

#endif
#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
	int width;
	int height;
	unsigned char* values;
} Matrix;

Matrix* initMatrix(int _width, int _height, unsigned char* _values);

Matrix* initSparseMatrix(int _width, int _height);

void freeMatrix(Matrix* m);

void insertMatrixValue(Matrix* m, int row, int col, unsigned char value);

unsigned char getMatrixValue(Matrix* m, int row, int col);

void printMatrix(Matrix* m);

#endif
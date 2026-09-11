#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

Matrix* initMatrix(int _width, int _height, unsigned char* _values) {
	Matrix* m = malloc(sizeof(Matrix));
	
	m->width = _width;
	m->height = _height;

	m->values = _values;
	
	return m;
}

Matrix* initSparseMatrix(int _width, int _height) {
	unsigned char* zeros = malloc(_width * _height * sizeof(unsigned char));
	Matrix* m = initMatrix(_width, _height, zeros);
	
	return m;
}

void freeMatrix(Matrix* m) {
	printf("Free called\n");
	free(m->values);
	printf("Values freed\n");
	free(m);
	printf("Matrix freed\n");
}

void insertMatrixValue(Matrix* m, int row, int col, unsigned char value) {
	if (!((0 <= row) && (row < m->height) && (0 <= col) && (col < m->width))) {
		printf("insertMatrixValue was not in bounds: tried at (row: %i, column: %i) for matrix with width: %i and height: %i\n", row, col, m->width, m->height);
		return;
	}

	m->values[row*(m->width) + col] = value;
}

unsigned char getMatrixValue(Matrix* m, int row, int col) {
	if ((0 <= row) && (row < m->height) && (0 <= col) && (col < m->width)) {
		return m->values[row*(m->width) + col];
	}
	
	printf("getMatrixValue was not in bounds\n");
	return 1;
}

void printMatrix(Matrix* m) {
	printf("Width of matrix to print: %i\n", m->width);
	printf("Height of matrix to print: %i\n", m->height);
	for (int i = 0; i < m->height; i++) {
		for (int j = 0; j < m->width; j++) {
			printf("%02x ", getMatrixValue(m, i, j));
		}
		printf("\n");
	}
}
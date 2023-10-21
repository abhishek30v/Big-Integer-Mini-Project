#ifndef BIG_INT_H
#define BIG_INT_H

typedef struct BigInt BigInt;

BigInt *BigIntCreate(const char *numStr);
int compare(BigInt *a, BigInt *b);
BigInt *createBigIntFromInt(int num);
void BigIntDestroy(BigInt *num);
void printBigInt(BigInt *num);
void appendDigit(BigInt *num, int digit);
BigInt *add(BigInt *a, BigInt *b);
BigInt *subtract(BigInt *a, BigInt *b);
BigInt *multiply(BigInt *a, BigInt *b);
BigInt *divide(BigInt *a, BigInt *b);
void copy(BigInt *dest, BigInt *src);
#endif

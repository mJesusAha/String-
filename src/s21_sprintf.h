#ifndef SRC_S21_SPRINTF_H
#define SRC_S21_SPRINTF_H

#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000000

int addInStr(char* str, const char* str_new);
int numberFloatStr(char* str, long double number, int* flags);
int numberIntInStr(char* str, long int number, const int* flags);
int strInNumber(const char* str);
void numberFloatInStr(char* str, double number, int accuracy);
int s21_sprintf(char* str, char* format, ...);
int specifiersU(char* str, int* flags, unsigned d);
void cleanMass(int* mass, int n);
void addFlagsSizeInStr(const int* flags, char* str, char* s);
void caseFunctions(char* p, char* str, int* flags);
void specifiersUType(char* str, int* flags);
void specifiersDType(char* str, int* flags);
void specifiersSType(char* str, int* flags);
void specifiersFType(char* str, int* flags);
void specifiersCType(char* str, int* flags);
int specifiersD(char* str, int* flags, long int d);
int specifiersF(char* str, int* flags, long double f);
void defaultCase(char* str, const char* p);
int flagsPlusMinusSpace(const char* p, int* flags);
void strPlusMinusSpace(char* str, int* flags, long double d);
void spaceBefore(int size, int width, char* str, const int* flags);
void spaceAfter(int size, int width, char* str, const int* flags);
int sizeFunction(char* p, int* flags);
int numberUIntInStr(char* str, long unsigned int number, const int* flags);
int specifiersUL(char* str, int* flags, long unsigned d);
int newMass(char* str, char* str2);
void forFunctionSprintfFlagsLH(int* flags, const char* p);
int flagsActiv(int* flags, int n1, int n2);
int pointingAccuracyOutput(const int* flags, const int f1, const int f2,
                           const int number, const int point, const int i,
                           const int j);
int intParthOne(char* str, long int* number);
int intParthTwo(long unsigned int num, const int* counter);
int IntParthThree(int flag1, int flag, int counter, char* str, long int number,
                  const int* flags);

#endif  // SRC_S21_SPRINTF_H

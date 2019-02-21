#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#define SEG printf("This line is giving a Segfault")
#define FREE free(x)
#define MAL malloc(size_t s)
#define PRINT printf(x)
#define CAL calloc(n,size_t s)
int checkToken(char *token);
char * trimSpecialChars(char *line);
char *setStr(char *s);
char ** initStrArray(int N, int K);
void stringToLower(char *string);
void stringToUpper(char *string);
char * stringCpy(char * first, char * second);
char *stringCat(char *first,char *second);
int indexOf(char *line, char c);
int indexOfSubstring(char *line, char *substring);
int isStringEmpty(char *string);
void splitByFirstOccurence(char *line, char *left, char *right, char del);
void deallocator(void *thingToFree);
int isEmpty(char * string);
void free_fields(char **ptr, int size);
int containsChar(char *line,char c);
int checkBefore(char *line , char del1, char del2);
char * removeFirstChar(char *str);
void splitContentLine(char *line, char *left, char *right);
int containsSubstring(char *line, char *sub);
int isFloat(char * string);
#endif
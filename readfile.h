//Header file for file to perform reading files and counting words

#ifndef READFILE_H //If this file has already been compiled and defined this prevents it from doing it again
#define READFILE_H //Defines READFILE_H the first time this file is compiled so it is not recompiled

//Includes all the libraries needed for this program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

//struct Word definition and typedef for struct Word
typedef struct Words {
	char *word; //Holds a word found the text file
	int count; //The number of times the word appears in the text file
	struct Words *next;
} Word; //Creates a typedef alias name Word for struct Word 

//Prototype declaration of program functions
void addToList(Word *head, char* input); //Searches for word in list of struct Words, increases count if already in list or adds to list
void freeList(Word *head); //Frees dynamically created character array words and struct Words
void printList(Word *head); //Writes every word in the file with it's associated count to standard output in an organized list
void readFile(int fd, Word *head); //Reads from a file and finds all words in the file by reading character by character 


#endif
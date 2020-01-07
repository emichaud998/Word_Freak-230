Group: Emily Michaud, Nick Michaud, and TJ Goldblatt

Video:
https://youtu.be/3UfQmop7bEw

General Overview:
The intent of the Word Freak project was to be able to take in a text file, through various means of parameters, read that file, and return a glossary of every word with a 
corresponding number showing how often it appears in that text, or even in multiple texts. 

Our Makefile compiles our readfile.c file and creates an executable file called wordfreak. The Makefile is designed to not only be able to create the wordfreak exec file, but 
to also allow debugging using gdb because we ran into issues initially with debugging code, so with some research we were able to figure out how to properly design the 
Makefile to enable wordfreak to be debuggable.

Our header file, readfile.h, defines a READFILE_H to ensure it hasn’t been compiled already. The header includes a definition for a structure called Words which stores a word 
char pointer, an int count, and a struct Words pointer called next which points to the next Word struct in the linked list. 

Our implementation of Word Freak begins in the main function of the readfile.c file, which first checks to see if any files were passed in through the command line, if not 
then it checks if there is an environmental variable called WORD_FREAK that has been set to a file, and finally if not the program checks if anything has been inputted into 
the standard input. When the program begins, argc, which is one of the arguments passed into the main function along with the character array argv, is checked to see if it 
holds a value greater than one, since the first argument is always the name of the program, so anything greater than one implies that additional arguments, in this case text 
files, have been included on the command line during execution. If argc is greater than one, then a for loop iterates through each of the arguments held in the array argv 
after argv[0], which holds the name of the program. Each file name held in argv is opened using O_RDONLY, meaning we are opening the file in read only mode, and the name of 
the current file passed in. Open returns a file descriptor that will return -1, if there is an error, or a different number corresponding to the location of the file instead. 
If there is no error, then file is then passed in to our readFile() method which takes in an integer that is a file directory, and the head pointer instance of our Word struct 
that is our linked list that stores all of the words. Using the read() system call, the program reads the opened file using it’s file descriptor one byte at a time, appending 
it to our buffer which stores the individual word, one character at a time using a while loop. The while loop continues as long as read() doesn’t return 0, which indicates the 
end of the file, or -1, which indicates there was an error which is written to the standard error file and the program is exited with a value of 1. Each time the program goes 
through the while loop, the next character in the file is read and inserted into buffer. If the character is a letter or if it is a ‘, buffer’s index is increased by one and 
the while loop continues reading in more characters. When the character read into buffer does not contain a character or ‘ this means it has reached the end of a word, and so 
the characters making up this word stored in buffer are terminated using the NULL character, and a new dynamically created character array holding this word is created using 
malloc and passed into the addtoList() function. This function loops through the linked list of Word structures and checks if the new word has already been added to the list, 
if it has the word, count is increased by one and the word is moved to the front of the list, and if it is not in the list, a new Word structure is created holding this new 
word using malloc. Anytime anything is dynamically created using malloc, the program checks to make sure malloc was successful by making sure it did not return a NULL pointer, 
and if it does an error message “Malloc Failure” is written to standard error and the program is exited. When the read() reaches the end of the file, the function returns, and 
the glossary of all the words in the file and their associated frequencies are displayed through passing in the head of the linked list of Words to the printList() function. 
This function traverses through the linked list of Words and creates a string with the format specifiers needed to display the glossary in a neatly formatted list, adding 
enough spaces to accommodate the longest word in the list, which is kept track of with the global variable MAX. This formatted string is then written to standard output for 
display using the write system call, which takes the file descriptor POSIX of standard output, the string, and the length of the string as arguments to write each word and 
their frequency in a list to standard output. If there is an error write() returns -1, which is written to the standard error file and the program is exited. Finally, if 
everything is successful the head of the list of words is passed to a function called free(), which frees all the dynamically created char arrays holding each word and all the 
dynamically created Word structures. 

If no files were passed in as arguments on the command line, the program then checks to see if there is an environmental variable called WORD_FREAK that has been set. If there 
is, the program gets the name of the file that WORD_FREAK is set to using the getenv() command. This file is then opened in the same way as the files previously, and if 
WORD_FREAK is not set to a valid file, open() returns -1 and the error message indicated by errno is written to the standard error file and the program is exited with a value 
of 1. Otherwise, open returns a file descriptor to the file held by WORD_FREAK which is then passed into the readFile() function to be processed as previously described. The 
file is then closed, the list of words and their frequencies are written to standard output, and the free function is called.

If there are no command line arguments and there is no environmental variable called WORD_FREAK, the program finally looks to read from the standard input file. The standard 
input file’s file descriptor, whose value of 0 is held by the POSIX name STDIN_FILENO, is passed into the readFile() to be read and processed. When the output of printing a 
file is inserted into the program through the use of the bash pipe operator, the standard input file contains the contents of that text file, which is read in the same way as 
passing in files through the command line or through an environmental variable. If nothing was inserted into the programs standard input, the program will wait for the user to 
type in their own text to standard input, and after hitting ctrl+D the program will read and process whatever text was typed into standard input. The standard input file is 
then closed, the list of words and their frequencies are written to standard output, and the free function is called.
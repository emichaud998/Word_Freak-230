#include "readfile.h"

int MAX = 0; //Global variable that keeps track of the length of the longest word- used to format printing list

/*
*Adds a Word structure to the list of all the words in the document or increases count of a word
*If the word is already in the list it increases the words count by one
*If the word is not yet in the list it adds the word to the list and sets the count to 1
*Whenever a word is found and the count is increased that word is moved to front of list
	Increases efficiency by making more frequent words closer to front of list
*Parameters:
	head: the "head" of the list of words
	input: word to be searched for to increase count in list or added to end of list 
*/
void addToList(Word *head, char* input){
	Word *current = head; //Temporary list pointer to search through linked list for the input word
	Word *prev = head; //Points to the word in the list before the one being compared to the input word
	
	/*Searches through word list for input word and increases count if found
	*Removes the found word from the list using the prev pointer and adds to the front of the list after the head
	*If not found a new Word structure is created and added to the end of the list
	*/
	while(current != NULL){
		//If the current Word structure holds a word that is the same as the word input (and not NULL for head)
		if((current->word != NULL) && strcmp(current->word, input) == 0){ 
			current->count++; //Increase the count of the word by one

			//Remove word from the list and add it to the front of the list after the head
			prev->next = current->next;
			current->next = head->next;
			head->next = current;
			free(input); //Frees the word that was created since it does not need to be added to the list

			return;
		}
		
		//If the word is not found in the list (the last Word structure did not contain the same word)
		if (current->next == NULL){ //Last Word in the list does not contain word
			Word *createWord = (Word *)malloc(sizeof(Word)); //Dynamically creates a new Word structure

			//If pointer returned by malloc is NULL there was an error allocating memory, prints error and exits
			if (createWord == NULL){
				write(STDERR_FILENO, "Malloc Failure.", sizeof("Malloc Failure."));
				exit(1);
			}

			createWord->word = input; //Sets the Word structure's word element to the input word
			createWord->count = 1; //Sets the count to 1 since it has been found 1 time in document
			createWord->next = NULL; //Last Word in the list points to NULL
			current->next = createWord; //Adds new Word structure to the end of list after current

			//If the length of the new word is longer than the MAX length, MAX now is the size of this new word
			if (strlen(createWord->word) > MAX){
				MAX = strlen(createWord->word);
			}

			return;
			
		}

		prev = current;
		current = current->next;
	}
}

/*
*Frees all the dynamically created Word structures and the words they contain
*Parameters:
	head = the "head" of the linked list of Word structs
*/
void freeList(Word *head){
	Word *temp = head->next; //Temporary pointer to go through linked list and free all dynamically created items
	Word *next = head->next; //Pointer used to hold the next Word in the linked list so temp can be freed
	free(head); //Frees the head of the list

	//Goes through all the Word structures in the list, freeing all the words and Word structures until it reaches NULL indicating the end of the list
	while(temp != NULL){
		next = temp->next; //Set to the next Word after temp so temp can be freed without losing the rest of the linked list
		free(temp->word); //Frees the dynamically created word the Word structure holds
		free(temp); //Frees the Word structure
		temp = next; //temp is set to the next Word in the linked list
	}

	//All pointers set to NULL to avoid dangeling pointers 
	head = NULL;
	temp = NULL;
	next = NULL;
}

/*
*Prints all the words found in the file and their associated frequencies to standard output
*Parameters:
	head = the "head" of the linked list of Words
*/
void printList(Word *head){
	Word *current = head->next; //Temporary pointer to go through linked list of Words and print the word and count of each

	//Traverses through all Word structs and creates a string containing each word and it's count and writes it to the stdout file 
	while(current != NULL){
		char charFormat[80]; //String used to contain the format for all the words and counts
		sprintf(charFormat, "%%-%is :  %%i \n", MAX); //Stores the formatted string in charFormat with format specifiers including left justifying word by MAX spaces
		char str[80]; //String used to insert each word and count into the formatted string
		sprintf(str, charFormat, current->word, current->count); //Inserts the current word and count into the format string and stores this in str
		int writeError = write(STDOUT_FILENO, str, strlen(str)); //Writes the string created and stored in str to stdout
		
		//If the write function returns a -1, writes the error to sderr and exits
		if (writeError == -1){
			char str[80];
			sprintf(str, "%s \n", strerror(errno)); //Creates a string with the error referred to by errno
			write(STDERR_FILENO, str, strlen(str)); //Writes error string to sterr
			exit(1);
		}

		current = current->next;
	}
}

/*
*Reads through the file letter by letter finding all the words in the file and either adding new ones to the list of words or increasing their counts
*Parameters:
	fd = file descriptor of the file to be read from
	head = the "head" of the linked list of Words
*/
void readFile(int fd, Word *head){
	char buffer[50]; //Array to hold letters as they are read in from the file
	int i = 0; //Used to insert letters at specific indexes in the buffer array
	int readReturn = read(fd, &buffer[i], 1); //Reads from the file and stores 1 byte (1 letter) in buffer- will return 0 to readreturn if end of file or -1 if error
	
	/*
	*Reads through the file letter by letter storing them in buffer until a word is completed and buffer is set back to index 0 to start a new word 
	*Created word is searched for in list
	*Stops when readReturn is 0 meaning the end of the file
	*/
	while (readReturn > 0){
		//If the last char stored in buffer is a letter (within the ascii values 65-90 and 97-122) or if it is a ' increase i and keep reading through file storing letter in next index of buffer
		if ((buffer[i]>=65 && buffer[i]<=90) || (buffer[i]>=97 && buffer[i]<=122) || ((buffer[i] == '\'') && (i>0))){
			
			//If the last char stored in buffer is a ' and the char stored before it was also a ' removes the double ' and creates word and searches for it/adds it to list 
			if ((buffer[i] == '\'') && buffer[i-1] == '\''){
				buffer[i-1] = '\0'; //Removes the repeated ' by places null character there to signifiy the end of the string word
				char *newWord = (char *)malloc((i)*sizeof(char)); //Creates a new dynamically created char array
			   
				//If pointer returned by malloc is NULL there was an error allocating memory, prints error and exits
				if (newWord == NULL){
					write(STDERR_FILENO, "Malloc Failure.", sizeof("Malloc Failure."));
					exit(1);
				}
				
				strcpy(newWord, buffer); //Copies the word stored in buffer up to the newline character to the dynamically created char array
				addToList(head, newWord); //Searches for this new word in list and increases count/adds to list
				i = 0; //Sets i back to zero to start storing new word in buffer
			} 
			
			else{
				i++; //Keep indexing through buffer storing each letter read in array until the end of a word
			}
		}

		/*
		*Else if the last char stored was not a letter or colon and the index is not at zero (meaning no letters have been added yet) this is the end of a word
		*Creates a new string for the created word and searches for it in the list of words
		*/
		else if (i != 0){
				buffer[i] = '\0'; //Places null character at the end of the word to terminate the string at the end of the word
				char *newWord = (char *)malloc((i+1)*sizeof(char)); //Creates a new dynamically created char array
				
				//If pointer returned by malloc is NULL there was an error allocating memory, prints error and exits
				if (newWord == NULL){
					write(STDERR_FILENO, "Malloc Failure.", sizeof("Malloc Failure."));
					exit(1);
				}
			   
				strcpy(newWord, buffer); //Copies the word stored in buffer up to the newline character to the dynamically created char array
				addToList(head, newWord); //Searches for this new word in list and increases count/adds to list
				i = 0; //Sets i back to zero to start storing new word in buffer    
		}

		readReturn = read(fd, &buffer[i], 1); //Reads the next character from the file and stores it to the memory location at index i of buffer
	}
	
	//If readReturn returned -1, writes the error to stderr and exits
	if (readReturn == -1){
		char str[80];
		sprintf(str, "%s \n", strerror(errno)); //Creates a string with the error referred to by errno
		write(STDERR_FILENO, str, strlen(str)); //Writes error string to sterr
		exit(1);
	}

	return;
}

/*
*Main function that checks if file was passed in through command line, environmental variable, or standard input and then reads from the file
*Parameters:
	argc = the total number of arguments passed into main with one being the name of the program
	argv = array containing all the arguments written in the command line- argv[0] always stores the name of the program
*Returns:
	*Returns 0 if the function ran and completed successfully 
*/
int main(int argc, char *argv[]){
	//Creates a new Word structure with head and next set to NULL and count set to 0 to serve as the head of the list of Words to be created
	Word *head = (Word *)malloc(sizeof(Word));
	
	//If pointer returned by malloc is NULL there was an error allocating memory, prints error and exits
	if (head == NULL){
		write(STDERR_FILENO, "Malloc Failure.", sizeof("Malloc Failure."));
		exit(1);
	}

	head->word = NULL;
	head->count = 0;
	head->next = NULL;

	/*
	*If argc is greater than one this means more than just the program name was written on the command line- file was passed in through command line
	*Opens the files stored in argv[] from the command line and reads from them and closes them when finished
	*/
	if (argc > 1){
		//Loops through all of the files passed in through the command line that are stored in argv[] after argv[0] which holds the program name- opens and reads
		for (int i = 1; i < argc; i++){
			int fd = open(argv[i], O_RDONLY); //Opens the files stored in argv and stores the file descriptor in fd
			
			//If the file descriptor is -1, there was an error, error written to stderr and program exited 
			if(fd == -1){
				char str[80];
				sprintf(str, "%s \n", strerror(errno)); //Creates a string with the error referred to by errno 
				write(STDERR_FILENO, str, strlen(str)); //Writes error string to sterr
				exit(1);
			}
			readFile(fd, head); //Reads the file letter by letter and adds words to list with their frequencies
			close(fd); //Closes the file
		}
	}
	
	//If no file passed in from command line checks if the environmental variable WORD_FREAK has been set
	else if (getenv("WORD_FREAK") != NULL){
		char *envFile = getenv("WORD_FREAK"); //Stores the file name WORD_FREAK is set to in the envFile
		int fd = open(envFile, O_RDONLY); //Opens the file stored in WORD_FREAK and stores the file descriptor in fd
		
		//If the file descriptor is -1, there was an error, error written to stderr and program exited 
		if(fd == -1){
			char str[80];
			sprintf(str, "%s \n", strerror(errno)); //Creates a string with the error referred to by errno, which is created into a string using strerror()
			write(STDERR_FILENO, str, strlen(str)); //Writes error string to sterr
			exit(1);
		}
		readFile(fd, head); //Reads the file letter by letter and adds words to list with their frequencies
		close(fd); //Closes the file
	}

	//If no file passed in from command line or environmental variable, checks if file was passed in through standard input
	else{
		readFile(STDIN_FILENO, head); //Reads standard input file letter by letter and adds words to list with their frequencies
		close(STDIN_FILENO); //Closes standard input file
	}
	
	printList(head); //Prints all the words and their frequencies by writing them to stdout
	free(head); //Frees all the Word structs and the words they hold
	return 0;
}

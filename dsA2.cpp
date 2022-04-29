/*
  FILE : dsA2.cpp
* PROJECT :  Assignment 2#
* PROGRAMMER : Mahmood Al-Zubaidi
* FIRST VERSION :19/july/2021
* DESCRIPTION : The purpose of this function is to orgnize the inputted words from file into a hash table and a link list then comparing them together
*/




#include "linkedList.cpp"
#include "dsA2.h"

#define TABLE_SIZE 127
#define MAX_WORDS 10000

unsigned int hash(char* words) {

	int i, length = strnlen(words, MAX_WORDS);

	unsigned int hash_value = 0;

	for (i = 0; i < length; i++)
	{
		hash_value += words[i];
		hash_value = hash_value * words[i];
	}
	return hash_value % TABLE_SIZE;
}





int getWord(WordNode* head, char words[]);
void insert(char* words, WordNode* hash_table, WordNode** head);
WordNode* findWord(WordNode* longList, char* searchWord, int comparisonCount[]);
void searchForWordTwice(char* searchWord, WordNode* longList, WordNode* hash_table, int comparisonCount[]);
void print(int counter, int totalLis, int totalHash);
void freeData(WordNode* head);

int totalLis = 0;
int counter = 0;
int totalHash = 0;

int main() {
	WordNode* hash_table = (WordNode*)malloc(TABLE_SIZE * sizeof(WordNode));
	int i;
	for (i = 0; i < TABLE_SIZE; i++) {
		hash_table[i].next = NULL;
	}

	WordNode* longList = NULL;


	char words[MAX_WORDS];
	char searchWord[MAX_WORDS];
	char thirdords[MAX_WORDS];

	int comparisonCount[20] = { 0 };

	FILE* ifp = NULL;

	ifp = fopen("..\\words.txt", "r");
	if (ifp == NULL) {
		printf("cant's open file");
		return -1;
	}
	while (fscanf(ifp, "%s", words) != EOF) {

		insert(words, hash_table, &longList);
	}



	while (true) {
		printf("\nEnter the word that you're searching for: ");
		scanf("%s", searchWord);
		counter++;
		if (strchr(searchWord, '.') != NULL) {
			fclose(ifp);
			freeData(longList);
			for (int i = 0; i < TABLE_SIZE; i++) {
				 WordNode* curr = hash_table[i].next;
				 free(curr);				
			}
			break;
		}
		searchForWordTwice(searchWord, longList, hash_table, comparisonCount);

	}

	print(counter, totalLis, totalHash);

	return 0;
}




/** Function: insert
* Description: it inserts the words from the file into the linked list and into the hash table
* Parameters: a words string that contains the file's words, the hashtable array in order to insert into the hashtable, and the pointer to pointer head which throu it we insert into the linked list
* Returns: nothing
*/
void insert(char* words, WordNode* hash_table, WordNode** head) {
	WordNode* beforeElement = NULL;
	WordNode* afterElement = NULL;
	WordNode* tail = NULL;

	unsigned int hash_value = hash(words);
	WordNode* newNode = NULL;

	newNode = (WordNode*)malloc(sizeof(WordNode));
	newNode->words = (char*)malloc((strlen(words) + 1) * sizeof(char));
	strcpy(newNode->words, words);
	newNode->prev = newNode->next = NULL;



	if (hash_table[hash_value].next == NULL) {
		hash_table[hash_value].next = newNode;
	}
	else if (strcmp(hash_table[hash_value].next->words, words) >= 0) {
		newNode->next = hash_table[hash_value].next;
		hash_table[hash_value].next->prev = newNode;

		hash_table[hash_value].next = newNode;
	}

	else {
		beforeElement = hash_table[hash_value].next;
		afterElement = hash_table[hash_value].next->next;

		while (afterElement != NULL) {
			if (strcmp(afterElement->words, words) >= 0) {
				break;
			}

			beforeElement = afterElement;
			afterElement = afterElement->next;
		}
		newNode->prev = beforeElement;
		newNode->next = afterElement;
		beforeElement->next = newNode;

	}



	WordNode* newBlock = NULL;
	WordNode* beforeElement2 = NULL;
	WordNode* afterElement2 = NULL;


	newBlock = (WordNode*)malloc(sizeof(WordNode));
	if (newBlock == NULL) {
		printf("Error! No more memorry\n");
	}

	newBlock->words = (char*)malloc((strlen(words) + 1) * sizeof(char));

	strcpy(newBlock->words, words);
	newBlock->prev = newBlock->next = NULL;


	if (*head == NULL) {
		*head = newBlock;
	}


	else if (strcmp((*head)->words, words) >= 0) {
		newBlock->next = *head;
		(*head)->prev = newBlock;
		*head = newBlock;
	}

	else {

		beforeElement2 = *head;
		afterElement2 = (*head)->next;

		while (afterElement2 != NULL)
		{
			if (strcmp(afterElement2->words, words) >= 0)
			{
				break;
			}
			beforeElement2 = afterElement2;
			afterElement2 = afterElement2->next;
		}

		newBlock->prev = beforeElement2;
		newBlock->next = afterElement2;
		beforeElement2->next = newBlock;
	}
}


/** Function: findword
* Description: it searches for the inputted word in the linked list by comparing the word with the list's fileds
* Parameters: the list, the word that we wanna seartch for and the counter to count the number of times stcmpt has been called
* Returns: a pointer to the node that has been found or null if it could not find it
*/
WordNode* findWord(WordNode* longList, char* searchWord, int comparisonCount[]) {

	WordNode* next = NULL;
	WordNode* curr = longList;
	int c[1] = { 0 };
	while (curr != NULL) {
		comparisonCount[1]++;

		if (strcmp(searchWord, curr->words) == 0) {
			c[0]++;
			break;
		}
		curr = curr->next;
	}
	if (c[0] != 0) {
		printf("\n%s was found in the linked list in %d comparisons\n", searchWord, comparisonCount[1]);
	}
	else {
		printf("\n%s was NOT found in the linked list in %d comparisons\n", searchWord, comparisonCount[1]);
		return NULL;
	}

	while (comparisonCount[1] != NULL) {
		totalLis += comparisonCount[1];
		break;
	}
}


/** Function: searchForWordTwice
* Description: it calls the above function to find the word in the list and it also finds the words in the hash table by comparing the inputted word with the bucket based on the hash value
* Parameters: the word we wanna search for, the list, the hash table and the counter to count the times we call strcmp
* Returns: nothing
*/
void searchForWordTwice(char* searchWord, WordNode* longList, WordNode* hash_table, int comparisonCount[]) {
	findWord(longList, searchWord, comparisonCount);

	WordNode* curr = NULL;
	int c[1] = { 0 };

	unsigned int hash_value = hash(searchWord);

	curr = hash_table[hash_value].next;
	while (curr != NULL) {
		comparisonCount[2]++;

		if (strcmp(searchWord, curr->words) == 0) {
			c[0]++;
			break;
		}
		curr = curr->next;
	}



	if (c[0] != 0) {
		printf("%s was found in the Hash Table in %d comparisons\n", searchWord, comparisonCount[2]);
	}
	else {
		printf("%s was NOT found in the Hash Table in %d comparisons\n", searchWord, comparisonCount[2]);

	}

	while (comparisonCount[2] != NULL) {
		totalHash += comparisonCount[2];
		break;
	}
}


/** Function: print
* Description: it prints the number of times the user had searched for a certain words, Total number of comparisons in linked list And the Total number of comparisons in hash table.
* Parameters: the counter, the number of list's comparisons and the number of the hash table's compariosons.
* Returns: nothing
*/
void print(int counter, int totalLis, int totalHash) {
	printf("\n\nTotal number of searches\t%d\n", counter - 1);
	printf("Total number of comparisons in linked list\t%d\n", totalLis);
	printf("Total number of comparisons in hash table\t%d\n", totalHash);
}



/** Function: freeData
* Description: it frees the node out of memorry.
* Parameters: pointer to the node.
* Returns: nothing.
*/
void freeData(WordNode* head) {
	WordNode* curr = NULL;
	WordNode* next = NULL;
	curr = head;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
}
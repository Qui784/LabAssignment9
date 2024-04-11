#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType *record;
    struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
	return x % 31;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		//	pointer to current element in array
		struct HashType *current = &pHashArray[i];

        printf("index %d -> ", i);
		
        while (current != NULL)
        {
            if (current->record != NULL) {
                printf("%d, %c, %d -> ", current->record->id, current->record->name, current->record->order);
            }
            current = current->next;
        }
        printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation

	//	size of hash array
    int hashSz = 10;

	// allocate memory for a hash table array
    struct HashType *hashArray = (struct HashType *)malloc(hashSz * sizeof(struct HashType));
	
	//	loop through hash array and initialize data to NULL
   	for (int i = 0; i < hashSz; ++i)
    {
        hashArray[i].record = NULL;
        hashArray[i].next = NULL;
    }

    //	insert records into hash table
    for (int i = 0; i < recordSz; ++i)
    {
		//	get index from hash function
        int index = hash(pRecords[i].id);

		//	current element in hash table
        struct HashType *current = &hashArray[index];
		
		//	move current pointer
        while (current->next != NULL)
        {
            current = current->next;
        }

		//	allocate memory for a new node
        struct HashType *newNode = (struct HashType *)malloc(sizeof(struct HashType));
		
		//	add new node to hash table
        newNode->record = &pRecords[i];
        newNode->next = NULL;
        current->next = newNode;
    }

    //	display records in hash table
    displayRecordsInHash(hashArray, hashSz);

    //	free memory
    for (int i = 0; i < hashSz; ++i)
    {
        struct HashType *current = hashArray[i].next;
        while (current != NULL)
        {
            struct HashType *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashArray);
    free(pRecords);
    
    return 0;
}
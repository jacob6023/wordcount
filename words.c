#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define TABLE_SIZE 1000         // Size of the hash table
#define MAX_WORD_LENGTH 100    // Maximum word length

typedef struct Node {
    char* word;         // Pointer to store the word
    int count;          // Count of the word occurrences
    struct Node* next;  // Pointer to the next Node (for handling collisions)
} Node;

typedef struct HashTable {
    Node* array[TABLE_SIZE];  // Array of Nodes (the hash table)
} HashTable;
// Function to calculate the hash value for a word
int hash(char* word) {
    int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        hash = (hash * 31 + word[i]) % TABLE_SIZE;
    }
    return hash;
}

// Function to insert a word into the hash table
void insert(HashTable* ht, char* word) {
    int index = hash(word);
    Node* temp = ht->array[index];

    // Traverse the linked list at the index.
    while (temp != NULL) {
        // If the word already exists, increment the count
        if (strcmp(temp->word, word) == 0) {
            temp->count++;
            return;
        }
        temp = temp->next;
    }

    // If the word does not exist create node and add it to the table.
    Node* newNode = malloc(sizeof(Node));
    //dupes word and allocates memory for heap
    newNode->word = strdup(word);
    //make count 1 because its a new occurance of the word.
    newNode->count = 1;
    newNode->next = ht->array[index];
    ht->array[index] = newNode;
}

// Displays the word counts stored in the hash table
void displayWordCounts(HashTable* ht) {
    //printf("Word Counts:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* temp = ht->array[i];
        while (temp != NULL) {
            //printf("%s: %d\n", temp->word, temp->count);
            
            size_t wordLen = strlen(temp->word);

            int countLen = (temp->count % 10) + 1;

            char buffer[wordLen + countLen + 4];

            int size = snprintf(buffer, sizeof(buffer), "%s: %d\n", temp->word, temp->count);

            ssize_t bytes_written = write(1, buffer, size);

            if (bytes_written == -1) {
                printf("Error: Failed to write.");
            }

            temp = temp->next;
        }
    }
}

// Function to free the memory used by the hash table
void freeHashTable(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = ht->array[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp->word);
            free(temp);
        }
    }
    free(ht);
}

int checkAscii(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '\'')) {
        return 0;
    } else if (c == '-') {
        return 1;
    } else {
        return 2;
    }
}

void fileRead((HashTable* ht, char *path) {
    char buf[2];
    char* input = malloc(1);
    int length = 0;
    int bytes = 0, fd;
    int inWord = 0, hyphenCheck = 0, inApostr = 0;

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("Unable to open file at path: %s\n", path);
        return;
    }
    memset(input, 0, 1);
    do {
        (bytes = read(fd, buf, 1));
        //printf("Letter: %c and inWord: %d and bytes: %d\n", buf[0], inWord, bytes);
        if (bytes == 0 && inWord == 1) {
            insert(ht, input);
            //printf("Word: %s\n\n", input);
            memset(input, 0, length + 1);
        } else {
            if (checkAscii(buf[0]) == 1 && hyphenCheck == 0 && inWord == 1 && inApostr == 0) {
                hyphenCheck = 1;
            } else {
                inApostr = 0;
                if (checkAscii(buf[0]) == 0) {
                    if (buf[0] == '\'') {inApostr = 1;}
                    inWord = 1;
                    if (hyphenCheck == 1) {
                        hyphenCheck = 0;
                        length += 1;
                        input = realloc(input, length + 1);
                        strcat(input, "-");
                    }
                    length += 1;
                    input = realloc(input, length + 1);
                    strncat(input, &buf[0], 1);
                } else {
                    hyphenCheck = 0;
                    if (inWord == 1) {
                        insert(ht, input);
                        //printf("Word: %s\n\n", input);
                        length = 0;
                        memset(input, 0, length + 1);
                        free(input);
                        input = NULL;
                        input = malloc(1);
                        inWord = 0;
                        memset(input, 0, 1);
                    }
                }
            }
            
        }
    } while (bytes > 0);
    free(input);
    close(fd);
}

void recursiveSearch(HashTable* ht, char *path, int passes) {
    struct dirent *entry;
    struct stat info;

    DIR *dir = opendir(path);

    if (passes == 0) {
        stat(path, &info);
        if (S_ISREG(info.st_mode)) {
            //printf("File from argument: %s\n", path);

            fileRead(ht, path);

            return;
        }
    }
    
    if (dir == NULL) {
        printf("Error opening path %s during recursiveSearch.\n", path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        //skips "." and ".." directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char entryPath[4096];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

        //printf("%s\n", entryPath);

        if (stat(entryPath, &info) != 0) {
            printf("Error getting file info for %s in recursiveSearch.\n", path);
            return;
        }

        //checks if it is a directory
        if(S_ISDIR(info.st_mode)) {
            //Is a directory
            //printf("Directory\n");
            recursiveSearch(ht, entryPath, passes + 1);
        } else if (S_ISREG(info.st_mode)) {
            //printf("File: %s\n", entryPath);

            int nameLength = strlen(entry->d_name);
            if (nameLength >= 4 && strcmp(entry->d_name + nameLength - 4, ".txt") == 0) {
                //printf("File ends in .txt\n\n");

                fileRead(ht, entryPath);
            }
            
        }

        //if not a file or directory, ignores
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Error: No Argument");
    } else {

        HashTable* ht = malloc(sizeof(HashTable));

        for (int i = 1; i < argc; i++) {
            recursiveSearch(ht, argv[i], 0);
        }

        displayWordCounts(ht);
        freeHashTable(ht);

    }
}

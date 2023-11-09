#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
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
    newNode->word = strdup(word);
    newNode->count = 1;
    newNode->next = ht->array[index];
    ht->array[index] = newNode;
}

// Displays the word counts stored in the hash table
void displayWordCounts(HashTable* ht) {
    printf("Word Counts:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* temp = ht->array[i];
        while (temp != NULL) {
            printf("%s: %d\n", temp->word, temp->count);
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

int main(int argc, char *argv[]){
    
}


void recursiveSearch(char *path, int passes) {
    struct dirent *entry;
    struct stat info;

    DIR *dir = opendir(path);

    if (passes == 0) {
        stat(path, &info);
        if (S_ISREG(info.st_mode)) {
            printf("File from argument: %s\n", path);

            //Insert call to read here

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

        printf("%s\n", entryPath);

        if (stat(entryPath, &info) != 0) {
            printf("Error getting file info for %s in recursiveSearch.\n", path);
            return;
        }

        //checks if it is a directory
        if(S_ISDIR(info.st_mode)) {
            //Is a directory
            printf("Directory\n");
            recursiveSearch(entryPath, passes + 1);
        } else if (S_ISREG(info.st_mode)) {
            printf("File: %s\n", entryPath);

            int nameLength = strlen(entry->d_name);
            if (nameLength > 4 && strcmp(entry->d_name + nameLength - 4, ".txt") == 0) {
                printf("File ends in .txt\n\n");

                //Insert call to read here

            }
            
        }

        //if not a file or directory, ignores
    }
    closedir(dir);
}

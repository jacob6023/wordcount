#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>



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

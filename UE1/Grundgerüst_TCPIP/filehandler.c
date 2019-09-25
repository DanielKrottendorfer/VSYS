#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int doesDirectoryExist(char* d) {
    DIR* dir = opendir(d);
    if (dir) {
        closedir(dir);
        return 1;
    } else if (ENOENT == errno) {
        printf("Directory does NOT exitst\n");
        return 0;
    } else {
        printf("Could not ACCESS directory\n");
        return 0;
    }
}
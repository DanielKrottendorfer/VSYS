#include <stdio.h>
#include <dirent.h>
#include <errno.h>


int doesDirectoryExist(const char* d) 
{
    DIR* dir = opendir(d);
    if (dir) {
        closedir(dir);
        return 0;
    } else if (ENOENT == errno) {
        printf("Directory does NOT exitst\n");
        return -1;
    } else {
        printf("Could not ACCESS directory\n");
        return -1;
    }
}

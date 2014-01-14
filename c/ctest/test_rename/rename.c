#include <stdio.h>

int main(void)
{
    char oldname[80];
    char newname[80];
    // prompt for file to rename and new name
    printf("File to rename: ");
    gets(oldname);
    printf("New name: ");
    gets(newname);
    // Rename the file
    if (0 == rename(oldname, newname))
    {
        printf("Renamed %s to %s.\n", oldname, newname);
    }
    else
    {
        perror("rename");
    }
    return 0;
}


/* fread example: read a complete file */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;

    pFile = fopen("myfile.bin", "rb");
    if (NULL == pFile)
    {
        fputs("File error", stderr);
        exit(1);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char) *lSize);
    if (NULL == buffer)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    // copy the file into the bufer:
    result = fread(buffer, 1, lSize, pFile);
    if (lSize != result)
    {
        fputs("Reading error", stderr);
        exit(3);
    }

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose(pFile);
    free(buffer);
    return 0;
}

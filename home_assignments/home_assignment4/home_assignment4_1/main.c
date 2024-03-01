#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Returns the number of lines containing a string `query` in a file
 * pointed by the path `path`.
 */
int matched_lines(char *path, char *query)
{
    // Allocate buffer to hold one line of text.
    char line[1000 + 2];  // Space for 1000 characters plus \n and \0.

    // Open the file for reading in the text mode.
    FILE *f = fopen(path, "rt");
    assert(f != NULL);

    int count = 0;

    // Read the file line by line.
    while (fgets(line, sizeof(line), f) != NULL) {
        // Count the number of lines containing `query`.
        if (strstr(line, query) != NULL) {
            count += 1;
        }
    }

    // Close the file.
    fclose(f);

    return count;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        return 1;
    }

    // Assume that the query is given as the first command line argument.
    char *query = argv[1];
    
    // Assume that the rest of command line arguments are file paths to work with.
    char **files = &argv[2];
    int nfiles = argc - 2;

    // Sequentially count occurences of the query in each file.
    for (int i = 0; i < nfiles; ++i) {
        int result = matched_lines(files[i], query);
        printf("%s:%d\n", files[i], result);
    }

    return 0;
}


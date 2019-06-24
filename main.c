#include <stdio.h>
#include <stdlib.h>
#include "label.h"

void usage() {
    printf("Start program in this format: .\\idtoss <idocfile.txt>\n");
}
int main(int argc, char *argv[])
{
    FILE *fp;
// open the idoc file provided as a command line parameter
    if (argc != 2) {
        usage();
        return EXIT_FAILURE;
    }
    fp = fopen(argv[1],"r");
    if (fp == NULL) {
        printf("File not found.\n");
        return EXIT_FAILURE;
    }

    int i;
    char str[MAX_CHARS];
    while (fgets(str, MAX_CHARS, fp) != NULL)
        printf("%s\n", str);

// read the file, building a columns instance and corresponding
// labels array instance that contains the contents of the idoc file
// write a tab-delimited spreadsheet based on the columns and labels
// structs.
}

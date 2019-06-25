#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"

// length of the "_dlmtd" suffix added to the output file name if the input file
// is missing the _IDOC extension
#define                    FN_SUFFIX  6

void usage() {
    printf("Start program in this format: .\\idtoss <idocfile.txt>\n");
}
int main(int argc, char *argv[])
{
    FILE *fpin, *fpout;
    char *textfile = (char *)malloc((int)strlen(argv[1]) * sizeof(char) + FN_SUFFIX);

    // the spreadsheet column headings
    Column_header columns;

    // the dynamically allocated Label_record array
    Label_record *labels[1000];

// open the idoc file provided as a command line parameter
    if (argc != 2) {
        usage();
        return EXIT_FAILURE;
    }
    fpin = fopen(argv[1],"r");
    if (fpin == NULL) {
        printf("File not found.\n");
        return EXIT_FAILURE;
    }

    // determine output filename based on whether *IDoc exists

    char *sp = stristr (argv[1], "_IDoc");
    if (sp) {
        int ipos = sp - argv[1];
        strncpy(textfile, argv[1], ipos);
        textfile[ipos] = '\0';
        strcat(textfile, ".txt");
    } else {
        strncpy(textfile, argv[1], (int)(strlen(argv[1]) - 4));
        textfile[(int)(strlen(argv[1]) - 4)] = '\0';
        strcat(textfile, "_dlmtd.txt");
    }
    fpout = fopen(textfile, "w");

    labels[0] = (Label_record *)malloc(sizeof(Label_record));
    if (!populate(fpin, labels[0], &columns) == 0)
        EXIT_FAILURE;
    printf("%s\n", labels[0]->material);

// read the file, building a columns instance
//
// and corresponding
// labels array instance that contains the contents of the idoc file
// write a tab-delimited spreadsheet based on the columns and labels
// structs.
    free(labels[0]);
    free(textfile);
    fclose(fpout);
    fclose(fpin);
}

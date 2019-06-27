/**
 * @file idtoss.c
 * @author Jeff Kottiel
 * Main program for the IDoc (Intermediate Document) conversion program. The IDOC
 * text file is converted into a tab-delimited text formatted spreadsheet that
 * can be opened in Excel.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"

/** length of the "_dlmtd" suffix added to the output file name if the input file      */
/** is missing the _IDOC extension                                                     */
#define SUFFIX  6

/** global variable labels holds the labels records                                    */
Label_record **labels;

/** Keeps track of the number of records in labels                                     */
int labels_len = 0;

/** The total capacity of the records array                                            */
int labels_cap = 0;

/**
 *  Prints out a usage message indicating idtoss's command line options.
 */
void usage() {
    fprintf(stderr, "Usage: idtoss <idocfile.txt>\n");
}

int col_headings(FILE *fpout, Column_header *cols) {
    if (cols->label)          fprintf(fpout, "%s", "LABEL");
    if (cols->material)       fprintf(fpout, "%s", "\tMATERIAL");
    if (cols->tdline)         fprintf(fpout, "%s", "\tTDLINE");
    if (cols->templatenumber) fprintf(fpout, "%s", "\tTEMPLATENUMBER");
}

print_records(FILE *fpout) {
    for (int i = 1; i <= labels_len; i++) {
        fprintf(fpout, "\n");
        fprintf(fpout, "%s",    labels[i]->label);
        fprintf(fpout, "\t%s",  labels[i]->material);
        if (labels[i]->tdline)   fprintf(fpout, "\t%s", labels[i]->tdline);
        if (labels[i]->template) fprintf(fpout, "\t%s", labels[i]->template);
    }
}
/**
 * Starting point for idtoss. Process command line argument, then
 * process IDoc file.
 *
 * @param argc is the number of command-line arguments.
 * @param argv list of command-line arguments.
 * @return program exit status
 */
int main(int argc, char *argv[]) {
    FILE *fpin, *fpout;

    // dynamically allocate memory for output text file name
    char *textfile = (char *) malloc((int) strlen(argv[1]) * sizeof(char) + SUFFIX);

    // the spreadsheet column headings struct
    Column_header columns;

    // open the idoc file provided as a command line parameter
    if (argc != 2) {
        usage();
        return EXIT_FAILURE;
    }
    fpin = fopen(argv[1], "r");
    if (fpin == NULL) {
        printf("File not found.\n");
        return EXIT_FAILURE;
    }

    // determine and open output filename
    char *sp = stristr(argv[1], "_IDoc");
    if (sp) {
        int ipos = (int) (sp - argv[1]);
        strncpy(textfile, argv[1], (size_t) ipos);
        textfile[ipos] = '\0';
        strcat(textfile, ".txt");
    } else {
        strncpy(textfile, argv[1], (size_t) (strlen(argv[1]) - 4));
        textfile[(int) (strlen(argv[1]) - 4)] = '\0';
        strcat(textfile, "_dlmtd.txt");
    }
    fpout = fopen(textfile, "w");
    if (fpout == NULL) {
        fprintf(stderr, "Error opening output file \"%s.\"\n", textfile);
        fclose(fpin);
        return EXIT_FAILURE;
    };

    // initialize the labels array of pointers to Label_records
    if (labels_init() != 0) {
        fprintf(stderr, "Problem initializing labels array. Exiting.\n");
        fclose(fpout);
        fclose(fpin);
        return EXIT_FAILURE;
    }

    // populate the labels array with label records
    if (!populate_records(fpin, &columns) == 0) {
        printf("Error occurred on line %d of input file.\n", labels_len);
        fclose(fpout);
        fclose(fpin);
        return EXIT_FAILURE;
    }

    // print column headings
    col_headings(fpout, &columns);

    // print label records
    printf("%d record(s) written to %s\n", labels_len, textfile);
    print_records(fpout);

    // free dynamically allocated memory
    labels_free();
    free(textfile);

    // close files
    fclose(fpout);
    fclose(fpin);
}

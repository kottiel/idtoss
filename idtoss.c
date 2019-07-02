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

    if (cols->label)            fprintf(fpout, "%s", "LABEL");
    if (cols->material)         fprintf(fpout, "%s", "\tMATERIAL");
    if (cols->tdline)           fprintf(fpout, "%s", "\tTDLINE");
    if (cols->templatenumber)   fprintf(fpout, "%s", "\tTEMPLATENUMBER");
    if (cols->revision)         fprintf(fpout, "%s", "\tREVISION");
    if (cols->size)             fprintf(fpout, "%s", "\tSIZE");
    if (cols->level)            fprintf(fpout, "%s", "\tLEVEL");
    if (cols->quantity)         fprintf(fpout, "%s", "\tQUANTITY");
    if (cols->barcodetext)      fprintf(fpout, "%s", "\tBARCODETEXT");
    if (cols->ltnumber)         fprintf(fpout, "%s", "\tLTNUMBER");
    if (cols->caution)          fprintf(fpout, "%s", "\tCAUTION");
    if (cols->consultifu)       fprintf(fpout, "%s", "\tCONSULTIFU");
    if (cols->latex)            fprintf(fpout, "%s", "\tCONTAINSLATEX");
    if (cols->donotusedam)      fprintf(fpout, "%s", "\tDONOTUSEDAM");
    if (cols->latexfree)        fprintf(fpout, "%s", "\tLATEXFREE");
    if (cols->maninbox)         fprintf(fpout, "%s", "\tMANINBOX");
    if (cols->noresterile)      fprintf(fpout, "%s", "\tNORESTERILE");
    if (cols->nonsterile)       fprintf(fpout, "%s", "\tNONSTERILE");
    if (cols->pvcfree)          fprintf(fpout, "%s", "\tPVCFREE");
    if (cols->reusable)         fprintf(fpout, "%s", "\tREUSABLE");
    if (cols->singleuse)        fprintf(fpout, "%s", "\tSINGLEUSE");
    if (cols->singlepatientuse) fprintf(fpout, "%s", "\tSINGLEPATIENUSE");
    if (cols->electroifu)       fprintf(fpout, "%s", "\tELECTROSURIFU");
    if (cols->keepdry)          fprintf(fpout, "%s", "\tKEEPDRY");
    if (cols->expdate)          fprintf(fpout, "%s", "\tEXPDATE");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->ecrep)            fprintf(fpout, "%s", "\tECREP");
    if (cols->keepawayheat)     fprintf(fpout, "%s", "\tKEEPAWAYHEAT");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tMANUFACTURER");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tMFGDATE");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tPHTDEHP");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tPHTBBP");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tPHTDINP");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->lotgraphic)       fprintf(fpout, "%s", "\tLOTGRAPHIC");




    return 0;
}

void print_field(FILE *fpout, char *value) {
    fprintf(fpout, "\t");
    if (value)
        fprintf(fpout, "%s", value);
}

void print_records(FILE *fpout, Column_header *cols) {
    for (int i = 1; i <= labels_len; i++) {
        fprintf(fpout, "\n");
        fprintf(fpout, "%s",           labels[i]->label);
        fprintf(fpout, "\t%s",         labels[i]->material);
        if (cols->tdline)              print_field(fpout, labels[i]->tdline);
        if (cols->templatenumber)      print_field(fpout, labels[i]->template);
        if (cols->revision)            print_field(fpout, labels[i]->revision);
        if (cols->size)                print_field(fpout, labels[i]->size);
        if (cols->level)               print_field(fpout, labels[i]->level);
        if (cols->quantity)            print_field(fpout, labels[i]->quantity);
        if (cols->barcodetext)         print_field(fpout, labels[i]->gtin);
        if (cols->ltnumber)            print_field(fpout, labels[i]->ipn);

        if (cols->caution)
            if (labels[i]->caution)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->consultifu)
            if (labels[i]->consultifu)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->latex)
            if (labels[i]->latex)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->donotusedam)
            if (labels[i]->donotusedamaged)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->latexfree)
            if (labels[i]->latexfree)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->maninbox)
            if (labels[i]->maninbox)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->noresterile)
            if (labels[i]->noresterilize)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->nonsterile)
            if (labels[i]->nonsterile)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->pvcfree)
            if (labels[i]->pvcfree)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->reusable)
            if (labels[i]->reusable)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->singleuse)
            if (labels[i]->singleuseonly)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->singlepatientuse)
            if (labels[i]->singlepatientuse)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->electroifu)
            if (labels[i]->electroifu)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->keepdry)
            if (labels[i]->keepdry)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->expdate)
            if (labels[i]->expdate)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");




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
    Column_header columns = {0};

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
    print_records(fpout, &columns);

    // free dynamically allocated memory
    labels_free();
    free(textfile);

    // close files
    fclose(fpout);
    fclose(fpin);
}

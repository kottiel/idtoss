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

    if (cols->label)              fprintf(fpout, "%s", "LABEL");
    if (cols->material)           fprintf(fpout, "%s", "\tMATERIAL");
    if (cols->tdline)             fprintf(fpout, "%s", "\tTDLINE");
    if (cols->templatenumber)     fprintf(fpout, "%s", "\tTEMPLATENUMBER");
    if (cols->revision)           fprintf(fpout, "%s", "\tREVISION");
    if (cols->size)               fprintf(fpout, "%s", "\tSIZE");
    if (cols->level)              fprintf(fpout, "%s", "\tLEVEL");
    if (cols->quantity)           fprintf(fpout, "%s", "\tQUANTITY");
    if (cols->barcodetext)        fprintf(fpout, "%s", "\tBARCODETEXT");
    if (cols->ltnumber)           fprintf(fpout, "%s", "\tLTNUMBER");
    if (cols->caution)            fprintf(fpout, "%s", "\tCAUTION");
    if (cols->consultifu)         fprintf(fpout, "%s", "\tCONSULTIFU");
    if (cols->latex)              fprintf(fpout, "%s", "\tCONTAINSLATEX");
    if (cols->donotusedam)        fprintf(fpout, "%s", "\tDONOTUSEDAM");
    if (cols->latexfree)          fprintf(fpout, "%s", "\tLATEXFREE");
    if (cols->maninbox)           fprintf(fpout, "%s", "\tMANINBOX");
    if (cols->noresterile)        fprintf(fpout, "%s", "\tNORESTERILE");
    if (cols->nonsterile)         fprintf(fpout, "%s", "\tNONSTERILE");
    if (cols->pvcfree)            fprintf(fpout, "%s", "\tPVCFREE");
    if (cols->reusable)           fprintf(fpout, "%s", "\tREUSABLE");
    if (cols->singleuse)          fprintf(fpout, "%s", "\tSINGLEUSE");
    if (cols->singlepatientuse)   fprintf(fpout, "%s", "\tSINGLEPATIENUSE");
    if (cols->electroifu)         fprintf(fpout, "%s", "\tELECTROSURIFU");
    if (cols->keepdry)            fprintf(fpout, "%s", "\tKEEPDRY");
    if (cols->expdate)            fprintf(fpout, "%s", "\tEXPDATE");
    if (cols->lotgraphic)         fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->ecrep)              fprintf(fpout, "%s", "\tECREP");
    if (cols->keepawayheat)       fprintf(fpout, "%s", "\tKEEPAWAYHEAT");
    if (cols->lotgraphic)         fprintf(fpout, "%s", "\tMANUFACTURER");
    if (cols->lotgraphic)         fprintf(fpout, "%s", "\tMFGDATE");
    if (cols->lotgraphic)         fprintf(fpout, "%s", "\tPHTDEHP");
    if (cols->lotgraphic)         fprintf(fpout, "%s", "\tPHTBBP");
    if (cols->lotgraphic)         fprintf(fpout, "%s", "\tPHTDINP");
    if (cols->lotgraphic)         fprintf(fpout, "%s", "\tLOTGRAPHIC");
    if (cols->refnumber)          fprintf(fpout, "%s", "\tREFNUMBER");
    if (cols->ref)                fprintf(fpout, "%s", "\tREF");
    if (cols->rxonly)             fprintf(fpout, "%s", "\tRXONLY");
    if (cols->serial)             fprintf(fpout, "%s", "\tSERIAL");
    if (cols->sizelogo)           fprintf(fpout, "%s", "\tSIZELOGO");
    if (cols->tfxlogo)            fprintf(fpout, "%s", "\tTFXLOGO");
    if (cols->address)            fprintf(fpout, "%s", "\tADDRESS");
    if (cols->cautionstate)       fprintf(fpout, "%s", "\tCAUTIONSTATE");
    if (cols->ce0120)             fprintf(fpout, "%s", "\tCE0120");
    if (cols->coostate)           fprintf(fpout, "%s", "\tCOOSTATE");
    if (cols->distby)             fprintf(fpout, "%s", "\tDISTRIBUTEDBY");
    if (cols->ecrepaddress)       fprintf(fpout, "%s", "\tECREPADDRESS");
    if (cols->flgraphic)          fprintf(fpout, "%s", "\tFLGRAPHIC");
    if (cols->labelgraph1)        fprintf(fpout, "%s", "\tLABELGRAPH1");
    if (cols->labelgraph2)        fprintf(fpout, "%s", "\tLABELGRAPH2");
    if (cols->latexstate)         fprintf(fpout, "%s", "\tLATEXSTATEMENT");
    if (cols->logo1)              fprintf(fpout, "%s", "\tLOGO1");
    if (cols->logo2)              fprintf(fpout, "%s", "\tLOGO2");
    if (cols->logo3)              fprintf(fpout, "%s", "\tLOGO3");
    if (cols->logo4)              fprintf(fpout, "%s", "\tLOGO4");
    if (cols->logo5)              fprintf(fpout, "%s", "\tLOGO5");
    if (cols->manufacturedby)     fprintf(fpout, "%s", "\tMANUFACTUREDBY");
    if (cols->patentstatement)    fprintf(fpout, "%s", "\tPATENTSTA");
    if (cols->sterilitystatement) fprintf(fpout, "%s", "\tSTERILESTA");
    if (cols->sterilitytype)      fprintf(fpout, "%s", "\tSTERILITYTYPE");
    if (cols->version)            fprintf(fpout, "%s", "\tVERSION");
    if (cols->insertgraphic)      fprintf(fpout, "%s", "\tINSERTGRAPHIC");
    if (cols->mdr1)               fprintf(fpout, "%s", "\tMDR1");
    if (cols->mdr2)               fprintf(fpout, "%s", "\tMDR2");
    if (cols->mdr3)               fprintf(fpout, "%s", "\tMDR3");
    if (cols->mdr4)               fprintf(fpout, "%s", "\tMDR4");
    if (cols->mdr5)               fprintf(fpout, "%s", "\tMDR5");





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

        if (cols->lotgraphic)
            if (labels[i]->lotgraphic)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->ecrep)
            if (labels[i]->ecrep)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->keepawayheat)
            if (labels[i]->keepawayheat)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->manufacturer)
            if (labels[i]->manufacturer)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->mfgdate)
            if (labels[i]->mfgdate)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->phtdehp)
            if (labels[i]->phtdehp)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->phtbbp)
            if (labels[i]->phtbbp)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->phtdinp)
            if (labels[i]->phtdinp)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->refnumber)
            if (labels[i]->refnumber)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->ref)
            if (labels[i]->ref)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->rxonly)
            if (labels[i]->rxonly)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->serial)
            if (labels[i]->serial)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->sizelogo)
            if (labels[i]->sizelogo)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");
        if (cols->tfxlogo)
            if (labels[i]->tfxlogo)
                print_field(fpout, "Y");
            else
                print_field(fpout, "");

        if (cols->address)            print_field(fpout, labels[i]->address);
        if (cols->cautionstate)       print_field(fpout, labels[i]->cautionstatement);
        if (cols->ce0120)             print_field(fpout, labels[i]->cemark);
        if (cols->coostate)           print_field(fpout, labels[i]->coostate);
        if (cols->distby)             print_field(fpout, labels[i]->distby);
        if (cols->ecrepaddress)       print_field(fpout, labels[i]->ecrepaddress);
        if (cols->flgraphic)          print_field(fpout, labels[i]->flgraphic);
        if (cols->labelgraph1)        print_field(fpout, labels[i]->labelgraph1);
        if (cols->labelgraph2)        print_field(fpout, labels[i]->labelgraph2);
        if (cols->latexstate)         print_field(fpout, labels[i]->latexstatement);
        if (cols->logo1)              print_field(fpout, labels[i]->logo1);
        if (cols->logo2)              print_field(fpout, labels[i]->logo2);
        if (cols->logo3)              print_field(fpout, labels[i]->logo3);
        if (cols->logo4)              print_field(fpout, labels[i]->logo4);
        if (cols->logo5)              print_field(fpout, labels[i]->logo5);
        if (cols->manufacturedby)     print_field(fpout, labels[i]->manufacturedby);
        if (cols->patentstatement)    print_field(fpout, labels[i]->patentstatement);
        if (cols->sterilitystatement) print_field(fpout, labels[i]->sterilitystatement);
        if (cols->sterilitytype)      print_field(fpout, labels[i]->sterilitytype);
        if (cols->version)            print_field(fpout, labels[i]->version);
        if (cols->insertgraphic)      print_field(fpout, labels[i]->insertgraphic);
        if (cols->mdr1)               print_field(fpout, labels[i]->mdr1);
        if (cols->mdr2)               print_field(fpout, labels[i]->mdr2);
        if (cols->mdr3)               print_field(fpout, labels[i]->mdr3);
        if (cols->mdr4)               print_field(fpout, labels[i]->mdr4);
        if (cols->mdr5)               print_field(fpout, labels[i]->mdr5);
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

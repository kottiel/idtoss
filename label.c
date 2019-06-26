//
// Created by jkottiel on 6/24/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "label.h"


/**
    This function initializes the dynamically allocated Label_records structure.
    @return 0 if successful, -1 if unsuccessful.
*/
int labels_init() {

    // tracks labels Label_records structure capacity
    labels_cap = LABELS_INIT_SIZE;

    // one time only - dynamically allocate some pointers to Label_records
    if ((labels = (Label_record **) malloc(labels_cap * sizeof(Label_record *))) == NULL)
        return -1;

    return 0;
}

/**
    This function doubles the size of the dynamically allocated labels pointer array.
    @return 0 if successful, -1 if unsuccessful.
*/
static int labels_expand() {

    labels_cap *= 2;
    if ((labels = (Label_record **)realloc(labels, labels_cap * sizeof(Label_record*))) == NULL)
        return -1;
    else
        return 0;
}

/**
    This function frees the dynamically allocated labels structure.
    @return 0 if successful, -1 if unsuccessful.
*/
void labels_free() {

    for (int i = 0; i < labels_len; i++)
        free(labels[i]);

    free(labels);
}

/**
    This function dynamically allocates memory for a new labels record and copies
    the temporary Label_record built from the IDoc records into the labels array.
    @param newlabel contains the new label to add to the labels array
    @return 0 if successful, -1 if unsuccessful.
*/
int labels_append(Label_record *newlabel) {

    // obtain memory for the new new label
    if ((labels[labels_len] = (Label_record *)malloc(sizeof(Label_record))) == NULL)
        return -1;

    strcpy(labels[labels_len]->material, newlabel->material);
    strcpy(labels[labels_len]->label, newlabel->label);
    labels[labels_len]->tdline = newlabel->tdline;

    // expand labels structure as needed
    if (labels_len >= labels_cap) {
        if (!labels_expand() == 0)
            return -1;
    }
    return 0;
}

char *stristr(const char *str1, const char *str2) {
    const char *p1 = str1;
    const char *p2 = str2;
    const char *r = *p2 == 0 ? str1 : 0;

    while (*p1 != 0 && *p2 != 0) {
        if (tolower((unsigned char) *p1) == tolower((unsigned char) *p2)) {
            if (r == 0) {
                r = p1;
            }

            p2++;
        } else {
            p2 = str2;
            if (r != 0) {
                p1 = r + 1;
            }

            if (tolower((unsigned char) *p1) == tolower((unsigned char) *p2)) {
                r = p1;
                p2++;
            } else {
                r = 0;
            }
        }

        p1++;
    }

    return *p2 == 0 ? (char *) r : 0;
}

/**
 * left trim the whitespace from the left side of a string
 * @param str is the string to be trimmed
 * @return the number of spaces removed from str
 */
int ltrim(char *str) {

    int str_length = (int) strlen(str);
    if (str == NULL || str_length == 0)
        return 0;

    int i = 0;
    while (isspace(str[i]))
        i++;

    memmove(str, str + i, str_length - 1);
    return i;
}

/**
 * right trim the whitespace from the right side of a string
 * @param str is the string to be trimmed
 * @return the number of spaces removed from str
 */
int rtrim(char *str) {

    int str_length = (int) strlen(str);
    if (str == NULL || str_length == 0)
        return 0;

    int i = str_length - 1;
    while (isspace(str[i]))
        i--;

    str[i + 1] = '\0';
    return str_length - i - 2;
}

/**
 * a boolean function to check whether an IDOC record is a material record
 * @param str is the line of the input file being checked
 * @return true if the control portion contains the MATNR macro value
 */
bool is_matrec(char *str) {
    return (strncmp(str, MATNR, 6) == 0);
}

bool is_lblrec(char *str) {
    return (strncmp(str, LABEL, 6) == 0);
}

bool is_tdlinerec(char *str) {
    return (strncmp(str, TDLINE, 6) == 0);
}

bool is_descrec(char *str) {
    return (strncmp(str, DESCR, 6) == 0);
}


Column_header readheader(char *str) {

}

int populate_records(FILE *fpin, Column_header *cols) {

    // a temporary label that is populated before being added to the labels array
    Label_record lbl = {0};

    char str[MAX_CHARS];
    char tdline_tmp[TDLINE_LEN] = {0};
    char seq_num[SEQ_NUM_LEN];
    char *cp;
    char current_matnr[LRG] = {0};
    char current_label[SML] = {0};

    int cur_tdline_len = 0;

    int matl_seq_num = 0;
    int labl_seq_num = 0;
    int tdln_seq_num = 0;
    int desc_seq_num = 0;

    // discard the IDoc header record
    fgets(str, MAX_CHARS, fpin);

    // get the first matnr record
    fgets(str, MAX_CHARS, fpin);

    // allocate memory for the Column headings label_record
    labels[labels_len++] = (Label_record *)malloc(sizeof(Label_record));

    // allocate memory for the first label_record corresponding to row 1
    labels[labels_len] = (Label_record *)malloc(sizeof(Label_record));

    if (is_matrec(str)) {

        // get the matnr record seq_num
        strncpy(seq_num, str + MATNR_SEQ_NUM_START, SEQ_NUM_LEN);
        seq_num[SEQ_NUM_LEN] = '\0';
        matl_seq_num = atoi(seq_num);

        cp = str + LABEL_START;
        strncpy(lbl.material, cp, LRG);
        rtrim(lbl.material);
        strcpy(current_matnr, lbl.material);

    } else {
        printf("Missing material number record on line 2 of IDoc. Aborting\n");
        return -1;
    }

    while ((fgets(str, MAX_CHARS, fpin) != NULL) && (!is_matrec(str))) {

        // label record
        if (is_lblrec(str)) {
            // get the seq_num associated with this label record
            strncpy(seq_num, str + GNRIC_SEQ_NUM_START, SEQ_NUM_LEN);
            seq_num[6] = '\0';
            labl_seq_num = atoi(seq_num);

            if (labl_seq_num == matl_seq_num) {
                cp = str + LABEL_START;
                strncpy(lbl.label, cp, SML);
                lbl.label[SML - 1] = '\0';
                strcpy(current_label, lbl.label);
            } else {
                printf("Label sequence number %d doesn't match matnr sequence number. Aborting\n", labl_seq_num);
                return -1;
            }
        }

        // tdline record
        if (is_tdlinerec(str)) {
            if (strncmp(current_label, str + LABEL_CODE_START, 9) != 0) {
                printf("Label number mismatch on TDLINE record of IDoc. Aborting\n");
                return -1;
            } else {
                // get the seq_num associated with this tdline record
                strncpy(seq_num, str + GNRIC_SEQ_NUM_START, SEQ_NUM_LEN);
                seq_num[6] = '\0';
                tdln_seq_num = atoi(seq_num);

                if (labl_seq_num == matl_seq_num) {
                    cp = str + TDLINE_START;
                    strncpy(tdline_tmp, cp, TDLINE_LEN - 1);
                    ltrim(tdline_tmp);
                    rtrim(tdline_tmp);

                    // determine whether we're creating or appending tdline
                    if (cur_tdline_len > 0) {
                        //appending to an existing tdline
                        lbl.tdline = (char *) realloc(lbl.tdline, strlen(tdline_tmp) + cur_tdline_len + 1);
                        strcat(lbl.tdline, tdline_tmp);
                        cur_tdline_len += strlen(tdline_tmp);
                    } else {
                        //creating the first line of a tdline
                        lbl.tdline = (char *) malloc(strlen(tdline_tmp) + 1);
                        strcpy(lbl.tdline, tdline_tmp);
                        cur_tdline_len = strlen(tdline_tmp);
                    }
                } else {
                    printf("TDline sequence number mismatch on TDLINE record of IDoc. Aborting\n");
                    return -1;
                }
            }
        }
        // descr record
        if (is_descrec(str)) {

        }
    }
    labels_append(&lbl);
    return 0;
}

//
// Created by jkottiel on 6/24/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "label.h"

// the long string              long
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

    int str_length = (int)strlen(str);
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

    int str_length = (int)strlen(str);
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

int populate(FILE *fpin, Label_record *lp, Column_header *cols) {

    char str[MAX_CHARS];
    char tdline_tmp[TDLINE_LEN] = {0};
    char *cp;
    char current_matnr[LRG] = {0};
    char current_label[SML] = {0};

    // discard the IDoc header record
    fgets(str, MAX_CHARS, fpin);

    // get the first matnr record
    fgets(str, MAX_CHARS, fpin);

    if (is_matrec(str)) {
        cp = str + LABEL_START;
        strncpy(lp->material, cp, LRG);
        rtrim(lp->material);
        strcpy(current_matnr, lp->material);

    } else {
        printf("Missing material number record on line 2 of IDoc. Aborting\n");
        return -1;
    }

    while ((fgets(str, MAX_CHARS, fpin) != NULL) && (!is_matrec(str))) {

        // label record
        if (is_lblrec(str)) {
            cp = str + LABEL_START;
            strncpy(lp->label, cp, SML);
            lp->label[SML-1] = '\0';
            strcpy(current_label, lp->label);
        }

        // tdline record
        if (is_tdlinerec(str))
            if (strncmp(current_label, str + LABEL_CODE_START, 9) != 0) {
                printf("Label number mismatch on TDLINE record of IDoc. Aborting\n");
                return -1;
            } else {
                cp = str + TDLINE_START;
                strncpy(tdline_tmp, cp, TDLINE_LEN);
                ltrim(tdline_tmp);
                rtrim(tdline_tmp);

                // determine whether we're creating or appending tdline
                if (tdline_tmp) {
                    int cur_size = strlen(lp->tdline);
                    lp->tdline = realloc(lp->tdline, (int) strlen(tdline_tmp) + cur_size);
                    strcat(lp->tdline, tdline_tmp);
                }

            }

        // descr record
        //if (is_descrec(str))
    }
    return 0;
}

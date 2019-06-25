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
 * right trim the whitespace from the right side of a string
 * @param str
 * @param str_length
 * @return the number of spaces removed from str
 */
int rtrim(char *str, int str_length) {

    if (str == NULL || (int) strlen(str) == 0)
        return 0;

    int i = str_length - 1;
    while (isspace(str[i]))
        i--;

    str[i + 1] = '\0';
    return str_length - i - 2;
}

bool is_matnr(char *str) {
    return (strncmp(str, MATNR, 6) == 0);
}


Column_header readheader(char *str) {

}

int populate(FILE *fpin, Label_record *lp, Column_header *cols) {

    char str[MAX_CHARS];
    char *cp;

    // discard the IDoc header record
    fgets(str, MAX_CHARS, fpin);

    // get the first matnr record
    fgets(str, MAX_CHARS, fpin);
    if (is_matnr(str)) {
        cp = str + 63;
        strncpy(lp->material, cp, LRG);
        rtrim(lp->material, (int) strlen(lp->material));
    } else {
        printf("Missing material number record on line 2 of IDoc. Aborting");
        return -1;
    }

    while ((fgets(str, MAX_CHARS, fpin) != NULL) && (!is_matnr(str))) {

        // label record

        // tdline record

        // descr record
    }
    return 0;
}

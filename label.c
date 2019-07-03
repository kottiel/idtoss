//
// Created by jkottiel on 6/24/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include "label.h"

/*
 * Copyright (c) 1998, 2015 Todd C. Miller <millert@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns strlen(src); if retval >= dsize, truncation occurred.
 */
size_t strlcpy(char *dst, const char *src, size_t dsize)
{
    const char *osrc = src;
    size_t nleft = dsize;

    /* Copy as many bytes as will fit. */
    if (nleft != 0) {
        while (--nleft != 0) {
            if ((*dst++ = *src++) == '\0')
                break;
        }
    }

    /* Not enough room in dst, add NUL and traverse rest of src. */
    if (nleft == 0) {
        if (dsize != 0)
            *dst = '\0';		/* NUL-terminate dst */
        while (*src++)
            ;
    }

    return(src - osrc - 1);	/* count does not include NUL */
}

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
    if ((labels = (Label_record **) realloc(labels, labels_cap * sizeof(Label_record *))) == NULL)
        return -1;
    else
        return 0;
}

/**
    This function frees the dynamically allocated labels structure.
    @return 0 if successful, -1 if unsuccessful.
*/
void labels_free() {

    for (int i = 1; i <= labels_len; i++)
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
    if ((labels[++labels_len] = (Label_record *) calloc(1, sizeof(Label_record))) == NULL)
        return -1;

    strlcpy(labels[labels_len]->material,   newlabel->material,    sizeof(labels[labels_len]->material));
    strlcpy(labels[labels_len]->label,      newlabel->label,       sizeof(labels[labels_len]->label));

    labels[labels_len]->tdline = newlabel->tdline;

    strlcpy(labels[labels_len]->template,   newlabel->template,    sizeof(labels[labels_len]->template));
    strlcpy(labels[labels_len]->revision,   newlabel->revision,    sizeof(labels[labels_len]->revision));
    strlcpy(labels[labels_len]->size,       newlabel->size,        sizeof(labels[labels_len]->size));
    strlcpy(labels[labels_len]->level,      newlabel->level,       sizeof(labels[labels_len]->level));
    strlcpy(labels[labels_len]->quantity,   newlabel->quantity,    sizeof(labels[labels_len]->quantity));
    strlcpy(labels[labels_len]->gtin,       newlabel->gtin,        sizeof(labels[labels_len]->gtin));
    strlcpy(labels[labels_len]->ipn,        newlabel->ipn,         sizeof(labels[labels_len]->ipn));

    if (newlabel->caution)
        labels[labels_len]->caution          = true;
    if (newlabel->consultifu)
        labels[labels_len]->consultifu       = true;
    if (newlabel->consultifu)
        labels[labels_len]->consultifu       = true;
    if (newlabel->donotusedamaged)
        labels[labels_len]->donotusedamaged  = true;
    if (newlabel->latex)
        labels[labels_len]->latex            = true;
    if (newlabel->latexfree)
        labels[labels_len]->latexfree        = true;
    if (newlabel->maninbox)
        labels[labels_len]->maninbox         = true;
    if (newlabel->nonsterile)
        labels[labels_len]->nonsterile       = true;
    if (newlabel->noresterilize)
        labels[labels_len]->noresterilize    = true;
    if (newlabel->pvcfree)
        labels[labels_len]->pvcfree          = true;
    if (newlabel->reusable)
        labels[labels_len]->reusable         = true;
    if (newlabel->singlepatientuse)
        labels[labels_len]->singlepatientuse = true;
    if (newlabel->singleuseonly)
        labels[labels_len]->singleuseonly    = true;
    if (newlabel->expdate)
        labels[labels_len]->expdate          = true;
    if (newlabel->lotgraphic)
        labels[labels_len]->lotgraphic       = true;
    if (newlabel->ecrep)
        labels[labels_len]->ecrep            = true;
    if (newlabel->keepawayheat)
        labels[labels_len]->keepawayheat     = true;
    if (newlabel->manufacturer)
        labels[labels_len]->manufacturer     = true;
    if (newlabel->mfgdate)
        labels[labels_len]->mfgdate          = true;
    if (newlabel->phtdehp)
        labels[labels_len]->phtdehp          = true;
    if (newlabel->phtbbp)
        labels[labels_len]->phtbbp           = true;
    if (newlabel->phtdinp)
        labels[labels_len]->phtdinp          = true;
    if (newlabel->refnumber)
        labels[labels_len]->refnumber        = true;
    if (newlabel->ref)
        labels[labels_len]->ref              = true;
    if (newlabel->rxonly)
        labels[labels_len]->rxonly           = true;
    if (newlabel->serial)
        labels[labels_len]->serial           = true;
    if (newlabel->sizelogo)
        labels[labels_len]->sizelogo         = true;
    if (newlabel->tfxlogo)
        labels[labels_len]->tfxlogo          = true;

    strlcpy(labels[labels_len]->address,            newlabel->address,            sizeof(labels[labels_len]->address));
    strlcpy(labels[labels_len]->cautionstatement,   newlabel->cautionstatement,   sizeof(labels[labels_len]->cautionstatement));
    strlcpy(labels[labels_len]->cemark,             newlabel->cemark,             sizeof(labels[labels_len]->cemark));
    strlcpy(labels[labels_len]->coostate,           newlabel->coostate,           sizeof(labels[labels_len]->coostate));
    strlcpy(labels[labels_len]->distby,             newlabel->distby,             sizeof(labels[labels_len]->distby));
    strlcpy(labels[labels_len]->ecrepaddress,       newlabel->ecrepaddress,       sizeof(labels[labels_len]->ecrepaddress));
    strlcpy(labels[labels_len]->flgraphic,          newlabel->flgraphic,          sizeof(labels[labels_len]->flgraphic));
    strlcpy(labels[labels_len]->labelgraph1,        newlabel->labelgraph1,        sizeof(labels[labels_len]->labelgraph1));
    strlcpy(labels[labels_len]->labelgraph2,        newlabel->labelgraph2,        sizeof(labels[labels_len]->labelgraph2));
    strlcpy(labels[labels_len]->latexstatement,     newlabel->latexstatement,     sizeof(labels[labels_len]->latexstatement));
    strlcpy(labels[labels_len]->logo1,              newlabel->logo1,              sizeof(labels[labels_len]->logo1));
    strlcpy(labels[labels_len]->logo2,              newlabel->logo2,              sizeof(labels[labels_len]->logo2));
    strlcpy(labels[labels_len]->logo3,              newlabel->logo3,              sizeof(labels[labels_len]->logo3));
    strlcpy(labels[labels_len]->logo4,              newlabel->logo4,              sizeof(labels[labels_len]->logo4));
    strlcpy(labels[labels_len]->logo5,              newlabel->logo5,              sizeof(labels[labels_len]->logo5));
    strlcpy(labels[labels_len]->manufacturedby,     newlabel->manufacturedby,     sizeof(labels[labels_len]->manufacturedby));
    strlcpy(labels[labels_len]->patentstatement,    newlabel->patentstatement,    sizeof(labels[labels_len]->patentstatement));
    strlcpy(labels[labels_len]->sterilitystatement, newlabel->sterilitystatement, sizeof(labels[labels_len]->sterilitystatement));
    strlcpy(labels[labels_len]->sterilitytype,      newlabel->sterilitytype,      sizeof(labels[labels_len]->sterilitytype));
    strlcpy(labels[labels_len]->version,            newlabel->version,            sizeof(labels[labels_len]->version));
    strlcpy(labels[labels_len]->insertgraphic,      newlabel->insertgraphic,      sizeof(labels[labels_len]->insertgraphic));

    strlcpy(labels[labels_len]->mdr1,               newlabel->mdr1,               sizeof(labels[labels_len]->mdr1));
    strlcpy(labels[labels_len]->mdr2,               newlabel->mdr2,               sizeof(labels[labels_len]->mdr2));
    strlcpy(labels[labels_len]->mdr3,               newlabel->mdr3,               sizeof(labels[labels_len]->mdr3));
    strlcpy(labels[labels_len]->mdr4,               newlabel->mdr4,               sizeof(labels[labels_len]->mdr4));
    strlcpy(labels[labels_len]->mdr5,               newlabel->mdr5,               sizeof(labels[labels_len]->mdr5));

    // expand labels structure as needed
    if (labels_len >= labels_cap - 1) {
        if (labels_expand() != 0)
            return -1;
    }
    return 0;
}


/**
 * search a string for the existence of a substr - case insensitive.
 * @param str1 is the string that is being searched
 * @param str2 is the substring that we're searching for
 * @return the position of the substr in the search string or 0 if it does not exist
 */
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

/**
 * a boolean function to check whether an IDOC record is a label record
 * @param str is the line of the input file being checked
 * @return true if the control portion contains the LABEL macro value
 */
bool is_lblrec(char *str) {
    return (strncmp(str, LABEL, 6) == 0);
}

/**
 * a boolean function to check whether an IDOC record is a tdline record
 * @param str is the line of the input file being checked
 * @return true if the control portion contains the TDLINE macro value
 */
bool is_tdlinerec(char *str) {
    return (strncmp(str, TDLINE, 6) == 0);
}

/**
 * a boolean function to check whether an IDOC record is a description record
 * @param str is the line of the input file being checked
 * @return true if the control portion contains the DESCR macro value
 */
bool is_descrec(char *str) {
    return (strncmp(str, DESCR, 6) == 0);
}

int parse_descr(char *name, Label_record *lbl, char *value, char *graphicname, Column_header *cols) {

    if (strcmp(name, "TEMPLATENUMBER") == 0) {
        cols->templatenumber = true;
        strlcpy(lbl->template, value, sizeof(lbl->template));
    }
    if (strcmp(name, "REVISION") == 0) {
        cols->revision = true;
        strlcpy(lbl->revision, value, sizeof(lbl->revision));
    }
    if (strcmp(name, "SIZE") == 0) {
        cols->size = true;
        strlcpy(lbl->size, value, sizeof(lbl->size));
    }
    if (strcmp(name, "LEVEL") == 0) {
        cols->level = true;
        strlcpy(lbl->level, value, sizeof(lbl->level));
    }
    if (strcmp(name, "QUANTITY") == 0) {
        cols->quantity = true;
        strlcpy(lbl->quantity, value, sizeof(lbl->quantity));
    }
    if (strcmp(name, "BARCODETEXT") == 0) {
        cols->barcodetext = true;
        strlcpy(lbl->gtin, value, sizeof(lbl->gtin));
    }
    if (strcmp(name, "LTNUMBER") == 0) {
        cols->ltnumber = true;
        strlcpy(lbl->ipn, value, sizeof(lbl->ipn));
    }
    if (strncmp(name, "GRAPHIC", 7) == 0) {

        // determine which GRAPHICxx element is listed
        if (stristr(graphicname, "Caution")) {
            cols->caution = true;
            lbl->caution = true;
        } else if (stristr(graphicname, "ConsultIFU")) {
            cols->consultifu = true;
            lbl->consultifu = true;
        } else if (stristr(graphicname, "Latex Free")) {
            cols->latexfree = true;
            lbl->latexfree = true;
        } else if (stristr(graphicname, "Latex")) {
            cols->latex = true;
            lbl->latex = true;
        } else if (stristr(graphicname, "DoNotUsePakDam")) {
            cols->donotusedam = true;
            lbl->donotusedamaged = true;
        } else if (stristr(graphicname, "ManInBox")) {
            cols->maninbox = true;
            lbl->maninbox = true;
        } else if (stristr(graphicname, "DoNotRe-sterilize")) {
            cols->noresterile = true;
            lbl->noresterilize = true;
        } else if (stristr(graphicname, "Non-sterile")) {
            cols->nonsterile = true;
            lbl->nonsterile = true;
        } else if (stristr(graphicname, "PVC_Free")) {
            cols->pvcfree = true;
            lbl->pvcfree = true;
        } else if (stristr(graphicname, "REUSABLE")) {
            cols->reusable = true;
            lbl->reusable = true;
        } else if (stristr(graphicname, "SINGLEUSE")) {
            cols->singleuse = true;
            lbl->singleuseonly = true;
        } else if (stristr(graphicname, "SINGLEPATIENUSE")) {
            cols->singlepatientuse = true;
            lbl->singlepatientuse = true;
        } else if (stristr(graphicname, "ElectroSurIFU")) {
            cols->electroifu = true;
            lbl->electroifu = true;
        } else if (stristr(graphicname, "KeepDry")) {
            cols->keepdry = true;
            lbl->keepdry = true;
        }
    }
        if (strcmp(name, "EXPDATE") == 0) {
            cols->expdate = true;
            lbl->expdate = true;
        }
        if (strcmp(name, "LOTGRAPHIC") == 0) {
            cols->lotgraphic = true;
            lbl->lotgraphic = true;
        }
        if (strcmp(name, "ECREP") == 0) {
            cols->ecrep = true;
            if (strncasecmp(value, "y", 1) == 0)
                lbl->ecrep = true;
            else
                lbl->ecrep = false;
        }

        if (strcmp(name, "KEEPAWAYHEAT") == 0) {
            cols->keepawayheat = true;
            lbl->keepawayheat = true;
        }
        if (strcmp(name, "MANUFACTURER") == 0) {
            cols->manufacturer = true;
            lbl->manufacturer = true;
        }
        if (strcmp(name, "MFGDATE") == 0) {
            cols->mfgdate = true;
            lbl->mfgdate = true;
        }
        if (strcmp(name, "PHTDEHP") == 0) {
            cols->phtdehp = true;
            lbl->phtdehp = true;
        }
        if (strcmp(name, "PHTBBP") == 0) {
            cols->phtbbp = true;
            lbl->phtbbp = true;
        }
        if (strcmp(name, "PHTDINP") == 0) {
            cols->phtdinp = true;
            lbl->phtdinp = true;
        }
        if (strcmp(name, "REFNUMBER") == 0) {
            cols->refnumber = true;
            lbl->refnumber = true;
        }
        if (strcmp(name, "REF") == 0) {
            cols->ref = true;
            lbl->ref = true;
        }
        if (strcmp(name, "RXONLY") == 0) {
            cols->rxonly = true;
            lbl->rxonly = true;
        }
        if (strcmp(name, "SERIAL") == 0) {
            cols->serial = true;
            lbl->serial = true;
        }
        if (strcmp(name, "SIZELOGO") == 0) {
            cols->sizelogo = true;
            lbl->sizelogo = true;
        }
        if (strcmp(name, "TFXLOGO") == 0) {
            cols->tfxlogo = true;
            lbl->tfxlogo = true;
        }
        if (strcmp(name, "PHTDINP") == 0) {
            cols->phtdinp = true;
            lbl->phtdinp = true;
        }
        if (strcmp(name, "ADDRESS") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "CAUTIONSTATE") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "CE0120") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "COOSTATE") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "DISTRIBUTEDBY") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "ECREPADDRESS") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "FLGRAPHIC") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LABELGRAPH1") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LABELGRAPH2") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LATEXSTATEMENT") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LOGO1") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LOGO2") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LOGO3") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LOGO4") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "LOGO5") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "MANUFACTUREDBY") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "PATENTSTA") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "STERILESTA") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "STERILITYTYPE") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "VERSION") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "INSERTGRAPHIC") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "MDR1") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "MDR2") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "MDR3") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "MDR4") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        if (strcmp(name, "MDR5") == 0) {
            cols->address = true;
            strlcpy(lbl->address, value, sizeof(lbl->address));
        }
        return 0;

}

/**
 * Read through the entire IDoc input file and build an array of Label_records
 * @param fpin is the IDoc input file
 * @param cols is a struct that contains column headings
 * @return 0 if successfully processed or -1 if there was an error
 */
int populate_records(FILE *fpin, Column_header *cols) {

    // a temporary label that is populated before being added to the labels array
    Label_record lbl = {0};
    Label_record emptylbl = {0};

    char str[MAX_CHARS];
    char pathngraphic[MAX_PATHNGRAPHIC];
    char tdline_tmp[TDLINE_LEN] = {0};

    char desc_col_name[MED + 1] = {0};
    char desc_value[MED + 1] = {0};
    char desc_graphic[MED + 1] = {0};

    char seq_num[SEQ_NUM_LEN + 1];
    char *cp;
    char current_matnr[LRG] = {0};
    char current_label[SML] = {0};

    int cur_tdline_len = 0;

    int matl_seq_num = 0;
    int prim_label_seq_num = 0;
    int sec_label_seq_num = 0;
    int tdln_seq_num = 0;
    int desc_seq_num = 0;

    // discard the IDoc header record
    fgets(str, MAX_CHARS, fpin);

    while (fgets(str, MAX_CHARS, fpin) != NULL) {

        // matnr record
        if (is_matrec(str)) {
            if (strlen(lbl.label) == LABEL_LEN) {

                // if the previous record didn't have a dedicated matnr record
                strcpy(lbl.material, current_matnr);

                labels_append(&lbl);
                lbl = emptylbl;
                cur_tdline_len = 0;
            }

            // get the matnr record seq_num
            strncpy(seq_num, str + PRIMARY_SEQ_NUM_START, SEQ_NUM_LEN);
            seq_num[SEQ_NUM_LEN] = '\0';

            matl_seq_num = atoi(seq_num);

            cp = str + LABEL_START;
            strncpy(lbl.material, cp, LRG);
            rtrim(lbl.material);
            strcpy(current_matnr, lbl.material);

            // add material to the cols column headings struct
            cols->material = true;
        }

        // label record
        if (is_lblrec(str)) {
            if (strlen(lbl.label) == LABEL_LEN) {

                // if the previous record didn't have a dedicated matnr record
                strcpy(lbl.material, current_matnr);

                labels_append(&lbl);
                lbl = emptylbl;
                cur_tdline_len = 0;
            }

            // get the primary seq_num associated with this label record
            strncpy(seq_num, str + PRIMARY_SEQ_NUM_START, SEQ_NUM_LEN);
            seq_num[SEQ_NUM_LEN] = '\0';
            prim_label_seq_num = atoi(seq_num);

            // get the secondary seq_num associated with this label record
            strncpy(seq_num, str + SECONDARY_SEQ_NUM_START, SEQ_NUM_LEN);
            seq_num[SEQ_NUM_LEN] = '\0';
            sec_label_seq_num = atoi(seq_num);

            if (sec_label_seq_num == matl_seq_num) {
                cp = str + LABEL_START;
                strncpy(lbl.label, cp, SML);
                lbl.label[SML - 1] = '\0';
                strcpy(current_label, lbl.label);

                // add label to the cols column headings struct
                cols->label = true;

            } else {
                printf("Label sequence number %d doesn't match matnr sequence number.", sec_label_seq_num);
                printf("Aborting\n");
                return -1;
            }
        }

        // tdline record
        if (is_tdlinerec(str)) {
            if (strncmp(current_label, str + LABEL_CODE_START, LABEL_LEN) != 0) {
                printf("Label number mismatch on TDLINE record of IDoc. Aborting\n");
                return -1;
            } else {
                // get the seq_num associated with this tdline record
                strncpy(seq_num, str + SECONDARY_SEQ_NUM_START, SEQ_NUM_LEN);
                seq_num[SEQ_NUM_LEN] = '\0';
                tdln_seq_num = atoi(seq_num);

                if (tdln_seq_num == prim_label_seq_num) {
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

                    // add tdline to the cols column headings struct
                    cols->tdline = true;

                } else {
                    printf("TDline sequence number mismatch on TDLINE record of IDoc. Aborting\n");
                    return -1;
                }
            }
        }
        // descr record
        if (is_descrec(str)) {

            // get the seq_num associated with this descr record
            strncpy(seq_num, str + SECONDARY_SEQ_NUM_START, SEQ_NUM_LEN);
            seq_num[SEQ_NUM_LEN] = '\0';
            desc_seq_num = atoi(seq_num);

            if (desc_seq_num == prim_label_seq_num) {

                // get descr column name
                cp = str + DESC_COL_START;
                strncpy(desc_col_name, cp, MED);
                ltrim(desc_col_name);
                rtrim(desc_col_name);

                // get desc value
                cp = str + DESCR_VAL_START;
                strncpy(desc_value, cp, MED);
                ltrim(desc_value);
                rtrim(desc_value);

                // get desc GRAPHIC name (if applicable)
                if (strncmp(desc_col_name, "GRAPHIC", 7) == 0) {

                    cp = str + DESCR_GRAPHIC_START;
                    strlcpy(pathngraphic, str + DESCR_GRAPHIC_START, sizeof(pathngraphic));
                }


                parse_descr(desc_col_name, &lbl, desc_value, pathngraphic, cols);

            } else {
                printf("Descr sequence number mismatch on descr record of IDoc. Aborting\n");
                return -1;
            }
        }
    }
    // if the previous record didn't have a dedicated matnr record
    strcpy(lbl.material, current_matnr);

    // print the last record
    labels_append(&lbl);

    return 0;
}

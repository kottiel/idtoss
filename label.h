//
// Created by jkottiel on 6/22/2019.
//

#ifndef IDTOSS_LABEL_H
#define IDTOSS_LABEL_H

/**
    @file label.h
    @author Jeff Kottiel (jhkottie)
    Together with txtfile.c, this component is responsible for functionality
    pertaining to building the tab-delimited spreadsheet file.
*/

#ifndef LABEL_H
#define LABEL_H

#include <stdbool.h>

/* the spreadsheet's initial capacity */
#define INITIAL_CAP                      3
#define MAX_CHARS                      526
#define MAX_FILENAMELEN                 31
#define MATNR                      "Z2BTMH"
#define LABEL                      "Z2BTLH"
#define TDLINE                     "Z2BTTX"
#define DESCR                      "Z2BTLC"
#define LABEL_START                     63
#define LABEL_CODE_START                82
#define TDLINE_START                    92

#define MAX_COLUMNS          1000
#define TDLINE_LEN            131
#define SEQ_NUM_LEN             6
#define MED                    30
#define LRG                    41
#define SML                    10
#define MATERIAL_REC          "02"

/* the column that the matnr seq number begins*/
#define MATNR_SEQ_NUM_START    49

/* the column that a non-matnr seq number begins*/
#define GNRIC_SEQ_NUM_START    55


#define LABEL_REC             "03"
#define TDLINE_REC            "04"
#define CHAR_REC              "04"


#define TAB                  '\t'

/** global variable spreadsheet that holds the label records  */
extern char **spreadsheet;
extern int spreadsheet_cap;
extern int spreadsheet_row_number;


/**

*/
typedef struct {
    char material[LRG];
    char coostate[LRG];
    char address[MED];
    /* char barcode1[MED];*/
    char cautionstatement[MED];
    char cemark[MED];

    char distby[MED];
    char ecrepaddress[MED];
    char flgraphic[MED];
    char insertgraphic[MED];
    char labelgraph1[MED];
    char labelgraph2[MED];
    char latexstatement[MED];
    char logo1[MED];
    char logo2[MED];
    char logo3[MED];
    char logo4[MED];
    char logo5[MED];
    char mdr1[MED];
    char mdr2[MED];
    char mdr3[MED];
    char mdr4[MED];
    char mdr5[MED];
    char manufacturedby[MED];
    char patentstatement[MED];
    char size[MED];
    char sterilitystatement[MED];
    char sterilitytype[MED];
    char temperaturerange[MED];
    char version[MED];
    char gtin[15];
    char level[11];
    char label[10];
    char ipn[10];
    char quantity[LRG];
    char template[8];
    char bomlevel[5];
    char revision[4];
    char *tdline;

    bool caution;
    bool consultifu;
    bool donotusedamaged;
    bool ecrep;
    bool electroifu;
    bool expdate;
    bool keepdry;
    bool keepawayheat;
    bool latex;
    bool latexfree;
    bool lotgraphic;
    bool maninbox;
    bool manufacturer;
    bool mfgdate;
    bool nonsterile;
    bool noresterilize;
    bool phtbbp;
    bool phtdinp;
    bool phtdehp;
    bool pvcfree;
    bool ref;
    bool refnumber;
    bool reusable;
    bool serial;
    bool sizelogo;
    bool rxonly;
    bool singlepatientuse;
    bool singleuseonly;
    bool tfxlogo;
} Label_record;

/**

*/
typedef struct {
    unsigned short address;
    unsigned short barcodetext;
    /*unsigned short barcode1;*/
    unsigned short bomlevel;
    unsigned short caution;
    unsigned short cautionstate;
    unsigned short ce0120;
    unsigned short consultifu;
    unsigned short coostate;
    unsigned short distby;
    unsigned short donotusedam;
    unsigned short ecrep;
    unsigned short ecrepaddress;
    unsigned short electroifu;
    unsigned short expdate;
    unsigned short flgraphic;
    unsigned short insertgraphic;
    unsigned short keepawayheat;
    unsigned short keepdry;
    unsigned short label;
    unsigned short labelgraph1;
    unsigned short labelgraph2;
    unsigned short latex;
    unsigned short latexfree;
    unsigned short latexstate;
    unsigned short manufacturedby;
    unsigned short level;
    unsigned short logo1;
    unsigned short logo2;
    unsigned short logo3;
    unsigned short logo4;
    unsigned short logo5;
    unsigned short mdr1;
    unsigned short mdr2;
    unsigned short mdr3;
    unsigned short mdr4;
    unsigned short mdr5;
    unsigned short lotgraphic;
    unsigned short ltnumber;
    unsigned short maninbox;
    unsigned short manufacturer;
    unsigned short material;
    unsigned short mfgdate;
    unsigned short nonsterile;
    unsigned short noresterile;
    unsigned short patentstatement;
    unsigned short phtdehp;
    unsigned short phtbbp;
    unsigned short phtdinp;
    unsigned short pvcfree;
    unsigned short quantity;
    unsigned short ref;
    unsigned short refnumber;
    unsigned short reusable;
    unsigned short revision;
    unsigned short rxonly;
    unsigned short serial;
    unsigned short singleuse;
    unsigned short singlepatientuse;
    unsigned short size;
    unsigned short sizelogo;
    unsigned short sterilitystatement;
    unsigned short sterilitytype;
    unsigned short tdline;
    unsigned short templatenumber;
    unsigned short tfxlogo;
    unsigned short version;

} Column_header;

/**

*/
Column_header readheader(char *str);

char* stristr( const char* str1, const char* str2 );

/**
 */
int populate_record(FILE *fpin, Label_record *lp, Column_header *cols);
bool is_matrec(char *str);

#endif //LABEL_H
#endif //IDTOSS_LABEL_H

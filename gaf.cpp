/***************************************************************
 **   DLSTORM
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         gaf.cpp
 **   Description:  GAF manager
 **   Author:       Seth Parson aka Deadline
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 ***************************************************************/

#include "gaf.h"

CGAF *pGAF;
char  szTopCWD[1024];
char  szCWD[1024];
char  szDir[1024];

GAF_SCANCALLBACK dir_callback(GAFFile_ElmHeader *ElmInfo, LPSTR FullPath) {
    switch (ElmInfo->Type) {
        case GAFELMTYPE_FILE: printf("%20ld %-25s \n", ElmInfo->FileSize, ElmInfo->Name); break;
        case GAFELMTYPE_DIR: printf("====== DIR: %-25s \n", ElmInfo->Name); break;
        default: break;
    }
    return 0;
}

GAF_SCANCALLBACK dir_callback_extract(GAFFile_ElmHeader *ElmInfo, LPSTR FullPath) {
    switch (ElmInfo->Type) {
        case GAFELMTYPE_FILE:
            printf("%20ld %s/%-25s \n", ElmInfo->FileSize, szDir, ElmInfo->Name);  // wut
            pGAF->ExtractFile_ToFile(va("%s/%s", szDir, ElmInfo->Name), ElmInfo->Name);
            break;
        case GAFELMTYPE_DIR:
            printf("====== DIR: %-25s \n", ElmInfo->Name);
            strcpy(szDir, ElmInfo->Name);
            dlcs_chdir(szTopCWD);
            if (!dlcs_isdir(szDir)) dlcs_mkdir(szDir);
            dlcs_chdir(szDir);
            break;
        default: break;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);
    char file[1024];
    memset(file, 0, 1024);
    strcpy(file, "gaf.gaf");
    char res[1024];
    memset(res, 0, 1024);
    unsigned int i;
    memset(szTopCWD, 0, 1024);
    strcpy(szTopCWD, dlcs_getcwd());

    pGAF = new CGAF();

    if (argc < 3) {
        print_help();
        return 1;
    }

    strcpy(file, argv[1]);

    switch (argv[2][1]) {
        case 'a':
            for (i = 3; i < strlen(argv[2]); i++) {
                res[i - 3] = argv[2][i];
            }
            printf("Adding file %s to %s...\n", res, file);
            pGAF->Open(file);
            pGAF->AddFile(res, res);
            pGAF->Close();
            break;

        case 'z':
            for (i = 3; i < strlen(argv[2]); i++) {
                res[i - 3] = argv[2][i];
            }
            printf("Adding directory %s to %s...\n", res, file);
            pGAF->Open(file);
            pGAF->AddDir(res);
            pGAF->Close();
            break;

        case 'd':
            for (i = 3; i < strlen(argv[2]); i++) {
                res[i - 3] = argv[2][i];
            }
            printf("Removing file %s from %s...\n", res, file);
            pGAF->Open(file);
            pGAF->RemoveFile(res);
            pGAF->Close();
            break;

        case 'c':
            for (i = 3; i < strlen(argv[2]); i++) {
                res[i - 3] = argv[2][i];
            }
            printf("Removing directory %s from %s...\n", res, file);
            pGAF->Open(file);
            pGAF->RemoveDir(res);
            pGAF->Close();
            break;

        case 'x':
            printf("EXTRACT\n");
            printf("Working in %s\n", szTopCWD);
            memset(szDir, 0, 1024);
            memset(szCWD, 0, 1024);
            dlcs_chdir(szTopCWD);
            pGAF->Open(file);
            pGAF->ScanTree("", (GAF_SCANCALLBACK)dir_callback_extract);
            pGAF->Close();
            break;

        case 'l':
            printf("LIST CONTENTS\n");
            pGAF->Open(file);
            pGAF->ScanTree("", (GAF_SCANCALLBACK)dir_callback);
            pGAF->Close();
            break;

        default:
            printf("Unknown or missing command. \n");
            print_help();
            break;
    }

    DEL(pGAF);

    return 0;
}

void print_help(void) {
    printf("\n");

    printf("GAF Game Archive File by Seth Parson sethcoder.com\n");
    printf("=======================================================\n");
    printf("VERSION: %s compiled %s @ %s \n", GAFMAKEVERSION, __DATE__, __TIME__);
    printf("=======================================================\n");
    printf("USAGE:\n");
    printf("gaf <gaf file> [-a:<filename>] [-z:<dir>] [-d:<file>] [-c:<dir>] [-x]\n");
    printf("-a add a file to the GAF (This will create a new GAF if it does not exist)\n");
    printf("-z add directory to the GAF (This will create a new GAF if it does not exist)\n");
    printf("-d remove a file from the GAF\n");
    printf("-c remove a directory from the GAF\n");
    printf("-x extract GAF into the working directory\n");
    printf("-l list contents\n");
}

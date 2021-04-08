#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct T_T9 {

    const char * T9;

} T_T9;

typedef struct T_PBX {

    char * number;
    char * name;

} TPBX;

int validateT9 (char * name, char * number) {

    T_T9 m_T9[10];
    int result = 0;
    m_T9[0].T9 = "<>/`~.,?':;]}[{=+-_)(*&^%$#@!0";
    m_T9[1].T9 = " 1";
    m_T9[2].T9 = "abc2ABC";
    m_T9[3].T9 = "def3DEF";
    m_T9[4].T9 = "ghi4GHI";
    m_T9[5].T9 = "jkl5JKL";
    m_T9[6].T9 = "mno6MNO";
    m_T9[7].T9 = "pqrs7PQRS";
    m_T9[8].T9 = "tuv8TUV";
    m_T9[9].T9 = "wxyz9WXYZ";



    // Bart
    // 2278
    // i = 0 i < 4
    // j = 0; j < 7 // number[j] = 2

    for (int i = 0; i < (int)strlen(name); i++) {
        for (int j = 0; j < (int)strlen(m_T9[(int)(number[i]-'0')].T9); j++) {
            if ( name[i] == m_T9[(int)(number[i]-'0')].T9[j]) {
                result++;//
                break;
            }

        }
    }



    return result;


}




int * searchMatches(TPBX * data, int cntDB, char * number, int * cnt_dups) {

    int * indexes;
    int max = 1;

    indexes = (int*)malloc(1 * sizeof(int));




    for (int i = 0; i < cntDB; i++) {
        if (strcmp(data[i].number, number) == 0) {
            if (max >= *cnt_dups) {
                indexes = (int*)realloc(indexes, max * sizeof (int) );
                max += 100;
            }

            indexes[*cnt_dups] = i;
            (*cnt_dups)++;

        }
        else if ((int)strlen(data[i].name) != (int)strlen(number))
            continue;
        else if ((int)strlen(data[i].name) == validateT9(data[i].name, number)) {
            if (max >= *cnt_dups) {
                indexes = (int*)realloc(indexes, max * sizeof (int) );
                max += 100;
            }

            indexes[*cnt_dups] = i;
            (*cnt_dups)++;
        }

    }

    return indexes;
}

char validateNumber(char * number) {
    char * ptr;
    for (ptr = number; *ptr != '\0'; ptr++) {
        if (*ptr != '0' && *ptr != '1' && *ptr != '2' && *ptr != '3' && *ptr != '4'
            && *ptr != '5' && *ptr!= '6' && *ptr != '7' && *ptr != '8' && *ptr != '9')
            return 0;
    }

    return 1;
}


char readData(TPBX ** data, int * cntDB, int * max) {

    size_t buff = 0;
    char operation;
    char * readedLine = NULL;
    char * name = NULL,  *number = NULL;
    int readed_first;

    if ( getline(&readedLine, &buff, stdin) == -1 ) {

        free(readedLine);
        return -1;
    }


    number = (char*)malloc(strlen(readedLine) * sizeof(char));


    if (sscanf (readedLine, "%c %s %n", &operation, number, &readed_first) != 2
        || (operation!= '?' && operation != '+' && operation != '-')
        || !validateNumber(number)
        || readedLine[1] != ' ') {
        free(number);
        free(readedLine);
        printf("INVALID COMMAND\n");
        return 1;
    }

    if (operation == '+') {
        char quote1;
        name = (char*)malloc(strlen(readedLine) * sizeof(char));
        if (sscanf (readedLine + readed_first, "%c%[^\n]%*c", &quote1, name) != 2
            || quote1 != '"'
            || name[strlen(name) - 1] != '"' ) {
            free(readedLine);
            free(number);
            free(name);
            printf("INVALID COMMAND\n");
            return 1;

        }
        readedLine[strlen(readedLine) - 1] = '\0';
        // printf("%s %lu", name, strlen(name));
        // // printf("readed second %d\n", readed_second);
        // printf("readed first %d\n", readed_first);
        // printf("readed at all %lu\n", strlen(readedLine));
        for (int i = readed_first + (int)strlen(name) + 1; i < (int)strlen(readedLine); i++)
        {
            if (readedLine[i] != '\0') {
                free(readedLine);
                free(number);
                free(name);
                printf("INVALID COMMAND\n");
                return 1;
            }
        }

        name[strlen(name)-1] ='\0';

        for (int i = 0; i < *cntDB; i++) {
            if (strcmp((*data)[i].number, number) == 0) {
                char * tmp = (*data)[i].name;
                (*data)[i].name = name;
                free(tmp);
                printf("UPDATED\n");
                free(readedLine);
                free(number);
                return 3;
            }

        }

        if ( *cntDB >= *max ) { // give some memory to array of structs
            if ( *max < 1000 ) { *max = *max + 100; }
            else { *max += *max / 2; }
            *data = (TPBX*)realloc(*data, *max * sizeof ( **data ) );
        }
        //printf("Size inside function: %lu %lu", sizeof(*data), sizeof(**data));

        (*data)[*cntDB].number = number;
        (*data)[*cntDB].name = name;
        (*cntDB)++;
        printf("NEW\n");
        free(readedLine);

        return 2;
    }


    if (operation == '-') {
        if (*cntDB == 0) {
            free(readedLine);
            free(number);
            printf("INVALID COMMAND\n");
            return 1;

        }
        for (int i = 0; i < *cntDB; i++) {
            if (strcmp((*data)[i].number, number) == 0) {

                if (i == *cntDB - 1) {
                    free((*data)[i].number);
                    free((*data)[i].name);
                    (*cntDB)--;
                    printf("DELETED\n");
                    free(number);
                    free(readedLine);
                    return 4;
                }

                char * tmp1 = (*data)[i].number;
                char * tmp2 = (*data)[i].name;

                for (int j = i; j < *cntDB; j++) {

                    (*data)[j].number = (*data)[j+1].number;
                    (*data)[j].name = (*data)[j+1].name;
                    if (j + 1 == *cntDB - 1) {
                        (*data)[j+1].number = tmp1;
                        (*data)[j+1].name = tmp2;
                        free((*data)[j+1].number);
                        free((*data)[j+1].name);
                        (*cntDB)--;
                        free(number);
                        free(readedLine);
                        printf("DELETED\n");
                        return 4;
                    }

                }
            }
        }
        free(readedLine);
        free(number);
        printf("NOT FOUND\n");
        return 1;
    }

    if (operation == '?') {

        if (*cntDB == 0) {
            printf("INVALID COMMAND\n");
            free(number);
            free(readedLine);
            return 1;
        }

        int cnt_dups = 0;
        int * result = searchMatches(*data, *cntDB, number, &cnt_dups);

        if (cnt_dups == 0) {
            printf("NOT FOUND\n");
            free(result);
            free(number);
            free(readedLine);
            return 1;
        }

        else if (cnt_dups == 1) {
            printf("FOUND %s (%s)\n", (*data)[result[0]].number, (*data)[result[0]].name);
            free(result);
            free(number);
            free(readedLine);
            return 5;
        }

        else if (cnt_dups > 1) {
            printf("AMBIGUOUS (%d matches)\n", cnt_dups);
            free(result);
            free(number);
            free(readedLine);
            return 6;
        }


        // for (int i = 0; i < cnt_dups; i++) {
        // 	printf("%d\n", result[i]);
        // }

    }






    return 0;


}


int main() {


    TPBX * data = NULL;
    int cntDB = 0;
    char response;
    int max = 0;
    // data = (TPBX*)malloc(1000 * sizeof(*data));
    //printf("Size of data %lu %lu\n", sizeof(*data), sizeof(data));
    printf("PBX configuration (+ = set, - = delete, ? = test, EOF = quit):\n");
    while(1) {
        response = readData(&data, &cntDB, &max);
        // printf("Size of data %lu\n", sizeof(*data));
        if (response == -1) {
            break;
        }


    }
    for (int i = 0; i < cntDB; i++) {
        free(data[i].name);
        free(data[i].number);
    }

    free(data);
    // printf("------Database------- \n");
    // for (int i = 0; i < cntDB; i++) {
    //     printf("%s %s\n", data[i].number, data[i].name);
    // }




    return 0;
}

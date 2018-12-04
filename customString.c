//
// Created by herojeff on 2018/12/04.
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "customString.h"
#include "dataType.h"

char* tolowerString(char *content){
    char* return_array = malloc(sizeof(char));
    for(int i=0;i<strlen(content);i++){
        return_array = realloc(return_array, sizeof(char)*(i+1));
        *(return_array+i) = tolower(*(content+i));
    }
    return_array = realloc(return_array, sizeof(char)*strlen(content)+1);
    *(return_array+strlen(content)) = '\0';
    return return_array;
}
char* tolowerCapitalizer(char *content){
    char* return_array = malloc(sizeof(char)+1);
    *(return_array+0) = toupper(*content);
    int bigFlag = 0;
    for(int i=1;i<strlen(content);i++){
        return_array = realloc(return_array, sizeof(char)*(i+1));
        if(!bigFlag) {
            *(return_array + i) = tolower(*(content + i));
        }
        else{
            *(return_array + i) = toupper(*(content + i));
            bigFlag = 0;
        }

        if(*(return_array + i) == '-'){
            bigFlag = 1;
        }
    }
    return_array = realloc(return_array, sizeof(char)*strlen(content)+1);
    *(return_array+strlen(content)) = '\0';
    return return_array;
}
char *split_front(char *str, const char *delim) {
    char *p = strstr(str, delim);

    if (p == NULL) {
        return NULL;
    }
    else {
        *p = '\0';
        return str;
    }
}
char *split_back(char *str, const char *delim){
    char *p = strstr(str, delim);

    if (p == NULL) {
        return NULL;
    }
    else {
        return p + strlen(delim);
    }
}
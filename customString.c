//
// Created by herojeff on 2018/12/04.
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "customString.h"
#include "dataType.h"

int stringIncludesMarker(char *string){ //Wait, wrong method name here.
    if(strstr(string, "::")==NULL)
        return 0;
    else
        return 1;
}
int forcedIntegerInput(int figures){
    int result=0;
    while (1){
        char temp = getchar();
        if(temp == '\n'){
            break;
        }
        else if(isdigit(temp)){
            result = result*10 + (temp - '0');
        }
        else{
            return FAIL_NOT_A_NUMBER;
        }
    }
    if(result>=pow(10.0, figures)){
        return FAIL_TOO_MANY_FIGURES;
    }
    else if(result<pow(10.0, figures-1)){
        return FAIL_LACK_OF_FIGURES;
    }
    else{
        return result;
    }
}

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
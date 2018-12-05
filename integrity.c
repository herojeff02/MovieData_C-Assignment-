//
// Created by herojeff on 2018/12/04.
//

#include <string.h>
#include "stdio.h"
#include "integrity.h"
#include "dataType.h"

int integrityMovie(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testMovieFile, "r");
    }
    else{
        fp = fopen(movieFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((movies + i)->enabled)){
            i++;
        }

        sprintf(gen_line, "%d::%s (%d)::", ((movies + i)->movie_id), (movies + i)->title, (movies + i)->release_year);
        for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
            strcat(gen_line, genre_list[*((movies + i)->genre + j)]);
            if (j != ((movies + i)->sizeof_genre) - 1 && (movies + i)->sizeof_genre != 0) {
                strcat(gen_line, "|");
            }
        }
        strcat(gen_line, "\n");
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);
    return 0;
}
int integrityTag(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testTagFile, "r");
    }
    else{
        fp = fopen(tagFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((tags + i)->enabled)){
            i++;
        }

        Tag temp = *(tags + i);
        sprintf(gen_line, "%d::%d::%s::%lld\n", temp.user_id, temp.movie_id, temp.tag, temp.timestamp);
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);
    return 0;
}
int integrityUser(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testUserFile, "r");
    }
    else{
        fp = fopen(userFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((users + i)->enabled)){
            i++;
        }

        sprintf(gen_line, "%d::%s::%s::", (users + i)->user_id, (users + i)->user_name, (users + i)->password);
        for (int j = 0; j < (users + i)->sizeof_favourites; j++) {
            char a[2];
            a[0] = *(((users + i)->favourite_index) + j) + '0';
            a[1] = '\0';
            strcat(gen_line, a);
            if (j != ((users + i)->sizeof_favourites) - 1) {
                strcat(gen_line, "|");
            }
        }
        strcat(gen_line, "\n");
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);

    return 0;
}
int integrityFavourite(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testFavouriteFile, "r");
    }
    else{
        fp = fopen(favouriteFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((favourites + i)->enabled)){
            i++;
        }

        sprintf(gen_line, "%d::%d", (favourites + i)->user_id, (favourites + i)->movie_id);
        strcat(gen_line, "\n");
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);

    return 0;
}
int integrity() { //returns 1 when there's a problem.
    if(!integrityMovie()){
        if(!integrityTag()){
            if(!integrityUser()){
                if(!integrityFavourite()){
                    return 0;
                }
                printf("\n---integrity fault with favourite---\n");
                return 1;
            }
            printf("\n---integrity fault with user---\n");
            return 1;
        }
        printf("\n---integrity fault with tag---\n");
        return 1;
    }
    printf("\n---integrity fault with movie---\n");
    return 1;
}


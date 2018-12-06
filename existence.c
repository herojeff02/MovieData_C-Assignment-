//
// Created by herojeff on 2018/12/04.
//

#include <string.h>
#include "existence.h"
#include "dataType.h"

int movieIDExists(int movie_id){
    for(int i=0;i<movie_count;i++){
        if((movies + i)->movie_id == movie_id){
            return 1;
        }
    }
    return 0;
}
int movieIndexExists(int index){
    if(index<movie_count) {
        return 1;
    }
    return 0;
}

int userIDExists(int user_id) {
    for (int i = 0; i < user_count; i++) {
        if ((users + i)->user_id == user_id) {
            return 1;
        }
    }
    return 0;
}
int userIndexExists(int index) {
    if (index < user_count) {
        return 1;
    }
    return 0;
}
int favouriteUserIDExists(int user_id) {
    for (int i = 0; i < favourite_count; i++) {
        if ((favourites + i)->user_id == user_id) {
            return 1;
        }
    }
    return 0;
}
int favouriteMovieIDExists(int movie_id) {
    for (int i = 0; i < favourite_count; i++) {
        if ((favourites + i)->movie_id == movie_id) {
            return 1;
        }
    }
    return 0;
}
int favouriteIndexExists(int index) {
    if (index < favourite_count) {
        return 1;
    }
    return 0;
}

int movieTitleExists(char *title) {
    for (int i = 0; i < movie_count; i++) {
        if (strcmp((movies + i)->title, title)) {
            return 1;
        }
    }
    return 0;
}
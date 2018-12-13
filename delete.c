//
// Created by herojeff on 2018/12/04.
//

#include <stdlib.h>
#include <stdio.h>
#include "delete.h"
#include "existence.h"
#include "indexFinder.h"
#include "dataType.h"
#include "save.h"
#include "init.h"

int deleteMovie_ByIndex(int index) {
    if (!movieIndexExists(index)) {
        return FAIL_NO_SUCH_INDEX;
    }
    (movies + index)->enabled = 0;
    saveMovie();
    initMovie();
    return SUCCESS;
}

int deleteTag_ByIndex(int *indexes) {
    ////add tagIndexExists here
    for(int i=0;;i++){
        if(*(indexes+i) == END_OF_INT_ARRAY){
            break;
        }
        else{
            int index = *(indexes+i);
            if (!tagIndexExists(index)) {
                continue;
            }
            (tags + index)->enabled = 0;
        }
    }
    saveTag();
    initTag();
    return SUCCESS;
}

int deleteFavourite_ByIndex(int index) {
    ////add favouriteIndexExists here
    if (!favouriteIndexExists(index)) {
        return FAIL_NO_SUCH_INDEX;
    }
    (favourites + index)->enabled = 0;

//    printf(",,,%d", (favourites + index)->user_id);

    deleteFavourite_FromUser((favourites + index)->user_id, index);

    saveFavourite();
    initFavourite();

    return SUCCESS;
}

void deleteFavourite_FromUser(int user_id, int fav_index){
    int user_index;
    user_index = userIndex_ByUserID(user_id);

    int i;
    for(i=0;i<(users+user_index) -> sizeof_favourites;i++){
        if(*((users+user_index) -> favourite_index+i)==fav_index){
            break;
        }

    }
    for(int j=i;j<(users+user_index) -> sizeof_favourites;j++){
        if(j<((users+user_index) -> sizeof_favourites)-1) {
            *((users+user_index) -> favourite_index + j) = *((users+user_index) -> favourite_index + j + 1);
        }
    }
    (users+user_index) -> sizeof_favourites--;
    printf("///%d", (users+user_index) -> sizeof_favourites);

    saveUser();
    initUser();
}

int deleteUser_ByIndex(int index) {
    if (!userIndexExists(index)) {
        return FAIL_NO_SUCH_INDEX;
    }
    (users + index)->enabled = 0;
    saveUser();
    initUser();

    for (int i = 0; i < favourite_count; i++) {
        if ((favourites + i)->user_id == (users + index)->user_id) {
            deleteFavourite_ByIndex(i);
        }
    }

    return SUCCESS;
}
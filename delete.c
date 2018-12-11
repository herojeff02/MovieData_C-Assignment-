//
// Created by herojeff on 2018/12/04.
//

#include <stdlib.h>
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

int deleteTag_ByIndex(int index) {
    ////add tagIndexExists here
    if (!tagIndexExists(index)) {
        return FAIL_NO_SUCH_INDEX;
    }
    (tags + index)->enabled = 0;
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

    deleteFavourite_FromUser((favourites + index)->user_id, (favourites + index)->movie_id);

    saveFavourite();
    initFavourite();
    return SUCCESS;
}

void deleteFavourite_FromUser(int user_id, int fav_index){
    int user_index;
    user_index = userIndex_ByUserID(user_id);

    int shift_flag=0;
    for(int i=0;i<(users+user_index) -> sizeof_favourites;i++){
        if(!shift_flag && *((users+user_index) -> favourite_index+i)==fav_index){
            shift_flag=1;
        }
        if(shift_flag){
            if(i<(users+user_index) -> sizeof_favourites-1) {
                *((users+user_index) -> favourite_index + i) = *((users+user_index) -> favourite_index + i + 1);
            }
        }
    }
    (users+user_index) -> sizeof_favourites--;
    (users+user_index) -> favourite_index = realloc((users+user_index) -> favourite_index, sizeof(int)*((users+user_index) -> sizeof_favourites));
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
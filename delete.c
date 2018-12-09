//
// Created by herojeff on 2018/12/04.
//

#include "delete.h"
#include "existence.h"
#include "dataType.h"
#include "save.h"
#include "init.h"

int deleteMovie_ByIndex(int index){
    if(!movieIndexExists(index)){
        return FAIL_NO_SUCH_INDEX;
    }
    (movies + index) -> enabled = 0;
    saveMovie();
    initMovie();
    return SUCCESS;
}
int deleteTag_ByIndex(int index){
    ////add tagIndexExists here
    if(index>=tag_count){
        return FAIL_NO_SUCH_INDEX;
    }
    (tags + index) -> enabled = 0;
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
    saveFavourite();
    initFavourite();
    return SUCCESS;
}
int deleteUser_ByIndex(int index) {
    if (!userIndexExists(index)) {
        return FAIL_NO_SUCH_INDEX;
    }
    (users + index)->enabled = 0;
    saveUser();
    initUser();

    for(int i=0;i<favourite_count;i++){
        if((favourites+i)->user_id == (users+index)->user_id){
            deleteFavourite_ByIndex(i);
        }
    }

    return SUCCESS;
}
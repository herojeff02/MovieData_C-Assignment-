//
// Created by herojeff on 2018/12/04.
//

#include <stdlib.h>
#include <string.h>
#include "append.h"
#include "dataType.h"
#include "save.h"
#include "init.h"
#include "indexFinder.h"
#include "existence.h"

int addMovieEntity(int movie_id, char *title, int release_year, int *genre, short genre_count){
    if(movieIDExists(movie_id)){
        return FAIL_MOVIE_ID_ALREADY_EXISTS;
    }
    if(release_year > 9999 || release_year < 1000){
        return FAIL_INVALID_YEAR;
    }

    movies = (Movie *) realloc(movies, (movie_count+1) * sizeof(Movie));

    //0
    (movies + movie_count)->movie_id = movie_id;

    //1
    (movies + movie_count)->release_year = release_year;
    (movies + movie_count)->title = malloc(strlen(title) * sizeof(char));
    strcpy((movies + movie_count)->title, title);

    //2
    (movies + movie_count)->genre = (int *) malloc(sizeof(int)*genre_count);
    for(int i=0;i<genre_count;i++){
        *(((movies + movie_count)->genre)+i) = *(genre+i);
    }
    (movies + movie_count)->sizeof_genre = genre_count;
    (movies + movie_count) -> enabled = 1;

    movie_count++;
    saveMovie();
    initMovie();
    return SUCCESS;
}
int addTagEntity(int user_id, int movie_id, char *tag, long long timestamp){
    tags = (Tag *) realloc(tags, (tag_count+1) * sizeof(Tag));
    //0
    (tags+tag_count) -> user_id = user_id;

    //1
    (tags+tag_count) -> movie_id = movie_id;

    //2
    (tags+tag_count) -> tag = malloc((strlen(tag)+1)* sizeof(char));
    strcpy((tags+tag_count) -> tag, tag);

    //3
    (tags+tag_count) -> timestamp = timestamp;

    (tags+tag_count) -> enabled = 1;

    tag_count++;
    return SUCCESS;
}
int addUserEntity(int user_id, char *userName, char *password){
    users = (User *) realloc(users, (user_count+1) * sizeof(User));
    //0
    (users+user_count) -> user_id = user_id;

    //1
    (users+user_count) -> user_name = malloc((strlen(userName)+1)* sizeof(char));
    strcpy((users+user_count) -> user_name, userName);

    //2
    (users+user_count) -> password = malloc((strlen(password)+1)* sizeof(char));
    strcpy((users+user_count) -> password, password);

    //3
    (users+user_count)->favourite_index = malloc(sizeof(int));
    (users+user_count)->sizeof_favourites = 0;

    (users+user_count) -> enabled = 1;

    user_count++;
    return SUCCESS;
}
int addFavouriteEntity(int user_id, int movie_id){
    favourites = (Favourite *) realloc(favourites, (favourite_count+1) * sizeof(Favourite));

    (favourites+favourite_count) -> user_id = user_id;
    (favourites+favourite_count) -> movie_id = movie_id;

    (favourites+favourite_count) -> enabled = 1;

    favourite_count++;

    int user_index = userIndex_ByUserID(user_id);
    addToUserFavouriteIndex(user_index, movie_id);

    return SUCCESS;
}
int addToUserFavouriteIndex(int user_index, int movieID){
    (users+user_index)->favourite_index = realloc((users+user_index)->favourite_index, sizeof(int)*((users+user_index)->sizeof_favourites+1));
    int counter = (users+user_index)->sizeof_favourites;
    *(((users+user_index) -> favourite_index) + counter) = movieID;

    (users+user_index)->sizeof_favourites++;
}
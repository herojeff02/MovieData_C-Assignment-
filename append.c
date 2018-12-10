//
// Created by herojeff on 2018/12/04.
//

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "append.h"
#include "dataType.h"
#include "save.h"
#include "init.h"
#include "indexFinder.h"
#include "existence.h"

int addMovieEntity(int movie_id, char *title, int release_year, int *genre, short genre_count) {
    if (movieIDExists(movie_id)) {
        return FAIL_MOVIE_ID_ALREADY_EXISTS;
    }
    if (release_year > 9999 || release_year < 1000) {
        return FAIL_INVALID_YEAR;
    }

    movies = (Movie *) realloc(movies, (movie_count + 1) * sizeof(Movie));

    //0
    (movies + movie_count)->movie_id = movie_id;

    //1
    (movies + movie_count)->release_year = release_year;
    (movies + movie_count)->title = malloc(strlen(title) * sizeof(char));
    strcpy((movies + movie_count)->title, title);

    //2
    if (genre_count != 0) {
        (movies + movie_count)->genre = (int *) malloc(sizeof(int) * genre_count);
        for (int i = 0; i < genre_count; i++) {
            *(((movies + movie_count)->genre) + i) = *(genre + i);
        }
        (movies + movie_count)->sizeof_genre = genre_count;
    } else {
        *((movies + movie_count)->genre) = (int) malloc(sizeof(int));
        char arr[] = "(no genres listed)";
        *((movies + movie_count)->genre) = genreIndex_ByString(arr);
        (movies + movie_count)->sizeof_genre = 0;
    }

    (movies + movie_count)->enabled = 1;

    movie_count++;
    saveMovie();
    initMovie();
    return SUCCESS;
}

int addGenre_ToMovie(int movie_index, char *genre) {
    if ((movies + movie_index)->sizeof_genre == 0) {
        *((movies + movie_index)->genre) = genreIndex_ByString(genre);
        (movies + movie_index)->sizeof_genre = 1;
    } else {
        short flag = 0;
        int temp = genreIndex_ByString(genre);
        for (int i = 0; i < (movies + movie_index)->sizeof_genre; ++i) {
            if (*((movies + movie_index)->genre + i) == temp ||
                *((movies + movie_index)->genre + i) == genreIndex_ByString("(no genres listed)")) {
                flag = 1;
            }
        }
        if (!flag) {
            (movies + movie_index)->genre = realloc((movies + movie_index)->genre,
                                                    sizeof(int) * ((movies + movie_index)->sizeof_genre + 1));
            int counter = (movies + movie_index)->sizeof_genre;
            *(((movies + movie_index)->genre) + counter) = genreIndex_ByString(genre);

            (movies + movie_index)->sizeof_genre++;
        }

    }
    saveMovie();
    initMovie();
}

int removeGenre_FromMovie(int movie_index, char *genre) {


    saveMovie();
    initMovie();
}

int addTagEntity(int user_id, int movie_id, char *tag) {
    tags = (Tag *) realloc(tags, (tag_count + 1) * sizeof(Tag));
    //0
    (tags + tag_count)->user_id = user_id;

    //1
    (tags + tag_count)->movie_id = movie_id;

    //2
    (tags + tag_count)->tag = malloc((strlen(tag) + 1) * sizeof(char));
    strcpy((tags + tag_count)->tag, tag);

    //3
    (tags + tag_count)->timestamp = time(NULL);

    (tags + tag_count)->enabled = 1;

    tag_count++;

    saveTag();
    initTag();

    return SUCCESS;
}

int addUserEntity(int user_id, char *userName, char *password) {
    users = (User *) realloc(users, (user_count + 1) * sizeof(User));
    //0
    (users + user_count)->user_id = user_id;

    //1
    (users + user_count)->user_name = malloc((strlen(userName) + 1) * sizeof(char));
    strcpy((users + user_count)->user_name, userName);

    //2
    (users + user_count)->password = malloc((strlen(password) + 1) * sizeof(char));
    strcpy((users + user_count)->password, password);

    //3
    (users + user_count)->favourite_index = malloc(sizeof(int));
    (users + user_count)->sizeof_favourites = 0;

    (users + user_count)->enabled = 1;

    user_count++;

    saveUser();
    initUser();

    return SUCCESS;
}

int addFavouriteEntity(int user_id, int movie_id) {
    favourites = (Favourite *) realloc(favourites, (favourite_count + 1) * sizeof(Favourite));

    (favourites + favourite_count)->user_id = user_id;
    (favourites + favourite_count)->movie_id = movie_id;

    (favourites + favourite_count)->enabled = 1;

    favourite_count++;

    int user_index = userIndex_ByUserID(user_id);
    addFavouriteIndex_ToUser(user_index, movie_id);

    saveFavourite();
    initFavourite();

    return SUCCESS;
}

int addFavouriteIndex_ToUser(int user_index, int favourite_index) {
    (users + user_index)->favourite_index = realloc((users + user_index)->favourite_index,
                                                    sizeof(int) * ((users + user_index)->sizeof_favourites + 1));
    *(((users + user_index)->favourite_index) + (users + user_index)->sizeof_favourites) = favourite_index;

    (users + user_index)->sizeof_favourites++;

    saveUser();
    initUser();

    return SUCCESS;
}
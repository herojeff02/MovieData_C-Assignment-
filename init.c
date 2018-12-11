//
// Created by herojeff on 2018/12/04.
//

#include <stdio.h>
#include <string.h>
#include "init.h"
#include "dataType.h"
#include "stdlib.h"
#include "customString.h"
#include "indexFinder.h"

void initMovie() {
    genre_list_cursor = 0;

    //init movie

    movies = (Movie *) malloc(sizeof(Movie));
    genre_list = (char **) malloc(sizeof(char *));

    FILE *fp;
    if (TEST_FILE_FLAG) {
        fp = fopen(testMovieFile, "r");
    } else {
        fp = fopen(movieFile, "r");
    }
    char line[LINE_LENGTH];
    int index = 0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        movies = (Movie *) realloc(movies, (index + 1) * sizeof(Movie));

        char *split_t = split_back(line, "::");
        char *split0 = split_front(line, "::");
        char *split2 = split_back(split_t, "::");
        char *split1 = split_front(split_t, "::");

        //0
        (movies + index)->movie_id = atoi(split0);

        //1
        strtok(split1 + strlen(split1) - 5, ")");
        char *potential = split1 + strlen(split1) - 4;
        int potential_int = atoi(potential);
        (movies + index)->release_year = potential_int;
        (movies + index)->title = malloc(strlen(split1) * sizeof(char));
        strtok(split1 + strlen(split1) - 7, " (");
        strcpy((movies + index)->title, split1);

        //2
        (movies + index)->genre = (int *) malloc(sizeof(int));

        if (!strcmp(split2, "\n")) {
            *((movies + index)->genre) = genreIndex_ByString("(no genres listed)");
            (movies + index)->sizeof_genre = 0;
        } else {
            split2 = strtok(split2, "\n");

            short cnt;
            if (strstr(split2, "|")) {
                cnt = 0;
                char *genre_split = strtok(split2, "|");
                while (genre_split != NULL) {
                    (movies + index)->genre = realloc((movies + index)->genre, (cnt + 1) * sizeof(int));
                    *(((movies + index)->genre) + cnt) = genreIndex_ByString(genre_split);
                    genre_split = strtok(NULL, "|");
                    cnt++;
                }
                (movies + index)->sizeof_genre = cnt;
            } else {
                if(genreIndex_ByString(split2) == genreIndex_ByString("(no genres listed)")){
                    (movies + index)->sizeof_genre = 0;
                }
                else {
                    *(((movies + index)->genre)) = genreIndex_ByString(split2);
                    (movies + index)->sizeof_genre = 1;
                }
            }
        }
        (movies + index)->enabled = 1;

        index++;

    }
    fclose(fp);

    movie_count = index;
    return;
}

void initTag() {
    FILE *fp;
    char line[LINE_LENGTH];
    int index = 0;

    //init tag
    tags = (Tag *) malloc(sizeof(Tag));

    if (TEST_FILE_FLAG) {
        fp = fopen(testTagFile, "r");
    } else {
        fp = fopen(tagFile, "r");
    }
    index = 0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        tags = (Tag *) realloc(tags, (index + 1) * sizeof(Tag));

        char *split_t = split_back(line, "::");
        char *split0 = split_front(line, "::"); //tag0
        char *split_t_t = split_back(split_t, "::");
        char *split1 = split_front(split_t, "::"); //tag1
        char *split3 = split_back(split_t_t, "::");
        char *split2 = split_front(split_t_t, "::");

        //0
        (tags + index)->user_id = atoi(split0);

        //1
        (tags + index)->movie_id = atoi(split1);

        //2
        (tags + index)->tag = malloc((strlen(split2) + 1) * sizeof(char));
        strcpy((tags + index)->tag, split2);

        //3
        split3 = strtok(split3, "\n");
        (tags + index)->timestamp = atoll(split3);

        (tags + index)->enabled = 1;

        index++;
    }
    fclose(fp);

    tag_count = index;
    return;
}

void initUser() {
    users = malloc(sizeof(User));
    FILE *fp;
    char line[LINE_LENGTH];
    int index = 0;

    if (TEST_FILE_FLAG) {
        fp = fopen(testUserFile, "r");
        if (fp == NULL) {
            fp = fopen(testUserFile, "w+");
        }
    } else {
        fp = fopen(userFile, "r");
        if (fp == NULL) {
            fp = fopen(testUserFile, "w+");
        }
    }

    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        users = (User *) realloc(users, (index + 1) * sizeof(User));

        char *split_t = split_back(line, "::");
        char *split0 = split_front(line, "::"); //tag0
        char *split_t_t = split_back(split_t, "::");
        char *split1 = split_front(split_t, "::"); //tag1
        char *split3 = split_back(split_t_t, "::");
        char *split2 = split_front(split_t_t, "::");

        //0
        (users + index)->user_id = atoi(split0);

        //1
        (users + index)->user_name = malloc((strlen(split1) + 1) * sizeof(char));
        strcpy((users + index)->user_name, split1);

        //2
        (users + index)->password = malloc((strlen(split2) + 1) * sizeof(char));
        strcpy((users + index)->password, split2);

        //3
        (users + index)->favourite_index = (int *) malloc(sizeof(int));
        if (!strcmp(split3, "\n")) {
            (users + index)->sizeof_favourites = 0;
        } else {
            split3 = strtok(split3, "\n");
            int cnt;
            if (strstr(split3, "|")) {
                cnt = 0;
                char *favourite_index_split = strtok(split3, "|");
                while (favourite_index_split != NULL) {
                    (users + index)->favourite_index = realloc((users + index)->favourite_index,
                                                               (cnt + 1) * sizeof(int));
                    *(((users + index)->favourite_index) + cnt) = atoi(favourite_index_split);
                    favourite_index_split = strtok(NULL, "|");
                    cnt++;
                }
                (users + index)->sizeof_favourites = cnt;
            } else {
                *(((users + index)->favourite_index)) = atoi(split3);
                (users + index)->sizeof_favourites = 1;
            }
        }

        (users + index)->enabled = 1;


        index++;
    }

    user_count = index;
    fclose(fp);
}

void initFavourite() {
    FILE *fp;
    char line[LINE_LENGTH];
    int index = 0;

    //init tag
    favourites = (Favourite *) malloc(sizeof(Favourite));

    if (TEST_FILE_FLAG) {
        fp = fopen(testFavouriteFile, "r");
        if (fp == NULL) {
            fp = fopen(testFavouriteFile, "w+");
        }
    } else {
        fp = fopen(favouriteFile, "r");
        if (fp == NULL) {
            fp = fopen(favouriteFile, "w+");
        }
    }
    index = 0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        favourites = (Favourite *) realloc(favourites, (index + 1) * sizeof(Favourite));

        char *split1 = split_back(line, "::");
        char *split0 = split_front(line, "::");

        //0
        (favourites + index)->user_id = atoi(split0);

        //1
        (favourites + index)->movie_id = atoi(split1);

        (favourites + index)->enabled = 1;

        index++;
    }
    fclose(fp);

    favourite_count = index;
    return;
}

void init() {
    initMovie();
    initTag();
    initUser();
    initFavourite();
}
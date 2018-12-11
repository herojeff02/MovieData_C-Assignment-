//
// Created by herojeff on 2018/12/04.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "indexFinder.h"
#include "dataType.h"
#include "customString.h"
#include "existence.h"

int *genreIndex_ByMovieID(int movieID) {
    int *return_array;
    int index = movieIndex_ByID(movieID);
    return_array = malloc(sizeof(int) * ((movies + index)->sizeof_genre) + 1);
    memcpy(return_array, (movies + index)->genre, sizeof(int) * ((movies + index)->sizeof_genre));
    *(return_array + (movies + index)->sizeof_genre) = END_OF_INT_ARRAY;
    return return_array;
}

int *movieIndex_ByTitle(char *title) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < movie_count; i++) {
        if (strstr(tolowerString((movies + i)->title), tolowerString(title))) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int movieIndex_ByMatchingTitle(char *title, int year) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < movie_count; i++) {
        if (!strcmp(tolowerString((movies + i)->title), tolowerString(title)) && (movies + i)->release_year == year) {
            return i;
        }
    }
    return FAIL_NO_SUCH_MOVIE_ID;
}

int* movieIndex_ByMatchingTitle_WithoutYear(char *title) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < movie_count; i++) {
        if (!strcmp(tolowerString((movies + i)->title), tolowerString(title))) {
            count++;
            return_array = realloc(return_array, sizeof(int)*count);
            *(return_array+count-1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int)*(count+1);
    *(return_array+count) = END_OF_INT_ARRAY;
    return return_array;
}

int genreIndex_ByString(char *genre) {
    char *temp = malloc(sizeof(char) * strlen(genre));
    strcpy(temp, genre);
    int flag = 1;
    int i = 0;
    strcpy(temp, tolowerCapitalizer(temp));
    for (; i < genre_list_cursor; i++) {
        if (!strcmp(temp, *(genre_list + i))) {
            flag = 0;
            break;
        }
    }
    if (flag) {
        genre_list = (char **) realloc(genre_list, sizeof(char *) * (genre_list_cursor + 1));
        *(genre_list + genre_list_cursor) = (char *) malloc(sizeof(char) * strlen(temp));
        strcpy(*(genre_list + genre_list_cursor), temp);
        genre_list_cursor++;
    }
    return i;
}

char *string_ByGenreIndex(int genre) {
    return *(genre_list + genre);
}

int compare(const void *first, const void *second) {
    if (*(int *) first > *(int *) second)
        return 1;
    else if (*(int *) first < *(int *) second)
        return -1;
    else
        return 0;
}

int *tempSortGenre(int index) {
    int *ar = malloc(sizeof(int) * (movies + index)->sizeof_genre);
    memcpy(ar, (movies + index)->genre, ((movies + index)->sizeof_genre) * sizeof(int));
    qsort(ar, (size_t) ((movies + index)->sizeof_genre), sizeof(int), compare);
    return ar;
}

int *movieIndex_ByGenre(int *genre, short genre_count) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < movie_count; i++) {
        if ((movies + i)->sizeof_genre == genre_count) {
            int flag = 1;
            int *ar = malloc(sizeof(int) * genre_count);
            memcpy(ar, tempSortGenre(i), genre_count * sizeof(int));
            qsort(genre, (size_t) genre_count, sizeof(int), compare);
            for (int j = 0; j < genre_count; j++) {
                if (*(ar + j) != *(genre + j)) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                count++;
                return_array = realloc(return_array, sizeof(int) * count);
                *(return_array + count - 1) = i;
            }
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int movieIndex_ByID(int movie_id) {
    if (!movieIDExists(movie_id)) {
        return FAIL_NO_SUCH_MOVIE_ID;
    }
    for (int i = 0; i < movie_count; i++) {
        if ((movies + i)->movie_id == movie_id) {
            return i;
        }
    }
}

int userIndex_ByUserID(int user_id) {
    for (int i = 0; i < user_count; i++) {
        if ((users + i)->user_id == user_id) {
            return i;
        }
    }
    return FAIL_NO_SUCH_USER_ID;
}

int *tagIndex_ByUserID(int user_id) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < tag_count; i++) {
        if ((tags + i)->user_id == user_id) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int *tagIndex_ByMovieID(int movie_id) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < tag_count; i++) {
        if ((tags + i)->movie_id == movie_id) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int *tagIndex_ByContent(char *content) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < tag_count; i++) {
        if (strstr(((tags + i)->tag), content) != NULL) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int *tagIndex_ByDoubleID(int user_id, int movie_id) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < tag_count; i++) {
        if ((tags + i)->user_id == user_id && (tags + i)->movie_id == movie_id) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int *tagIndex_ByTag(char *tag) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < tag_count; i++) {
        if (strstr(tolowerString((tags + i)->tag), tolowerString(tag))) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

//// check code from here
int *favouriteIndex_ByUserID(int user_id) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < favourite_count; i++) {
        if ((favourites + i)->user_id == user_id) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int *favouriteIndex_ByMovieID(int movie_id) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < favourite_count; i++) {
        if ((favourites + i)->movie_id == movie_id) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int *favouriteIndex_ByDoubleID(int user_id, int movie_id) {
    int *return_array = malloc(sizeof(int));
    int count = 0;
    for (int i = 0; i < favourite_count; i++) {
        if ((favourites + i)->user_id == user_id && (favourites + i)->movie_id == movie_id) {
            count++;
            return_array = realloc(return_array, sizeof(int) * count);
            *(return_array + count - 1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int) * (count + 1));
    *(return_array + count) = END_OF_INT_ARRAY;
    return return_array;
}

int userIndex_ByName(char *name) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp((users + i)->user_name, name) == 0) {
            return i;
        }
    }
    return FAIL_NO_SUCH_USER_NAME;
}

Tag tagFinder_ByIndex(int tag_index) {
    return tags[tag_index];
}

Favourite favouriteFinder_ByIndex(int fav_index) {
    return favourites[fav_index];
}
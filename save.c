//
// Created by herojeff on 2018/12/04.
//

#include <stdio.h>
#include "save.h"
#include "dataType.h"

void saveUser() {
    FILE *fp1;
    if (TEST_FILE_FLAG) {
        fp1 = fopen(testUserFile, "w");
    } else {
        fp1 = fopen(userFile, "w");
    }

    if (user_count != 0) {
        for (int i = 0; i < user_count; i++) {
            if ((users + i)->enabled) {
                fprintf(fp1, "%d::%s::%s::", (users + i)->user_id, (users + i)->user_name, (users + i)->password);
                for (int j = 0; j < (users + i)->sizeof_favourites; j++) {
                    fprintf(fp1, "%d", *(((users + i)->favourite_index) + j));
                    if (j != ((users + i)->sizeof_favourites) - 1) {
                        fprintf(fp1, "|");
                    }
                }
                fprintf(fp1, "\n");
            }
        }
    }
    else{
        fprintf(fp1, "\n");
    }

    fclose(fp1);
}

void saveMovie() {
    FILE *fp1;
    if (TEST_FILE_FLAG) {
        fp1 = fopen(testMovieFile, "w");
    } else {
        fp1 = fopen(movieFile, "w");
    }

    if (movie_count != 0) {
        for (int i = 0; i < movie_count; i++) {
            if ((movies + i)->enabled) {
                int k = 0;
                fprintf(fp1, "%d::%s (%d)::", (movies + i)->movie_id, (movies + i)->title, (movies + i)->release_year);
                if ((movies + i)->sizeof_genre == 0) {
					fprintf(fp1, "(no genres listed)\n");
					continue;
                } else {
                    k = (movies + i)->sizeof_genre;
                }
                for (int j = 0; j < k; j++) {
                    fprintf(fp1, "%s", genre_list[*((movies + i)->genre + j)]);
                    if (j != ((movies + i)->sizeof_genre) - 1 && (movies + i)->sizeof_genre != 0) {
                        fprintf(fp1, "|");
                    }
                }
                fprintf(fp1, "\n");
            }
        }
    }
    else{
        fprintf(fp1, "\n");
    }

    fclose(fp1);
}

void saveTag() {
    FILE *fp2;
    if (TEST_FILE_FLAG) {
        fp2 = fopen(testTagFile, "w");
    } else {
        fp2 = fopen(tagFile, "w");
    }

    if (tag_count != 0) {
        for (int i = 0; i < tag_count; i++) {
            if ((tags + i)->enabled) {
                Tag temp = *(tags + i);
                fprintf(fp2, "%d::%d::%s::%lld\n", temp.user_id, temp.movie_id, temp.tag, temp.timestamp);
            }
        }
    }
    else{
        fprintf(fp2, "\n");
    }

    fclose(fp2);
}

void saveFavourite() {
    FILE *fp2;
    if (TEST_FILE_FLAG) {
        fp2 = fopen(testFavouriteFile, "w");
    } else {
        fp2 = fopen(favouriteFile, "w");
    }

    if (favourite_count != 0) {
        for (int i = 0; i < favourite_count; i++) {
            if ((favourites + i)->enabled) {
                Favourite temp = *(favourites + i);
                fprintf(fp2, "%d::%d\n", temp.user_id, temp.movie_id);
            }
        }
    }
    else{
        fprintf(fp2, "\n");
    }

    fclose(fp2);
}

void save() {
    saveMovie();
    saveTag();
    saveUser();
    saveFavourite();
}
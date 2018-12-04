//
// Created by herojeff on 2018/12/04.
//

#ifndef MOVIEDATA_C_ASSIGNMENT_ADDENTITY_H
#define MOVIEDATA_C_ASSIGNMENT_ADDENTITY_H

int addMovieEntity(int movie_id, char *title, int release_year, int *genre, short genre_count);
int addTagEntity(int user_id, int movie_id, char *tag);
int addUserEntity(int user_id, char *userName, char *password);
int addFavouriteEntity(int user_id, int movie_id);
int addToUserFavouriteIndex(int user_index, int movieID);

#endif //MOVIEDATA_C_ASSIGNMENT_ADDENTITY_H

//
// Created by herojeff on 2018/12/04.
//

#ifndef MOVIEDATA_C_ASSIGNMENT_INDEXFINDER_H
#define MOVIEDATA_C_ASSIGNMENT_INDEXFINDER_H

int* movieIndex_ByTitle(char *title); ////returns END_OF_INT_ARRAY at end of array
int* movieIndex_ByGenre(int *genre, short genre_count); ////returns END_OF_INT_ARRAY at end of array
int movieIndex_ByID(int movie_id);

int userIndex_ByUserID(int user_id);

int genreIndex_ByString(char *genre);
int* genreIndex_ByMovieID(int movieID);

int* tagIndex_ByUserID(int user_id); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByMovieID(int movie_id); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByDoubleID(int user_id, int movie_id); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByTag(char *tag); ////returns END_OF_INT_ARRAY at end of array

int* favouriteIndex_ByUserID(int user_id); ////returns END_OF_INT_ARRAY at end of array
int* favouriteIndex_ByMovieID(int movie_id); ////returns END_OF_INT_ARRAY at end of array
int favouriteIndex_ByDoubleID(int user_id, int movie_id);

#endif //MOVIEDATA_C_ASSIGNMENT_INDEXFINDER_H

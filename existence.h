//
// Created by herojeff on 2018/12/04.
//

#ifndef MOVIEDATA_C_ASSIGNMENT_EXISTENCE_H
#define MOVIEDATA_C_ASSIGNMENT_EXISTENCE_H

int movieIDExists(int movie_id);
int movieIndexExists(int index);
int userIDExists(int user_id);
int userIDExists_InTag(int user_id);
int userIndexExists(int index);
int userNameExists(char *name);
int tagIndexExists(int index);
int tagExists(char *content);
int favouriteUserIDExists(int user_id);
int favouriteMovieIDExists(int movie_id);
int favouriteIndexExists(int index);
int movieTitleExists(char *title);
int multipleMatchingMovieTitleExists(char *title);
int matchingMovieTitleExists(char *title);

#endif //MOVIEDATA_C_ASSIGNMENT_EXISTENCE_H

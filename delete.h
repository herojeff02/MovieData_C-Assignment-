//
// Created by herojeff on 2018/12/04.
//

#ifndef MOVIEDATA_C_ASSIGNMENT_DELETE_H
#define MOVIEDATA_C_ASSIGNMENT_DELETE_H

int deleteMovie_ByIndex(int index);
int deleteTag_ByIndex(int index);
int deleteUser_ByIndex(int index);
int deleteFavourite_ByIndex(int index);
void deleteFavourite_FromUser(int user_index, int movie_index);

#endif //MOVIEDATA_C_ASSIGNMENT_DELETE_H

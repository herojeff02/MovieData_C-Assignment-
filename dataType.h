//
// Created by herojeff on 2018/12/04.
//

#ifndef MOVIEDATA_C_ASSIGNMENT_DATATYPE_H
#define MOVIEDATA_C_ASSIGNMENT_DATATYPE_H

#define testTagFile "testtags.dat"
#define testMovieFile "testmovies.dat"
#define testUserFile "testusers.dat"
#define testFavouriteFile "testfavourites.dat"
#define tagFile "tags.dat"
#define movieFile "movies.dat"
#define userFile "users.dat"
#define favouriteFile "favourites.dat"

#define TEST_FILE_FLAG 1
#define LINE_LENGTH 1000

#define END_OF_INT_ARRAY -500

#define SUCCESS -1000

#define FAIL_MOVIE_ID_ALREADY_EXISTS -2000
#define FAIL_USER_ID_ALREADY_EXISTS -4000
#define FAIL_NO_SUCH_USER_ID -3000
#define FAIL_NO_SUCH_MOVIE_ID -5000
#define FAIL_NO_SUCH_INDEX -6000

#define FAIL_INVALID_YEAR -7000
#define FAIL_TOO_MANY_FIGURES -9000
#define FAIL_NOT_A_NUMBER -8000
#define FAIL_LACK_OF_FIGURES -10000

#define FAIL_NO_SUCH_USER_NAME -11000
#define FAIL_NO_SUCH_USER_PASSWORD -12000


typedef struct{
    //please do not add more than 2 billion movies. You're not going to, are you?
    int movie_id;
    char *title;
    int release_year;
    int *genre;
    short sizeof_genre;
    short enabled;
}Movie;
typedef struct {
    int user_id;
    int movie_id;
    char *tag;
    long long timestamp;
    short enabled;
}Tag;
typedef struct {
    int user_id;
    int movie_id;
    short enabled;
}Favourite;
typedef struct {
    int user_id;
    char *user_name;
    char *password;//yup, plaintext password right here.
    int *favourite_index;
    short enabled;
    int sizeof_favourites;
}User;

extern Movie *movies;
extern Tag *tags;
extern User *users;
extern Favourite *favourites;

extern int movie_count;
extern int tag_count;
extern int user_count;
extern int favourite_count;

extern char **genre_list;
extern int genre_list_cursor;

#endif //MOVIEDATA_C_ASSIGNMENT_DATATYPE_H

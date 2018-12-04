#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define END_OF_INT_ARRAY -500

#define FAIL_MOVIE_ID_ALREADY_EXISTS -2000
#define FAIL_USER_ID_ALREADY_EXISTS -4000
#define FAIL_NO_SUCH_USER_ID -7000
#define FAIL_NO_SUCH_MOVIE_ID -5000
#define FAIL_NO_SUCH_INDEX -6000
#define FAIL_INVALID_YEAR -3000
#define SUCCESS -1000

#define TEST_FILE_FLAG 1
#define LINE_LENGTH 1000

#define testTagFile "testtags.dat"
#define testMovieFile "testmovies.dat"
#define testUserFile "testusers.dat"
#define testFavouriteFile "testfavourites.dat"
#define tagFile "tags.dat"
#define movieFile "movies.dat"
#define userFile "users.dat"
#define favouriteFile "favourites.dat"

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

Movie *movies;
Tag *tags;
User *users;
Favourite *favourites;
int movie_count;
int tag_count;
int user_count;
int favourite_count;

char **genre_list;
int genre_list_cursor=0;

char *split_front(char *str, const char *delim) {
    char *p = strstr(str, delim);

    if (p == NULL) {
        return NULL;
    }
    else {
        *p = '\0';
        return str;
    }
}
char *split_back(char *str, const char *delim){
    char *p = strstr(str, delim);

    if (p == NULL) {
        return NULL;
    }
    else {
        return p + strlen(delim);
    }
}

//modes
void init();
void menu();
void save();
int selectMenu();
void close();
void addMovie();
void removeMovie();
void addTag();
void removeTag();
void addFavourite();
void removeFavourite();
void searchByUserID(); //input: user_id - output: all tags made by the user, movie info of tags, favourites list, movie info of favourites
void searchByMovieTitle(); //input: title - output: genre, tags, release_year, favourited by whom?, similar movies
void retire();

char* tolowerString(char* content);
char* tolowerCapitalizer(char* content);

//methods
int genreIndex_ByString(char *genre);
int* genreIndex_ByMovieID(int movieID);

void testPrint();

int addMovieEntity(int movie_id, char *title, int release_year, int *genre, short genre_count);
int addTagEntity(int user_id, int movie_id, char *tag, long long timestamp);
int addUserEntity(int user_id, char *userName, char *password);
int addUserEntity_TagData(int user_id);
int addFavouriteEntity(int user_id, int movie_id);

int movieIDExists(int movie_id);
int movieIndexExists(int index);
int userIndexExists(int index);
int userNameExists(char *name);
int tagIndexExists(int index);

int deleteMovie_ByIndex(int index);
int deleteTag_ByIndex(int index);
int deleteUser_ByIndex(int index);
int deleteFavourite_ByIndex(int index);

void printMovieInfo(int movie_id);
void printAllTag(int movie_id);
void printTag(int index);

int* movieIndex_ByTitle(char *title); ////returns END_OF_INT_ARRAY at end of array
int* movieIndex_ByGenre(int *genre, short genre_count); ////returns END_OF_INT_ARRAY at end of array
int movieIndex_ByID(int movie_id);

int userIndex_ByUserID(int user_id);
int* tagIndex_ByUserID(int user_id); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByMovieID(int movie_id); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByDoubleID(int user_id, int movie_id); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByTag(char *tag); ////returns END_OF_INT_ARRAY at end of array
int* favouriteIndex_ByUserID(int user_id); ////returns END_OF_INT_ARRAY at end of array
int* favouriteIndex_ByMovieID(int movie_id); ////returns END_OF_INT_ARRAY at end of array
int favouriteIndex_ByDoubleID(int user_id, int movie_id);

int integrityMovie(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testMovieFile, "r");
    }
    else{
        fp = fopen(movieFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((movies + i)->enabled)){
            i++;
        }

        sprintf(gen_line, "%d::%s (%d)::", ((movies + i)->movie_id), (movies + i)->title, (movies + i)->release_year);
        for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
            strcat(gen_line, genre_list[*((movies + i)->genre + j)]);
            if (j != ((movies + i)->sizeof_genre) - 1) {
                strcat(gen_line, "|");
            }
        }
        strcat(gen_line, "\n");
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);
    return 0;
}
int integrityTag(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testTagFile, "r");
    }
    else{
        fp = fopen(tagFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((tags + i)->enabled)){
            i++;
        }

        Tag temp = *(tags + i);
        sprintf(gen_line, "%d::%d::%s::%lld\n", temp.user_id, temp.movie_id, temp.tag, temp.timestamp);
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);
    return 0;
}
int integrityUser(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testUserFile, "r");
    }
    else{
        fp = fopen(userFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((users + i)->enabled)){
            i++;
        }

        sprintf(gen_line, "%d::%s::%s::", (users + i)->user_id, (users + i)->user_name, (users + i)->password);
        for (int j = 0; j < (users + i)->sizeof_favourites; j++) {
            char a[2];
            a[0] = *(((users + i)->favourite_index) + j) + '0';
            a[1] = '\0';
            strcat(gen_line, a);
            if (j != ((users + i)->sizeof_favourites) - 1) {
                strcat(gen_line, "|");
            }
        }
        strcat(gen_line, "\n");
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);

    return 0;
}
int integrityFavourite(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testFavouriteFile, "r");
    }
    else{
        fp = fopen(favouriteFile, "r");
    }
    char line[LINE_LENGTH];
    char gen_line[LINE_LENGTH];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((favourites + i)->enabled)){
            i++;
        }

        sprintf(gen_line, "%d::%d", (favourites + i)->user_id, (favourites + i)->movie_id);
        strcat(gen_line, "\n");
        if(strcmp(line, gen_line)) {
            printf("%s\n", line);
            printf("%s\n", gen_line);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);

    return 0;
}
int integrity() { //returns 1 when there's a problem.
    if(!integrityMovie()){
        if(!integrityTag()){
            if(!integrityUser()){
                if(!integrityFavourite()){
                    return 0;
                }
                printf("\n---integrity fault with favourite---\n");
                return 1;
            }
            printf("\n---integrity fault with user---\n");
            return 1;
        }
        printf("\n---integrity fault with tag---\n");
        return 1;
    }
    printf("\n---integrity fault with movie---\n");
    return 1;
}

void initMovie(){
    genre_list_cursor=0;

    //init movie

    movies = (Movie *) malloc(sizeof(Movie));
    genre_list = (char **)malloc(sizeof(char*));

    FILE *fp;
    if (TEST_FILE_FLAG){
        fp = fopen(testMovieFile, "r");
    }
    else{
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
        strtok(split1+strlen(split1)-5, ")");
        char *potential = split1+strlen(split1)-4;
        int potential_int = atoi(potential);
        (movies + index)->release_year = potential_int;
        (movies + index)->title = malloc(strlen(split1) * sizeof(char));
        strtok(split1+strlen(split1)-7, " (");
        strcpy((movies + index)->title, split1);

        //2
        split2 = strtok(split2, "\n");
        int cnt;
        (movies + index)->genre = (int *) malloc(sizeof(int));
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
            *(((movies + index)->genre)) = genreIndex_ByString(split2);
            (movies + index)->sizeof_genre = 1;
        }

        (movies+index) -> enabled = 1;

        index++;
    }
    fclose(fp);

    movie_count = index;
    return;
}
void initTag(){
    FILE *fp;
    char line[LINE_LENGTH];
    int index = 0;

    //init tag
    tags = (Tag *) malloc(sizeof(Tag));

    if (TEST_FILE_FLAG){
        fp = fopen(testTagFile, "r");
    }
    else{
        fp = fopen(tagFile, "r");
    }
    index = 0;
    while(fgets(line, sizeof(line) - 1, fp) != NULL){
        tags = (Tag *) realloc(tags, (index+1) * sizeof(Tag));

        char *split_t = split_back(line, "::");
        char *split0 = split_front(line, "::"); //tag0
        char *split_t_t = split_back(split_t, "::");
        char *split1 = split_front(split_t, "::"); //tag1
        char *split3 = split_back(split_t_t, "::");
        char *split2 = split_front(split_t_t, "::");

        //0
        (tags+index) -> user_id = atoi(split0);

        //1
        (tags+index) -> movie_id = atoi(split1);

        //2
        (tags+index) -> tag = malloc((strlen(split2)+1)* sizeof(char));
        strcpy((tags+index) -> tag, split2);

        //3
        split3 = strtok(split3, "\n");
        (tags+index) -> timestamp = atoll(split3);

        (tags+index) -> enabled = 1;

        index++;
    }
    fclose(fp);

    tag_count = index;
    return;
}
void initUser(){
    users = malloc(sizeof(User));
    FILE *fp;
    char line[LINE_LENGTH];
    int index = 0;

    if (TEST_FILE_FLAG){
        fp = fopen(testUserFile, "r");
    }
    else{
        fp = fopen(userFile, "r");
    }

    while(fgets(line, sizeof(line) - 1, fp) != NULL){
        users = (User *) realloc(users, (index+1) * sizeof(User));

        char *split_t = split_back(line, "::");
        char *split0 = split_front(line, "::"); //tag0
        char *split_t_t = split_back(split_t, "::");
        char *split1 = split_front(split_t, "::"); //tag1
        char *split3 = split_back(split_t_t, "::");
        char *split2 = split_front(split_t_t, "::");

        //0
        (users+index) -> user_id = atoi(split0);

        //1
        (users+index) -> user_name = malloc((strlen(split1)+1)* sizeof(char));
        strcpy((users+index) -> user_name, split1);

        //2
        (users+index) -> password = malloc((strlen(split2)+1)* sizeof(char));
        strcpy((users+index) -> password, split2);

        //3
        split3 = strtok(split3, "\n");
        int cnt;
        (users + index)->favourite_index = (int *) malloc(sizeof(int));
        if (strstr(split3, "|")) {
            cnt = 0;
            char *favourite_index_split = strtok(split3, "|");
            while (favourite_index_split != NULL) {
                (users + index)->favourite_index = realloc((users + index)->favourite_index, (cnt + 1) * sizeof(int));
                *(((users + index)->favourite_index) + cnt) = atoi(favourite_index_split);
                favourite_index_split = strtok(NULL, "|");
                cnt++;
            }
            (users + index)->sizeof_favourites = cnt;
        } else {
            *(((users + index)->favourite_index)) = atoi(split3);
            (users + index)->sizeof_favourites = 1;
        }

        (users+index) -> enabled = 1;


        index++;
    }

    user_count = index;
    fclose(fp);
}
void initFavourite(){
    FILE *fp;
    char line[LINE_LENGTH];
    int index = 0;

    //init tag
    favourites = (Favourite *) malloc(sizeof(Favourite));

    if (TEST_FILE_FLAG){
        fp = fopen(testFavouriteFile, "r");
    }
    else{
        fp = fopen(favouriteFile, "r");
    }
    index = 0;
    while(fgets(line, sizeof(line) - 1, fp) != NULL){
        favourites = (Favourite *) realloc(favourites, (index+1) * sizeof(Favourite));

        char *split1 = split_back(line, "::");
        char *split0 = split_front(line, "::");

        //0
        (favourites+index) -> user_id = atoi(split0);

        //1
        (favourites+index) -> movie_id = atoi(split1);

        (favourites+index) -> enabled = 1;

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

void saveUser(){
    FILE *fp1;
    if(TEST_FILE_FLAG){
        fp1 = fopen(testUserFile, "w");
    }
    else{
        fp1 = fopen(userFile, "w");
    }

    if(user_count != 0) {
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

    fclose(fp1);
}
void saveMovie(){
    FILE *fp1;
    if(TEST_FILE_FLAG){
        fp1 = fopen(testMovieFile, "w");
    }
    else{
        fp1 = fopen(movieFile, "w");
    }

    if(movie_count != 0) {
        for (int i = 0; i < movie_count; i++) {
            if ((movies + i)->enabled) {
                fprintf(fp1, "%d::%s (%d)::", (movies + i)->movie_id, (movies + i)->title, (movies + i)->release_year);
                for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
                    fprintf(fp1, "%s", genre_list[*((movies + i)->genre + j)]);
                    if (j != ((movies + i)->sizeof_genre) - 1) {
                        fprintf(fp1, "|");
                    }
                }
                fprintf(fp1, "\n");
            }
        }
    }

    fclose(fp1);
}
void saveTag(){
    FILE *fp2;
    if(TEST_FILE_FLAG){
        fp2 = fopen(testTagFile, "w");
    }
    else{
        fp2 = fopen(tagFile, "w");
    }

    if(tag_count != 0) {
        for (int i = 0; i < tag_count; i++) {
            if((tags+i)->enabled) {
                Tag temp = *(tags + i);
                fprintf(fp2, "%d::%d::%s::%lld\n", temp.user_id, temp.movie_id, temp.tag, temp.timestamp);
            }
        }
    }

    fclose(fp2);
}
void saveFavourite(){
    FILE *fp2;
    if(TEST_FILE_FLAG){
        fp2 = fopen(testFavouriteFile, "w");
    }
    else{
        fp2 = fopen(favouriteFile, "w");
    }

    if(tag_count != 0) {
        for (int i = 0; i < favourite_count; i++) {
            if((favourites+i)->enabled) {
                Favourite temp = *(favourites + i);
                fprintf(fp2, "%d::%d\n", temp.user_id, temp.movie_id);
            }
        }
    }

    fclose(fp2);
}
void save(){
    saveMovie();
    saveTag();
    saveUser();
    saveFavourite();
}

void testPrint(){
    if(movie_count != 0) {
        for (int i = 0; i < movie_count; i++) {
            printf("%d::%s (%d)::", (movies + i)->movie_id, (movies + i)->title, (movies + i)->release_year);
            for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
                printf("%s|", genre_list[*((movies + i)->genre + j)]);
            }
            printf("\b\n");
        }
    }

    if(tag_count != 0) {
        for (int i = 0; i < tag_count; i++) {
            Tag temp = *(tags + i);
            printf("%d::%d::%s::%lld\n", temp.user_id, temp.movie_id, temp.tag, temp.timestamp);
        }
    }
}

int genreIndex_ByString(char *genre){
    int flag=1;
    int i;
    strcpy(genre, tolowerCapitalizer(genre));
    for (i = 0; i < genre_list_cursor; i++) {
        if (!strcmp(genre, *(genre_list+i))) {
            flag=0;
            break;
        }
    }
    if(flag){
        genre_list = (char **)realloc(genre_list,sizeof(char*)*(genre_list_cursor+1));
        *(genre_list+genre_list_cursor) = (char *) malloc(sizeof(char)*strlen(genre));
        strcpy(*(genre_list+genre_list_cursor), genre);
        genre_list_cursor++;
    }
    return i;
}

void retire(){
    free(movies);
    free(tags);
    free(users);
    free(favourites);
}

int* genreIndex_ByMovieID(int movieID){
    int *return_array;
    int index = movieIndex_ByID(movieID);
    return_array = malloc(sizeof(int) * ((movies+index)->sizeof_genre)+1);
    memcpy(return_array, (movies+index)->genre, sizeof(int)*((movies+index)->sizeof_genre));
    *(return_array + (movies+index)->sizeof_genre) = END_OF_INT_ARRAY;
    return return_array;
}

int movieIDExists(int movie_id){
    for(int i=0;i<movie_count;i++){
        if((movies + i)->movie_id == movie_id){
            return 1;
        }
    }
    return 0;
}
int movieIndexExists(int index){
    if(index<movie_count) {
        return 1;
    }
    return 0;
}

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

int addToUserFavouriteIndex(int user_index, int movieID){
    (users+user_index)->favourite_index = realloc((users+user_index)->favourite_index, sizeof(int)*((users+user_index)->sizeof_favourites+1));
    int counter = (users+user_index)->sizeof_favourites;
    *(((users+user_index) -> favourite_index) + counter) = movieID;

    (users+user_index)->sizeof_favourites++;
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

int deleteMovie_ByIndex(int index){
    if(!movieIndexExists(index)){
        return FAIL_NO_SUCH_INDEX;
    }
    (movies + index) -> enabled = 0;
    saveMovie();
    initMovie();
    return SUCCESS;
}
int deleteTag_ByIndex(int index){
    ////add tagIndexExists here
    if(index>=tag_count){
        return FAIL_NO_SUCH_INDEX;
    }
    (tags + index) -> enabled = 0;
    saveTag();
    initTag();
    return SUCCESS;
}

char* tolowerString(char *content){
    char* return_array = malloc(sizeof(char));
    for(int i=0;i<strlen(content);i++){
        return_array = realloc(return_array, sizeof(char)*(i+1));
        *(return_array+i) = tolower(*(content+i));
    }
    return_array = realloc(return_array, sizeof(char)*strlen(content)+1);
    *(return_array+strlen(content)) = '\0';
    return return_array;
}
char* tolowerCapitalizer(char *content){
    char* return_array = malloc(sizeof(char)+1);
    *(return_array+0) = toupper(*content);
    int bigFlag = 0;
    for(int i=1;i<strlen(content);i++){
        return_array = realloc(return_array, sizeof(char)*(i+1));
        if(!bigFlag) {
            *(return_array + i) = tolower(*(content + i));
        }
        else{
            *(return_array + i) = toupper(*(content + i));
            bigFlag = 0;
        }

        if(*(return_array + i) == '-'){
            bigFlag = 1;
        }
    }
    return_array = realloc(return_array, sizeof(char)*strlen(content)+1);
    *(return_array+strlen(content)) = '\0';
    return return_array;
}

int* movieIndex_ByTitle(char *title){
    int *return_array = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<movie_count;i++){
        if(strstr(tolowerString((movies + i)->title), tolowerString(title))){
            count++;
            return_array = realloc(return_array, sizeof(int)*count);
            *(return_array+count-1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int)*(count+1));
    *(return_array+count) = END_OF_INT_ARRAY;
    return return_array;
}

int compare(const void *first, const void *second){
    if (*(int*)first > *(int*)second)
        return 1;
    else if (*(int*)first < *(int*)second)
        return -1;
    else
        return 0;
}
int* tempSortGenre(int index){
    int *ar = malloc(sizeof(int)*(movies+index)->sizeof_genre);
    memcpy(ar, (movies+index)->genre, ((movies+index)->sizeof_genre)* sizeof(int));
    qsort(ar, (size_t)((movies+index)->sizeof_genre), sizeof(int), compare);
    return ar;
}
int* movieIndex_ByGenre(int *genre, short genre_count){
    int *return_array = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<movie_count;i++) {
        if ((movies+i)->sizeof_genre == genre_count) {
            int flag=1;
            int *ar = malloc(sizeof(int)*genre_count);
            memcpy(ar, tempSortGenre(i), genre_count* sizeof(int));
            qsort(genre, (size_t)genre_count, sizeof(int), compare);
            printf("%d %d %d %d\n", *ar, *genre, *(ar+1), *(genre+1));
            for(int j=0;j<genre_count;j++) {
                if(*(ar+j) != *(genre+j)){
                    flag=0;
                    break;
                }
            }
            if(flag){
                count++;
                return_array = realloc(return_array, sizeof(int) * count);
                *(return_array + count - 1) = i;
            }
        }
    }
    return_array = realloc(return_array, sizeof(int)*(count+1));
    *(return_array+count) = END_OF_INT_ARRAY;
    return return_array;
}
int movieIndex_ByID(int movie_id){
    if(!movieIDExists(movie_id)){
        return FAIL_NO_SUCH_MOVIE_ID;
    }
    for(int i=0;i<movie_count;i++) {
        if((movies + i)->movie_id == movie_id){
            return i;
        }
    }
}
int userIndex_ByUserID(int user_id){
    for(int i=0;i<user_count;i++){
        if((users+i)->user_id == user_id){
            return i;
        }
    }
    return FAIL_NO_SUCH_USER_ID;
}
int* tagIndex_ByUserID(int user_id){
    int *return_array = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if((tags + i)->user_id == user_id){
            count++;
            return_array = realloc(return_array, sizeof(int)*count);
            *(return_array+count-1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int)*(count+1));
    *(return_array+count) = END_OF_INT_ARRAY;
    return return_array;
}
int* tagIndex_ByMovieID(int movie_id){
    int *return_array = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if((tags + i)->movie_id == movie_id){
            count++;
            return_array = realloc(return_array, sizeof(int)*count);
            *(return_array+count-1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int)*(count+1));
    *(return_array+count) = END_OF_INT_ARRAY;
    return return_array;
}
int* tagIndex_ByDoubleID(int user_id, int movie_id){
    int *return_array = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if((tags + i)->user_id == user_id && (tags + i)->movie_id == movie_id){
            count++;
            return_array = realloc(return_array, sizeof(int)*count);
            *(return_array+count-1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int)*(count+1));
    *(return_array+count) = END_OF_INT_ARRAY;
    return return_array;
}
int* tagIndex_ByTag(char *tag){
    int *return_array = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if(strstr(tolowerString((tags + i)->tag), tolowerString(tag))){
            count++;
            return_array = realloc(return_array, sizeof(int)*count);
            *(return_array+count-1) = i;
        }
    }
    return_array = realloc(return_array, sizeof(int)*(count+1));
    *(return_array+count) = END_OF_INT_ARRAY;
    return return_array;
}
int* favouriteIndex_ByUserID(int user_id){
    int user_index = userIndex_ByUserID(user_id);
    int *return_array = (users+user_index) -> favourite_index;
    int index_count = (users+user_index) -> sizeof_favourites;
    return_array = realloc(return_array, sizeof(int)*(index_count+1));
    *(return_array+index_count) = END_OF_INT_ARRAY;
    return return_array;
}
int* favouriteIndex_ByMovieID(int movie_id){

}
int favouriteIndex_ByDoubleID(int user_id, int movie_id);

int main(){
    clock_t t;
    t = clock();

    init();
    addMovie();
//addUserEntity(30000, "user_name", "1524124");
//addFavouriteEntity();
    if(integrity()){
        printf("Error occured while reading file. Please check file content.\n");
    }
    save();
    if(integrity()){
        printf("Error occured while saving file. Please check file content.\n");
    }
    retire();

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("execution time : %lf seconds", time_taken);

    return 0;
}

//박정훈의 코드
//박정훈의 코드
//박정훈의 코드


void addMovie() {
    Movie *movieEntity;
    movieEntity = (Movie *) malloc(sizeof(Movie));
    char movieIDEntity[12];
    movieEntity -> title = (char *) malloc(sizeof(movieEntity->title)*300);
    char releaseYearEntity[5];
    char sizeof_genreEntity[3];
    movieEntity -> genre = (int *)malloc(sizeof(int)*100);

    while (1) {
        int temp;
        printf("Movie ID to add: ");
        scanf(" %s", movieIDEntity);
        for (int i = 0; i < strlen(movieIDEntity); i++) {
            if(isdigit (*movieIDEntity+i)==0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt=0;
        for(int i=0; i < strlen(movieIDEntity); i++) {
            if(isdigit (*movieIDEntity+i)!=0)
                cnt++;
        }
        if(cnt==strlen(movieIDEntity) &&cnt!=0) {
            temp=atoi(movieIDEntity);
            movieEntity -> movie_id = temp;
            break;
        }
    }

    printf("Title: ");
//    char titleEntity;
//    for(int i=0; titleEntity[i]=getchar() = '\n'; i++) {
//        movieEntity -> title = (char *) realloc(movieEntity -> title, sizeof((movieEntity -> title)+1));
//        printf("%s",titleEntity);
//    }
    scanf(" %s", movieEntity ->title);

    while (1) {
        int temp;
        printf("Release year: ");
        scanf(" %s", releaseYearEntity);
        for (int i = 0; i < strlen(releaseYearEntity); i++) {
            if(isdigit (*releaseYearEntity+i)==0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt=0;
        for(int i=0; i < strlen(releaseYearEntity); i++) {
            if(isdigit(*releaseYearEntity+i)!=0)
                cnt++;
        }
        if (cnt==strlen(releaseYearEntity) && cnt!=0) {
            temp=atoi(releaseYearEntity);
            movieEntity -> release_year = temp;
            break;
        }
    }

    while (1) {
        int temp;
        printf("How many genre do you want to add: ");
        scanf(" %s", sizeof_genreEntity);
        for (int i = 0; i < strlen(sizeof_genreEntity); i++) {
            if(isdigit (*sizeof_genreEntity+i)==0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt=0;
        for(int i=0; i < strlen(sizeof_genreEntity); i++) {
            if(isdigit(*sizeof_genreEntity+i)!=0)
                cnt++;
        }

        if (cnt==strlen(sizeof_genreEntity) && cnt!=0) {
            temp=atoi(sizeof_genreEntity);
            movieEntity -> sizeof_genre = temp;
            break;
        }
    }

    int genreIndex[100];
    for(int i=1;i<=movieEntity->sizeof_genre;i++) {
        char *inputGenre;
        inputGenre=(char *)malloc(sizeof(char)*20);
        printf("Genre %d: ",i);
        scanf(" %s",inputGenre);
        genreIndex[i]=genreIndex_ByString(inputGenre);
        //movieEntity -> genre = (int *)realloc(movieEntity -> genre, sizeof(int)+1);
    }
    for(int i=1; i<=movieEntity->sizeof_genre;i++) {
        movieEntity -> genre = &genreIndex[i];
    }

    int resultData = addMovieEntity(movieEntity->movie_id, movieEntity->title, movieEntity->release_year, movieEntity->genre, movieEntity->sizeof_genre);
    if (resultData==SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!\n");
        return;
    }
    else if (resultData==FAIL_MOVIE_ID_ALREADY_EXISTS)
    {
        printf("Already exist movie\n");
        return;
    }
    else if (resultData==FAIL_INVALID_YEAR)
    {
        printf("Invalid year\n");
        return;
    }
    else
    {
        printf("Have some problem. Add again\n");
        return;
    }
}
void removeMovie() {
    int movieIDRemove;
    while(1) {
        printf("Movie ID to remove: ");
        scanf("%d",&movieIDRemove);
        if (movieIDExists(movieIDRemove)==1) {
            break;
        }
        else {
            printf("movie ID doesn't exist\n");
        }
    }
    int movieIndexRemove;
    movieIndexRemove=movieIndex_ByID(movieIDRemove);
    if(deleteMovie_ByIndex(movieIndexRemove)==SUCCESS) {
        printf("REMOVE SUCCESSFULLY!!");
        return;
    }
    else {
        printf("Have some problem. Remove again");
        return;
    }
}
void addTag() {
    Tag *tagEntity;
    tagEntity = (Tag *)malloc(sizeof(Tag));
    char userIDEntity[7];
    char movieIDEntity[12];
    tagEntity -> tag = (char *) malloc(sizeof(tagEntity->tag)*100);
    char timestampEntity[12];

    while (1) {
        int temp;
        printf("User ID to add: ");
        scanf(" %s", userIDEntity);
        for (int i = 0; i < strlen(userIDEntity); i++) {
            if(isdigit (*userIDEntity+i)==0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt=0;
        for(int i=0; i < strlen(userIDEntity); i++) {
            if(isdigit (*userIDEntity+i)!=0)
                cnt++;
        }
        if(cnt==strlen(userIDEntity) &&cnt!=0) {
            temp=atoi(userIDEntity);
            tagEntity -> user_id = temp;
            break;
        }
    }
    while (1) {
        int temp;
        printf("Movie ID to add: ");
        scanf(" %s", movieIDEntity);
        for (int i = 0; i < strlen(movieIDEntity); i++) {
            if(isdigit (*movieIDEntity+i)==0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt=0;
        for(int i=0; i < strlen(movieIDEntity); i++) {
            if(isdigit (*movieIDEntity+i)!=0)
                cnt++;
        }
        if(cnt==strlen(movieIDEntity) &&cnt!=0) {
            temp=atoi(movieIDEntity);
            tagEntity -> movie_id = temp;
            break;
        }
    }
    printf("Tag: ");
    ////modify here
    scanf(" %s",tagEntity->tag);
    while (1) {
        long long temp;
        printf("timestamp to add: ");
        scanf(" %s", timestampEntity);
        for (int i = 0; i < strlen(timestampEntity); i++) {
            if(isdigit (*timestampEntity+i)==0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt=0;
        for(int i=0; i < strlen(timestampEntity); i++) {
            if(isdigit (*timestampEntity+i)!=0)
                cnt++;
        }
        if(cnt==strlen(timestampEntity) &&cnt!=0) {
            temp=atoi(timestampEntity);
            tagEntity -> timestamp = temp;
            break;
        }
    }

    int resultData=addTagEntity(tagEntity->user_id, tagEntity->movie_id, tagEntity->tag, tagEntity->timestamp);

    if (resultData==SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!");
        return;
    }
    else
    {
        printf("Have some problem. Add again");
        return;
    }
}
void removeTag() {
    int tagRemove;
    while(1) {
        printf("Tag to remove: ");
        scanf("%d",&tagRemove);
        if ((tagRemove)==1) {
            break;
        }
        else {
            printf("Tag doesn't exist\n");
        }
    }
    int tagIndexRemove;
    tagIndexRemove= *tagIndex_ByUserID(tagRemove);
    if(deleteTag_ByIndex(tagIndexRemove)==SUCCESS) {
        printf("REMOVE SUCCESSFULLY!!");
        return;
    }
    else {
        printf("Have some problem. Remove again");
        return;
    }
}
void addFavourite() {
    Favourite *favouriteEntity;
    favouriteEntity = (Favourite *)malloc(sizeof(Favourite));
    char userIDEntity[7];
    char movieIDEntity[12];

    while (1) {
        int temp;
        printf("User ID to add: ");
        scanf(" %s", userIDEntity);
        for (int i = 0; i < strlen(userIDEntity); i++) {
            if (isdigit(*userIDEntity + i) == 0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt = 0;
        for (int i = 0; i < strlen(userIDEntity); i++) {
            if (isdigit(*userIDEntity + i) != 0)
                cnt++;
        }
        if (cnt == strlen(userIDEntity) && cnt != 0) {
            temp = atoi(userIDEntity);
            favouriteEntity->user_id = temp;
            break;
        }
    }
    while (1) {
        int temp;
        printf("Movie ID to add: ");
        scanf(" %s", movieIDEntity);
        for (int i = 0; i < strlen(movieIDEntity); i++) {
            if (isdigit(*movieIDEntity + i) == 0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt = 0;
        for (int i = 0; i < strlen(movieIDEntity); i++) {
            if (isdigit(*movieIDEntity + i) != 0)
                cnt++;
        }
        if (cnt == strlen(movieIDEntity) && cnt != 0) {
            temp = atoi(movieIDEntity);
            favouriteEntity->movie_id = temp;
            break;
        }
    }

    int resultData = addFavouriteEntity(favouriteEntity->user_id, favouriteEntity->movie_id);

    if (resultData == SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!");
        return;
    }
    else
    {
        printf("Have some problem. Add again");
        return;
    }
}

int favouriteExists(int userID) {
    for (int i = 0; i < favourite_count; i++) {
        if ((favourites + i)->user_id == userID) {
            return 1;
        }
    }
    return 0;
}

void removeFavourite() {
    int favouriteRemove;
    while (1) {
        printf("User ID to remove favourite: ");
        scanf("%d", &favouriteRemove);
        if (favouriteExists(favouriteRemove) == 1) {
            break;
        }
        else {
            printf("Tag doesn't exist\n");
        }
    }
    int favouriteIndexRemove;
    favouriteIndexRemove = *favouriteIndex_ByUserID(favouriteRemove);
    if (deleteFavourite_ByIndex(favouriteIndexRemove) == SUCCESS) {
        printf("REMOVE SUCCESSFULLY!!");
        return;
    }
    else {
        printf("Have some problem. Remove again");
        return;
    }
}

void addUser() {
    User *userEntity;
    userEntity = (User *)malloc(sizeof(User));
    char userIDEntity[7];
    userEntity->user_name = (char *)malloc(sizeof(userEntity->user_name) * 100);
    userEntity->password = (char *)malloc(sizeof(userEntity->password) * 100);

    while (1) {
        int temp;
        printf("User ID to add: ");
        scanf(" %s", userIDEntity);
        for (int i = 0; i < strlen(userIDEntity); i++) {
            if (isdigit(*userIDEntity + i) == 0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt = 0;
        for (int i = 0; i < strlen(userIDEntity); i++) {
            if (isdigit(*userIDEntity + i) != 0)
                cnt++;
        }
        if (cnt == strlen(userIDEntity) && cnt != 0) {
            temp = atoi(userIDEntity);
            userEntity->user_id = temp;
            break;
        }
    }

    printf("user name: ");
    ////modify here
    scanf(" %s", userEntity->user_name);

    printf("password: ");
    ////modify here
    scanf(" %s", userEntity->password);

    int resultData = addUserEntity(userEntity->user_id, userEntity->user_name, userEntity->password);

    if (resultData == SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!");
        return;
    }
    else
    {
        printf("Have some problem. Add again");
        return;
    }
}

int favouriteIndexExists(int index) {
    if (index < favourite_count) {
        return 1;
    }
    return 0;
}
int userIDExists(int userID) {
    for (int i = 0; i < user_count; i++) {
        if ((users + i)->user_id == userID) {
            return 1;
        }
    }
    return 0;
}
int userIndexExists(int index) {
    if (index < user_count) {
        return 1;
    }
    return 0;
}

void removeUser() {
    int userIDRemove;
    while (1) {
        printf("User ID to remove: ");
        scanf("%d", &userIDRemove);
        if (userIDExists(userIDRemove) == 1) {
            break;
        }
        else {
            printf("User ID doesn't exist\n");
        }
    }
    int userIndexRemove;
    userIndexRemove = userIndex_ByUserID(userIDRemove);
    if (deleteUser_ByIndex(userIndexRemove) == SUCCESS) {
        printf("REMOVE SUCCESSFULLY!!");
        return;
    }
    else {
        printf("Have some problem. Remove again");
        return;
    }
}

int getnum() {
    int num = 0;
    printf("Please select an item: ");
    scanf("%d", &num);
    return num;
}

int selectMenu() {
    printf("<Movie data management program>\n");
    printf("1: Add movie \n2: Remove movie \n"
           "3: Add tag \n4: Remove tag \n"
           "5: Add favourite \n6: Remove favourite \n"
           "7: Add user \n8: Remove user \n"
           "9: Search by user ID \n10: Search by movie title \n"
           "0: Close\n");
    return getnum();
}

void searchByUserID() {
    User *userEntity;
    userEntity = (User *)malloc(sizeof(User));
    char userIDEntity[7];
    while (1) {
        int temp;
        printf("User ID to search: ");
        scanf(" %s", userIDEntity);
        for (int i = 0; i < strlen(userIDEntity); i++) {
            if (isdigit(*userIDEntity + i) == 0) {
                printf("It is not number\n");
                break;
            }
        }
        int cnt = 0;
        for (int i = 0; i < strlen(userIDEntity); i++) {
            if (isdigit(*userIDEntity + i) != 0)
                cnt++;
        }
        if (cnt == strlen(userIDEntity) && cnt != 0) {
            temp = atoi(userIDEntity);
            userEntity->user_id = temp;
            break;
        }
    }

    int resultData = userIDExists(userEntity->user_id);

    if (resultData == 1) {
        printf("user ID: %d\n", userEntity->user_id);
        printf("user name: %s\n", userEntity->user_name);
        printf("password: %s\n", userEntity->password);
        return;
    }
    else if (resultData == 0) {
        printf("There is no such user ID\n");
        return;
    }
    else {
        printf("Have some problem\n");
        return;
    }

}
void searchByMovieTitle() {
    Movie *movieEntity;
    movieEntity = (Movie *)malloc(sizeof(Movie));
    movieEntity->title = (char *)malloc(sizeof(movieEntity->title) * 300);

    printf("Title to search: ");
    //    char titleEntity;
    //    for(int i=0; titleEntity[i]=getchar() = '\n'; i++) {
    //        movieEntity -> title = (char *) realloc(movieEntity -> title, sizeof((movieEntity -> title)+1));
    //        printf("%s",titleEntity);
    //    }
    scanf(" %s", movieEntity->title);

    int resultData = movieTitleExists(movieEntity->title);

    if (resultData == 1) {
        printf("movie ID: %d\n", movieEntity->movie_id);
        printf("movie title: %s\n", movieEntity->title);
        printf("release year: %d\n", movieEntity->release_year);
        printf("genre: ");
        for(int i=0; i<movieEntity->sizeof_genre; i++) {
            int* genreIndex=genreIndex_ByMovieID(movieEntity->movie_id);

        }
        return;
    }
    else if (resultData == 0) {
        printf("There is no such user ID\n");
        return;
    }
    else {
        printf("Have some problem\n");
        return;
    }
}

void searchByMovieTitle() {
    char *searchMovieTitle;
    printf("Movie title to search: ");
    scanf(" %s",searchMovieTitle);

}

void close(){
    save();
    if(!integrity()){
        retire();
    }
    exit(0);
}

void menu() {
    selectMenu();
    int key;
    key = selectMenu();
    while (selectMenu() != 0)
    {
        switch (key)
        {
            case 1: addMovie(); break;
            case 2: removeMovie(); break;
            case 3: addTag(); break;
            case 4: removeTag(); break;
            case 5: addFavourite(); break;
            case 6: removeFavourite(); break;
            case 7: addUser(); break;
            case 8: removeUser(); break;
            case 9: searchByUserID(); break;
            case 10: searchByMovieTitle(); break;
            case 0: close(); break;
            default: printf("No number.\n"); break;
        }
    }
    return;
}

//todo : rename varaiables firstSecond -> first_second
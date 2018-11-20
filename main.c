#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define END_OF_INT_ARRAY -500

#define FAIL_MOVIE_ID_ALREADY_EXISTS -2000
#define FAIL_USER_ID_ALREADY_EXISTS -4000
#define FAIL_NO_SUCH_MOVIE_ID -5000
#define FAIL_NO_SUCH_INDEX -6000
#define FAIL_INVALID_YEAR -3000
#define SUCCESS -1000

#define TEST_FILE_FLAG 1

#define testTagFile "testtags.dat"
#define testMovieFile "testmovies.dat"
#define testUserFile "testusers.dat"
#define testFavouriteFile "testfavourites.dat"
#define tagFile "tags.dat"
#define movieFile "movies.dat"
#define userFile "users.dat"
#define favouriteFile "favourites.dat"

typedef struct {
    //please do not add more than 2 billion movies. You're not going to, are you?
    int movieID;
    char *title;
    int releaseYear;
    int *genre;
    short sizeof_genre;
    short enabled;
}Movie;
typedef struct {
    int userID;
    int movieID;
    char *tag;
    long long timestamp;
    short enabled;
}Tag;
typedef struct {
    int userID;
    int movieID;
    short enabled;
}Favourite;
typedef struct {
     int userID;
     char *userName;
     char *password;//yup, plaintext password right here.
     int *favouriteIndex;
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

char **genreList;
int genreListCursor=0;

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
void searchByUserID(); //input: userID - output: all tags made by the user, movie info of tags, favourites list, movie info of favourites
void searchByMovieTitle(); //input: title - output: genre, tags, releaseYear, favourited by whom?, similar movies
void retire();

//methods
int genreIndex_ByString(char *genre);
int* genreIndex_ByMovieID(int movieID);

void testPrint();

int addMovieEntity(int movieID, char *title, int releaseYear, int *genre, short genre_count);
int addTagEntity(int userID, int movieID, char *tag, long long timestamp);
int addUserEntity(int userID, char *userName, char *password);
int addUserEntity_TagData(int userID);
int addFavouriteEntity(int userID, int movieID);

int movieIDExists(int movieID);
int movieIndexExists(int index);
int userIndexExists(int index);
int userNameExists(char *name);

int deleteMovie_ByIndex(int index);
int deleteTag_ByIndex(int index);
int deleteUser_ByIndex(int index);
int deleteFavourite_ByIndex(int index);

void printMovieInfo(int movieID);
void printAllTag(int movieID);
void printTag(int index);

int* movieIndex_ByTitle(char *title); ////returns END_OF_INT_ARRAY at end of array
int* movieIndex_ByGenre(int *genre, short genre_count); ////returns END_OF_INT_ARRAY at end of array
int movieIndex_ByID(int movieID);

int* tagIndex_ByUserID(int userID); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByMovieID(int movieID); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByDoubleID(int userID, int movieID); ////returns END_OF_INT_ARRAY at end of array
int* tagIndex_ByTag(char *tag); ////returns END_OF_INT_ARRAY at end of array

int* favouriteIndex_ByUserID(int userID); ////returns END_OF_INT_ARRAY at end of array
int* favouriteIndex_ByMovieID(int movieID); ////returns END_OF_INT_ARRAY at end of array
int favouriteIndex_ByDoubleID(int userID, int movieID);

int integrityMovie(){
    FILE *fp;
    if(TEST_FILE_FLAG){
        fp = fopen(testMovieFile, "r");
    }
    else{
        fp = fopen(movieFile, "r");
    }
    char line[1000];
    char genLine[1000];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((movies + i)->enabled)){
            i++;
        }

        sprintf(genLine, "%d::%s (%d)::", ((movies + i)->movieID), (movies + i)->title, (movies + i)->releaseYear);
        for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
            strcat(genLine, genreList[*((movies + i)->genre + j)]);
            if (j != ((movies + i)->sizeof_genre) - 1) {
                strcat(genLine, "|");
            }
        }
        strcat(genLine, "\n");
        if(strcmp(line, genLine)) {
            printf("%s\n", line);
            printf("%s\n", genLine);
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
    char line[1000];
    char genLine[1000];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((tags + i)->enabled)){
            i++;
        }

        Tag temp = *(tags + i);
        sprintf(genLine, "%d::%d::%s::%lld\n", temp.userID, temp.movieID, temp.tag, temp.timestamp);
        if(strcmp(line, genLine)) {
            printf("%s\n", line);
            printf("%s\n", genLine);
            printf("i : %d\n", i);
            return 1;
        }
        i++;
    }

    fclose(fp);
    return 0;
}
int integrityUser(){
    return 0;
}
int integrityFavourite(){
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

int main(){
    init();
    if(integrity()){
        printf("Error occured while reading file. Please check file content.");
    }
    save();
    if(integrity()){
        printf("Error occured while saving file. Please check file content.");
    }

    retire();
    return 0;
}

void initMovie(){
    genreListCursor=0;

    //init movie

    movies = (Movie *) malloc(sizeof(Movie));
    genreList = (char **)malloc(sizeof(char*));

    FILE *fp;
    if (TEST_FILE_FLAG){
        fp = fopen(testMovieFile, "r");
    }
    else{
        fp = fopen(movieFile, "r");
    }
    char line[1000];
    int index = 0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        movies = (Movie *) realloc(movies, (index + 1) * sizeof(Movie));

        char *split_t = split_back(line, "::");
        char *split0 = split_front(line, "::");
        char *split2 = split_back(split_t, "::");
        char *split1 = split_front(split_t, "::");

        //0
        (movies + index)->movieID = atoi(split0);

        //1
        strtok(split1+strlen(split1)-5, ")");
        char *potential = split1+strlen(split1)-4;
        int potentialInt = atoi(potential);
        (movies + index)->releaseYear = potentialInt;
        (movies + index)->title = malloc(strlen(split1) * sizeof(char));
        strtok(split1+strlen(split1)-7, " (");
        strcpy((movies + index)->title, split1);

        //2
        split2 = strtok(split2, "\n");
        int cnt;
        (movies + index)->genre = (int *) malloc(sizeof(int));
        if (strstr(split2, "|")) {
            cnt = 0;
            char *genreSplit = strtok(split2, "|");
            while (genreSplit != NULL) {
                (movies + index)->genre = realloc((movies + index)->genre, (cnt + 1) * sizeof(int));
                *(((movies + index)->genre) + cnt) = genreIndex_ByString(genreSplit);
                genreSplit = strtok(NULL, "|");
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
    char line[1000];
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
        (tags+index) -> userID = atoi(split0);

        //1
        (tags+index) -> movieID = atoi(split1);

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
    char line[1000];
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
        (users+index) -> userID = atoi(split0);

        //1
        (users+index) -> userName = malloc((strlen(split1)+1)* sizeof(char));
        strcpy((users+index) -> userName, split1);

        //2
        (users+index) -> password = malloc((strlen(split2)+1)* sizeof(char));
        strcpy((users+index) -> password, split2);

        //3
        split3 = strtok(split3, "\n");
        int cnt;
        (users + index)->favouriteIndex = (int *) malloc(sizeof(int));
        if (strstr(split3, "|")) {
            cnt = 0;
            char *favouriteIndexSplit = strtok(split3, "|");
            while (favouriteIndexSplit != NULL) {
                (users + index)->favouriteIndex = realloc((users + index)->favouriteIndex, (cnt + 1) * sizeof(int));
                *(((users + index)->favouriteIndex) + cnt) = atoi(favouriteIndexSplit);
                favouriteIndexSplit = strtok(NULL, "|");
                cnt++;
            }
            (users + index)->sizeof_favourites = cnt;
        } else {
            *(((users + index)->favouriteIndex)) = atoi(split3);
            (users + index)->sizeof_favourites = 1;
        }

        (users+index) -> enabled = 1;


        index++;
    }

    user_count = index;
    fclose(fp);
}
void initFavourite(){

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
                fprintf(fp1, "%d::%s::%s::", (users + i)->userID, (users + i)->userName, (users + i)->password);
                for (int j = 0; j < (users + i)->sizeof_favourites; j++) {
                    fprintf(fp1, "%d", *(((users + i)->favouriteIndex) + j));
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
                fprintf(fp1, "%d::%s (%d)::", (movies + i)->movieID, (movies + i)->title, (movies + i)->releaseYear);
                for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
                    fprintf(fp1, "%s", genreList[*((movies + i)->genre + j)]);
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
                fprintf(fp2, "%d::%d::%s::%lld\n", temp.userID, temp.movieID, temp.tag, temp.timestamp);
            }
        }
    }

    fclose(fp2);
}
void saveFavourite(){

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
            printf("%d::%s (%d)::", (movies + i)->movieID, (movies + i)->title, (movies + i)->releaseYear);
            for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
                printf("%s|", genreList[*((movies + i)->genre + j)]);
            }
            printf("\b\n");
        }
    }

    if(tag_count != 0) {
        for (int i = 0; i < tag_count; i++) {
            Tag temp = *(tags + i);
            printf("%d::%d::%s::%lld\n", temp.userID, temp.movieID, temp.tag, temp.timestamp);
        }
    }
}

int genreIndex_ByString(char *genre){
    int flag=1;
    int i;
    for (i = 0; i < genreListCursor; i++) {
        if (!strcmp(genre, *(genreList+i))) {
            flag=0;
            break;
        }
    }
    if(flag){
        genreList = (char **)realloc(genreList,sizeof(char*)*(genreListCursor+1));
        *(genreList+genreListCursor) = (char *) malloc(sizeof(char)*strlen(genre));
        strcpy(*(genreList+genreListCursor), genre);
        genreListCursor++;
    }
    return i;
}

//void menu() {
//    int key = 0;
//    while ((key = selectMenu()) != 0)
//    {
//        switch (key)
//        {
//            case 1: addMovie(); break;
//            case 2: removeMovie(); break;
//            case 3: addTag(); break;
//            case 4: removeTag(); break;
//            case 5: addFavourite(); break;
//            case 6: searchByUserID(); break;
//            case 7: searchByMovieTitle(); break;
//            case 0: close(); break;
//            default: printf("No number.\n"); break;
//        }
//    }
//    return;
//}

void retire(){
    free(movies);
    free(tags);
    free(users);
    free(favourites);
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
           "5: Add favourite \n6: Search by user ID \n"
           "7: Search by movie title \n0: Close\n");
    return getnum();
}

void close(){
    save();
    if(integrity()){

    }
    exit(0);
}

int* genreIndex_ByMovieID(int movieID){
    int *returnArray;
    int index = movieIndex_ByID(movieID);
    returnArray = malloc(sizeof(int) * ((movies+index)->sizeof_genre)+1);
    memcpy(returnArray, (movies+index)->genre, sizeof(int)*((movies+index)->sizeof_genre));
    *(returnArray + (movies+index)->sizeof_genre) = END_OF_INT_ARRAY;
    return returnArray;
}

int movieIDExists(int movieID){
    for(int i=0;i<movie_count;i++){
        if((movies + i)->movieID == movieID){
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

int addMovieEntity(int movieID, char *title, int releaseYear, int *genre, short genre_count){
    if(movieIDExists(movieID)){
        return FAIL_MOVIE_ID_ALREADY_EXISTS;
    }
    if(releaseYear > 9999 || releaseYear < 1000){
        return FAIL_INVALID_YEAR;
    }

    movies = (Movie *) realloc(movies, (movie_count) * sizeof(Movie));

    //0
    (movies + movie_count)->movieID = movieID;

    //1
    (movies + movie_count)->releaseYear = releaseYear;
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
int addTagEntity(int userID, int movieID, char *tag, long long timestamp){
    //0
    (tags+tag_count) -> userID = userID;

    //1
    (tags+tag_count) -> movieID = movieID;

    //2
    (tags+tag_count) -> tag = malloc((strlen(tag)+1)* sizeof(char));
    strcpy((tags+tag_count) -> tag, tag);

    //3
    (tags+tag_count) -> timestamp = timestamp;

    (tags+tag_count) -> enabled = 1;

    tag_count++;
    return SUCCESS;
}
int addUserEntity(int userID, char *userName, char *password){

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
    char* returnArray = malloc(sizeof(char));
    for(int i=0;i<strlen(content);i++){
        returnArray = realloc(returnArray, sizeof(char)*(i+1));
        *(returnArray+i) = tolower(*(content+i));
    }
    returnArray = realloc(returnArray, sizeof(char)*strlen(content)+1);
    *(returnArray+strlen(content)) = '\0';
    return returnArray;
}

int* movieIndex_ByTitle(char *title){
    int *returnArray = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<movie_count;i++){
        if(strstr(tolowerString((movies + i)->title), tolowerString(title))){
            count++;
            returnArray = realloc(returnArray, sizeof(int)*count);
            *(returnArray+count-1) = i;
        }
    }
    returnArray = realloc(returnArray, sizeof(int)*(count+1));
    *(returnArray+count) = END_OF_INT_ARRAY;
    return returnArray;
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
    int *returnArray = malloc(sizeof(int));
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
                returnArray = realloc(returnArray, sizeof(int) * count);
                *(returnArray + count - 1) = i;
            }
        }
    }
    returnArray = realloc(returnArray, sizeof(int)*(count+1));
    *(returnArray+count) = END_OF_INT_ARRAY;
    return returnArray;
}
int movieIndex_ByID(int movieID){
    if(!movieIDExists(movieID)){
        return FAIL_NO_SUCH_MOVIE_ID;
    }
    for(int i=0;i<movie_count;i++) {
        if((movies + i)->movieID == movieID){
            return i;
        }
    }
}
int* tagIndex_ByUserID(int userID){
    int *returnArray = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if((tags + i)->userID == userID){
            count++;
            returnArray = realloc(returnArray, sizeof(int)*count);
            *(returnArray+count-1) = i;
        }
    }
    returnArray = realloc(returnArray, sizeof(int)*(count+1));
    *(returnArray+count) = END_OF_INT_ARRAY;
    return returnArray;
}
int* tagIndex_ByMovieID(int movieID){
    int *returnArray = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if((tags + i)->movieID == movieID){
            count++;
            returnArray = realloc(returnArray, sizeof(int)*count);
            *(returnArray+count-1) = i;
        }
    }
    returnArray = realloc(returnArray, sizeof(int)*(count+1));
    *(returnArray+count) = END_OF_INT_ARRAY;
    return returnArray;
}
int* tagIndex_ByDoubleID(int userID, int movieID){
    int *returnArray = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if((tags + i)->userID == userID && (tags + i)->movieID == movieID){
            count++;
            returnArray = realloc(returnArray, sizeof(int)*count);
            *(returnArray+count-1) = i;
        }
    }
    returnArray = realloc(returnArray, sizeof(int)*(count+1));
    *(returnArray+count) = END_OF_INT_ARRAY;
    return returnArray;
}
int* tagIndex_ByTag(char *tag){
    int *returnArray = malloc(sizeof(int));
    int count=0;
    for(int i=0;i<tag_count;i++){
        if(strstr(tolowerString((tags + i)->tag), tolowerString(tag))){
            count++;
            returnArray = realloc(returnArray, sizeof(int)*count);
            *(returnArray+count-1) = i;
        }
    }
    returnArray = realloc(returnArray, sizeof(int)*(count+1));
    *(returnArray+count) = END_OF_INT_ARRAY;
    return returnArray;
}
int* favouriteIndex_ByUserID(int userID);
int* favouriteIndex_ByMovieID(int movieID);
int favouriteIndex_ByDoubleID(int userID, int movieID);
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define fail_movieIDExists 2
#define fail_invalidYear 3
#define success 1

#define testing_file 1

#define testTagFile "testtags.dat"
#define testMovieFile "testmovies.dat"
#define tagFile "tags.dat"
#define movieFile "movies.dat"

typedef struct {
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
     char *password;
     int *favouriteIndex;
}User;

Movie *movies;
Tag *tags;
Favourite *favourites;
int movie_count;
int tag_count;
int favourite_count;

char *genreList[50];
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
char *split_back(char *str, const char *delim)
{
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

//methods
int genreIndex_ByString(char *genre);
int addMovieEntity(int movieID, char *title, int releaseYear, int *genre, short genre_count);
int addTagEntity(int userID, int movieID, char *tag, long long timestamp);
int addUserEntity(int userID, char *userName, char *password);

void deleteMovie_ByIndex(int index);
void deleteTag_ByIndex(int index);

void printMovieInfo(int movieID);
void printAllTag(int movieID);
void printTag(int index);
int* movieIndex_ByTitle(char *title); ////returns -1 at end of array
int* movieIndex_ByGenre(int *genre, short genre_count); ////returns -1 at end of array
int movieIndex_ByID(int movieID);
int* tagIndex_ByUserID(int userID); ////returns -1 at end of array
int* tagIndex_ByMovieID(int movieID); ////returns -1 at end of array
int* tagIndex_ByDoubleID(int userID, int movieID); ////returns -1 at end of array
int* tagIndex_ByTag(char *tag); ////returns -1 at end of array
int* favouriteIndex_ByUserID(int userID); ////returns -1 at end of array
int* favouriteIndex_ByMovieID(int movieID); ////returns -1 at end of arrayreturns -1 at end of array
int favouriteIndex_ByDoubleID(int userID, int movieID);

int integrity() {
    FILE *fp;
    if(testing_file){
        fp = fopen(testMovieFile, "r");
    }
    else{
        fp = fopen(movieFile, "r");
    }
    char line[500];
    char genLine[500];
    int i=0;
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        while(!((movies + i)->enabled)){
            i++;
        }

//        strcpy(genLine, "");
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
            printf("%s\n",genLine);
            printf("i : %d\n",i);
//            return 1;
        }
        i++;
//        strcpy(line, "");
    }

    return 0;
}

int main(){
    init();
    save();
//    int ar[] = {1,3};
    int *result = tagIndex_ByTag("!");
    for(int i=0;;i++){
        if(*(result+i) == -1)
            break;
        printf("%d ",*(result+i));
    }

    if(integrity()){
        printf("something is seriously wrong with the data");
    }

    ////remove annotation to view movie import result

//    if(movie_count != 0) {
//        for (int i = 0; i < movie_count; i++) {
//            printf("%d::%s (%d)::", (movies + i)->movieID, (movies + i)->title, (movies + i)->releaseYear);
//            for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
//                printf("%s|", genreList[*((movies + i)->genre + j)]);
//            }
//            printf("\b\n");
//        }
//    }

    ////remove annotation to view tag import result
//    if(tag_count != 0) {
//        for (int i = 0; i < tag_count; i++) {
//            Tag temp = *(tags + i);
//            printf("%d::%d::%s::%lld\n", temp.userID, temp.movieID, temp.tag, temp.timestamp);
//        }
//    }

    free(movies);
    free(tags);
    return 0;
}
void initMovie(){
    genreList[0] = "Action";
    genreListCursor=1;

    //init movie

    movies = (Movie *) malloc(sizeof(Movie));

    FILE *fp;
    if (testing_file){
        fp = fopen(testMovieFile, "r");
    }
    else{
        fp = fopen(movieFile, "r");
    }
    char line[500];
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
    char line[500];
    int index = 0;

    //init tag
    tags = (Tag *) malloc(sizeof(Tag));

    if (testing_file){
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

}
void init() {
    initMovie();
    initTag();
}

void saveMovie(){
    FILE *fp1;
    if(testing_file){
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
    if(testing_file){
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
void save(){
    saveMovie();
    saveTag();
}

int genreIndex_ByString(char *genre){
    int flag=1;
    int i;
    for (i = 0; i < genreListCursor; i++) {
        if (!strcmp(genre, genreList[i])) {
            flag=0;
            break;
        }
    }
    if(flag){
        genreList[genreListCursor] = (char *) malloc(sizeof(char)*strlen(genre));
        strcpy(genreList[genreListCursor], genre);
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

int getnum() {
    int num = 0;
    printf("Please select an item: ");
    scanf("%d", &num);
    return num;
}

int selectMenu() {
    printf("<Movie data management program>\n");
    printf("1: Add movie \n2: Remove movie \n3: Add tag \n4: Remove tag \n5: Add favourite \n6: Search by user ID \n7: Search by movie title \n0: Close\n");
    return getnum();
}

void close(){
    save();
    if(integrity()){

    }
    exit(0);
}

int addMovieEntity(int movieID, char *title, int releaseYear, int *genre, short genre_count){
    movies = (Movie *) realloc(movies, (movie_count + 1) * sizeof(Movie));

    //0
    (movies + movie_count)->movieID = movieID;

    //1
    (movies + movie_count)->releaseYear = releaseYear;
    (movies + movie_count)->title = malloc(strlen(title) * sizeof(char));
    strcpy((movies + movie_count)->title, title);

    //2
    (movies + movie_count)->genre = (int *) malloc(sizeof(int)*genre_count);
    for(int i=0;i<genre_count;i++){
        *(((movies + movie_count)->genre)+i) = (genre+i);
    }
    (movies + movie_count)->sizeof_genre = genre_count;
    (movies + movie_count) -> enabled = 1;

    movie_count++;
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
}
int addUserEntity(int userID, char *userName, char *password);

void deleteMovie_ByIndex(int index){
    if(index>=movie_count){
        return;
    }
    (movies + index) -> enabled = 0;
    saveMovie();
    initMovie();
}
void deleteTag_ByIndex(int index){
    if(index>=tag_count){
        return;
    }
    (tags + index) -> enabled = 0;
    saveTag();
    initTag();
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
    *(returnArray+count) = -1;
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
    *(returnArray+count) = -1;
    return returnArray;
}
int movieIndex_ByID(int movieID){
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
    *(returnArray+count) = -1;
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
    *(returnArray+count) = -1;
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
    *(returnArray+count) = -1;
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
    *(returnArray+count) = -1;
    return returnArray;
}
int* favouriteIndex_ByUserID(int userID);
int* favouriteIndex_ByMovieID(int movieID);
int favouriteIndex_ByDoubleID(int userID, int movieID);
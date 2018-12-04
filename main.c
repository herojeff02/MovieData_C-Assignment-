#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "append.h"
#include "delete.h"
#include "indexFinder.h"
#include "dataType.h"
#include "init.h"
#include "save.h"
#include "integrity.h"
#include "customString.h"
#include "existence.h"


//modes
void menu();
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

void printAllTag(int movie_id);
void printMovieInfo(int movie_id);
void printTag(int index);

//methods
void startTimer();
void endTimer();
void testPrint();

clock_t t;
void startTimer(){
    t = clock();
}
void endTimer(){
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("execution time : %lf seconds", time_taken);
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

void retire(){
    free(movies);
    free(tags);
    free(users);
    free(favourites);
}


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

    int resultData=addTagEntity(tagEntity->user_id, tagEntity->movie_id, tagEntity->tag);

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
    tagIndexRemove= * tagIndex_ByUserID(tagRemove);
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

void removeFavourite() {
    int favouriteRemove;
    while (1) {
        printf("User ID to remove favourite: ");
        scanf("%d", &favouriteRemove);
        if (favouriteIDExists(favouriteRemove) == 1) {
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

void close(){
    save();
    integrity();
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

int main(){
//    startTimer();
//
    init();
//    addFavourite();
////addUserEntity(30000, "user_name", "1524124");
////addFavouriteEntity();
//    if(integrity()){
//        printf("Error occured while reading file. Please check file content.\n");
//    }
//    save();
//    if(integrity()){
//        printf("Error occured while saving file. Please check file content.\n");
//    }
//    retire();
//    endTimer();

    printf("%s", string_ByGenreIndex(3));

    return 0;
}
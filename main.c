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
/*
1. 영화 정보, 태그 정보 등등 출력하는 부분 함수로 분할
(인덱스 넣으면 그 인덱스에 해당하는 영화/태그/사용자/즐겨찾기에 해당하는 정보가 출력되는 함수. 
우리 여태까지 예제에서 하던 것과 같은 형식이야. 이해 안 되면 예제 참고하거나 나한테 물어보거나! 맘대로 하지 말고.)
2. removeMovie 등등등등 함수에서 printf("Movie ID to remove: “); 존나 불친절. 제목 입력하면 검색해서 지우도록 수정.
3. removeTag에서 tag ID를 받을 게 아니라 movie_id와 user_id를 받아야 태그 하나를 특정해서 지울 수 있겠지? 둘 다 입력 받고 int* tagIndex_ByDoubleID(int user_id, int movie_id); 써서 지워. 
removeFavourite도 마찬가지로 favouriteIndex_ByDoubleID 있어…
4. addMovie 등 함수에서 숫자 입력 받으면서 알파벳인지, 지정한 자릿수 안 넘는지 검사하는 부분을 내가 짜놓은 forcedIntegerInput, stringIncludesMarker 등 함수로 대체
5. 우리 주요 기능 중 하나였던 장르별 영화 추천 만들어줘. int* movieIndex_ByGenre(int *genre, short genre_count); 여기다가 장르 목록 넣으면 됨.
6. 위에 써놓은 거 말고도 우리 계획 세우면서 만든 ppt에 있던 내용 전부 만들어줘. 검색/삭제/추가 등에 필요한 함수 없으면 직접 만들지 말고 나한테 말해.
UI쪽은 알아서 하고. 2번에 써놓은 것처럼 사용하기 편하게!!
*/

void addMovie() {
    Movie *movieEntity;
    movieEntity = (Movie *) malloc(sizeof(Movie));
    char movieIDEntity[6];
	movieEntity->title = (char *)malloc(sizeof(char) * 200);
    char releaseYearEntity[5];
	char sizeof_genreEntity[3];

	getchar();

	//movie_id
    while (1) {
        printf("Movie ID to add: ");
		scanf("%s", movieIDEntity);
		int result = forcedIntegerInput(movieIDEntity,5,1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else {
			movieEntity->movie_id = result;
			break;
		}
    }
	
	//movie title
    printf("Title: ");
	rewind(stdin);
	scanf("%[^\n]", movieEntity->title);
	getchar();

	//release year
    while (1) {
        printf("Release year: ");
		scanf("%s", releaseYearEntity);
		int result = forcedIntegerInput(releaseYearEntity,4,0);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small. Please input over 1000\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big. Please input under 10000\n");
		}
		else {
			movieEntity->release_year = result;
			break;
		}
    }

	//genre size
    while (1) {
        printf("How many genre do you want to add: ");
		scanf("%s", sizeof_genreEntity);
		int result = forcedIntegerInput(sizeof_genreEntity,2,1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else {
			movieEntity->sizeof_genre = result;
			break;
		}
    }

	//genre
	movieEntity->genre = (int *)malloc(sizeof(int)*movieEntity->sizeof_genre);
	int index=movieIndex_ByID(movieEntity->movie_id);
    for(int i=0;i<movieEntity->sizeof_genre;i++) {
        char inputGenre[30];
        printf("Genre %d: ",i+1);
		//rewind(stdin);
        scanf(" %s",inputGenre);
		*((movieEntity)->genre + i) = genreIndex_ByString(inputGenre);
		printf("%s", *((movieEntity)->genre + i));
    }

	//final
    int resultData = addMovieEntity(movieEntity->movie_id, movieEntity->title, movieEntity->release_year, movieEntity->genre, movieEntity->sizeof_genre);
    if (resultData==SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!\n");
        return menu();
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
		rewind(stdin);
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
		rewind(stdin);
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
		rewind(stdin);
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
	rewind(stdin);
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
    int userID;
	int movieID;
    while(1) {
        printf("User ID: ");
		rewind(stdin);
        scanf("%d",&userID);
        if (userIDExists(userID)==1) {
            break;
        }
        else {
            printf("User ID doesn't exist\n");
        }
    }
	while (1) {
		printf("Movie ID: ");
		rewind(stdin);
		scanf("%d", &movieID);
		if (userIDExists(movieID) == 1) {
			break;
		}
		else {
			printf("Movie ID doesn't exist\n");
		}
	}
    int tagIndexRemove;
    tagIndexRemove= * tagIndex_ByDoubleID(userID, movieID);
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
		rewind(stdin);
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
		rewind(stdin);
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
    int userID;
	int movieID;
    while (1) {
        printf("User ID to remove favourite: ");
		rewind(stdin);
        scanf("%d", &userID);
        if (favouriteUserIDExists(userID) == 1) {
            break;
        }
        else {
            printf("User ID doesn't exist\n");
        }
    }
	while (1) {
		printf("Movie ID to remove favourite: ");
		rewind(stdin);
		scanf("%d", &movieID);
		if (favouriteMovieIDExists(movieID) == 1) {
			break;
		}
		else {
			printf("Movie ID doesn't exist\n");
		}
	}
    int favouriteIndexRemove;
    favouriteIndexRemove = favouriteIndex_ByDoubleID(userID, movieID);
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
		rewind(stdin);
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
	rewind(stdin);
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

int selectMenu() {
	int num;
	printf("<Movie data management program>\n");
	printf("1: Add movie \n2: Remove movie \n"
		"3: Add tag \n4: Remove tag \n"
		"5: Add favourite \n6: Remove favourite \n"
		"7: Add user \n8: Remove user \n"
		"9: Search by user ID \n10: Search by movie title \n"
		"0: Close\n");
	printf("Please select a number: ");
	scanf("%d", &num);
	return num;
}

void menu() {
	int num = selectMenu();
    while (num != 0)
    {
        switch (num)
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
    startTimer();

    //init();
    menu();
////addUserEntity(30000, "user_name", "1524124");
////addFavouriteEntity();
    if(integrity()){
        printf("Error occured while reading file. Please check file content.\n");
    }

   save();
   if(integrity()){
      printf("Error occured while saving file. Please check file content.\n");
    }
   retire();
   endTimer();

   printf("%s", string_ByGenreIndex(3));

   return 0;
}
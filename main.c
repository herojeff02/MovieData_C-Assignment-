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
void recommendMovie();
void retire();

void printAllTag(int movie_id);
void printMovieInfo(int movie_id);
void printTag(int index);

//methods
void startTimer();
void endTimer();
void testPrint();

clock_t t;
void startTimer() {
	t = clock();
}
void endTimer() {
	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds
	printf("execution time : %lf seconds", time_taken);
}

void testPrint() {
	if (movie_count != 0) {
		for (int i = 0; i < movie_count; i++) {
			printf("%d::%s (%d)::", (movies + i)->movie_id, (movies + i)->title, (movies + i)->release_year);
			for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
				printf("%s|", genre_list[*((movies + i)->genre + j)]);
			}
			printf("\b\n");
		}
	}

	if (tag_count != 0) {
		for (int i = 0; i < tag_count; i++) {
			Tag temp = *(tags + i);
			printf("%d::%d::%s::%lld\n", temp.user_id, temp.movie_id, temp.tag, temp.timestamp);
		}
	}
}

void retire() {
	free(movies);
	free(tags);
	free(users);
	free(favourites);
}
/*
우리 여태까지 예제에서 하던 것과 같은 형식이야. 이해 안 되면 예제 참고하거나 나한테 물어보거나! 맘대로 하지 말고.)
5. 우리 주요 기능 중 하나였던 장르별 영화 추천 만들어줘. int* movieIndex_ByGenre(int *genre, short genre_count); 여기다가 장르 목록 넣으면 됨.
6. 위에 써놓은 거 말고도 우리 계획 세우면서 만든 ppt에 있던 내용 전부 만들어줘. 검색/삭제/추가 등에 필요한 함수 없으면 직접 만들지 말고 나한테 말해.
UI쪽은 알아서 하고. 2번에 써놓은 것처럼 사용하기 편하게!!
*/

void addMovie() {
	Movie *movieEntity;
	movieEntity = (Movie *)malloc(sizeof(Movie));
	char movieIDEntity[6];
	movieEntity->title = (char *)malloc(sizeof(char) * 200);
	char releaseYearEntity[5];
	char sizeof_genreEntity[3];

	//movie_id
	while (1) {
		printf("Movie ID to add: ");
		scanf("%s", movieIDEntity);
		int result = forcedIntegerInput(movieIDEntity, 5, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
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

	//release year
	while (1) {
		printf("Release year: ");
		scanf("%s", releaseYearEntity);
		int result = forcedIntegerInput(releaseYearEntity, 4, 0);
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
		int result = forcedIntegerInput(sizeof_genreEntity, 2, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			movieEntity->sizeof_genre = result;
			break;
		}
	}

	//genre
	movieEntity->genre = (int *)malloc(sizeof(int)*movieEntity->sizeof_genre);
	int index = movieIndex_ByID(movieEntity->movie_id);
	for (int i = 0; i < movieEntity->sizeof_genre; i++) {
		char inputGenre[30];
		printf("Genre %d: ", i + 1);
		rewind(stdin);
		scanf(" %s", inputGenre);
		*((movieEntity)->genre + i) = genreIndex_ByString(inputGenre);
	}

	//final
	int resultData = addMovieEntity(movieEntity->movie_id, movieEntity->title, movieEntity->release_year, movieEntity->genre, movieEntity->sizeof_genre);
	if (resultData == SUCCESS)
	{
		printf("ADD SUCCESSFULLY!!\n");
		return;
	}
	else if (resultData == FAIL_MOVIE_ID_ALREADY_EXISTS)
	{
		printf("Already exist movie\n");
		return;
	}
	else if (resultData == FAIL_INVALID_YEAR)
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
	//have to update "more than 2 movies"
	char *movieTitleRemove;
	movieTitleRemove = (char *)malloc(sizeof(char) * 200);

	while (1) {
		printf("Movie title to remove: ");
		rewind(stdin);
		scanf("%[^\n]", movieTitleRemove);

		int movieIndexRemove;
		movieIndexRemove = *movieIndex_ByMatchingTitle(movieTitleRemove);
		if (deleteMovie_ByIndex(movieIndexRemove) == SUCCESS) {
			printf("REMOVE SUCCESSFULLY!!\n");
			return;
		}
		else {
			printf("Have some problem. Remove again\n");
			return;
		}
	}
}
void addTag() {
	Tag *tagEntity;
	tagEntity = (Tag *)malloc(sizeof(Tag));
	char userIDEntity[7];
	char movieIDEntity[6];
	tagEntity->tag = (char *)malloc(sizeof(char) * 100);

	//user_id
	while (1) {
		printf("User ID to add: ");
		scanf(" %s", userIDEntity);
		int result = forcedIntegerInput(userIDEntity, 6, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			tagEntity->user_id = result;
			break;
		}
	}

	//movie_id
	while (1) {
		printf("Movie ID to add: ");
		scanf(" %s", movieIDEntity);
		int result = forcedIntegerInput(movieIDEntity, 5, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			tagEntity->movie_id = result;
			break;
		}
	}

	//tag
	printf("tag: ");
	rewind(stdin);
	scanf("%[^\n]", tagEntity->tag);

	//final
	int resultData = addTagEntity(tagEntity->user_id, tagEntity->movie_id, tagEntity->tag);
	if (resultData == SUCCESS)
	{
		printf("ADD SUCCESSFULLY!!\n");
		return;
	}
	else
	{
		printf("Have some problem. Add again\n");
		return;
	}
}
void removeTag() {
	int userID;
	int movieID;
	char userIDEntity[7];
	char movieIDEntity[6];

	//user_id
	while (1) {
		printf("User ID: ");
		rewind(stdin);
		scanf("%s", userIDEntity);
		int result = forcedIntegerInput(userIDEntity, 6, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			if (userIDExists(result) == 1) {
				userID = result;
				break;
			}
			else {
				printf("User ID doesn't exist\n");
			}
		}
	}

	//movie_id
	while (1) {
		printf("Movie ID: ");
		rewind(stdin);
		scanf("%s", movieIDEntity);
		int result = forcedIntegerInput(movieIDEntity, 5, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			if (movieIDExists(result) == 1) {
				movieID = result;
				break;
			}
			else {
				printf("Movie ID doesn't exist\n");
			}
		}
	}

	//final
	int tagIndexRemove;
	tagIndexRemove = *tagIndex_ByDoubleID(userID, movieID);
	if (deleteTag_ByIndex(tagIndexRemove) == SUCCESS) {
		printf("REMOVE SUCCESSFULLY!!\n");
		return;
	}
	else {
		printf("Have some problem. Remove again.\n");
		return;
	}
}
void addFavourite() {
	Favourite *favouriteEntity;
	favouriteEntity = (Favourite *)malloc(sizeof(Favourite));
	char userIDEntity[7];
	char movieIDEntity[6];

	//user_id
	while (1) {
		printf("User ID to add: ");
		scanf(" %s", userIDEntity);
		int result = forcedIntegerInput(userIDEntity, 6, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			favouriteEntity->user_id = result;
			break;
		}
	}

	//movie_id
	while (1) {
		printf("Movie ID to add: ");
		scanf("%s", movieIDEntity);
		int result = forcedIntegerInput(movieIDEntity, 5, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			favouriteEntity->movie_id = result;
			break;
		}
	}
	
	//final
	int resultData = addFavouriteEntity(favouriteEntity->user_id, favouriteEntity->movie_id);

	if (resultData == SUCCESS)
	{
		printf("ADD SUCCESSFULLY!!\n");
		return;
	}
	else
	{
		printf("Have some problem. Add again\n");
		return;
	}
}
void removeFavourite() {
	int userID;
	int movieID;
	char userIDEntity[7];
	char movieIDEntity[6];

	//user_id
	while (1) {
		printf("User ID: ");
		rewind(stdin);
		scanf("%s", userIDEntity);
		int result = forcedIntegerInput(userIDEntity, 6, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			if (favouriteUserIDExists(result) == 1) {
				userID = result;
				break;
			}
			else {
				printf("User ID doesn't exist\n");
			}
		}
	}

	//movie_id
	while (1) {
		printf("Movie ID: ");
		rewind(stdin);
		scanf("%s", movieIDEntity);
		int result = forcedIntegerInput(movieIDEntity, 5, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			if (favouriteMovieIDExists(result) == 1) {
				movieID = result;
				break;
			}
			else {
				printf("Movie ID doesn't exist\n");
			}
		}
	}

	//final
	int favouriteIndexRemove;
	favouriteIndexRemove = *favouriteIndex_ByDoubleID(userID, movieID);
	if (deleteFavourite_ByIndex(favouriteIndexRemove) == SUCCESS) {
		printf("REMOVE SUCCESSFULLY!!\n");
		return;
	}
	else {
		printf("Have some problem. Remove again\n");
		return;
	}
}
void addUser() {
	User *userEntity;
	userEntity = (User *)malloc(sizeof(User));
	char userIDEntity[7];
	userEntity->user_name = (char *)malloc(sizeof(char) * 100);
	userEntity->password = (char *)malloc(sizeof(char) * 100);

	//user_id
	while (1) {
		printf("User ID to add: ");
		scanf(" %s", userIDEntity);
		int result = forcedIntegerInput(userIDEntity, 6, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			userEntity->user_id = result;
			break;
		}
	}

	//user_name
	printf("User name: ");
	rewind(stdin);
	scanf("%[^\n]", userEntity->user_name);

	//password
	printf("password: ");
	rewind(stdin);
	scanf("%[^\n]", userEntity->password);

	//final
	int resultData = addUserEntity(userEntity->user_id, userEntity->user_name, userEntity->password);

	if (resultData == SUCCESS)
	{
		printf("ADD SUCCESSFULLY!!\n");
		return;
	}
	else
	{
		printf("Have some problem. Add again\n");
		return;
	}
}
void removeUser() {
	int userID;
	char userIDEntity[7];
	while (1) {
		printf("User ID: ");
		rewind(stdin);
		scanf("%s", userIDEntity);
		int result = forcedIntegerInput(userIDEntity, 6, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			if (userIDExists(result) == 1) {
				userID = result;
				break;
			}
			else {
				printf("User ID doesn't exist\n");
			}
		}
	}

	int userIndexRemove;
	userIndexRemove = userIndex_ByUserID(userID);
	if (deleteUser_ByIndex(userIndexRemove) == SUCCESS) {
		printf("REMOVE SUCCESSFULLY!!\n");
		return;
	}
	else {
		printf("Have some problem. Remove again.\n");
		return;
	}
}
void searchByUserID() {
	User *userEntity;
	Tag *tagEntity;
	Favourite *favouriteEntity;
	userEntity = (User *)malloc(sizeof(User));
	tagEntity = (Tag *)malloc(sizeof(Tag));
	favouriteEntity = (Favourite *)malloc(sizeof(Favourite));
	int userID;
	char userIDEntity[7];
	while (1) {
		printf("User ID to search: ");
		scanf(" %s", userIDEntity);
		int result = forcedIntegerInput(userIDEntity, 6, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			userID = result;
			break;
		}
	}

	int index_user = userIndex_ByUserID(userID);
	if (index_user == FAIL_NO_SUCH_USER_ID) {
		printf("No such user ID\n");
		return;
	}
	else {
		int resultData = userIDExists(userEntity->user_id);

		if (resultData == 1) {
			printf("user name: %s\n", userEntity->user_name);
		}
		else if (resultData == 0) {
			//Maybe it doesn't come to here
			printf("There is no such user ID\n");
			return;
		}
		else {
			//Maybe it doesn't come to here
			printf("Have some problem\n");
			return;
		}
	}
	/*
	if (userIDExists_InTag(userID) == 1) {
		int *index_tag;
		index_tag = (int*)malloc(sizeof(int));
		index_tag = tagIndex_ByUserID(userID);
		printf("Movie ID: %d, Tag: %s\n",);
	}
	else {
		printf("There's no tag");
	}*/
	return;
}
void searchByMovieTitle() {
	char *searchMovieTitle;
	searchMovieTitle = (char *)malloc(sizeof(char) * 200);

	while (1) {
		printf("Title to search: ");
		rewind(stdin);
		scanf("%[^\n]", searchMovieTitle);
		//if movie doesn't exist, it becomes to error...
		if (movieTitleExists(searchMovieTitle) == 1) {
			break;
		}
		else {
			printf("Movie doesn't exist\n");
		}
	}

	int index = *movieIndex_ByMatchingTitle(searchMovieTitle);

	printf("Movie ID: %d\n", ((movies + index)->movie_id));
	printf("Release year: %d\n", ((movies + index)->release_year));
	printf("Genre: ");
	int *genreIndex;
	genreIndex = (int*)malloc(sizeof(int));
	genreIndex = genreIndex_ByMovieID(((movies + index)->movie_id));
	if (((movies + index)->sizeof_genre) == 0) {
		printf("No genre.");
	}
	else {
		for (int i = 0; i < ((movies + index)->sizeof_genre); i++) {
			char *genre;
			genre = (char*)malloc(sizeof(char) * 20);
			genre = string_ByGenreIndex(genreIndex[i]);
			if (i < ((movies + index)->sizeof_genre) - 1) {
				printf("%s|", genre);
			}
			else {
				printf("%s\n", genre);
			}
		}
	}
	return;
}
void recommendMovie() {
	char genreSize[3];
	short genre_size;
	int *genreIndex;
	int *movieIndex;
	genreIndex = (int*)malloc(sizeof(int));
	movieIndex = (int*)malloc(sizeof(int));
	while (1) {
		printf("How many genres will you recommend: ");
		scanf("%s", genreSize);
		int result = forcedIntegerInput(genreSize, 2, 1);
		if (result == FAIL_NOT_A_NUMBER) {
			printf("It's not a number\n");
		}
		else if (result == FAIL_TOO_MANY_FIGURES) {
			printf("It's too big\n");
		}
		else if (result == FAIL_LACK_OF_FIGURES) {
			printf("It's too small\n");
		}
		else {
			genre_size = result;
			break;
		}
	}
	//here is an error
	for (int i = 0; i < genre_size; i++) {
		char genre[20];
		printf("Genre %d: ", i+1);
		rewind(stdin);
		scanf("%s", genre);
		genreIndex = realloc(genreIndex, genre_size * sizeof(int));
		*(genreIndex+i) = genreIndex_ByString(genre);
	}
	movieIndex=movieIndex_ByGenre(genreIndex, genre_size);
	char *movietitle;
	movietitle = (char *)malloc(sizeof(char) * 200);
	movietitle = movies->title;
	printf("%s", movietitle);
}

void close() {
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
		"11: Recommend movie \n0: Close\n");
	printf("Please select a number: ");
	scanf("%d", &num);
	return num;
}

void menu() {
	int num = 1;
	while (num != 0)
	{
		num = selectMenu();
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
		case 11: recommendMovie(); break;
		case 0: close(); break;
		default: printf("No number.\n"); break;
		}
	}
	return;
}

int main() {
	startTimer();

	init();
	menu();
	////addUserEntity(30000, "user_name", "1524124");
	////addFavouriteEntity();
	if (integrity()) {
		printf("Error occured while reading file. Please check file content.\n");
	}

	save();
	if (integrity()) {
		printf("Error occured while saving file. Please check file content.\n");
	}
	retire();
	endTimer();

	printf("%s", string_ByGenreIndex(3));

	return 0;
}
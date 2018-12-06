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
1. ��ȭ ����, �±� ���� ��� ����ϴ� �κ� �Լ��� ����
(�ε��� ������ �� �ε����� �ش��ϴ� ��ȭ/�±�/�����/���ã�⿡ �ش��ϴ� ������ ��µǴ� �Լ�.
�츮 ���±��� �������� �ϴ� �Ͱ� ���� �����̾�. ���� �� �Ǹ� ���� �����ϰų� ������ ����ų�! ����� ���� ����.)
2. removeMovie ����� �Լ����� printf("Movie ID to remove: ��); ���� ��ģ��. ���� �Է��ϸ� �˻��ؼ� ���쵵�� ����.
3. removeTag���� tag ID�� ���� �� �ƴ϶� movie_id�� user_id�� �޾ƾ� �±� �ϳ��� Ư���ؼ� ���� �� �ְ���? �� �� �Է� �ް� int* tagIndex_ByDoubleID(int user_id, int movie_id); �Ἥ ����.
removeFavourite�� ���������� favouriteIndex_ByDoubleID �־
4. addMovie �� �Լ����� ���� �Է� �����鼭 ���ĺ�����, ������ �ڸ��� �� �Ѵ��� �˻��ϴ� �κ��� ���� ¥���� forcedIntegerInput, stringIncludesMarker �� �Լ��� ��ü
5. �츮 �ֿ� ��� �� �ϳ����� �帣�� ��ȭ ��õ �������. int* movieIndex_ByGenre(int *genre, short genre_count); ����ٰ� �帣 ��� ������ ��.
6. ���� ����� �� ���� �츮 ��ȹ ����鼭 ���� ppt�� �ִ� ���� ���� �������. �˻�/����/�߰� � �ʿ��� �Լ� ������ ���� ������ ���� ������ ����.
UI���� �˾Ƽ� �ϰ�. 2���� ����� ��ó�� ����ϱ� ���ϰ�!!
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
		scanf(" %[^\n]", movieTitleRemove);

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
	favouriteIndexRemove = favouriteIndex_ByDoubleID(userID, movieID);
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
		for (int i = 0; i < movieEntity->sizeof_genre; i++) {
			int* genreIndex = genreIndex_ByMovieID(movieEntity->movie_id);

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
		"0: Close\n");
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
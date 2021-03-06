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

int logged_in_user_index = -1;

//modes
void menu();

int selectMenu();

void close();

void addMovie();

void removeMovie();

void addTag();

void addUser();

void removeTag();

void addFavourite();

void removeFavourite();

void searchByUserName();

void searchByMovieTitle();

void searchTag();

void recommendMovie();

void retire();

void login();

void logout();

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
    double time_taken = ((double) t) / CLOCKS_PER_SEC; // in seconds
    printf("execution time : %lf seconds", time_taken);
}

void login() {
    char mode[3];
    short _mode;
    printf("<Movie data management program>\n");
    printf("1: register\n2: login\n3: exit\n");
    while (1) {
        scanf(" %s", mode);
        _mode = forcedIntegerInput(mode, 1, 0);
        short escape_flag = 0;
        if (_mode == 1 || _mode == 2 || _mode == 3) {
            break;
        }
        printf("nonexistent mode. try again: \n");
    }

    if (_mode == 2) {
        char name[100], password[100];
        int temp_login = -1;
        printf("enter your username: \n");
        while (1) {
            scanf(" %s", name);
            if (userIndex_ByName(name) != FAIL_NO_SUCH_USER_NAME) {
                temp_login = userIndex_ByName(name);
                break;
            }
            printf("user not found. try again: \n");
        }
        int i = 3;
        printf("enter your password: \n");
        while (1) {
            i--;
            scanf(" %s", password);
            if (strcmp((users + temp_login)->password, password) == 0) {
                logged_in_user_index = temp_login;
                break;
            }
            if (i == 0) {
                printf("login failed. who are you?\n");
                exit(-10);
            }
            printf("wrong password. try again. \n%d attempt(s) remaining: \n", i);
        }
        printf("WELCOME, %s\n", name);
    } else if (_mode == 1) {
        addUser();
        printf("WELCOME, %s\n", (users + logged_in_user_index)->user_name);
    } else {
        exit(0);
    }
}

void logout() {
    login();
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

void addMovie() {
    Movie *movieEntity;
    movieEntity = (Movie *) malloc(sizeof(Movie));
    char movieIDEntity[6];
    movieEntity->title = (char *) malloc(sizeof(char) * 200);
    char releaseYearEntity[5];
    char sizeof_genreEntity[3];

    //movie_id
    while (1) {
        printf("Movie ID to add: ");
        scanf("%s", movieIDEntity);
        int result = forcedIntegerInput(movieIDEntity, 5, 1);
        if (result == FAIL_NOT_A_NUMBER) {
            printf("It's not a number\n");
        } else if (result == FAIL_TOO_MANY_FIGURES) {
            printf("It's too big\n");
        } else if (result == FAIL_LACK_OF_FIGURES) {
            printf("It's too small\n");
        } else {
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
        } else if (result == FAIL_LACK_OF_FIGURES) {
            printf("It's too small. Please input over 1000\n");
        } else if (result == FAIL_TOO_MANY_FIGURES) {
            printf("It's too big. Please input under 10000\n");
        } else {
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
        } else if (result == FAIL_TOO_MANY_FIGURES) {
            printf("It's too big\n");
        } else if (result == FAIL_LACK_OF_FIGURES) {
            printf("It's too small\n");
        } else {
            movieEntity->sizeof_genre = result;
            break;
        }
    }

    //genre
    movieEntity->genre = (int *) malloc(sizeof(int) * movieEntity->sizeof_genre);
    for (int i = 0; i < movieEntity->sizeof_genre; i++) {
        char inputGenre[30];
        printf("Genre %d: ", i + 1);
        rewind(stdin);
        scanf(" %s", inputGenre);
        *((movieEntity)->genre + i) = genreIndex_ByString(inputGenre);
    }

    //final
    int resultData = addMovieEntity(movieEntity->movie_id, movieEntity->title, movieEntity->release_year,
                                    movieEntity->genre, movieEntity->sizeof_genre);
	free(movieEntity);
	free(movieEntity->title);
	free(movieEntity->genre);
    if (resultData == SUCCESS) {
        printf("ADD SUCCESSFULLY!!\n");
        return;
    } else if (resultData == FAIL_MOVIE_ID_ALREADY_EXISTS) {
        printf("Already exist movie\n");
        return;
    } else if (resultData == FAIL_INVALID_YEAR) {
        printf("Invalid year\n");
        return;
    } else {
        printf("Have some problem. Add again\n");
        return;
    }
}

void removeMovie() {
    //have to update "more than 2 movies"
    char *movieTitleRemove;
    movieTitleRemove = (char *) malloc(sizeof(char) * 200);

    while (1) {
        printf("Movie title to remove: ");
        rewind(stdin);
        scanf("%[^\n]", movieTitleRemove);

        int *movieIndexRemove;
        movieIndexRemove = movieIndex_ByMatchingTitle_WithoutYear(movieTitleRemove);
		free(movieTitleRemove);
		int cnt = 0;
		while(1) {
			if (*(movieIndexRemove + cnt) ==END_OF_INT_ARRAY) {
				break;
			}
			cnt++;
		}

		if (cnt > 1) {
			printf("More than 1 movies exist. Please select it.\n");
			for (int i = 0;i < cnt;i++) {
				printf("%d. release year: %d\n", i + 1, (movies + *(movieIndexRemove + i))->release_year);
			}
			int num;
			printf("Which one would you like to remove? Select the index: \n");
			scanf("%d", &num);
			for (int i = 0;i < cnt;i++) {
				if (num - 1 == i) {
					if (deleteMovie_ByIndex(*(movieIndexRemove + i)) == SUCCESS) {
						printf("REMOVE SUCCESSFULLY!!\n");
						return;
					}
					else {
						printf("Have some problem. Remove again\n");
						return;
					}
				}
			}
		}
		else if (cnt == 1) {
			if (deleteMovie_ByIndex(*(movieIndexRemove)) == SUCCESS) {
				printf("REMOVE SUCCESSFULLY!!\n");
				return;
			}
			else {
				printf("Have some problem. Remove again\n");
				return;
			}
		}
		else {
			printf("Movie doesn't exist.");
			return;
		}
    }
}

void addTag() {
    Tag *tagEntity;
    tagEntity = (Tag *) malloc(sizeof(Tag));
    tagEntity->tag = (char *) malloc(sizeof(char) * 100);

    //user_id
    tagEntity->user_id = (users + logged_in_user_index)->user_id;

    //movie_title
    char title[200];
    char num1[20];
    int *movieIndex;
	int num;
    printf("Movie title: \n");
    while (1) {
        rewind(stdin);
        scanf("%[^\n]", title);
        if (matchingMovieTitleExists(title)) {
            movieIndex = movieIndex_ByMatchingTitle_WithoutYear(title);
            int cnt = 0;
            while (1) {
                if (*(movieIndex + cnt) == END_OF_INT_ARRAY) {
                    break;
                }
                cnt++;
            }

            if (cnt > 1) {
                printf("More than 1 movies exist. Please select one.\n");
                for (int i = 0; i < cnt; i++) {
                    printf("%d. release year: %d\n", i + 1, (movies + *(movieIndex + i))->release_year);
                }
                while (1) {
                    printf("Which one would you like to add the tag to? Select the index: \n");
                    scanf(" %s", num1);
                    int result = forcedIntegerInput(num1, 1, 1);
                    if (result == FAIL_NOT_A_NUMBER) {
                        printf("It's not a number\n");
                    } else if (result == FAIL_TOO_MANY_FIGURES) {
                        printf("It's too big\n");
                    } else if (result == FAIL_LACK_OF_FIGURES) {
                        printf("It's too small\n");
                    } else {
                        num = result - 1;
                        break;
                    }
                }
            } else if (cnt == 1) {
                num = 0;
                break;
            }
            break;
        } else {
            printf("Movie doesn't exist in our DB. Maybe something else?:\n");
        }
    }
    tagEntity->movie_id = (movies + *(movieIndex+num))->movie_id;

    //tag
    printf("tag: ");
    rewind(stdin);
    scanf("%[^\n]", tagEntity->tag);

    //final
    int resultData = addTagEntity(tagEntity->user_id, tagEntity->movie_id, tagEntity->tag);
//	free(tagEntity);
//	free(tagEntity->tag);
    if (resultData == SUCCESS) {
        printf("ADD SUCCESSFULLY!!\n");
        return;
    } else {
        printf("Have some problem. Add again\n");
        return;
    }
}

void removeTag() {
    int userID;
    int *movieIndex;
    char title[200];
	int num;
	char num1[2];

    //user_id
    userID = (users + logged_in_user_index)->user_id;

    //movie_id
    printf("Movie title: \n");
    while (1) {
        rewind(stdin);
        scanf("%[^\n]", title);
        if (matchingMovieTitleExists(title)) {
            movieIndex = movieIndex_ByMatchingTitle_WithoutYear(title);
			int cnt = 0;
			while (1) {
				if (*(movieIndex + cnt) == END_OF_INT_ARRAY) {
					break;
				}
				cnt++;
			}

			if (cnt > 1) {
				printf("More than 1 movies exist. Please select it.\n");
				for (int i = 0;i < cnt;i++) {
					printf("%d. release year: %d\n", i + 1, (movies + *(movieIndex + i))->release_year);
				}
				while (1) {
					printf("Which one would you like to remove? Select the index: \n");
					scanf(" %s", num1);
					int result = forcedIntegerInput(num1, 1, 1);
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
						num = result - 1;
						break;
					}
				}
				break;
			}
			else if (cnt == 1) {
				num = 0;
				break;
			}
        } 
		else {
            printf("Movie doesn't exist in our DB. Maybe something else?:\n");
        }
    }

    //final
    int *tagIndexRemove = tagIndex_ByDoubleID(userID, (movies + *(movieIndex + num))->movie_id);
    int tag_temp_count = 0;
    while (1) {
        if (*(tagIndexRemove + tag_temp_count) == END_OF_INT_ARRAY) {
            break;
        }
        tag_temp_count++;
    }
	if (tag_temp_count == 0) {
		printf("You didn't leave any tags for %s\n", (movies + *(movieIndex + num))->title);
	}
	else {
		if (deleteTag_ByIndex(tagIndexRemove) == SUCCESS) {
			printf("REMOVE SUCCESSFULLY!!\n");
			return;
		}
		else {
			printf("Have some problem. Remove again.\n");
			return;
		}
	}
}

void addFavourite() {
    Favourite favouriteEntity;

    //user_id
    favouriteEntity.user_id = (users + logged_in_user_index)->user_id;

    //movie_title
    int *movieIndex;
    char title[200];
	int num;
	char num1[20];
    printf("Movie title: \n");
    while (1) {
        rewind(stdin);
        scanf("%[^\n]", title);
        if (matchingMovieTitleExists(title)) {
            movieIndex = movieIndex_ByMatchingTitle_WithoutYear(title);
			int cnt = 0;
			while (1) {
				if (*(movieIndex + cnt) == END_OF_INT_ARRAY) {
					break;
				}
				cnt++;
			}

			if (cnt > 1) {
                printf("More than 1 movies exist. Please select it.\n");
                for (int i = 0;i < cnt;i++) {
                    printf("%d. release year: %d\n", i + 1, (movies + *(movieIndex + i))->release_year);
                }
                while (1) {
                    printf("Which one would you like to select? Select the index: \n");
                    scanf(" %s", num1);
                    int result = forcedIntegerInput(num1, 1, 1);
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
                        num = result - 1;
                        break;
                    }
                }
                break;
			}
			else if (cnt == 1) {
				num = 0;
				break;
			}
        }
		else {
            printf("Movie doesn't exist in our DB. Maybe something else?\n");
        }
    }

    //final
    int resultData = addFavouriteEntity(favouriteEntity.user_id, (movies + *(movieIndex+num))->movie_id);

    if (resultData == SUCCESS) {
        printf("ADD SUCCESSFULLY!!\n");
        return;
    } else {
        printf("Have some problem. Add again\n");
        return;
    }
}

void removeFavourite() {
    int userID;

    //user_id
    userID = (users + logged_in_user_index)->user_id;

    //movie_title
    int *movieIndex;
    char title[200];
	int num;
	char num1[5];
    printf("Movie title: \n");
    while (1) {
        rewind(stdin);
        scanf("%[^\n]", title);
        if (matchingMovieTitleExists(title)) {
            movieIndex = movieIndex_ByMatchingTitle_WithoutYear(title);
			int cnt = 0;
			while (1) {
				if (*(movieIndex + cnt) == END_OF_INT_ARRAY) {
					break;
				}
				cnt++;
			}

			if (cnt > 1) {
                printf("More than 1 movies exist. Please select one.\n");
                for (int i = 0;i < cnt;i++) {
                    printf("%d. release year: %d\n", i + 1, (movies + *(movieIndex + i))->release_year);
                }
                while (1) {
                    printf("Which one would you like to remove? Select the index: \n");
                    scanf("%s", num1);
                    int result = forcedIntegerInput(num1, 1, 1);
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
                        num = result - 1;
                        break;
                    }
                }
                break;
			}
			else if (cnt == 1) {
				num = 0;
				break;
			}
        }
		else {
            printf("Movie doesn't exist in our DB. Maybe something else?\n");
        }
    }

    //final
	if (!favouriteMovieIDExists((movies + *(movieIndex + num))->movie_id)) {
		printf("You didn't favourite this movie.\n");
		return;
	}
	else {
        int *favouriteIndexRemove = favouriteIndex_ByDoubleID(userID, (movies + *(movieIndex + num))->movie_id);
        int count = 0;
        while (1) {
            if (*(favouriteIndexRemove + count) == END_OF_INT_ARRAY) {
                break;
            }
            count++;
        }
        for (int i = 0; i < count; i++) {
            if (deleteFavourite_ByIndex(*(favouriteIndexRemove + i)) == SUCCESS) {
                printf("Removed all entities successfully!!\n");
            } else {
                printf("Have some problem. Remove again\n");
            }
        }
    }
}

void addUser() {
    User userEntity;
    char userIDEntity[7];
    userEntity.user_name = (char *) malloc(sizeof(char) * 100);
    userEntity.password = (char *) malloc(sizeof(char) * 100);

    //user_id
    while (1) {
        printf("User ID to add: ");
        scanf(" %s", userIDEntity);
        int result = forcedIntegerInput(userIDEntity, 6, 1);
        if (result == FAIL_NOT_A_NUMBER) {
            printf("It's not a number\n");
        } else if (result == FAIL_TOO_MANY_FIGURES) {
            printf("It's too big\n");
        } else if (result == FAIL_LACK_OF_FIGURES) {
            printf("It's too small\n");
        } else {
            if (userIDExists(result)) {
                printf("id already exists.\n");
            } else {
                userEntity.user_id = result;
                break;
            }
        }
    }

    //user_name
    printf("User name: ");
    while (1) {
        rewind(stdin);
        scanf(" %s", userEntity.user_name);
        if (!userNameExists(userEntity.user_name)) {
            break;
        } else {
            printf("user name has already been taken. try something else:\n");
        }
    }
	//password
	while (1) {
	
		printf("Password: ");
		rewind(stdin);
		scanf("%[^\n]", userEntity.password);
		char *password;
		password = (char*)malloc(sizeof(char) * 100);
		printf("Enter password again: ");
		rewind(stdin);
		scanf("%[^\n]", password);

		if (!strcmp(userEntity.password,password)) {
			//free(password);
			break;
		}
		else {
			printf("password is not coincidence.\n");
		}
	}
    

    //final
    int resultData = addUserEntity(userEntity.user_id, userEntity.user_name, userEntity.password);
	free(userEntity.user_name);
	free(userEntity.password);
    if (resultData == SUCCESS) {
        printf("ADD SUCCESSFULLY!!\n");
        logged_in_user_index = userIndex_ByUserID(userEntity.user_id);
        return;
    } else {
        printf("Have some problem. Add again\n");
        return;
    }
}

void removeUser() {
    int userID = (users + logged_in_user_index)->user_id;

    int userIndexRemove;
    userIndexRemove = userIndex_ByUserID(userID);
    if (deleteUser_ByIndex(userIndexRemove) == SUCCESS) {
        printf("REMOVE SUCCESSFULLY!!\n");
        logout();
        return;
    } else {
        printf("Have some problem. Remove again.\n");
        return;
    }
}

void searchByUserName() {
	//user_name
    char name[100];
    User userEntity;
    while (1) {
        printf("User name to search for: \n");
        scanf(" %s", name);
        if (userNameExists(name)) {
            break;
        } else {
            printf("User not found. Please try a different name: \n");
        }
    }

    int index_user = userIndex_ByName(name);
    userEntity.user_name = malloc(sizeof(char) * strlen(name));
    strcpy(userEntity.user_name, name);
    userEntity.user_id = (users + index_user)->user_id;

    //title and tag
    if (userIDExists_InTag(userEntity.user_id) == 1) {
        for (int i = 0; i < tag_count; i++) {
            if (userEntity.user_id == (tags + i)->user_id) {
                if(movieIDExists((tags + i)->movie_id)){
                    printf("Tag \"%s\" for movie: %s\n", (tags + i)->tag,
                           (movies + movieIndex_ByID((tags + i)->movie_id))->title);
                }
                else{
                    printf("Tag \"%s\" for unregistered movie\n", (tags + i)->tag);
                }
            }
        }
    } else {
        printf("There's no %s's tag\n", userEntity.user_name);
    }

    //favourites
    if (favouriteUserIDExists(userEntity.user_id) == 1) {
        for (int i = 0; i < favourite_count; i++) {
            if (userEntity.user_id == (favourites + i)->user_id) {
                printf("Favourited movie title: %s (%d)\n", (movies + movieIndex_ByID((favourites + i)->movie_id))->title, (movies + movieIndex_ByID((favourites + i)->movie_id))->release_year);
            }
        }
    } else {
        printf("There's no %s's favourite\n", userEntity.user_name);
    }
	free(userEntity.user_name);
}

void searchByMovieTitle() {
    char *searchMovieTitle;
    int *indexes;
    int result = 1;
    searchMovieTitle = (char *) malloc(sizeof(char) * 200);

	//title
    while (1) {
        printf("Title to search: ");
        rewind(stdin);
        scanf("%[^\n]", searchMovieTitle);
        if (movieTitleExists(searchMovieTitle)) {
			break;
        }
		else {
            printf("No such movie to be found. Try something else: \n");
        }
    }

    int page = -1;
    while (1) {
        int cnt = 0;
        char num[5];
        short break_flag = 0;
        indexes = movieIndex_ByTitle(searchMovieTitle);
        while (1) {
            if (*(indexes + cnt) == END_OF_INT_ARRAY) {
                break;
            }
            cnt++;
        }
		free(searchMovieTitle);

        if (result == 1) {
            page++;
            int start = page * 10;
            int end = (page + 1) * 10;
            if (end > cnt) {
                break_flag = 1;
                end = cnt;
            }

            for (int i = start; i < end; i++) {
                //movie ID, release year, genre
                printf("--------------------------------------\n%s (%d) has id: %d\n", ((movies + *(indexes + i))->title),
                       ((movies + *(indexes + i))->release_year), ((movies + *(indexes + i))->movie_id));
                printf("Genre: ");
                int *genreIndex;
                genreIndex = genreIndex_ByMovieID(((movies + *(indexes + i))->movie_id));
                if (((movies + *(indexes + i))->sizeof_genre) == 0) {
                    printf("No genre.\n");
                } else {
                    for (int j = 0; j < ((movies + *(indexes + i))->sizeof_genre); j++) {
                        char *genre;
                        genre = string_ByGenreIndex(genreIndex[j]);
                        if (j < ((movies + *(indexes + i))->sizeof_genre) - 1) {
                            printf("%s, ", genre);
                        } else {
                            printf("%s\n", genre);
                        }
                    }
                }

                //tag
                int k = 0;
                for (int j = 0; j < tag_count; j++) {
                    if ((movies + *(indexes + i))->movie_id == (tags + j)->movie_id) {
                        if (userIDExists(userIndex_ByUserID((tags + j)->user_id))) {
                            printf("    User %s left a tag: \"%s\"\n",
                                   (users + userIndex_ByUserID((tags + j)->user_id))->user_name, (tags + j)->tag);
                        } else {
                            printf("    Unknown user left a tag: \"%s\"\n", (tags + j)->tag);
                        }
                        k++;
                    }
                }
                if (k == 0) {
                    printf("There are no tags of the movie.\n");
                }

                //favourite
                int l = 0;
                for (int j = 0; j < favourite_count; j++) {
                    if ((movies + *(indexes + i))->movie_id == (favourites + j)->movie_id) {
                        l++;
                        if(userIDExists((favourites + j)->user_id)) {
                            printf("    User %s likes movie <%s>\n",
                                   (users + userIndex_ByUserID((favourites + j)->user_id))->user_name,
                                   (movies + *(indexes + i))->title);
                        }
                        else{
                            printf("    Unregistered user likes movie <%s>\n",
                                   (movies + *(indexes + i))->title);
                        }
                    }
                }
                if (l == 0) {
                    printf("Nobody added this movie to favourites.\n");
                }

            }
        } else if (result == 2) {
            break;
        }

        if (break_flag) {
            printf("Reached end of result.\n");
            break;
        }

        printf("1: Next page \n2: Return to main \n");
        printf("Please select a number: ");
        scanf(" %s", num);
        result = forcedIntegerInput(num, 2, 1);
    }
}

void searchTag() {
    char *searchTag;
    searchTag = (char *) malloc(sizeof(char) * 200);
    int *indexes;

    printf("Tag to search: ");
    while (1) {
        rewind(stdin);
        scanf("%[^\n]", searchTag);
        if (tagExists(searchTag) == 1) {
            indexes = tagIndex_ByContent(searchTag);
            break;
        } else {
            printf("Tag doesn't exist\n");
        }
    }
	free(searchTag);

    int count = 0;
    while (1) {
        if (*(indexes + count) == END_OF_INT_ARRAY) {
            break;
        }
        count++;
    }

    int page = -1;
    int result = 1;
    char num[4];
    short break_flag = 0;
    while (1) {
        if (result == 1) {
            page++;
            int start = page * 10;
            int end = (page + 1) * 10;
            if (end > count) {
                break_flag = 1;
                end = count;
            }

            for (int i = start; i < end; i++) {
                int user_index = userIndex_ByUserID((tags + *(indexes + i))->user_id);
                int movie_index = movieIndex_ByID((tags + *(indexes + i))->movie_id);

                struct tm *t;
                time_t temp_time = ((tags + *(indexes + i))->timestamp);
                t = localtime(&temp_time);

                if (user_index == FAIL_NO_SUCH_USER_ID) {
                    if (movie_index == FAIL_NO_SUCH_MOVIE_ID) {
                        printf("%d. unknown user tagged unregistered movie as \"%s\" on %d/%d/%d\n", i + 1,
                               (tags + *(indexes + i))->tag, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
                    } else {
                        printf("%d. unknown user tagged %s as \"%s\" on %d/%d/%d\n", i + 1,
                               (movies + movie_index)->title,
                               (tags + *(indexes + i))->tag, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
                    }
                } else {
                    if (user_index == FAIL_NO_SUCH_MOVIE_ID) {
                        printf("%d. %s tagged unregistered movie as \"%s\" on %d/%d/%d\n", i + 1,
                               (users + user_index)->user_name,
                               (tags + *(indexes + i))->tag, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
                    } else {
                        printf("%d. %s tagged %s as \"%s\" on %d/%d/%d\n", i + 1, (users + user_index)->user_name,
                               (movies + movie_index)->title,
                               (tags + *(indexes + i))->tag, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
                    }
                }

            }
        } else if (result == 2) {
            break;
        }

        if (break_flag) {
            printf("Reached end of result.\n");
            break;
        }
        printf("1: Next page \n2: Return to main \n");
        printf("Please select a number: ");
        scanf(" %s", num);
        result = forcedIntegerInput(num, 2, 1);
    }


}

void recommendMovie() {
    int *movieIndex;
    char *movietitle = (char *) malloc(sizeof(char) * 200);
    int num;
    int *temp_index;

    printf("Name the movie that suits your taste, and I'll bring up similar movies for you: ");
    while (1) {
        rewind(stdin);
        scanf("%[^\n]", movietitle);
        if (!matchingMovieTitleExists(movietitle)) {
            printf("That movie doesn't seem to exist in our DB. Maybe something else?");
        } else {
            temp_index = movieIndex_ByMatchingTitle_WithoutYear(movietitle);
            int cnt = 0;
            while (1) {
                if (*(temp_index + cnt) == END_OF_INT_ARRAY) {
                    break;
                }
                cnt++;
            }

            if (cnt > 1) {
                printf("More than 1 movies exist. Please select it.\n");
                for (int i = 0;i < cnt;i++) {
                    printf("%d. release year: %d\n", i + 1, (movies + *(temp_index + i))->release_year);
                }
                printf("Which one would you like to choose? Select the index: \n");
                scanf("%d", &num);
                num = num - 1;
            }
            else if (cnt == 1) {
                num = 0;
            }
            break;
        }
    }

    movieIndex = movieIndex_ByGenre((movies + *(temp_index+num))->genre, (movies + *(temp_index+num))->sizeof_genre);
    int count = 0;
    while (1) {
        if (*(movieIndex + count) == END_OF_INT_ARRAY) {
            break;
        }
        count++;
    }

    if (count != 0) {
        printf("Here are the recommendations tailored for you: \n");
        int page = -1;
        int result = 1;
        char num[4];
        short break_flag = 0;

        while (1) {
            if (result == 1) {
                page++;
                int start = page * 10;
                int end = (page + 1) * 10;
                if (end > count) {
                    break_flag = 1;
                    end = count;
                }

                //output movie
                for (int i = start; i < end; i++) {
                    if (strcmp(tolowerString(movietitle), tolowerString((movies + *(movieIndex + i))->title)))
                        printf("%d. %s (%d)\n", i + 1, (movies + *(movieIndex + i))->title,
                               (movies + *(movieIndex + i))->release_year);
                }
            } else if (result == 2) {
                break;
            }

            if (break_flag) {
                printf("Reached end of result.\n");
                break;
            }
            printf("1: Next page \n2: Return to main \n");
            printf("Please select a number: ");
            scanf(" %s", num);
            result = forcedIntegerInput(num, 2, 1);
        }
    } else {
        printf("Failed to fetch similar movies.");
    }
	free(movietitle);
}


void close() {
    save();
    integrity();
    retire();
    exit(0);
}

int selectMenu() {
    char num[3];
    printf("\n1: Add movie \n2: Remove movie \n"
           "3: Add tag \n4: Remove tag \n"
           "5: Add favourite \n6: Remove favourite \n"
           "7: Log out \n8: Remove account \n"
           "9: Search by user name \n10: Search by movie title \n"
           "11: Search for similar movies\n12: Search for tags\n0: Close\n");
    printf("Please select a number: ");
    scanf(" %s", num);
    int result = forcedIntegerInput(num, 2, 1);
    return result;
}

void menu() {
    int num = 1;
    while (num != 0) {
        num = selectMenu();
        switch (num) {
            case 1:
                addMovie();
                break;
            case 2:
                removeMovie();
                break;
            case 3:
                addTag();
                break;
            case 4:
                removeTag();
                break;
            case 5:
                addFavourite();
                break;
            case 6:
                removeFavourite();
                break;
            case 7:
                logout();
                break;
            case 8:
                removeUser();
                break;
            case 9:
                searchByUserName();
                break;
            case 10:
                searchByMovieTitle();
                break;
            case 11:
                recommendMovie();
                break;
            case 12:
                searchTag();
                break;
            case 0:
                close();
                break;
            default:
                printf("No number.\n");
                break;
        }
    }
    return;
}

int main() {
    startTimer();

    init();
    login();
    menu();

    //code below will not be executed since close() includes exit(0) command
    if (integrity()) {
        printf("Error occured while reading file. Please check file content.\n");
    }

    save();
    if (integrity()) {
        printf("Error occured while saving file. Please check file content.\n");
    }
    endTimer();

    return 0;
}
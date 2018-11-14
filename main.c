#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

Movie *movies;
Tag *tags;
Favourite *favourites;
int movie_count;
int tag_count;
int favourite_count;

const char *genreList[50];
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
void save();
void addMovie();
void removeMovie();
void addTag();
void removeTag();
void addFavourite();
void removeFavourite();
void searchByUserID(); //input: userID - output: all tags made by the user, movie info of tags, favourites list, movie info of favourites
void searchByMovieTitle(); //input: title - output: genre, tags, releaseYear, favourited by whom?, similar movies

//methods
void printMovieInfo(int movieID);
void printAllTag(int movieID);
void printTag(int index);
int* movieIndex_ByTitle(char *title);
int* movieIndex_ByGenre(int *genre);
int movieIndex_ByID(int movieID);
int* tagIndex_ByUserID(int userID);
int* tagIndex_ByMovieID(int movieID);
int* tagIndex_ByDoubleID(int userID, int movieID);
int* tagIndex_ByTag(char *tag);
int* favouriteIndex_ByUserID(int userID);
int* favouriteIndex_ByMovieID(int movieID);
int favouriteIndex_ByDoubleID(int userID, int movieID);

void integrity() {
    FILE *fp1 = fopen("outputmovie.txt", "r");
    FILE *fp2 = fopen("movies.dat", "r");

    char ch1 = getc(fp1);
    char ch2 = getc(fp2);
    int error = 0, potision = 0, line = 1;

    while (ch1 != EOF && ch2 != EOF) {
        potision++;
        if (ch1 == '\n' && ch2 == '\n') {
            line++;
            potision = 0;
        }
        if (ch1 != ch2) {
            error++;
            printf("Line Number : %d \tError"
                   " Position : %d \n", line, potision);
        }
        ch1 = getc(fp1);
        ch2 = getc(fp2);
    }
    printf("Error count on movies : %d\n", error);

    fclose(fp1);
    fclose(fp2);


    FILE *fp3 = fopen("outputtag.txt", "r");
    FILE *fp4 = fopen("tags.dat", "r");
    ch1 = getc(fp3);
    ch2 = getc(fp4);

    error = 0;
    potision = 0;
    line = 1;

    while (ch1 != EOF && ch2 != EOF) {
        potision++;
        if (ch1 == '\n' && ch2 == '\n') {
            line++;
            potision = 0;
        }
        if (ch1 != ch2) {
            error++;
            printf("Line Number : %d \tError"
                   " Position : %d \n", line, potision);
        }
        ch1 = getc(fp3);
        ch2 = getc(fp4);
    }
    printf("Error count on tags : %d\n", error);
}

int main(){
    init();
    save();
    integrity();

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

void init() {
    genreList[0] = "Action";
    genreList[1] = "Adventure";
    genreList[2] = "Animation";
    genreList[3] = "Children";
    genreList[4] = "Comedy";
    genreList[5] = "Crime";
    genreList[6] = "Documentary";
    genreList[7] = "Drama";
    genreList[8] = "Fantasy";
    genreList[9] = "Film-Noir";
    genreList[10] = "Horror";
    genreList[11] = "Musical";
    genreList[12] = "Mystery";
    genreList[13] = "Romance";
    genreList[14] = "Sci-Fi";
    genreList[15] = "Thriller";
    genreList[16] = "War";
    genreList[17] = "Western";
    genreListCursor=18;

    //init movie

    movies = (Movie *) malloc(sizeof(Movie));
    tags = (Tag *) malloc(sizeof(Tag));

    FILE *fp = fopen("movies.dat", "r");
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
        int flag;
        (movies + index)->genre = (int *) malloc(sizeof(int));
        if (strstr(split2, "|")) {
            cnt = 0;
            char *genreSplit = strtok(split2, "|");
            while (genreSplit != NULL) {
                flag=1;
                (movies + index)->genre = realloc((movies + index)->genre, (cnt + 1) * sizeof(int));
                for (int i = 0; i < genreListCursor; i++) {
                    if (!strcmp(genreSplit, genreList[i])) {
                        *(((movies + index)->genre) + cnt) = i;
                        flag=0;
                        break;
                    }
                }
                if(flag){
                    genreList[genreListCursor] = (char *) malloc(sizeof(char)*strlen(genreSplit));
                    strcpy(genreList[genreListCursor], genreSplit);
                    *(((movies + index)->genre) + cnt) = genreListCursor;

                    genreListCursor++;
                }
                genreSplit = strtok(NULL, "|");
                cnt++;
            }
            (movies + index)->sizeof_genre = cnt;
        } else {
            flag=1;
            for (int i = 0; i < genreListCursor; i++) {
                if (!strcmp(split2, genreList[i])) {
                    *(((movies + index)->genre)) = i;
                    flag=0;
                    break;
                }
            }
            if(flag){
                genreList[genreListCursor] = (char *) malloc(sizeof(char)*strlen(split2));
                strcpy(genreList[genreListCursor], split2);
                *(((movies + index)->genre)) = genreListCursor;
                genreListCursor++;
            }
            (movies + index)->sizeof_genre = 1;
        }

        index++;
    }
    fclose(fp);

    movie_count = index;

    //init tag

    fp = fopen("tags.dat", "r");
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

        index++;
    }
    fclose(fp);

    tag_count = index;

    return;
}

void save(){
    FILE *fp1 = fopen("outputmovie.txt", "w");
    FILE *fp2 = fopen("outputtag.txt", "w");

    if(movie_count != 0) {
        for (int i = 0; i < movie_count; i++) {
            fprintf(fp1, "%d::%s (%d)::", (movies + i)->movieID, (movies + i)->title, (movies + i)->releaseYear);
            for (int j = 0; j < (movies + i)->sizeof_genre; j++) {
                fprintf(fp1, "%s", genreList[*((movies + i)->genre + j)]);
                if(j != ((movies + i)->sizeof_genre)-1){
                    fprintf(fp1, "|");
                }
            }
            fprintf(fp1, "\n");
        }
    }

    if(tag_count != 0) {
        for (int i = 0; i < tag_count; i++) {
            Tag temp = *(tags + i);
            fprintf(fp2, "%d::%d::%s::%lld\n", temp.userID, temp.movieID, temp.tag, temp.timestamp);
        }
    }
}
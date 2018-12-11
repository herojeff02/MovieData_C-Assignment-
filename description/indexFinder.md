string의 끝은 널 문자로 판단이 가능하지만, int 배열의 경우 그럴 방법이 없기에 매크로를 사용해 END_OF_INT_ARRAY를 배열의 끝으로 판단할 수 있게 했습니다.
return을 위한 구조체를 만들어서 배열의 크기를 별도의 변수로 저장해도 됐겠지만, 메모리 면에서 저희가 사용한 방법이 더 효율적일 거라 판단했습니다.
물론 프런트엔드/컨트롤러 프로그래머의 수명이 감소한다는 문제가 있습니다.

int compare(const void *first, const void *second)
int *tempSortGenre(int index)
위 두 함수는 장르의 배열을 임시 정렬하기 위해 있습니다.
장르에서 검색하거나 장르끼리 비교할 경우 사용합니다.
영구적 배열 순서 변경이 아니기 때문에 temp를 이름에 붙였습니다.

int* movieIndex_ByTitle(char *title);
title이 포함된 모든 영화의 movies 배열의 인덱스를 한 정수 배열로 모아 반환합니다.
검색할 때 비교대상을 소문자로 바꿔 비교합니다.

int movieIndex_ByMatchingTitle(char *title, int year);
title과 일치하는 영화의 movies 배열의 인덱스를 한 개 찾아내 반환합니다.
같은 제목의 영화가 같은 해에 두 번 개봉하는 경우는 없다고 가정합니다.
검색할 때 비교대상을 소문자로 바꿔 비교합니다.

int* movieIndex_ByMatchingTitle_WithoutYear(char *title)
title의 제목을 가진 영화가 한 개 이상 있을 때만 사용해야 합니다.
영화의 제목과 title이 같은 movies의 인덱스 배열을 반환합니다.
movieIndex_ByMatchingTitle 함수를 사용할 때 같은 제목의 영화가 2개 이상이면 year를 선택하기 위해 필요합니다.

int* movieIndex_ByGenre(int *genre, short genre_count);
특정 장르 목록을 가진 영화의 목록을 반환합니다.

int movieIndex_ByID(int movie_id);
특정 id를 가진 영화의 인덱스를 반환합니다.
추가 과정에서 movie_id가 겹치는 경우 추가하지 않기 때문에 검색 결과는 무조건 하나라는 보장이 있습니다.

int userIndex_ByUserID(int user_id);
user_id를 가진 users의 인덱스를 반환합니다.

int genreIndex_ByString(char* *genre);*
전역 변수인 장르의 배열에서 string genre와 같은 장르의 인덱스를 반환합니다.
배열에서 string genre를 찾을 수 없다면 배열에 genre를 추가하고 그 인덱스를 반환합니다.

int* genreIndex_ByMovieID(int movieID);
movie_id를 가진 영화를 찾아 그 장르 배열을 반환합니다.

char* string_ByGenreIndex(int genre);
genreIndex_ByString와 반대 역할을 합니다.
int를 넣으면 string이 나옵니다.

int* tagIndex_ByUserID(int user_id);
user_id를 인자로 넘기면 그 유저가 남긴 모든 태그를 찾아 인덱스들을 배열로 반환합니다.

int* tagIndex_ByMovieID(int movie_id);
movie_id를 인자로 넘기면 그 유저가 남긴 모든 태그를 찾아 인덱스들을 배열로 반환합니다.

int* tagIndex_ByDoubleID(int user_id, int movie_id);
movie_id와 user_id를 인자로 넘기면 유저가 영화에 대해 남긴 모든 태그를 찾아 인덱스들을 배열로 반환합니다.

int* tagIndex_ByTag(char *tag);
string tag를 검색해 반환합니다. 모든 문자를 소문자로 변환해 검색합니다.

int* tagIndex_ByContent(char *content);
string tag를 검색해 반환합니다. 모든 문자를 소문자로 변환해 비교하지 않습니다.

int* favouriteIndex_ByUserID(int user_id);
favourites 배열에서 user_id와 연결된 인덱스 배열을 반환합니다.

int* favouriteIndex_ByMovieID(int movie_id);
favourites 배열에서 movie_id와 연결된 인덱스 배열을 반환합니다.

int* favouriteIndex_ByDoubleID(int user_id, int movie_id);
favourites 배열에서 movie_id, user_id와 연결된 인덱스 배열을 반환합니다.

Tag tagFinder_ByIndex(int tag_index);
tag_index에 해당하는 Tag 개체를 반환합니다.

Favourite favouriteFinder_ByIndex(int fav_index);
fav_index에 해당하는 Favourite 개체를 반환합니다.

int userIndex_ByName(char *name);
name(user_name)을 가진 User의 인덱스를 반환합니다.


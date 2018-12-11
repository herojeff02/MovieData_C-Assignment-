existence.c의 함수들은 인자와 그 조건이 존재하는지 확인합니다.

존재하면 1, 아니면 0을 반환합니다.



int movieIDExists(int movie_id);

movie_id를 가진 영화 구조체가 movies 배열에서 검색되면 1, 없으면 0을 반환합니다.



int movieIndexExists(int index);

movies 배열에 인덱스가 index인 개체가 존재하는지 확인합니다.



int userIDExists(int user_id);

users 배열에 ID가 user_id인 개체가 존재하는지 확인합니다.



int userIDExists_InTag(int user_id);

tags 배열에 user_id가 user_id인 개체가 존재하는지 확인합니다.



int userIndexExists(int index);

users 배열에 인덱스가 index인 개체가 존재하는지 확인합니다.



int userNameExists(char *name);

movies 배열에 사용자 이름이 name인 개체가 존재하는지 확인합니다.



int tagIndexExists(int index);

tags 배열에 인덱스가 index인 개체가 존재하는지 확인합니다.



int tagExists(char *content);

tags 배열에 내용이 content인 개체가 존재하는지 확인합니다.



int favouriteUserIDExists(int user_id);

favourite 배열의 Favourite 개체 중 user_id가 동일한 개체가 있는지 검사합니다.



int favouriteMovieIDExists(int movie_id);

favourite 배열의 Favourite 개체 중 movie_id가 동일한 개체가 있는지 검사합니다.



int favouriteIndexExists(int index);

favourites 배열에 인덱스가 index인 개체가 존재하는지 확인합니다.



int movieTitleExists(char *title);

영화 제목인 title을 movies 배열의 Movie 개체에서 검색해 title을 포함하는 결과가 있으면 1을 반환합니다.



int multipleMatchingMovieTitleExists(char *title);

영화 제목이 title과 일치하는 영화가 몇 개인지 정수로 반환합니다. 



int matchingMovieTitleExists(char *title);

영화 제목인 title을 movies 배열의 Movie 개체에서 검색해 title과 정확히 일치하는 결과가 있으면 1을 반환합니다.
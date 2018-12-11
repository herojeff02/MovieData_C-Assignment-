add 계열 함수는 구조체 배열에 개체를 추가하거나, 구조체의 내부 배열에 값을 추가하는 역할을 합니다.



int addMovieEntity(int movie_id, char *title, int release_year, int *genre, short genre_count);

Movie 구조체 하나를 movies 배열에 추가합니다. genre는 미리 genreIndex_ByString을 이용해 int로 변환한 뒤 인자로 사용합니다.

int addTagEntity(int user_id, int movie_id, char *tag);

Tag 구조체 하나를 tags 배열에 추가합니다.

int addUserEntity(int user_id, char *userName, char *password);

User 구조체 하나를 users 배열에 추가합니다. 회원 가입 역할을 합니다.

int addFavouriteEntity(int user_id, int movie_id);

사용자가 "좋아요" 처리한 영화를 favourites 배열에 추가합니다. 이때, addFavouriteIndex_ToUser도 호출합니다.

int addFavouriteIndex_ToUser(int user_index, int favourite_index);

user_index에 해당하는 users 배열의 User 개체에 새로 즐겨찾기한 값을 설정합니다.

int addGenre_ToMovie(int user_index, char *genre);
int removeGenre_FromMovie(int user_index, char *genre);
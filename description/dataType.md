전역 변수와 매크로, 구조체 등을 모아놓은 파일이다. 이 파일의 경우 헤더 파일에 대부분의 정보가 포함된다.

모든 구조체는 메모리 절약을 위해 포인터를 사용한다.



Movie 구조체는 int movie_id, char *title, int release_year, int *genre, short sizeof_genre, short enabled 값을 가진다.

movie_id, title, release_year, genre는 기존 데이터의 값을 저장하기 위한 용도이고, sizeof_genre는 장르 개수를 미리 계산해 사용할 때마다 크기를 셀 필요가 없게 한다. enabled는 구조체 삭제에 사용되는 값이다.



Tag 구조체는 user_id, int movie_id, char *tag, long long timestamp, short enabled 값을 가진다.

user_id, movie_id, tag, timestamp는 기존 데이터의 값을 저장하기 위한 용도이고, enabled는 구조체 삭제에 사용되는 값이다.



Favourite 구조체는 user_id, int movie_id, short enabled 값을 가진다.

user_id, movie_id는 어떤 사용자가 어떤 영화를 ''좋아요'' 처리하는지 저장하기 위해 사용한다. enabled는 구조체 삭제에 사용되는 값이다.



User 구조체는 int user_id, char *user_name, char *password, int *favourite_index, short enabled, int sizeof_favourites 값을 가진다.

user_id는 Tag, Favourite과 연결하기 위해 사용자 추가 시 지정하며, user_name, password는 사용자 이름과 비밀번호이다. favourite_index는 favourites 배열에서 어떤 Favourite 개체를 ''좋아요'' 처리했는지 빠르게 찾기 위해 있다. sizeof_favourites는 그 배열 크기를 저장한다. enabled는 구조체 삭제에 사용되는 값이다.



사용되는 전역 변수는 아래와 같다.

Movie *movies
Tag *tags
User *users
Favourite *favourites

위 네 개 포인터는 모든 Movie, Tag, User, Favourite 구조체를 저장하는 배열로 사용된다.



int movie_count
int tag_count
int user_count
int favourite_count

위 네 개 정수는 구조체의 크기를 각각 저장한다.



char **genre_list

genre_list는 파싱 중 찾은 장르의 목록을 저장한다.

int genre_list_cursor

genre_list_cursor는 장르 배열의 크기와 같은 역할을 하지만, 장르를 추가하는 시점에서의 가독성을 높이기 위해 cursor라는 이름을 사용했다.
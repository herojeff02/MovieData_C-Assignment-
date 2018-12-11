delete 계열 함수는 구조체 배열에서 특정 개체를 삭제하는 함수입니다.

파일에서 한 줄만 골라 지우는 것은 C에서 불가능에 가깝기 때문에, 구조체에 enabled 변수를 넣어 enabled가 0이면 save 계열 함수에서 파일에 쓰지 않는 방법을 취했습니다.

함수의 끝에서 관련된 save와 init을 한 번 호출함으로써 다른 함수에서 충돌이 일어나지 않게 처리했습니다.

int deleteMovie_ByIndex(int index);
int deleteTag_ByIndex(int index);
int deleteUser_ByIndex(int index);
int deleteFavourite_ByIndex(int index);


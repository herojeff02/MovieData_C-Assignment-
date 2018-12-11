integrity 계열의 함수는 파일의 무결성 검사에 사용됩니다.
개발과 안정화가 어느정도 진행돼, 파일이 정상적으로 저장되는 시점부터는 잘 확인할 필요가 없어졌습니다.
그러나 프로그램과 실행과 종료 전에 모든 파일의 데이터가 메모리에 있는 내용과 맞아떨어지는지 확인합니다. 코드의 구조는 save 계열 함수와 동일합니다.
파일에 문제가 있으면 문제가 있는 내용과 줄을 출력하고 1을 반환합니다.
문제가 없으면 0을 반환합니다.

int integrityMovie()
int integrityTag()
int integrityFavourite()
int integrityUser()
위 네 줄의 함수를 동시에 실행해 단 하나의 문제라도 있는지 찾아내는 함수가 아래의 integrity입니다.
int integrity()

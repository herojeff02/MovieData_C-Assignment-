init 계열 함수는 TEST_FILE_FLAG를 통해 시험용 파일을 사용할지, 실제 파일을 사용할지 결정합니다.
fgets를 이용해 파일을 줄 단위로 읽어 파싱 단계로 넘어갑니다.
customString.c에서 작성한 split_back, split_front를 사용합니다.
연도는 한 칸 후 괄호 안에 네 자리 숫자만이 존재하기 때문에 인덱스를 직접적으로 사용해 값을 읽었습니다.
모든 구조체 배열은 포인터를 사용하기 때문에 한 줄/한 요소(장르, 즐겨찾기 목록 등)를 읽을 때마다 realloc하는 코드가 있습니다.

void initMovie()
void initTag()
void initUser()
void initFavourite()

위 네 개 함수를 한번에 실행하는 함수가 아래의 init() 함수입니다.
void init()
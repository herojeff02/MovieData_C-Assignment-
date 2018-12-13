init 계열 함수는 TEST_FILE_FLAG를 통해 시험용 파일을 사용할지, 실제 파일을 사용할지 결정합니다.
fgets를 이용해 파일을 줄 단위로 읽어 파싱 단계로 넘어갑니다.
customString.c에서 작성한 split_back, split_front를 사용합니다.

strtok의 delim으로 "::"를 사용하면 함수가 두 콜론 중 하나만 이용하기 때문에, split_back과 split_front 함수는 strstr로 delim의 위치를 찾은 뒤 직접 널값을 대입해 문자열을 분리합니다. 상황에 따라 원본 데이터를 보존하지 못하는 등 문제가 있고, 모든 상황에서 작동하도록 코딩하지 않았기 때문에 strtok의 대체제로 사용하지는 않았습니다. 하지만, 영화 제목에 콜론 한 개가 있어 파싱/데이터가 틀어지는 상황을 막을 수 있었습니다.

연도는 한 칸 후 괄호 안에 네 자리 숫자만이 존재하기 때문에 인덱스를 직접적으로 사용해 값을 읽었습니다.
모든 구조체 배열은 포인터를 사용하기 때문에 한 줄/한 요소(장르, 즐겨찾기 목록 등)를 읽을 때마다 realloc하는 코드가 있습니다.

모든 함수는 그 구조체 배열의 길이를 전역 변수로 저장합니다.

void initMovie()

영화의 장르를 읽을 때 genreIndex_ByString 함수를 통해 string을 int 인덱스로 치환해 메모리를 절약하고, 새로운 장르에 대비할 수 있게 합니다.

void initTag()
void initUser()
void initFavourite()

위 네 개 함수를 한번에 실행하는 함수가 아래의 init() 함수입니다.
void init()


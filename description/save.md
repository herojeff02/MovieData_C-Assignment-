save 계열의 함수들은 기존 파일의 포맷을 유지하는 데 초점을 뒀습니다.

예를 들어 saveMovie 함수에서 장르가 여러 개이면 ''장르1|장르2|장르3'과 같이 기존의 포맷을 유지합니다.

각 구조체에 포함된 enabled 변수의 값을 확인해 저장 여부를 결정합니다.

void saveUser();
void saveMovie();
void saveTag();
void saveFavourite();

위 네 줄의 코드를 한번에 실행하는 함수가 아래의 save()입니다.

void save();
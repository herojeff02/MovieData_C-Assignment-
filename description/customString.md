char* tolowerString(char* content);

단일 char만 지원하는 tolower 함수를 이용한, 문자열 하나를 소문자로 변환하는 함수이다.



char* tolowerCapitalizer(char* content);

단일 char만 지원하는 tolower과 toupper 함수를 이용했다.

첫 글자와 '-' 다음에 오는 글자만 대문자, 나머지는 소문자로 변환한다.



char *split_front(char *str, const char *delim);

"::"를 strtok로 분리할 수 없어 작성한 함수이다.

strstr로 delim의 위치를 찾고 널값을 넣어 문자열을 분리해 반환한다.

원본 문자열의 손실이 일어나기 때문에 split_back을 먼저 사용해야 한다.



char *split_back(char *str, const char *delim);

"::"를 strtok로 분리할 수 없어 작성한 함수이다.

strstr로 delim의 위치를 찾고 그 뒤의 문자열을 반환한다.

원본 문자열의 손실이 일어나지 않는다.



int forcedIntegerInput(char *string, int figures, short is_maximum_figures);

string은 %s로 입력받은 정수와 같은 경우 생기는 정수의 문자열이다.

figures는 자릿수이다.

string을 우선 정수로 변환한 뒤 그 정수가 자릿수에 맞는지 판단한다.

is_maximum_figures는 최대 자릿수만을 사용할지 판단하는 기준이 된다.

정수가 아닌 경우 FAIL_NOT_A_NUMBER, 자릿수가 figures보다 큰 경우 FAIL_TOO_MANY_FIGURES, 자릿수가 figures보다 작은 경우 FAIL_LACK_OF_FIGURES가 반환되며 모든 조건에 맞는 정수인 경우 정수가 반환된다.



int stringIncludesMarker(char *string);

"::"가 string에 포함되는지 판단한다. 포함돼있으면 1, 아닐 경우 0을 반환한다.
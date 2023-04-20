/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 큐의 최대 크기 정의

typedef char element; // 큐의 요소의 데이터 타입을 char로 정의
typedef struct {
element queue[MAX_QUEUE_SIZE]; // 큐의 요소를 저장하는 배열
int front, rear; // 큐의 front와 rear 인덱스
}QueueType; // 큐 타입 정의

QueueType *createQueue(); // 큐 생성 함수 선언
int freeQueue(QueueType *cQ); // 큐 메모리 해제 함수 선언
int isEmpty(QueueType *cQ); // 큐가 비어있는지 확인하는 함수 선언
int isFull(QueueType *cQ); // 큐가 가득 차있는지 확인하는 함수 선언
void enQueue(QueueType *cQ, element item); // 큐에 요소를 추가하는 함수 선언
void deQueue(QueueType *cQ, element *item); // 큐에서 요소를 제거하고 반환하는 함수 선언
void printQ(QueueType *cQ); // 큐의 내용을 출력하는 함수 선언
void debugQ(QueueType *cQ); // 디버그용 정보 출력 함수 선언
element getElement(); // 새로운 요소를 입력받는 함수 선언

int main(void)
{
    QueueType *cQ = createQueue(); // 원형 큐 생성
    element data; // 큐에 삽입될 데이터

    char command; // 사용자가 입력한 명령어

	printf("[----- [김예진]  [2022041035] -----]\n");

    do {
        // 사용자 인터페이스
        printf("\n-----------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command); // 사용자로부터 명령어 입력

        switch (command) {
        case 'i': case 'I': // 데이터 삽입 명령
            data = getElement(); // 사용자로부터 데이터 입력
            enQueue(cQ, data); // 큐에 데이터 삽입
            break;
        case 'd': case 'D': // 데이터 삭제 명령
            deQueue(cQ, &data); // 큐에서 데이터 삭제
            break;
        case 'p': case 'P': // 큐 출력 명령
            printQ(cQ); // 큐의 데이터 출력
            break;
        case 'b': case 'B': // 디버깅 명령
            debugQ(cQ); // 큐의 내부 상태 출력
            break;
        case 'q': case 'Q': // 프로그램 종료 명령
            break;
        default: // 잘못된 명령어 입력
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }
    } while (command != 'q' && command != 'Q'); // 사용자가 프로그램 종료 명령을 입력하기 전까지 반복

    return 1; // 프로그램 종료
}


QueueType *createQueue()
{
	QueueType *cQ;  // QueueType 구조체 포인터 cQ를 선언한다.
	cQ = (QueueType *)malloc(sizeof(QueueType));  // 동적 할당을 통해 QueueType 구조체 크기만큼 메모리를 할당하고, 할당받은 메모리의 시작주소를 cQ에 저장
	cQ->front = 0;  // cQ의 front 멤버 변수를 0으로 초기화
	cQ->rear = 0;  // cQ의 rear 멤버 변수를 0으로 초기화
	return cQ;  // 생성된 QueueType 구조체 포인터 cQ를 반환
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // 만약 cQ가 NULL이라면 함수를 종료하고 1을 반환
    free(cQ); // cQ를 free() 함수를 이용하여 할당 해제
    return 1; // 함수를 종료하고 1을 반환
}

element getElement() 
{
    element item; // element형 변수 item 선언
    printf("Input element = "); 
    scanf(" %c", &item); // 사용자로부터 입력받은 값을 item에 저장
    return item; // item을 반환
}

int isEmpty(QueueType *cQ)
{
	// 큐의 front와 rear가 같은 경우, 큐가 비어있음을 의미
	if (cQ->front == cQ->rear){
		printf("원형 큐가 비어있습니다!");
		return 1;
	}
	// 큐가 비어있지 않은 경우
	else return 0;
}

int isFull(QueueType *cQ)
{
    // rear 다음 위치가 front인 경우, 큐가 꽉 찬 상태
    if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) {
        printf(" Circular Queue is full!");
        return 1;
    }
    else return 0;
}

void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return;  // 큐가 비어있으면 함수 종료
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 다음 위치로 이동
		cQ->queue[cQ->rear] = item; // 새로운 아이템을 rear 위치에 추가
	}
}

void deQueue(QueueType *cQ, element *item)
{
	if(isEmpty(cQ)) return;  // 큐가 비어있으면 함수 종료
	else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; // front 값을 1 증가시킴으로써 큐에서 첫 번째 아이템을 제거
		*item = cQ->queue[cQ->front]; // 제거된 아이템을 item에 저장
		return;
	}
}

void printQ(QueueType *cQ)
{
	int i, first, last;

	// first와 last를 각각 큐의 front와 rear+1로 초기화
	first = (cQ->front + 1)%MAX_QUEUE_SIZE;
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;

	// 큐의 요소들을 출력하는 부분
	printf("Circular Queue : [");
	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;
	}
	printf(" ]\n");
}

void debugQ(QueueType *cQ)
{
	printf("\n---DEBUG\n");
	// 큐의 모든 아이템을 순회하면서 출력
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		// 만약 현재 아이템이 front일 경우 "[i] = front"를 출력하고 다음 아이템으로 이동
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
		// 그 외에는 "[i] = 해당 아이템"을 출력
		printf("  [%d] = %c\n", i, cQ->queue[i]);
	}
	//printQ(cQ);
	// front와 rear의 값을 출력
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}


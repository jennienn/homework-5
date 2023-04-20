 /**
 * postfix.c
 *
 * School of Computer Science,
 * Chungbuk National University
 */
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */ 
typedef enum{
    lparen=0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times=7, /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5, /* + 덧셈 */
    minus=4, /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE]; // infix가 저장될 배열
char postfixExp[MAX_EXPRESSION_SIZE]; // postfix가 저장될 배열
char postfixStack[MAX_STACK_SIZE]; // postfix 변환을 위한 스택
int evalStack[MAX_STACK_SIZE]; // 계산을 위한 스택

int postfixStackTop = -1; // postfix 변환 스택의 top
int evalStackTop = -1; // 계산 스택의 top

int evalResult = 0; // 계산 결과 저장 변수

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

 int main() {

     printf("[----- [김예진]  [2022041035] -----]\n");
     char command; // 명령 입력을 저장할 변수

    // 명령 입력이 'q'나 'Q'가 아닌 동안 반복
     do{
printf("----------------------------------------------------------------\n");
           printf("               Infix to Postfix, then Evaluation\n");
printf("----------------------------------------------------------------\n");
           printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r, Quit=q\n");

printf("----------------------------------------------------------------\n");
           printf("Command = "); 
           scanf(" %c", &command); // 명령 입력

            switch(command) {
            case 'i': case 'I':
            getInfix(); // 함수 getInfix() 호출 
                break;
            case 'p': case 'P':
                toPostfix(); // 함수 toPostfix() 호출 
                break;
            case 'e': case 'E':
                evaluation(); // 함수 evaluation() 호출 
                break;
            case 'd': case 'D':
            debug(); // 함수 debug() 호출 
                break;
            case 'r': case 'R':
            reset(); // 함수 reset() 호출 
                break;
            case 'q': case 'Q':
                break;
            default:
            printf(">>>>>   Concentration!!   <<<<< \n");
           break;
    }

     }while(command != 'q' && command != 'Q'); // 종료 명령 입력 시 반복 종료

    return 1;
}

void postfixPush(char x)  // postfixStack에 x를 push하는 함수
{
    postfixStack[++postfixStackTop] = x; // postfixStackTop의 값을 1 증가시킨 후 x를 postfixStack 배열에 저장
}

char postfixPop() // postfixStack에서 top에 있는 값을 pop하고, 그 값을 반환하는 함수
{
     char x; // x 변수 선언
    if(postfixStackTop == -1) // postfixStack이 비어있으면
        return '\0'; // NULL 문자 반환
    else { // 그렇지 않으면
     x = postfixStack[postfixStackTop--]; // postfixStack의 top에 있는 값을 x에 할당하고 top 위치를 1 감소시킴
    }
return x; // x 값 반환
}  


void evalPush(int x) // evalStack에 x를 push하는 함수
{
    evalStack[++evalStackTop] = x; // evalStack의 값을 1 증가시킨 후 x를 evalStack 배열에 저장
}

int evalPop() // eval 스택에서 값을 pop하는 함수
{
    if(evalStackTop == -1) // 스택이 비어있는 경우
        return -1;
    else
        return evalStack[evalStackTop--]; // 스택에서 값을 pop하고 반환
}

/**
* infix expression을 입력받는다. 
* infixExp에는 입력된 값을 저장한다. 
*/

void getInfix() // 사용자로부터 infix 수식을 입력받는 함수
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol) // symbol을 받아, 해당 연산자의 우선순위(precedence)를 반환하는 함수
{

     switch(symbol) {
     case '(' : return lparen;
     case ')' : return rparen;
     case '+' : return plus;
     case '-' : return minus;
     case '/' : return divide;
     case '*' : return times;
     default : return operand;
     }
}

precedence getPriority(char x) // x를 받아 해당 연산자의 우선순위(precedence)를 반환하는 함수
{
     return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가 
 */

void charCat(char* c) // c가 가리키는 문자열을 postfixExp에 이어붙이는 함수
{
     if (postfixExp == '\0') // postfixExp가 비어있으면 c를 그대로 복사
           strncpy(postfixExp, c, 1);
     else // postfixExp가 비어있지 않으면 c를 postfixExp 뒤에 이어붙임
           strncat(postfixExp, c, 1);
}

/**
* infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다. 
* 변경된 postfix는 postFixExp에 저장된다.
*/

void toPostfix() // infix를 postfix로 변환하는 함수
{
/* infixExp의 문자 하나씩을 읽기위한 포인터 */ 
char *exp = infixExp;
char x; /* 문자하나를 임시로 저장하기 위한 변수 */

/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */ 
while(*exp != '\0')
{
           if(getPriority(*exp) == operand) // 현재 문자가 피연산자인 경우
           {
                x = *exp;
           charCat(&x);  // 현재 문자를 postfixExp에 추가
           }
        else if(getPriority(*exp) == lparen) { // 현재 문자가 왼쪽 괄호인 경우

            postfixPush(*exp); // postfix 스택에 push

         }
        else if(getPriority(*exp) == rparen) // 현재 문자가 오른쪽 괄호인 경우
        {
           while((x = postfixPop()) != '(') { // 왼쪽 괄호가 나올 때까지 postfix 스택에서 pop
                charCat(&x); // pop한 문자를 postfixExp에 추가
            } 
        }
        else { // 현재 문자가 연산자인 경우
            while(getPriority(postfixStack[postfixStackTop]) >=
getPriority(*exp)) // 현재 연산자보다 우선순위가 높거나 같은 연산자들을 postfix 스택에서 pop
            {
                x = postfixPop(); // pop한 연산자를 x에 저장
                charCat(&x); // pop한 연산자를 postfixExp에 추가
            }
            postfixPush(*exp); // 현재 연산자를 postfix 스택에 push
        }
        exp++; // 다음 문자로 이동
    }

    while(postfixStackTop != -1) // postfix 스택에 남은 연산자들을 postfixExp에 추가
    {
     x = postfixPop(); // postfix 스택에서 pop한 연산자를 x에 저장
     charCat(&x); // pop한 연산자를 postfixExp에 추가
    }
}



void debug() // debug 모드 함수
{
     printf("\n---DEBUG\n"); 
     printf("infixExp =  %s\n", infixExp); // infixExp 출력
     printf("postExp =  %s\n", postfixExp); //postfixExp 출력
     printf("eval result = %d\n", evalResult); // 계evalResult 출력

     printf("postfixStack : "); 
     for(int i = 0; i < MAX_STACK_SIZE; i++) // postfixStack의 모든 원소 출력
           printf("%c  ", postfixStack[i]);

     printf("\n"); 
}

void reset() // 초기화 함수
{
     // infixExp와 postfixExp를 빈 문자열로 초기화
     infixExp[0] = '\0';
     postfixExp[0] = '\0';

     // postfixStack의 모든 원소를 널 문자로 초기화
      for(int i = 0; i < MAX_STACK_SIZE; i++)
           postfixStack[i] = '\0';

     // postfixStackTop과 evalStackTop을 -1로 초기화
     postfixStackTop = -1;
     evalStackTop = -1;

     // evalResult를 0으로 초기화
     evalResult = 0;
}

void evaluation() // postfix 계산 함수
{
     int opr1, opr2, i;

     int length = strlen(postfixExp); // postfixExp 문자열의 길이를 구함
     char symbol;
     evalStackTop = -1; // eval 스택의 top을 초기화함

     for(i = 0; i < length; i++) // postfixExp 문자열을 탐색하며 계산을 수행함
     {
           symbol = postfixExp[i]; // postfixExp에서 현재 탐색하는 문자를 가져옴
           if(getToken(symbol) == operand) { // 만약 현재 문자가 operand(숫자)이면
                evalPush(symbol - '0'); // eval 스택에 현재 문자를 숫자로 변환하여 push함
           }
           else { // 만약 현재 문자가 operator(연산자)이면
                opr2 = evalPop(); // eval 스택에서 숫자 두 개를 pop하여
                opr1 = evalPop(); // opr2와 opr1에 저장함
                switch(getToken(symbol)) { // 현재 연산자에 따라 계산을 수행함
                case plus: evalPush(opr1 + opr2); break;
                case minus: evalPush(opr1 - opr2); break;
                case times: evalPush(opr1 * opr2); break;
                case divide: evalPush(opr1 / opr2); break;
                default: break;
                }
            } 
        }
     evalResult = evalPop(); // 계산 결과를 evalResult에 저장함
}


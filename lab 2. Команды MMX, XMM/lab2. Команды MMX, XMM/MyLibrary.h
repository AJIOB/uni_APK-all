#pragma once

//для .cpp и не только
#include "view/view.h"
#include "timers.h"
#include "random.h"

#define NUM_ROWS 1000000
#define NUM_COLUMNS 4

#ifndef WORD
#define WORD
typedef unsigned short int word;
typedef unsigned long int dword;
typedef unsigned long long int qword;
#endif

//основная функция
void defaultFunction();

//конец программы
inline void programEnd()
{
	std::cout <<  "Программа завершила свою работу. Для выхода введите любой символ" << std::endl;
	PauseConsole();
}

void init (word* A);

TIMER_T Asm(word* A, const word* B, const word* C, const word k);
TIMER_T MMX(word* A, const word* B, const word* C, const word k);
TIMER_T C_Only(word* A, const word* B, const word* C, const word k);

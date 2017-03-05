#pragma once

//для .cpp и не только
#include "view/view.h"
#include "timers.h"
#include "random.h"

#define SIZE_ROW 1000000
#define SIZE_COLUMN 8

#ifndef BYTE
#define BYTE
typedef unsigned char byte;
#endif

//основная функция
void defaultFunction();

//конец программы
inline void programEnd()
{
	std::cout <<  "Программа завершила свою работу. Для выхода введите любой символ" << std::endl;
	PauseConsole();
}

void init (byte* A);

TIMER_T Asm(byte* A, const byte* B, const byte* C, const byte k);
TIMER_T MMX(byte* A, const byte* B, const byte* C, const byte k);
TIMER_T C_Only(byte* A, const byte* B, const byte* C, const byte k);

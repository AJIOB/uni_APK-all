#pragma once

//для .cpp и не только
//#include <cmath>
#include "view/view.h"
#include "timers.h"
#include "random.h"

#define SIZE_ROW 30000
#define SIZE_COLUMN 8

typedef unsigned char byte;

//основная функция
void defaultFunction();

//конец программы
inline void programEnd()
{
	std::cout <<  "Программа завершила свою работу. Для выхода введите любой символ" << std::endl;
	PauseConsole();
}

void init (byte A[SIZE_ROW][SIZE_COLUMN]);

TIMER_T Asm(byte A[SIZE_ROW][SIZE_COLUMN], const byte B[SIZE_ROW][SIZE_COLUMN], const byte C[SIZE_ROW][SIZE_COLUMN], const byte& k);
TIMER_T C_Only(byte A[SIZE_ROW][SIZE_COLUMN], const byte B[SIZE_ROW][SIZE_COLUMN], const byte C[SIZE_ROW][SIZE_COLUMN], const byte& k);
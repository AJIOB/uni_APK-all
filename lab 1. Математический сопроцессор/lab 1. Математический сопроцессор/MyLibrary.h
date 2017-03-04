#pragma once

//для .cpp и не только
#include <cmath>
#include "view/view.h"
#include "timers.h"

//основная функция
void defaultFunction();

//конец программы
inline void programEnd()
{
	std::cout <<  "Программа завершила свою работу. Для выхода введите любой символ" << std::endl;
	PauseConsole();
}

TIMER_T Soproc(const double& start, const double& end, const double& gap);
TIMER_T C_Only(const double& start, const double& end, const double& gap);
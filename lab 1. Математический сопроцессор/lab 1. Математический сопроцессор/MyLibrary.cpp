//вычисления от a до b с шагом d
//17. F(x) = sin(x)*tg(x+5)

#include "MyLibrary.h"

typedef unsigned long long int ull;
typedef unsigned long int ul;

void defaultFunction()
{
	using namespace Stream;

	double begin, end, gap;

	OutputConsole("Введите первое рассчитываемое значение");
	Input(begin);

	OutputConsole("Введите последнее рассчитываемое значение");
	Input(end);

	do
	{
		OutputConsole("Введите шаг");
		Input(gap);
	} while (gap <= 0);

	TIMER_T resSopr = Soproc(begin, end, gap);
	TIMER_T resC = C_Only(begin, end, gap);

	OutputConsole("Сопроцессор сработал за " + std::to_string((ull)resSopr) + " мс");
	OutputConsole("С сработал за " + std::to_string((ull)resC) + " мс");
}

TIMER_T Soproc(const double& start, const double& end, const double& gap)
{
	long double lastRes = 0;
	long double five = 5;

	_asm finit;

	BEGIN_TIMER

	for (double i = start; i <= end; i += gap)
	{
		_asm
		{
			fld i
			fld five
			fadd
			fsincos
			fdiv		;tan (i+5)

			fld i
			fsin		;sin (i)

			fmul		;sin * tan
			
			fstp lastRes
		}
	}

	_asm fwait;

	END_TIMER

	OutputConsole("Последний результат из сопроцессора: " + std::to_string(lastRes));
	//OutputConsole("RES SOPR: " + std::to_string((ull)RES_TIMER));

	return RES_TIMER;
}

TIMER_T C_Only(const double& start, const double& end, const double& gap)
{
	long double lastRes = 0;

	BEGIN_TIMER

	for (double i = start; i <= end; i += gap)
	{
		lastRes = sin(i) * tan(i+5);
	}

	END_TIMER

	OutputConsole("Последний результат из С: " + std::to_string(lastRes));
	//OutputConsole("RES C: " + std::to_string((ull)RES_TIMER));

	return RES_TIMER;
}

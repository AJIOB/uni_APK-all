//2. Вычисление матричного  выражения  A = B + kC, где A, B, C – матрицы, k – скалярный множитель.

#include "MyLibrary.h"

void defaultFunction()
{
	using namespace Stream;

	byte A[SIZE_ROW][SIZE_COLUMN], B[SIZE_ROW][SIZE_COLUMN], C[SIZE_ROW][SIZE_COLUMN], k;

	init(B);
	init(C);

	OutputConsole("Введите скалярный множитель");
	Input(k);

	TIMER_T resAsm = Asm(A, B, C, k);
	TIMER_T resC = C_Only(A, B, C, k);

	OutputConsole("Ассемблер сработал за " + std::to_string(resAsm) + " мс");
	OutputConsole("С сработал за " + std::to_string(resC) + " мс");
}

void init(byte A[SIZE_ROW][SIZE_COLUMN])
{
	for (int i = 0; i < SIZE_ROW; i++)
	{
		for (int j = 0; j < SIZE_COLUMN; ++j)
		{
			A[i][j] = random();
		}
	}
}

TIMER_T Asm(byte A[SIZE_ROW][SIZE_COLUMN], const byte B[SIZE_ROW][SIZE_COLUMN], const byte C[SIZE_ROW][SIZE_COLUMN], const byte& k)
{

	byte lastRes = 0;

	BEGIN_TIMER

	for (int i = 0; i < SIZE_ROW; i++)
	{
		for (int j = 0; j < SIZE_COLUMN; ++j)
		{
			_asm
			{
				;init
				mov bl, [B+SIZE_COLUMN*i+j]
				mov al, k
				mov cl, [C+SIZE_COLUMN*i+j]

				mul cl	;k*C
				add bl	;b+k*C

				mov [A+SIZE_COLUMN*i+j], al
				mov lastRes, al
			}
		}
	}

	END_TIMER

	OutputConsole("Последний результат из сопроцессора: " + std::to_string(lastRes));
	OutputConsole("RES SOPR: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}
/*
TIMER_T Asm(const double& start, const double& end, const double& gap)
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
	OutputConsole("RES SOPR: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}
*/
TIMER_T C_Only(byte A[SIZE_ROW][SIZE_COLUMN], const byte B[SIZE_ROW][SIZE_COLUMN], const byte C[SIZE_ROW][SIZE_COLUMN], const byte& k)
{
	byte lastRes = 0;

	BEGIN_TIMER

	for (int i = 0; i < SIZE_ROW; i++)
	{
		for (int j = 0; j < SIZE_COLUMN; ++j)
		{
			lastRes = A[i][j] = B[i][j] + k * C[i][j];
		}
	}

	END_TIMER

	OutputConsole("Последний результат из С: " + std::to_string((long long)lastRes));
	OutputConsole("RES C: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}
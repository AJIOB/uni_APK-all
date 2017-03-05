//2. Вычисление матричного  выражения  A = B + kC, где A, B, C – матрицы, k – скалярный множитель.

#include "MyLibrary.h"

void defaultFunction()
{
	using namespace Stream;

	byte k;
	int k1;			//tmp to input

	byte* A = new byte[SIZE_ROW*SIZE_COLUMN];
	byte* B = new byte[SIZE_ROW*SIZE_COLUMN];
	byte* C = new byte[SIZE_ROW*SIZE_COLUMN];

	init(B);
	init(C);

	OutputConsole("Введите скалярный множитель");
	Input(k1);
	k = k1;

	TIMER_T resAsm = Asm(A, B, C, k);
	TIMER_T resMMX = MMX(A, B, C, k);
	TIMER_T resC = C_Only(A, B, C, k);
	
	OutputConsole("Ассемблер сработал за " + std::to_string(resAsm) + " мс");
	OutputConsole("MMX ассемблер сработал за " + std::to_string(resMMX) + " мс");
	OutputConsole("С сработал за " + std::to_string(resC) + " мс");
}

void init(byte* A)
{
	for (long long int i = 0; i < (SIZE_ROW * SIZE_COLUMN); i++)
	{
		A[i] = random() + 5*i + 9;
	}
}

TIMER_T Asm(byte* A, const byte* B, const byte* C, const byte k)
{
	byte lastRes = 0;

	BEGIN_TIMER

	_asm
	{
		pusha

		;init
		mov ecx, SIZE_ROW*SIZE_COLUMN
		xor esi, esi

	loop1:
		mov edi, B
		mov bl, byte ptr [edi + esi]
		mov al, k
		mov edi, C
		mov dl, byte ptr [edi + esi]

		imul dl	;k*C
		add al, bl	;b+k*C

		mov edi, A
		mov byte ptr [esi + edi], al
		mov lastRes, al

		inc esi
		loop loop1

		popa
	}

	END_TIMER

	OutputConsole("Последний результат из ассемблера: " + std::to_string((long long)lastRes));
	OutputConsole("RES ASM: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}

unsigned long long MMX(byte* A, const byte* B, const byte* C, const byte k)
{
	byte lastRes = 0;

	BEGIN_TIMER
	/* TODO
	_asm
	{
		pusha

		popa
	}*/

	END_TIMER

	OutputConsole("Последний результат из MMX ассемблера: " + std::to_string((long long)lastRes));
	OutputConsole("RES MMX ASM: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}

TIMER_T C_Only(byte* A, const byte* B, const byte* C, const byte k)
{
	byte lastRes = 0;

	BEGIN_TIMER

	for (long long int i = 0; i < (SIZE_ROW * SIZE_COLUMN); i++)
	{
		lastRes = A[i] = B[i] + k * C[i];
	}

	END_TIMER

	OutputConsole("Последний результат из С: " + std::to_string((long long)lastRes));
	OutputConsole("RES C: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}
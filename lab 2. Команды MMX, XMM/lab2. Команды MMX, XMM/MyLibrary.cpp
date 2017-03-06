//2. Вычисление матричного  выражения  A = B + kC, где A, B, C – матрицы, k – скалярный множитель.

#include "MyLibrary.h"

void defaultFunction()
{
	using namespace Stream;

	word k;
	int k1;			//tmp to input

	word* A = new word[NUM_ROWS*NUM_COLUMNS];
	word* B = new word[NUM_ROWS*NUM_COLUMNS];
	word* C = new word[NUM_ROWS*NUM_COLUMNS];

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

void init(word* A)
{
	for (long long int i = 0; i < (NUM_ROWS * NUM_COLUMNS); i++)
	{
		A[i] = random() + 5*i + 9;
	}
}

TIMER_T Asm(word* A, const word* B, const word* C, const word k)
{
	word lastRes = 0;

	BEGIN_TIMER

	_asm
	{
		pusha

		;init
		mov ecx, NUM_ROWS*NUM_COLUMNS
		xor esi, esi

	loop1:
		mov edi, B
		mov bx, word ptr [edi + esi]
		mov ax, k
		mov edi, C
		mov dx, word ptr [edi + esi]

		imul dx	;k*C
		add ax, bx	;b+k*C

		mov edi, A
		mov word ptr [esi + edi], ax

		add esi, 2
		loop loop1

		popa
	}

	END_TIMER

	lastRes = A[NUM_COLUMNS * NUM_ROWS - 1];

	OutputConsole("Последний результат из ассемблера: " + std::to_string((long long)lastRes));
	OutputConsole("RES ASM: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}

unsigned long long MMX(word* A, const word* B, const word* C, const word k)
{
	word lastRes = 0;
	qword sizeOfK = sizeof(k) * 8;

	BEGIN_TIMER
	// TODO
	_asm
	{
		pusha

		xor esi, esi
		xor eax, eax
		mov ax, k

		pxor MM2, MM2	;k для операций над элементами
		;pxor MM3, MM3	;k в аблосютном значении
		movd MM3, eax

		;нужно заполнить 4 блока числами k
		mov ecx, NUM_COLUMNS
	k_loop:
		psllq MM2, sizeOfK
		paddq MM2, MM3				;k (4 раза подряд)
		loop k_loop
		
		;количество итераций цикла
		mov ecx, NUM_ROWS
	loop1:
		mov edi, B
		movq MM0, [edi + esi]	;b
		mov edi, C
		movq MM1, [esi + edi]	;c

		pmullw MM1, MM2			; * words, save only low
		paddsw MM1, MM0			; + signed words

		mov edi, A
		movq [esi + edi], MM1	;a

		add esi, 8				;ведь длина одного mmx регистра - 8 байт

		loop loop1

		emms					;возврат к обычному режиму

		popa
	}

	END_TIMER

	lastRes = A[NUM_COLUMNS * NUM_ROWS - 1];

	OutputConsole("Последний результат из MMX ассемблера: " + std::to_string((long long)lastRes));
	OutputConsole("RES MMX ASM: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}

TIMER_T C_Only(word* A, const word* B, const word* C, const word k)
{
	word lastRes = 0;

	BEGIN_TIMER

	for (long long int i = 0; i < (NUM_ROWS * NUM_COLUMNS); i++)
	{
		A[i] = B[i] + k * C[i];
	}

	END_TIMER

	lastRes = A[NUM_COLUMNS * NUM_ROWS - 1];

	OutputConsole("Последний результат из С: " + std::to_string((long long)lastRes));
	OutputConsole("RES C: " + std::to_string(RES_TIMER));

	return RES_TIMER;
}
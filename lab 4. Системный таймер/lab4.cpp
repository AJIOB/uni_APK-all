/*
+ 1) c помощью системного таймера генерирует звук заданной частоты (по вариантам);
+ 2) выводит слово состояния для каждого канала в двоичном виде;
  3) определяет коэффициент деления для каждого канала в 16-ричном виде.
  *Реализовать генератор случайных чисел от нуля до заданного с клавиатуры числа.
*/

#include <dos.h>
#include <stdio.h>

const long int maxValue = 1193180;

int isTask = 0;

int arrSize = 52;
//freq = 0 => delay 
const int music[] = {
	123, 390,
	0, 10,
	123, 450,
	93, 50,
	123, 50,
	148, 50,
	185, 190,
	0, 10,
	185, 400,
	110, 600,

	148, 450,
	93, 50,
	165, 100,
	185, 190,
	0, 10,
	185, 400,
	139, 1000,

	69, 390,
	0, 10,
	69, 400,
	148, 200,
	139, 400,
	148, 200,
	139, 400,
	93, 200,

	123, 1000
};

int taskSize = 18;
const int task[] = {
	392, 400,
	329, 400,
	329, 400,
	392, 400,
	329, 400,
	329, 400,
	392, 400,
	349, 400,
	329, 400
};

struct oneByte{
	unsigned int b0 : 1;
	unsigned int b1 : 1;
	unsigned int b2 : 1;
	unsigned int b3 : 1;
	unsigned int b4 : 1;
	unsigned int b5 : 1;
	unsigned int b6 : 1;
	unsigned int b7 : 1;
};

void playOneBeep(int f, int sleepTime)
{
	int delim;
	outp(0x43, 0xB6);
	delim = maxValue / f;
	outp(0x42, delim % 256);
	outp(0x42, delim / 256);

	//включаем динамик
	delim = inp(0x61);
	delim |= 11;
	outp(0x61, delim);

	delay(sleepTime);

	//выключаем динамик
	delim = inp(0x61);
	delim &= 0xFC;
	outp(0x61, delim);
}

void playSound()
{
	int size = isTask == 0 ? arrSize : taskSize;
	const int *arr = isTask == 0 ? music : task;
	double k = 1.5;
	int i;

	for (i = 0; i < size; ++i, ++i)
	{
		if (arr[i] == 0)
		{
			delay(k * arr[i+1]);
		}
		else
		{
			playOneBeep(arr[i], k*arr[i+1]);
		}
	}
}

void printStateWords()
{
	int i;
	int port = 0x40;
	int valuep;
	int delta = 2;
	struct oneByte* ob = (oneByte*)&valuep;

	for (i = 0; i < 3; ++i, delta *= 2, port++)
	{
		outp(0x43, 0xE0 + delta);			//11 1(not save CE - current state of counter) 0(read value of channel) number_of_channel 0
		valuep = inp(port);
		printf("Port 0x%x: %d%d%d%d%d%d%d%d\n", port, ob->b7, ob->b6, ob->b5, ob->b4, ob->b3, ob->b2, ob->b1, ob->b0);
	}
}

void printDelims()
{
	//todo
}

int main()
{
	printf("Start values:\n");
	printStateWords();
	printDelims();

	playSound();

	printf("\nEnd values:\n");
	printStateWords();
	printDelims();
	return 0;
}

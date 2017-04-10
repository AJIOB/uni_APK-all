/*
+ 1) c помощью системного таймера генерирует звук заданной частоты (по вариантам);
+ 2) выводит слово состояния для каждого канала в двоичном виде;
+ 3) определяет коэффициент деления для каждого канала в 16-ричном виде.
  *Реализовать генератор случайных чисел от нуля до заданного с клавиатуры числа.
*/

#include <dos.h>
#include <stdio.h>

const unsigned long int maxValue = 1193180;

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

const unsigned int timesToRepeate = 30000;

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
	outp(0x43, 0xB6);						//10(channel 2) 11(RW lower, than higher) 011(mode 3: with autoloading) 0 (binary)
	delim = maxValue / f;
	outp(0x42, delim % 256);
	outp(0x42, delim / 256);

	//включаем динамик
	delim = inp(0x61);
	outp(0x61, delim | 0x03);

	delay(sleepTime);

	//выключаем динамик
	delim = inp(0x61);
	outp(0x61, delim & 0xFC);
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

//channel must be 0, 1 or 2 only
int getState(int channel)
{
	outp(0x43, 0xE0 + (2 << channel));			//11(RBC mode: read channel mode) 1(not save CE - current state of counter) 0(read value of channel) number_of_channel 0
	return inp(0x40 + channel);
}

void printStateWords()
{
	int i;
	int port = 0x40;
	int valuep;
	struct oneByte* ob = (oneByte*)&valuep;

	printf("State words:\n");

	for (i = 0; i < 3; ++i, port++)
	{
		valuep = getState(i);
		printf("Port 0x%X: %d%d%d%d%d%d%d%d\n", port, ob->b7, ob->b6, ob->b5, ob->b4, ob->b3, ob->b2, ob->b1, ob->b0);
	}
}

int max(unsigned int* m)
{
	unsigned int i, max = 0;

	for (i = 0; i < timesToRepeate; ++i)
	{
		if (max < m[i])
		{
			max = m[i];
		}
	}

	return max;
}

void printDelims()
{
	unsigned int m[timesToRepeate];
	int i;
	unsigned int j;
	int state;
	int port = 0x40;
	int delim = 0;
	int speakerWasOn = 0;

	printf("Delimiters:\n");

	for (i = 0; i < 3; ++i, port++)
	{
		if (i == 2)
		{
			//включаем только канал
			delim = inp(0x61);
			speakerWasOn = delim & 0x01;
			outp(0x61, delim | 0x01);
		}

		for (j = 0; j < timesToRepeate; j++)
		{
			delim = 0;
			state = getState(i);
			outp(0x43, (state & 0x0F) + i*0x40);			//(state & 0x0F) => SC (channel number) = 0, RW (get CE) = 0; than add channel number
			state = inp(port);
			delim = inp(port);
			delim <<= 8;
			delim += state;

			m[j] = delim;
		}

		printf("Port 0x%X: %X\n", port, max(m));
	}

	//выключаем только канал, если нужно
	delim = inp(0x61);
	speakerWasOn |= 0xFFFE;									//все биты, кроме последнего
	delim &= speakerWasOn;
	outp(0x61, delim | 0x01);
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

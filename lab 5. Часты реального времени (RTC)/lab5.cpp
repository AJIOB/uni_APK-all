/*
  1) считывает и устанавливает время в часах реального времени;
  2) реализует функцию задержки с точностью в миллисекунды;
  3) реализовать функции программируемого будильника.
  Пока выполняется задержка часы реального времени должны идти (задерживается лишь выполнение текущей программы/функции).
  *Будильник может устанавливаться и переустанавливаться до его срабатывания.
  *После установки будильника можно выполнять что-либо, например, смотреть текущее время, задавать новое время и т.д.
*/

#include <dos.h>
#include <stdio.h>

const unsigned long int maxValue = 1193180;

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

void playOneBeep(int f, int sleepTime)
{
	int delim;
	outp(0x43, 0xB6);						//10(channel 2) 11(RW lower, than higher) 011(mode 3: with autoloading) 0 (binary)
	delim = maxValue / f;
	outp(0x42, delim % 0x100);
	outp(0x42, delim / 0x100);

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
	int size = taskSize;
	const int *arr = task;
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

void readRTC()
{
	//todo
}

void writeRTC()
{
	//todo
}

void AJIOB_sleep()
{
	//todo
}

void setAlarm()
{
	//todo
}

void menu()
{
	do
	{
		printf("1. Read RTC time\n");
		printf("2. Write RTC time\n");
		printf("3. Sleep\n");
		printf("4. Set alarm\n");
		printf("0. Exit\n");

		char k = getchar();
		while(getchar() != '\n');
		switch (k)
		{
		case '0':
			return;
		case '1':
			readRTC();
			break;
		case '2':
			writeRTC();
			break;
		case '3':
			AJIOB_sleep();
			break;
		case '4':
			setAlarm();
			break;
		default:
			printf("Error selecting menu\n");
		}
	}
	while (1);
}

int main()
{
	menu();
	return 0;
}

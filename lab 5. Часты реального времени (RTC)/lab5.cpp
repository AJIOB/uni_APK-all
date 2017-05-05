/*
+ 1) считывать и устанавливать время в часах реального времени;
+ 2) реализовать функцию задержки с точностью в миллисекунды;
  3) реализовать функции программируемого будильника.
+ Пока выполняется задержка часы реального времени должны идти (задерживается лишь выполнение текущей программы/функции).
  *Будильник может устанавливаться и переустанавливаться до его срабатывания.
  *После установки будильника можно выполнять что-либо, например, смотреть текущее время, задавать новое время и т.д.
*/

#include <dos.h>
#include <stdio.h>

//---------------------1----------------------
void clear()
{
	while(getchar() != '\n');
}

int BCDfrom71()
{
	int inBCD = inp(0x71);
	return (inBCD % 0x10) + (inBCD / 0x10) * 10;
}

void BCDto71(int inBCD)
{
	outp(0x71, (inBCD % 10) + (inBCD / 10) * 0x10);
}

void readRTC()
{
	outp(0x70, 00);
	int seconds = BCDfrom71();
	outp(0x70, 02);
	int minutes = BCDfrom71();
	outp(0x70, 04);
	int hours = BCDfrom71();
	//для заполнения нулями оставшихся позиций (все время вывод по 2 цифры)
	printf("Current time: %02d:%02d:%02d\n", hours, minutes, seconds);
}

int timesTryingToSetTime = 3;

void writeRTC()
{
	int hours = 0, minutes = 0, seconds = 0;

	printf("Input current hours: ");
	scanf("%d", &hours);
	printf("Input current minutes: ");
	scanf("%d", &minutes);
	printf("Input current seconds: ");
	scanf("%d", &seconds);
	clear();

	outp(0x70, 0xA);
	int i = 0;
	for(; i < timesTryingToSetTime; ++i)
	{
		//check to 0xx..xx (time not updating now)
		if (!(inp(0x71) & 0x80))
		{
			//we found 0xx..xx
			break;
		}
	}

	if (i == timesTryingToSetTime)
	{
		printf("Cannot install RTC time\n");
		return;
	}

	//turn off clock automatic update
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x80);

	readRTC();
	printf("Installing new time...\n");

	//write clock value
	outp(0x70, 00);
	BCDto71(seconds);
	outp(0x70, 02);
	BCDto71(minutes);
	outp(0x70, 04);
	BCDto71(hours);

	readRTC();

	//turn on clock automatic update
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0x7F);
}

//--------------------2----------------------

long int sleepDuration;

void decrement()
{
	sleepDuration--;
}

void interrupt (*old_RTC_handler) (...);
void interrupt  new_RTC_handler(...) { decrement(); old_RTC_handler(); }

void AJIOB_sleep()
{
	printf("Input sleep duration in milliseconds: ");
	scanf("%ld", &sleepDuration);
	clear();

	_disable(); // disable interrupts handling (cli) [Interrupt flag]
	old_RTC_handler = getvect(0x70);
	setvect(0x70, new_RTC_handler);
	_enable(); // enable interrupt handling (sti) [Interrupt flag]

	readRTC();
	printf("We are sleeping...\n");

	//unmask interruptions from RTC
	int old_A1 = inp(0xA1);
	outp(0xA1, old_A1 & 0xFE);

	//turn on periondic interruption
	outp(0x70, 0xB);
	int old_71 = inp(0x71);
	outp(0x71, old_71 | 0x40);

	//wait
	while(sleepDuration > 0);

	readRTC();

	//turn off (restore previous) periondic interruption
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & (0xBF | old_71));

	//mask (restore previous) interruptions from RTC
	outp(0xA1, inp(0xA1) | (0x01 & old_A1));

	//return interrupt handlers back
	_disable(); // disable interrupts handling (cli) [Interrupt flag]
	setvect(0x70, old_RTC_handler);
	_enable(); // enable interrupt handling (sti) [Interrupt flag]
}

//----------------------------3-----------------------------------

int old_A1 = 0;
int old_71 = 0;
int isAlarmCompleted = 1;

void alarm();

void interrupt (*old_alarm_handler) (...);
void interrupt (*test_alarm_handler) (...);
void interrupt  new_alarm_handler(...) { alarm(); old_alarm_handler(); }

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void alarm()
{
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);
	screen += 80*15 + 40;
	screen->attr = 0x24;
	screen->attr = 0x22;
	
	isAlarmCompleted = 1;

	//mask (restore previous) interruptions from RTC
	outp(0xA1, inp(0xA1) | (0x01 & old_A1));

	//turn off (restore previous) alarm interruption
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & (0xDF | old_71));
}


void setAlarm()
{
	int hours = 0, minutes = 0, seconds = 0;

	printf("Input alarm hours: ");
	scanf("%d", &hours);
	printf("Input alarm minutes: ");
	scanf("%d", &minutes);
	printf("Input alarm seconds: ");
	scanf("%d", &seconds);
	clear();

	//write alarm value
	outp(0x70, 01);
	BCDto71(seconds);
	outp(0x70, 03);
	BCDto71(minutes);
	outp(0x70, 05);
	BCDto71(hours);

	_disable(); // disable interrupts handling (cli) [Interrupt flag]
	test_alarm_handler = getvect(0x4A);
	if (test_alarm_handler != new_alarm_handler)
	{
		old_alarm_handler = test_alarm_handler;
		setvect(0x4A, new_alarm_handler);

		printf("Alarm vector is installing...\n");
	}
	else
	{
		printf("Alarm vector is reinstalling...\n");
	}
	_enable(); // enable interrupt handling (sti) [Interrupt flag]

	if (isAlarmCompleted)
	{
		//turn on alarm interruption
		outp(0x70, 0xB);
		old_71 = inp(0x71);
		outp(0x71, old_71 | 0x20);

		//unmask interruptions from RTC
		old_A1 = inp(0xA1);
		outp(0xA1, old_A1 & 0xFE);

		printf("Alarm is installing...\n");
	}
	else
	{
		printf("Alarm is reinstalling...\n");
	}

	printf("Alarm is (re-)installed\n");
}

void uninstallAlarm()
{
	printf("Uninstalling alarm vector...\n");

	_disable(); // disable interrupts handling (cli) [Interrupt flag]
	test_alarm_handler = getvect(0x4A);
	if (test_alarm_handler == new_alarm_handler)
	{
		setvect(0x4A, old_alarm_handler);
		printf("Uninstalling alarm vector is successful\n");
	}
	else
	{
		printf("Uninstalling alarm vector is not required\n");
	}
	_enable(); // enable interrupt handling (sti) [Interrupt flag]
}

//-------------------------other----------------------------------

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
		clear();
		switch (k)
		{
		case '0':
			uninstallAlarm();
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

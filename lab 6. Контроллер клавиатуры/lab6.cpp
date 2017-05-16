/*
  Программируя клавиатуру, помигать её индикаторами (алгоритм мигания произвольный).
  Выводить на экран все коды возврата в 16-ричной форме.
*/

#include <dos.h>
#include <stdio.h>

typedef unsigned char byte;
typedef int bool;

#define true 1
#define false 0

//------------------------------------------------video buffer-------------------------------

struct VIDEO
{
	byte symb;
	byte attr;
};

#define green 0x20
#define red 0x40

//because bool is emulated
bool isKeyboardFree()
{
	return (inp(0x64) & 0x02) ? false : true;
}

const int defaultAttr = green;

void printRegister(int value)
{
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);
	
	for(int i = 0; i < 2; i++)
	{
		//get required bit
		int currBits = ((value & (0x0F << i)) >> i);
		if (currBits < 10)
		{
			screen->symb = currBits + '0';
		}
		else
		{
			screen->symb = currBits - 10 + 'A';
		}
		screen->attr = defaultAttr;
		screen++;
	}
}

VIDEO goodText[] = {
	{'g', green}, {'o', green}, {'o', green}, {'d', green}
};
VIDEO badText[] = {
	{' ', red}, {'b', red}, {'a', red}, {'d', red}
};

void printGoodBad(bool isGoodText)
{
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);

	//print in right corner
	screen += 76;

	VIDEO* it = isGoodText ? goodText : badText;

	for (int i = 0; i < 4; ++i)
	{
		screen->attr = it->attr;
		screen->symb = it->symb;
		screen++;
		it++;
	}
}

//---------------------------------------music-----------------------------------------
const unsigned long int maxFreq = 1193180;

const int freqHand = 329;
const int freqFoot = 392;
const int freqAll = 500;

void playOneBeep(int f, int sleepTime)
{
	int delim;
	outp(0x43, 0xB6);						//10(channel 2) 11(RW lower, than higher) 011(mode 3: with autoloading) 0 (binary)
	delim = maxFreq / f;
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

//----------------------------------------task-----------------------------------------

const int timesToTry = 3;
const int succeedWrite = 0xFA;
const int unsucceedWrite = 0xFE;

bool writeTo60(int value, bool showStatusRegister)
{
	int i = 0;
	for (; i < timesToTry; ++i)
	{
		//write value
		outp(0x60, value);

		//check return code
		int res = inp(0x60);
		if (showStatusRegister)
		{
			printRegister(res);
		}
		if (res == succeedWrite)
		{
			return true;
		}
	}

	return false;
}

bool setIndicators(bool onCaps, bool onNum, bool onScroll, bool showStatusRegister = true)
{
	//check keyboard buffer
	int i = 0;
	for (; i < timesToTry; ++i)
	{
		if (isKeyboardFree())
		{
			break;
		}
	}
	if (i >= timesToTry)
	{
		return false;
	}

	//write manage signal
	if (!writeTo60(0xED, showStatusRegister))
	{
		return false;
	}
	//write values of indicators
	if (!writeTo60((onCaps << 2) | (onNum << 1) | onScroll, showStatusRegister))
	{
		return false;
	}

	return true;
}

bool setIndicatorsWithCheck(bool onCaps, bool onNum, bool onScroll)
{
	bool res = setIndicators(onCaps, onNum, onScroll);
	printGoodBad(res);
	return res;
}

double durationCoefficient = 1;
const double oneSleepTimeInMS = 400;

//must be from 0 to 1. 
double coefficientOn = 0.7;

//--------------------------------delay times-----------------------------
double getDelayOn()
{
	return (oneSleepTimeInMS * durationCoefficient * coefficientOn);
}

double getDelayOff()
{
	return (oneSleepTimeInMS * durationCoefficient * (1 - coefficientOn));
}

double getDelayPeriod()
{
	return (getDelayOn() + getDelayOff());
}

//--------------------------------delays-----------------------------------
void delayOn()
{
	delay(getDelayOn());
}

void delayOff()
{
	delay(getDelayOff());
}

void delayPeriod()
{
	delay(getDelayPeriod());
}

//-------------------------------mixed (audio + lights)---------------------

bool exitOnError = true;

bool foots()
{
	//on
	if (!setIndicatorsWithCheck(true, false, false) && exitOnError)
	{
		return false;
	}
	playOneBeep(freqFoot, getDelayOn());
	//little off
	if (!setIndicatorsWithCheck(false, false, false) && exitOnError)
	{
		return false;
	}
	delayOff();

	return true;
}

bool hands()
{
	//on
	if (!setIndicatorsWithCheck(false, true, false) && exitOnError)
	{
		return false;
	}
	playOneBeep(freqHand, getDelayOn());
	//little off
	if (!setIndicatorsWithCheck(false, false, false) && exitOnError)
	{
		return false;
	}
	delayOff();

	//sleep one period
	delayPeriod();

	return true;
}

bool allLights()
{
	//on
	if (!setIndicatorsWithCheck(true, true, true) && exitOnError)
	{
		return false;
	}
	playOneBeep(freqAll, getDelayOn());
	//little off
	if (!setIndicatorsWithCheck(false, false, false) && exitOnError)
	{
		return false;
	}
	delayOff();

	return true;
}

int timesToRepeat = 6;

int keyboardBlink()
{
	
	for (int i = 0; i < timesToRepeat; ++i)
	{
		if (!foots())
		{
			return 1;
		}
		if (!foots())
		{
			return 1;
		}
		if (!hands())
		{
			return 1;
		}
	}

	allLights();

	printf("This is the end\nMelody: Queen - We will rock you\nMade by AJIOB\n");
	return 0;
}

void menu()
{
	printf("Exit on error? 1 - exit, 0 - not exit\n");
	scanf("%d", &exitOnError);

	printf("Times to repeat [int]: ");
	scanf("%d", &timesToRepeat);

	printf("Duration coefficient of sound [double]: ");
	scanf("%lf", &durationCoefficient);
}

int main()
{
	menu();

	int res = keyboardBlink();
	if (res)
	{
		printf("Error. Program is closing\n");
	}
	return res;
}

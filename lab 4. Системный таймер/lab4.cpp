#include <dos.h>

const long int maxValue = 1193180;


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
	double k = 1.5;
	int i;

	for (i = 0; i < arrSize; ++i, ++i)
	{
		if (music[i] == 0)
		{
			delay(k * music[i+1]);
		}
		else
		{
			playOneBeep(music[i], k*music[i+1]);
		}
	}
}

int main()
{
	playSound();
	return 0;
}
#include <dos.h>

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void get_reg();         	//get data from registres

//IRQ0-7
void interrupt (*int8) (...);
void interrupt (*int9) (...);
void interrupt (*intA) (...);
void interrupt (*intB) (...);
void interrupt (*intC) (...);
void interrupt (*intD) (...);
void interrupt (*intE) (...);
void interrupt (*intF) (...);

//IRQ8-15
void interrupt (*int0) (...);
void interrupt (*int1) (...);
void interrupt (*int2) (...);
void interrupt (*int3) (...);
void interrupt (*int4) (...);
void interrupt (*int5) (...);
void interrupt (*int6) (...);
void interrupt (*int7) (...);


// new interrupt handlers
void interrupt  new8(...) { get_reg(); int8(); }
void interrupt  new9(...) { get_reg(); int9(); }
void interrupt  newA(...) { get_reg(); intA(); }
void interrupt  newB(...) { get_reg(); intB(); }
void interrupt  newC(...) { get_reg(); intC(); }
void interrupt  newD(...) { get_reg(); intD(); }
void interrupt  newE(...) { get_reg(); intE(); }
void interrupt  newF(...) { get_reg(); intF(); }

void interrupt  new0(...) { get_reg(); int0(); }
void interrupt  new1(...) { get_reg(); int1(); }
void interrupt  new2(...) { get_reg(); int2(); }
void interrupt  new3(...) { get_reg(); int3(); }
void interrupt  new4(...) { get_reg(); int4(); }
void interrupt  new5(...) { get_reg(); int5(); }
void interrupt  new6(...) { get_reg(); int6(); }
void interrupt  new7(...) { get_reg(); int7(); }

//return 0, if no keyboard interruption (second bit)
int checkKeyBoardInt(int val)
{
	val/=2;
	return val%2;
}

//fill in the screen
void print(int val1, int val2, int val3, int val4, int val5, int val6)
{
	static int toSwitch = 0;
	char temp;
	int i;
	int deltaAddress = (80-8);				//num of last symbols in row
	
	toSwitch = (toSwitch + checkKeyBoardInt(val2)) % 4;
	
	unsigned char defaultAttr=0x5E;
	unsigned char keyboardAttr=0x2F;
	unsigned char currAttr=toSwitch>1?defaultAttr:keyboardAttr;
	
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);
	
	
	for(i=7;i>=0;i--)        //find bits
	{
		temp=val1%2;
		val1/=2;         //fill the screen
		screen->symb=temp+'0';
		screen->attr=defaultAttr;
		screen++;
	}
	
	screen+=deltaAddress;
	
	for(i=7;i>=0;i--)        //find bits
	{
		temp=val2%2;
		val2/=2;         //fill the screen
		screen->symb=temp+'0';
		screen->attr=currAttr;
		screen++;
	}
	
	screen+=deltaAddress;
	
	for(i=7;i>=0;i--)        //find bits
	{
		temp=val3%2;
		val3/=2;         //fill the screen
		screen->symb=temp+'0';
		screen->attr=currAttr;
		screen++;
	}
	
	screen+=deltaAddress;
	
	for(i=7;i>=0;i--)        //find bits
	{
		temp=val4%2;
		val4/=2;         //fill the screen
		screen->symb=temp+'0';
		screen->attr=currAttr;
		screen++;
	}
	
	screen+=deltaAddress;
	
	for(i=7;i>=0;i--)        //find bits
	{
		temp=val5%2;
		val5/=2;         //fill the screen
		screen->symb=temp+'0';
		screen->attr=currAttr;
		screen++;
	}
	
	screen+=deltaAddress;
	
	for(i=7;i>=0;i--)        //find bits
	{
		temp=val6%2;
		val6/=2;         //fill the screen
		screen->symb=temp+'0';
		screen->attr=keyboardAttr;
		screen++;
	}
}

// get data from registers
void get_reg()
{
	int var1, var2, var3, var4, var5, var6;
	
	outp(0x20, 0x0A);//master query register
	var1 = inp(0x20);
	outp(0x20, 0x0B);//master service register
	var2 = inp(0x20);
	//outp(0x21, 0x0B);//master mask register (check)
	var3 = inp(0x21);
	
	outp(0xA0, 0x0A);//slave query register
	var4 = inp(0xA0);
	outp(0xA0, 0x0B);//slave service register
	var5 = inp(0xA0);
	//outp(0xA1, 0x0B);//slave mask register
	var6 = inp(0xA1);
	
	print(var1, var2, var3, var4, var5, var6);
}

void init()
{
	// task - 08h/60h - basic vector
	
	//IRQ0-7
	int8 = getvect(0x08); // timer
	int9 = getvect(0x09); // keyboard
	intA = getvect(0x0A);
	intB = getvect(0x0B);
	intC = getvect(0x0C);
	intD = getvect(0x0D);
	intE = getvect(0x0E);
	intF = getvect(0x0F);

	int0 = getvect(0x70);
	int1 = getvect(0x71);
	int2 = getvect(0x72);
	int3 = getvect(0x73);
	int4 = getvect(0x74);
	int5 = getvect(0x75);
	int6 = getvect(0x76);
	int7 = getvect(0x77);

	setvect(0x08, new8);
	setvect(0x09, new9);
	setvect(0x0A, newA);
	setvect(0x0B, newB);
	setvect(0x0C, newC);
	setvect(0x0D, newD);
	setvect(0x0E, newE);
	setvect(0x0F, newF);

	setvect(0x70, new0);
	setvect(0x71, new1);
	setvect(0x72, new2);
	setvect(0x73, new3);
	setvect(0x74, new4);
	setvect(0x75, new5);
	setvect(0x76, new6);
	setvect(0x77, new7);

	_disable(); // disable interrupts handling (cli)

	/* MASTER */
	outp(0x20, 0x11);	//ICW1
	outp(0x21, 0x08);	//ICW2
	outp(0x21, 0x04);	//ICW3		connecting to IRQ2 (0000 0100)
	outp(0x21, 0x01);	//ICW4
	
	/* Slave */
	outp(0xA0, 0x11);	//ICW1
	outp(0xA1, 0x60);	//ICW2
	outp(0xA1, 0x02);	//ICW3		010b (connect to IRQ2)
	outp(0xA1, 0x01);	//ICW4

	_enable(); // enable interrupt handling (sti)
}

int main()
{
	unsigned far *fp;
	init();

	FP_SEG (fp) = _psp; // segment
	FP_OFF (fp) = 0x2c; // offset
	_dos_freemem(*fp);

	_dos_keep(0,(_DS -_CS)+(_SP/16)+1);//TSR
	return 0;
}


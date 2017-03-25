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

// new interrupt handlers
void interrupt  int60(...) { get_reg(); int8(); }
void interrupt  int61(...) { get_reg(); int9(); }

//return 0, if no keyboard interruption (second bit)
int checkKeyBoardInt(int val)
{
	val/=2;
	return val%2;
}

//fill in the screen
void print(int val1, int val2, int val3, int val4, int val5, int val6)
{
	int toSwitch = 0;
	char temp;
	int i;
	int deltaAddress = (80-8);				//num of last symbols in row
	
	toSwitch = (toSwitch + checkKeyBoardInt(val2)) % 2;
	
	unsigned char defaultAttr=0x5E;
	unsigned char keyboardAttr=0x2F;
	unsigned char currAttr=toSwitch==0?defaultAttr:keyboardAttr;
	
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
	//IRQ0-7
	int8 = getvect(0x08); // timer
	int9 = getvect(0x09); // keyboard

	setvect(0x70, int60); // 70-77
	setvect(0x71, int61);

	_disable(); // disable interrupts handling (cli)

	/* MASTER */
	outp(0x20, 0x11);	//ICW1
	outp(0x21, 0x70);	//ICW2	//todo:change
	outp(0x21, 0x04);	//ICW3	//todo:change
	outp(0x21, 0x01);	//ICW4
	
	//todo: add init slave

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


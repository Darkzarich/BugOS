asm(".code16gcc\n");

void load_screen();
void load_interface_pre();
void load_interface();
void load_pic_pre();
void load_pic();
void save();
void render(char current_color, int cur_x, int cur_y);

void setPixel(int x, int y, int color);
void setSquare(int x, int y, int color, int d_x, int d_y, char mode);
unsigned char decodeColor(unsigned char color);
char select_color(char current_color);

void sleep_t(unsigned int  clock);
int getKeyCode();
void turn_off();

int start(){

	int i, j;

	load_screen();
	setSquare(0, 0, 0, 320, 200, 0); // clear screen

	load_interface_pre();
	load_interface();

	int k;
	char current_color = 0;
	int x = 167;
	int y = 28;
	char hold = 0;
	char size = 1;

	while(1)
	{
		k = getKeyCode();

		if (k == 0x10) // key Q (exit)
		{
			turn_off();
		}
		if (k == 0x18) // key O - prev color
		{
			if (current_color > 0)
				current_color--;
			else
			{
				current_color = 16;
			}
			sleep_t(20000000);
		}
		if (k == 0x19) // key P - next color
		{
			if (current_color < 16) // limit color
				current_color++;
			else
			{
				current_color = 0;
			}
			sleep_t(20000000);
		}
		if (k == 0x11) // key W - UP
		{
			if (y > 28)
			{
				y--;
			}
			sleep_t(20000000);
		}
		if (k == 0x1E) // key A - LEFT
		{
			if (x > 167) 
			{
				x--;
			}
			sleep_t(20000000);
		}
		if (k == 0x20) // key D -  RIGHT
		{
			if (x < 264) 
			{	
				x++;
			}
			sleep_t(20000000);
		}
		if (k == 0x1F) // key S - DOWN
		{
			if (y < 125) 
			{
				y++;
			}
			sleep_t(20000000);
		}
		if (k == 0x31) // key N - New image
		{
			setSquare(167, 28, 15, 98, 98, 0);
		}
		if (k == 0x39) // key Space - Pic
		{
			if (hold == 0) hold = 1;
			else hold = 0;
			sleep_t(25000000);
		}
		if (k == 0x48) // key arrow up - size up
		{
			if (size < 100)
				size++;
			sleep_t(20000000);
		}
		if (k == 0x50) // key arrow down - size down
		{
			if (size > 1)
				size--;
			sleep_t(20000000);
		}
		if (k == 0x26) // key L - load pic
		{
			load_pic_pre();
			load_pic();
		}
		if (k == 0x21) // key F - save pic
		{
			save();
		}

		if (hold == 1)
			setSquare(x, y, select_color(current_color), size, size, 1);

		render(current_color, x, y);
	};

return 0;

}

int getKeyCode()
{
	asm("xor eax, eax");
	asm("in al, 0x60");
}

void sleep_t(unsigned int clock)
{
	unsigned int  i;
	for(i = 0; i<clock; i++){}
}

char select_color(char current_color)
{
		if (current_color == 0) return 15; // white
		if (current_color == 1) return 1; // blue
		if (current_color == 2) return 4; // red
		if (current_color == 3) return 40; // light red
		if (current_color == 4) return 14; // yellow
		if (current_color == 5) return 2; // green
		if (current_color == 6) return 10; // light green
		if (current_color == 7) return 42; // orange
		if (current_color == 8) return 11; // light cyan
		if (current_color == 9) return 5; // magenta
		if (current_color == 10) return 13; // light magenta
		if (current_color == 11) return 6; // brown
		if (current_color == 12) return 3; // cyan
		if (current_color == 13) return 9; // light blue
		if (current_color == 14) return 8; // dark gray
		if (current_color == 15) return 7; // light gray
		
		return 0; // null color (black)
}

unsigned char decodeColor(unsigned char color)
{
	switch(color)
	{
		case 0xFF: return 15; // white
		case 0xFC: return 1; // blue
		case 0x01: return 4; // red
		case 0xF9: return 40; // light red
		case 0xFB: return 14; // yellow
		case 0x71: return 2; // green
		case 0xFA: return 10; // light green
		case 0x67: return 42; // orange
		case 0xFE: return 11; // light cyan
		case 0xD5: return 5; // magenta
		case 0xFD: return 13; // light magenta
		case 0x66: return 6; // brown
		case 0xE0: return 3; // cyan
		case 0xD2: return 9; // light blue
		case 0xA4: return 8; // dark gray
		case 0x07: return 7; // light gray
		default: return 0; // null color (black)
	}
}

void setPixel(int x, int y, int color){

	int pos = (y*320)+x;
	char *p = (char*) 0xA0000;
	p += pos;
	*p = (char) color;
}

void setSquare(int x, int y, int color, int d_x, int d_y, char mode)
{
	int i, j;
	for(i = 0; i<d_x; i++)
	{	
		for(j = 0; j<d_y; j++)
		{
			if (mode != 0)
			{
				if ((x+j < 265) && (y+i < 126))
				{
					setPixel(x+j, y+i, color);
				}
			}
			else				
				setPixel(x+j, y+i, color);
		}
	}
}

void load_screen(){
	unsigned char* p = (unsigned char*) 0xA0000;
	unsigned char* pic;
	asm("mov dword ptr[bp-0x8], esi");

	//int pos = (y*320)+x;
	//p += pos;

	int i, j;

	for(i = 0; i<320*200; i++)
	{
		*p = decodeColor(*pic);
		*pic = 0x0;
		if (i == 0)
		{
			*p = decodeColor(0x00);
		}
		p++;
		pic++;
		if ((i%320 == 0) && (i != 0)) // new line
		{
			//p++;
			//pic++;
		}
	}

	// Loadbar
	for(i = 0; i<139; i++)
	{
		for(j = 0; j<22; j++)
		{
			setPixel(89+i, 122+j, 4);
		}
		sleep_t(3000000);
	}
}

void load_interface_pre()
{
	asm("mov ch, 0");
	asm("mov cl, 63");  // 66560
	asm("mov dh, 1");
	asm("mov dl, 0x80");
	asm("mov bx, 0x8000");
	asm("mov ah, 2");
	asm("mov al, 128");
	asm("int 0x13");
}

void load_interface()
{
	unsigned char* p = (unsigned char*) 0xA0000;
	unsigned char* pic = (unsigned char*) 0x8000;

	int i, j;

	for(i = 0; i<320*200; i++)
	{
		*p = decodeColor(*pic);
		if (i == 0)
		{
			*p = decodeColor(0x00);
		}
		p++;
		pic++;
	}
}

void turn_off()
{
	asm("mov ax, 0x5307");
	asm("xor bx, bx");
	asm("inc bx"); // (device ID for all power-managed devices)
	asm("mov cx, 3");
	asm("int 0x15"); // Advanced Power Management v1.2 - TURN OFF SYSTEM
}

void render(char current_color, int cur_x, int cur_y)
{
	// render current color
	int i, j;
	for(i = 0; i<15; i++)
	{
		for(j = 0; j<15; j++)
		{
			setPixel(115+j, 41+i, select_color(current_color));
		}
	}
}

void load_pic_pre()
{
	asm("mov ch, 0");
	asm("mov cl, 5"); // 131072 -> 140773
	asm("mov dh, 4");
	asm("mov dl, 0x80");
	asm("mov bx, 0x8000");
	asm("mov ah, 2");
	asm("mov al, 128");
	asm("int 0x13");
}

void load_pic(){
	unsigned char* p = (unsigned char*) 0xA0000;
	unsigned char* pic = (unsigned char*) 0x8000;

	int i, j;

	for(i = 0; i<320*200; i++)
	{
		*p = *pic;
		if (i == 0)
		{
			*p = 0;
		}
		p++;
		pic++;
	}
	*p = *pic;
}

void save()
{

	asm("xor ax, ax");
    asm("int 0x13");

    asm("mov si, es");
	asm("mov ax, 0xA000");
	asm("mov es, ax");
	asm("mov bx, 0");
	asm("mov ch, 0");
	asm("mov cl, 5"); // 131072 -> 140773
	asm("mov dh, 4");
	asm("mov dl, 0x80");
	asm("mov ah, 3"); // record
	asm("mov al, 128");
	asm("int 0x13");
	asm("mov es, si");
}

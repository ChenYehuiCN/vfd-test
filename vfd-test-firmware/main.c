/* main.c */

#include <reg52.h>

#define DIGIT_COUNT 8

sbit VF_ON = P0^0;
sbit VF_OK = P0^1;
sbit VGP_ON = P0^2;

volatile unsigned char gate = 0;
volatile unsigned char display_buffer[DIGIT_COUNT] = {
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

code unsigned short font_table[95] = {
	0x0000, // space
	0x0060, // !
	0x0840, // "
	0x4873, // #
	0x48B7, // $
	0xFC27, // %
	0x8C9A, // &
	0x0800, // '
	0x9000, // (
	0x2400, // )
	0xFC03, // *
	0x4803, // +
	0x2000, // ,
	0x0003, // -
	0x0000, // . 
	0x3000, // /
	0x30FC, // 0
	0x1060, // 1
	0x00DB, // 2
	0x00F1, // 3
	0x0067, // 4
	0x8096, // 5
	0x00BF, // 6
	0x00E0, // 7
	0x00FF, // 8
	0x00F7, // 9
	0x4800, // :
	0x2800, // ;
	0x9002, // <
	0x0013, // =
	0x2401, // >
	0x40C1, // ?
	0x08DD, // @
	0x00EF, // A
	0x48F1, // B
	0x009C, // C
	0x48F0, // D
	0x009E, // E
	0x008E, // F
	0x00BD, // G
	0x006F, // H
	0x4890, // I
	0x0078, // J
	0x900E, // K
	0x001C, // L
	0x146C, // M
	0x846C, // N
	0x00FC, // O
	0x00CF, // P
	0x80FC, // Q
	0x80CF, // R
	0x00B7, // S
	0x4880, // T
	0x007C, // U
	0x300C, // V
	0xA06C, // W
	0xB400, // X
	0x0077, // Y
	0x3090, // Z
	0x009C, // [
	0x8400, // backslash
	0x00F0, // ]
	0xA000, // ^
	0x0010, // _
	0x0400, // `
	0x401A, // a
	0x801E, // b
	0x001B, // c
	0x2071, // d
	0x201A, // e
	0x5003, // f
	0x1071, // g
	0x400E, // h
	0x4000, // i
	0x2808, // j
	0xD800, // k
	0x000C, // l
	0x402B, // m
	0x400A, // n
	0x003B, // o
	0x040E, // p
	0x1061, // q
	0x000A, // r
	0x8011, // s
	0x001E, // t
	0x0038, // u
	0x2008, // v
	0xA028, // w
	0xB400, // x
	0x0871, // y
	0x2012, // z
	0x2492, // {
	0x4800, // |
	0x9091, // }
	0x3003, // ~
};

void timer1_init(void)
{
	TMOD &= 0x0F;
	TMOD |= 0x20;
	TH1 = 0x00;
	TL1 = 0x00;
	ET1 = 1;
	EA  = 1;
	TR1 = 1;
}

void timer1_isr(void) interrupt 3
{
	if (VF_OK) {
		unsigned char ch = display_buffer[gate];
		unsigned short display;
		if ((ch < ' ') || (ch > '~'))
			display = 0x0000;
		else
			display = font_table[ch - ' '];
		VGP_ON = 1;
		P1 = 0x00;
		P3 = (P3 & 0x03) | (unsigned char)(display >> 8);
		P2 = (char)display;
		P1 = (unsigned char)0x01 << gate;
		gate = (gate + 1) % DIGIT_COUNT;
	} else
		VGP_ON = 0;
}

void main(void)
{
	P0 = 0x00;
	P1 = 0x00;
	P2 = 0x00;
	P3 = 0x00;
	VF_OK = 1;
	timer1_init();
	VF_ON = 1;
	while (1);
}

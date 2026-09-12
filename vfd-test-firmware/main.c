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

/*
 * Segment wiring used by this table:
 *
 *   A=a=P4, B=b=P3, C=c=P14, D=d=P5, E=e=P6, F=f=P11
 *   G1=g(left)=P12, G2=g(right)=P13
 *   H=upper-left diagonal=P10, J=upper vertical=P1
 *   K=upper-right diagonal=P2, L=lower-left diagonal=P7
 *   M=lower vertical=P8, N=lower-right diagonal=P9
 *
 * The table index is ASCII - ' '.  Bit 0 of each entry is P8/P2.0,
 * bit 7 is P1/P2.7, and bits 10..15 are P9..P14 on P3.2..P3.7.
 */
code unsigned short font_table[95] = {
	0x0000, // space
	0x8020, // !
	0x00A0, // "
	0xE0A9, // #
	0xF099, // $
	0xFCC3, // %
	0x2C9C, // &
	0x0080, // '
	0x0440, // (
	0x0802, // )
	0x6CC3, // *
	0x6081, // +
	0x0002, // ,
	0x6000, // -
	0x0000, // . (no decimal-point output is wired)
	0x0042, // /
	0x907E, // 0
	0x8060, // 1
	0x603C, // 2
	0xC038, // 3
	0xF020, // 4
	0x3418, // 5
	0xF01C, // 6
	0x8030, // 7
	0xF03C, // 8
	0xF038, // 9
	0x0081, // :
	0x0082, // ;
	0x2440, // <
	0x6008, // =
	0x4802, // >
	0x4031, // ?
	0x50BC, // @
	0xF034, // A
	0xC0B9, // B
	0x101C, // C
	0x80B9, // D
	0x301C, // E
	0x3014, // F
	0xD01C, // G
	0xF024, // H
	0x0099, // I
	0x802C, // J
	0x3444, // K
	0x100C, // L
	0x9864, // M
	0x9C24, // N
	0x903C, // O
	0x7034, // P
	0x943C, // Q
	0x7434, // R
	0xF018, // S
	0x0091, // T
	0x902C, // U
	0x1046, // V
	0x9426, // W
	0x0C42, // X
	0xF028, // Y
	0x005A, // Z
	0x101C, // [
	0x0C00, // backslash
	0x8038, // ]
	0x0402, // ^
	0x0008, // _
	0x0800, // `
	0x200D, // a
	0x340C, // b
	0x600C, // c
	0xC02A, // d
	0x200E, // e
	0x6041, // f
	0xC068, // g
	0x3005, // h
	0x0001, // i
	0x0086, // j
	0x04C1, // k
	0x1004, // l
	0xE005, // m
	0x2005, // n
	0xE00C, // o
	0x3804, // p
	0xC060, // q
	0x2004, // r
	0x4408, // s
	0x300C, // t
	0x800C, // u
	0x0006, // v
	0x8406, // w
	0x0C42, // x
	0xC0A8, // y
	0x200A, // z
	0x281A, // {
	0x0081, // |
	0x4458, // }
	0x6042, // ~
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

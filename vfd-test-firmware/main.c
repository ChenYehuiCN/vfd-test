/* main.c */

#include <reg52.h>

#define DIGIT_COUNT 8

sbit VF_ON = P0^0;
sbit VF_OK = P0^1;
sbit VGP_ON = P0^2;

volatile unsigned char gate = 0;
unsigned char display_buffer[DIGIT_COUNT] = {0};
code unsigned short font_table[95] = {
	0x0000, // space
	0x0000, // ! 
	0x0000, // " 
	0x0000, // # 
	0x0000, // $ 
	0x0000, // % 
	0x0000, // & 
	0x0000, // ' 
	0x0000, // ( 
	0x0000, // ) 
	0x0000, // * 
	0x0000, // + 
	0x0000, // , 
	0x0000, // - 
	0x0000, // . 
	0x0000, // slash
	0x0000, // 0 
	0x0000, // 1 
	0x0000, // 2 
	0x0000, // 3 
	0x0000, // 4 
	0x0000, // 5 
	0x0000, // 6 
	0x0000, // 7 
	0x0000, // 8 
	0x0000, // 9 
	0x0000, // : 
	0x0000, // ; 
	0x0000, // < 
	0x0000, // = 
	0x0000, // > 
	0x0000, // ? 
	0x0000, // @ 
	0x0000, // A 
	0x0000, // B 
	0x0000, // C 
	0x0000, // D 
	0x0000, // E 
	0x0000, // F 
	0x0000, // G 
	0x0000, // H 
	0x0000, // I 
	0x0000, // J 
	0x0000, // K 
	0x0000, // L 
	0x0000, // M 
	0x0000, // N 
	0x0000, // O 
	0x0000, // P 
	0x0000, // Q 
	0x0000, // R 
	0x0000, // S 
	0x0000, // T 
	0x0000, // U 
	0x0000, // V 
	0x0000, // W 
	0x0000, // X 
	0x0000, // Y 
	0x0000, // Z 
	0x0000, // [ 
	0x0000, // backslash
	0x0000, // ] 
	0x0000, // ^ 
	0x0000, // _ 
	0x0000, // ` 
	0x0000, // a 
	0x0000, // b 
	0x0000, // c 
	0x0000, // d 
	0x0000, // e 
	0x0000, // f 
	0x0000, // g 
	0x0000, // h 
	0x0000, // i 
	0x0000, // j 
	0x0000, // k 
	0x0000, // l 
	0x0000, // m 
	0x0000, // n 
	0x0000, // o 
	0x0000, // p 
	0x0000, // q 
	0x0000, // r 
	0x0000, // s 
	0x0000, // t 
	0x0000, // u 
	0x0000, // v 
	0x0000, // w 
	0x0000, // x 
	0x0000, // y 
	0x0000, // z 
	0x0000, // { 
	0x0000, // | 
	0x0000, // } 
	0x0000, // ~ 
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
		short display = font_table[display_buffer[gate] - ' '];
		VGP_ON = 1;
		P1 = 0x00;
		P3 = (char)(display >> 8);
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

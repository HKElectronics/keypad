/*
 * Keys - real.c
 *
 * Created: 2018-07-03 9:35:11 AM
 * Author : hinza
 */ 
#define DEBUG


#include <avr/io.h>
#include <util/delay.h>
//#include <avr/macros.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h> 

char scanKey();
char getCase(int column, int row);

/**********************************************************
*	Circuit connections:
*	
***NOT USED********
*	I2C EEPROM:
*	PC4 - SDA
*	PC5 - SCL
****************
*	RELAY CONTROL
*	Relay 1 - PC0
*	Relay 2 - PC1
*	Relay 3 - PC2
*	Relay 4 - PC3
*
*   KEYPAD
*
*       COL1 ->  PD0
*       COL2 ->  PD1
*       COL3 ->  PD2
*
*       ROW1 ->  PD3
*       ROW2 ->  PD4
*       ROW3 ->  PD5
*       ROW4 ->  PD6
*/
/*******************************************
*/
/************************************************************************/
/* Setup and initilization function										*/
/* DDR(X) = Data direction registers for port (X)						*/
/* I = Input, O = Output		set in binary, MSB X X X X X X X X LSB	*/
/************************************************************************/
void setup();
{
int temp = 0x00;
char key = 'X';
int flag = 0x00;
int strIndex = 0;
string codes[6][10];
string input[10];
codes[0] = '*7623782*'; // master set code
codes[1] = '*2873267*'; // master remove code
DDRA = 0x00;		// Set port A as inputs
DDRB = 0x00;		// Set port B as inputs
DDRC = 0x0F;		// Set port C as IIIIOOOO
DDRD = 0x07;		// Set port D as IIIIIOOO
}
/*master add keycode *7623782*# status light number # keycode #
  master remove keycode *2873267*# status light number #
*/
void loop()
	{
	key = 'M';
	flag = 0x00;
//setup keypad data direction register
	DDRD = 0x07;	
	strIndex=0;
		
			key = scanKey();
			if (key != 'Q' && key != 'M' && key != 'Z' && key != '#')
			{
				input[strIndex]=key;
				strIndex++;
				//write key to string
			}
			else if (key == '#')
				//test string
				if (strcmp(codes[0],input))
					{
						//call program function
					}
				else if (strcmp(codes[1],input))
					{
						//call delete function
					}
				else if (strcmp(codes[2],input))
					{
						//toggle relay 1
					}
				else if (strcmp(codes[3],input))
					{
						//toggle relay 2
					}
				else if (strcmp(codes[4],input))
					{
						//toggle relay 3
					}
				else if (strcmp(codes[5],input))
					{
						//toggle relay 4
					}
				else
					{
						//clear input
					}
			}



	}


char scanKey()
{
	for (int j=0x0; j<0x3; j++)//set column - test row
		{
			//set column high
			PORTD = j;
			for (int i=0x00; i<0x04; i++) //check for row
			{
				temp = scankey(i);
				if (temp != 0x00)
				{
					KeyPressed = getCase(j,i); //send position for decoding
					return (KeyPressed); //return pointer to array with column and row
				}
			}
		}
	}

/* Keypad pattern r c
// 00 01 02		1 2 3
// 10 11 12		4 5 6
// 20 21 22		7 8 9
// 30 31 32		* 0 #
*/
char getCase(int column, int row)
	{
		char key = 'Q';

		if (column = 0)
		{
			switch(row)
				 {
					case 0x00 :
					key = '1';
					break;
					case 0x01 :
					key = '4';
					break;
					case 0x02 :
					key = '7';
					break;
					case 0x03 :
					key = '*';
					break;
				} 
		}	
		if (column = 1)
		{
			switch(row)
				 {
					case 0x00 :
					key = '2';
					break;
					case 0x01 :
					key = '5';
					break;
					case 0x02 :
					key = '8';
					break;
					case 0x03 :
					key = '0';
					break;
				} 
		}
		if (column = 2)
		{
			switch(row)
				 {
					case 0x00 :
					key = '3';
					break;
					case 0x01 :
					key = '6';
					break;
					case 0x02 :
					key = '9';
					break;
					case 0x03 :
					key = '#';
					break;
				} 
		}
		return key;
	}
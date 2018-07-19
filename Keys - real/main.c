/*
 * Keys - real.c
 *
 * Created: 2018-07-03 9:35:11 AM
 * Author : hinza & karrasr
 */ 

/* All status are to be latched in ***PLC*** not through variables */

#include <avr/io.h>
#include <util/delay.h>
//#include <avr/macros.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h> 

char scanKey();
char transposeKey(int column, int row);
void setRelay(int relayNumber, int relayState);//set status on relay (1 on, 0 off)
void resetOutputs ();//set all relay states to 0
void errorOutput ();
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
//Global Variables
char key;
int flag;
int stringIndex;
char codes[7][10];
char input[10];

void setup(void)
{
key = 'X';
flag = 0x00;
stringIndex = 0;
codes[0][0] = codes[0][8] = (char) "*"; codes[0][1] = (char) "7"; codes[0][2] = (char) "6"; codes[0][3] = (char) "2"; codes[0][4] = (char) "3"; codes[0][5] = (char) "7"; codes[0][6] = (char) "8"; codes[0][7] = (char) "2";// master set code
codes[5][0] = codes[5][8] = (char) "*"; codes[5][1] = (char) "2"; codes[5][2] = (char) "8"; codes[5][3] = (char) "7"; codes[5][4] = (char) "3"; codes[5][5] = (char) "2"; codes[5][6] = (char) "6"; codes[5][7] = (char) "7"; // master remove code
codes[6][0] = codes[6][8] = (char) "*"; codes[6][1] = (char) "4"; codes[6][2] = (char) "*"; codes[6][3] = (char) "2"; codes[6][4] = (char) "6"; codes[6][5] = (char) "3"; codes[6][6] = (char) "*"; codes[6][7] = (char) "5"; // reset all outputs - does not wipe codes from eeprom
DDRA = 0x00;		// Set port A as inputs
DDRB = 0x00;		// Set port B as inputs
DDRC = 0x0F;		// Set port C as IIIIOOOO
DDRD = 0x07;		// Set port D as IIIIIOOO
}
/*master add keycode *7623782*# relay output # keycode #
  master remove keycode *2873267*# relay output #
*/
void loop(void)
{
	key = 'M';
	flag = 0x00;
//setup keypad data direction register
	DDRD = 0x07;	
	stringIndex=0;
		
	key = scanKey();
	if (key != 'Q' && key != 'M' && key != 'Z' && key != '#')
		{
			input[stringIndex]=key;
			stringIndex++;
			//write key to string
		}
	else if (key == '#')
		{
			//test string
			if ( strcmp(codes[0],input) )
				{
					//call program function
				}
			else if ( strcmp(codes[5],input) )
				{
					//call delete function
				}
			else if ( strcmp(codes[1],input) )
				{
					setRelay(1,1);
				}
			else if ( strcmp(codes[2],input) )
				{
					setRelay(2,1);
				}
			else if ( strcmp(codes[3],input) )
				{
					setRelay(3,1);
				}
			else if ( strcmp(codes[4],input) )
				{
					setRelay(4,1);
				}
			else if ( strcmp(codes[6],input) )
				{
					//reset locks
				}
			else
				{
					//clear string & flash error
					errorOutput();
						
					for (int i=0 ; i<10 ; i++)
					{
						input[i]='r';
					}
				}
			}
			if (stringIndex > 9)
				{
					//string too long error
					errorOutput();
					for (int i=0 ; i<10 ; i++)
						{
							input[i]='r';
						}
				}


	}


char scanKey()
{
	int debounce = 50; //debounce time in ms
	int temp = 0x00;
	char KeyPressed = "L";
	
	for (int j=0x0; j<0x3; j++)//set column - test row
		{
			//set column high
			PORTD = j;
			for (int i=0x00; i<0x04; i++) //check for row
			{
				temp = scankey(i);
				if (temp != 0x00)
				{
					KeyPressed = transposeKey(j,i); //send position for decoding
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
char transposeKey(int column, int row)
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

void setRelay(int relayNumber, int relayState)
{


}

void resetOutputs ()//set all relay states to 0
{
	for (int i = 1; i < 5; i++)
	{
		setRelay(i,0);
	}
}

void programCode (char codeToSave[10], int relayNumber, string* codes[][]);
//usage: programCode(input, code#, *codes)
{
	if (relayNumber > 0 && relayNumber < 5)
	{
		strcpy(codes,codeToSave[relayNumber]);
	}
	else
	errorOutput();
}

//usage removeCode(code#,*codes)
void removeCode (int relayNumber, string* codes[])
{
	char wipeString = '#';
	if (relayNumber > 0 && relayNumber <5)
	{
		for (int i=0;i<10;i++)
		{
			codes[relayNumber][i]=wipeString;
		}
	}
	else
	errorOutput();
}

void errorOutput ()//toggle relays - connected to lights & plc on and off 5 times
{
	int counter = 5;
	while (counter > 0)
	{
		for (int i=1; i<5; i++)
		{
			setRelay(i,1);
		}
		_delay_ms(500);
		resetOutputs();
		_delay_ms(500);
	}
}

  /************************************************************************/
  /* eeprom_read_block                          */
  /* reads pointer to variable containing information to write to EEPROM  */
  /* in read function, RAM_to_write_into is where the copy of the EEPROM  */
  /* will be stored                            */
  /* EEPROMAddress_of_first_byte is the address of the first byte of the  */
  /* block you will be writing into the EEPROM              */
  /************************************************************************/
  void read_codes(void)
  {
	  eeprom_read_block((void*)&codes, (char void*)EEPROMaDDRESSoFfIRSTbYTE, 70);
  }
  
  void write_codes(void)
  {
	  eeprom_update_block((void*)&codes, (char void*)EEPROMaDDRESSoFfIRSTbYTE, 70);
  }
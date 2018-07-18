/* Standard Macros */
/* You can totally get by without these, but why? */

/* Make sure we've already got io / sfr / pindefs loaded */
#ifndef   _AVR_IO_H_
#include  <avr/io.h>
#endif

/* Reminder: the following useful bit-twiddling macros are
   always included in avr/sfr_defs.h, which is called from
   avr/io.h 
 bit_is_set(sfr, bit)
 bit_is_clear(sfr, bit)
 loop_until_bit_is_set(sfr, bit)
 loop_until_bit_is_clear(sfr, bit)
*/
/************************************************************************/
/*		PORT is defined from the IO.h file for the chip being used.      */
/* USAGE: PORTX where X is the port to access							*/
/* example: PORTA														*/
/*			PORTB														*/
/************************************************************************/

/* Define up the full complement of bit-twiddling macros */
#define BV(bit)               (1 << bit)						// Bit test for (BIT) return: True/False
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))		// Set (BIT) to TRUE/1/HIGH for (PORT)
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit))		// Clear (BIT) (change to FALSE/0/LOW for (PORT)
#define toggle_bit(sfr, bit) (_SFR_BYTE(sfr) ^= BV(bit))		// Toggle (BIT) on (PORT) from 1 to 0 or 0 to 1
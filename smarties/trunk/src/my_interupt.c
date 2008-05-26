#include "smarties2.h"
#include "avr/interrupt.h"

extern smartie_sorter ss;

ISR (TIMER0_OVF_vect)
{
	// user input (rotary encoder) stuff
	if (IS_ROTENC_PUSH)
		ss.rotenc.pushtmp = ROTENC_PUSH;
	else if (ss.rotenc.pushtmp == ROTENC_PUSH)
	{
		ss.rotenc.push++;
		ss.rotenc.pushtmp = ROTENC_NONE;
	}

	if (IS_ROTENC_A)
	{
		if (ss.rotenc.rottmp == ROTENC_B)
		{
			ss.rotenc.left++;
			ss.rotenc.rottmp = ROTENC_A;	//store current position
		}
		else if (ss.rotenc.rottmp == ROTENC_BOTH)
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_A;	//store current position
		}
	}
	else if (IS_ROTENC_B)
	{
		if (ss.rotenc.rottmp == ROTENC_BOTH)
		{
			ss.rotenc.left++;
			ss.rotenc.rottmp = ROTENC_B;	//store current position
		}
		else if (ss.rotenc.rottmp == ROTENC_A)
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_B;	//store current position
		} 
	}
	else if (IS_ROTENC_BOTH)
	{
		if (ss.rotenc.rottmp == ROTENC_NONE)
		{
			ss.rotenc.left++;
			ss.rotenc.rottmp = ROTENC_BOTH;	//store current position
		}
		else if (ss.rotenc.rottmp == ROTENC_B)
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_BOTH;	//store current position
		}
	}
	else if (IS_ROTENC_NONE)
	{
		if (ss.rotenc.rottmp == ROTENC_A)
		{
			ss.rotenc.left++;
			ss.rotenc.rottmp = ROTENC_NONE;	//store current position
		}
		else if (ss.rotenc.rottmp == ROTENC_BOTH)
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_NONE;	//store current position
		}
	}
	
	// display stuff
	// sensor stuff
	// engine stuff
	// shaker stuff
}

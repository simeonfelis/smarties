#include "abstraction.h"
#include "smarties2.h"



extern smartie_sorter ss;


void engine_stuff ()
{
	//      C A T C H E R
	if (ss.catcher_Engine.status == start_working) // ramp up
	{
		// prepare the ramp up
		if (ss.catcher_Engine.status_tmp == idle)
		{
			ss.catcher_Engine.status_tmp = start_working;
			ss.catcher_Engine.rampup_steps = CATCH_RAMP_DURATION;
			ss.catcher_Engine.cycle_counter = 0;
		}
		
		// do the ramp up
		ss.catcher_Engine.cycle_counter++;
		if (ss.catcher_Engine.cycle_counter
				== CATCH_STEP_DURATION * ss.catcher_Engine.rampup_steps)
		{
			CATCH_MOVE_STEP();
			ss.catcher_Engine.cycle_counter = 0;
			ss.catcher_Engine.rampup_steps--;
			if (ss.catcher_Engine.rampup_steps == 0)
			{
				ss.catcher_Engine.status = working;
				ss.catcher_Engine.status_tmp = start_working;
			}
		}
	} // ramp up end

	if (ss.catcher_Engine.status == working) // just rotate 
	{
		ss.catcher_Engine.cycle_counter++;
		if (ss.catcher_Engine.cycle_counter == CATCH_STEP_DURATION)
		{
			ss.catcher_Engine.cycle_counter = 0;
			CATCH_MOVE_STEP();
		}
	} // just rotate end

	if (ss.catcher_Engine.status == stop_working) // ramp down
	{
		// prepare ramp down
		if (ss.catcher_Engine.status_tmp == working)
		{
			ss.catcher_Engine.status_tmp = stop_working;
			ss.catcher_Engine.rampdown_steps = 1;
			ss.catcher_Engine.cycle_counter = 0;
		}
		ss.catcher_Engine.cycle_counter++;
		if (ss.catcher_Engine.cycle_counter
				== CATCH_RAMP_DURATION * ss.catcher_Engine.rampdown_steps)
		{
			CATCH_MOVE_STEP();
			ss.catcher_Engine.cycle_counter = 0;
			ss.catcher_Engine.rampdown_steps++;
			if (ss.catcher_Engine.rampdown_steps == CATCH_RAMP_DURATION)
				ss.catcher_Engine.status = finished;
		}
	} // ramp down end

	
	//     R E V O L V E R
	if (ss.revolver_Engine.status == start_working) // ramp up
	{
		//prepare ramp up
		if (ss.revolver_Engine.status_tmp == idle)
		{
			ss.revolver_Engine.status_tmp = start_working;
			ss.revolver_Engine.rampup_steps = REV_RAMP_DURATION;
			ss.revolver_Engine.cycle_counter = 0;
		}
		
		// do the ramp up
		REV_MOVE_STEP();
		ss.revolver_Engine.cycle_counter++;
		if (ss.revolver_Engine.cycle_counter == 
			REV_STEP_DURATION * ss.revolver_Engine.rampup_steps )
		{
			ss.revolver_Engine.cycle_counter = 0;
			ss.revolver_Engine.rampup_steps--;
			if (ss.revolver_Engine.rampdown_steps == 0)
			{
				ss.revolver_Engine.status = working;
				ss.revolver_Engine.status_tmp = start_working;
			}
		}
	}
	if (ss.revolver_Engine.status == working) //just rotate
	{
		if (ss.revolver_Engine.status_tmp == start_working)
		{
			ss.revolver_Engine.status_tmp = working;
			ss.revolver_Engine.cycle_counter = 0;
		}
		
		ss.revolver_Engine.cycle_counter++;
		if (ss.revolver_Engine.cycle_counter == REV_STEP_DURATION)
		{
			REV_MOVE_STEP();
			ss.revolver_Engine.cycle_counter = 0;
		}
	}
}


void rotary_encoder_stuff ()
{
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
		else if (ss.rotenc.rottmp == ROTENC_NONE) // undecidable, choose myself
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_A;
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
		else if (ss.rotenc.rottmp == ROTENC_B) // undecidable, choose myself
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_B;
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
		else if (ss.rotenc.rottmp == ROTENC_A) // undecidable, choose myself
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_BOTH;
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
		else if (ss.rotenc.rottmp == ROTENC_B) // undecidable, choose myself
		{
			ss.rotenc.right++;
			ss.rotenc.rottmp = ROTENC_NONE;
		}
	}

}


void lightbarrier_stuff ()
{
	if (IS_LB_CATCHER)
	{
		ss.catcher_LB.status = lb_blocked;
		ss.catcher_LB.status_tmp = lb_blocked; 
	}
	else
	{
		ss.catcher_LB.status = lb_free;
		if (ss.catcher_LB.status_tmp == lb_blocked)
		{
			ss.catcher_LB.status_tmp = lb_free;
			ss.catcher_LB.passes++;
		}
	}
	
	if (IS_LB_REVOLVER)
	{
		ss.revolver_LB.status = lb_blocked;
		ss.revolver_LB.status_tmp = lb_blocked;
	}
	else
	{
		ss.revolver_LB.status = lb_free;
		if (ss.revolver_LB.status_tmp == lb_blocked)
		{
			ss.catcher_LB.status_tmp = lb_free;
			ss.catcher_LB.passes++;
		}
	}
}

void sensor_stuff ()
{
	
}


void shaker_stuff ()
{
	if (ss.shkr.statustmp == idle)
		if (ss.shkr.status == working)
		{
			ss.shkr.statustmp = working;
			ss.shkr.duration = 500; 		//500 ms
		}
	if (ss.shkr.status == working)
	{
		ss.shkr.duration--;
		if (ss.shkr.duration == 0)
		{
			ss.shkr.status = finished;
			ss.shkr.statustmp = working;
		}
	}
}
void display_stuff ()
{
	
}
void disp_set_mode ()
{
	
}

void disp_set_title ()
{
	
}

void disp_set_color ()
{
	
}

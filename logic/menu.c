// *************************************************************************************************
//
//	Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/ 
//	 
//	 
//	  Redistribution and use in source and binary forms, with or without 
//	  modification, are permitted provided that the following conditions 
//	  are met:
//	
//	    Redistributions of source code must retain the above copyright 
//	    notice, this list of conditions and the following disclaimer.
//	 
//	    Redistributions in binary form must reproduce the above copyright
//	    notice, this list of conditions and the following disclaimer in the 
//	    documentation and/or other materials provided with the   
//	    distribution.
//	 
//	    Neither the name of Texas Instruments Incorporated nor the names of
//	    its contributors may be used to endorse or promote products derived
//	    from this software without specific prior written permission.
//	
//	  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//	  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//	  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//	  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//	  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//	  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//	  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//	  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//	  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//	  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//	  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************
// Menu management functions.
// *************************************************************************************************


// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "display.h"

// logic
#include "menu.h"
#include "user.h"
#include "clock.h"
#include "date.h"
#include "alarm.h"
#include "stopwatch.h"
#include "battery.h"

#include "rfsimpliciti.h"

#include "rfbsl.h"

#ifdef CONFIG_EGGTIMER
#include "eggtimer.h"
#endif

#ifdef CONFIG_OTP
#include "otp.h"
#endif

// *************************************************************************************************
// Defines section
#define FUNCTION(function)  function


// *************************************************************************************************
// Global Variable section
const struct menu * ptrMenu_L1 = NULL;
const struct menu * ptrMenu_L2 = NULL;

// *************************************************************************************************
// Extern section
extern void menu_skip_next(line_t line); //ezchronos.c


void display_nothing(u8 line, u8 update) {}

u8 update_time(void)
{
	return (display.flag.update_time);
}
#ifdef CONFIG_STOP_WATCH
u8 update_stopwatch(void)
{
	return (display.flag.update_stopwatch);
}
#endif
u8 update_date(void)
{
	return (display.flag.update_date);
}
#ifdef CONFIG_ALARM
u8 update_alarm(void)
{
	return (display.flag.update_alarm);
}
#endif
u8 update_temperature(void)
{
	return (display.flag.update_temperature);
}
#ifdef CONFIG_BATTERY
u8 update_battery_voltage(void)
{
	return (display.flag.update_battery_voltage);
}
#endif
u8 update_acceleration(void)
{
	return (display.flag.update_acceleration);
}
#ifdef CONFIG_EGGTIMER
u8 update_eggtimer(void)
{
	return (display.flag.update_stopwatch);
}
#endif


// *************************************************************************************************
// User navigation ( [____] = default menu item after reset )
//
//	LINE1: 	[Time] -> Alarm -> Temperature -> Altitude -> Heart rate -> Speed -> Acceleration
//
//	LINE2: 	[Date] -> Stopwatch -> Battery  -> ACC -> PPT -> SYNC -> Calories/Distance --> RFBSL
// *************************************************************************************************

// Line1 - Time
const struct menu menu_L1_Time =
{
	FUNCTION(sx_time),			// direct function
	FUNCTION(mx_time),			// sub menu function
	FUNCTION(menu_skip_next),	// next item function
	FUNCTION(display_time),		// display function
	FUNCTION(update_time),		// new display data
};

// Line1 - Alarm
#ifdef CONFIG_ALARM
const struct menu menu_L1_Alarm =
{
	FUNCTION(sx_alarm),			// direct function
	FUNCTION(mx_alarm),			// sub menu function
	FUNCTION(menu_skip_next),	// next item function
	FUNCTION(display_alarm),	// display function
	FUNCTION(update_alarm),		// new display data
};
#endif

// Line2 - Date
const struct menu menu_L2_Date =
{
	FUNCTION(sx_date),			// direct function
	FUNCTION(mx_date),			// sub menu function
	FUNCTION(menu_skip_next),	// next item function
	FUNCTION(display_date),		// display function
	FUNCTION(update_date),		// new display data
};

// Line2 - Stopwatch
#ifdef CONFIG_STOP_WATCH
const struct menu menu_L2_Stopwatch =
{
	FUNCTION(sx_stopwatch),		// direct function
	FUNCTION(menu_skip_next),	// next item function
	FUNCTION(mx_stopwatch),		// sub menu function
	FUNCTION(display_stopwatch),// display function
	FUNCTION(update_stopwatch),	// new display data
};
#endif

#ifdef CONFIG_EGGTIMER
//  Line2 - Eggtimer (Counts down from set time)
const struct menu menu_L2_Eggtimer =
{
        FUNCTION(sx_eggtimer),          // direct function
        FUNCTION(mx_eggtimer),          // sub menu function
        FUNCTION(menu_skip_next),	// next item function
        FUNCTION(display_eggtimer),// display function
        FUNCTION(update_eggtimer),      // new display data
};
#endif

#ifdef CONFIG_OTP
//  Line2 - OTP (Google OTP -- Authenticator)
const struct menu menu_L2_Otp =
{
        FUNCTION(sx_otp),          	// direct function
        FUNCTION(dummy),          	// sub menu function
        FUNCTION(menu_skip_next),	// next item function
        FUNCTION(display_otp),		// display function
        FUNCTION(update_otp),      	// new display data
};
#endif

// Line2 - Battery 
#ifdef CONFIG_BATTERY
const struct menu menu_L2_Battery =
{
	FUNCTION(dummy),					// direct function
	FUNCTION(sx_rfbsl),					//sub function calls RFBSL
	FUNCTION(menu_skip_next),			// next item function
	FUNCTION(display_battery_V),		// display function
	FUNCTION(update_battery_voltage),	// new display data
};
#endif

#ifdef CONFIG_USEPPT
// Line2 - PPT (button events via SimpliciTI)
const struct menu menu_L2_Ppt =
{
	FUNCTION(sx_ppt),				// direct function
	FUNCTION(dummy),				// sub menu function
	FUNCTION(menu_skip_next),		// next item function
	FUNCTION(display_ppt),			// display function
	FUNCTION(update_time),			// new display data
};
#endif

// Line2 - SXNC (synchronization/data download via SimpliciTI)
const struct menu menu_L2_Sync =
{
	FUNCTION(sx_sync),				// direct function
	FUNCTION(dummy),				// sub menu function
	FUNCTION(menu_skip_next),		// next item function
	FUNCTION(display_sync),			// display function
	FUNCTION(update_time),			// new display data
};

// Line2 - RFBSL
const struct menu menu_L2_RFBSL =
{
	FUNCTION(sx_rfbsl),				// direct function
	FUNCTION(mx_rfbsl),				// sub menu function
	FUNCTION(nx_rfbsl),				// next item function
	FUNCTION(display_rfbsl),		// display function
	FUNCTION(update_time),			// new display data
};



// *************************************************************************************************
// menu array

const struct menu *menu_L1[]={
	&menu_L1_Time,
	#ifdef CONFIG_ALARM
	&menu_L1_Alarm,
	#endif
};

const int menu_L1_size=sizeof(menu_L1)/sizeof(struct menu*);
int menu_L1_position=0;

const struct menu *menu_L2[]={
	#ifdef CONFIG_DATE
	&menu_L2_Date,
	#endif
	#ifdef CONFIG_STOP_WATCH
	&menu_L2_Stopwatch,
	#endif
	#ifdef CONFIG_EGGTIMER
	&menu_L2_Eggtimer,
	#endif
	#ifdef CONFIG_BATTERY
	&menu_L2_Battery,
	#endif
	#ifdef CONFIG_USEPPT
	&menu_L2_Ppt,
	#endif
	&menu_L2_Sync,
	&menu_L2_RFBSL,
	#ifdef CONFIG_OTP
	&menu_L2_Otp,
	#endif
};

const int menu_L2_size=sizeof(menu_L2)/sizeof(struct menu*);
int menu_L2_position=0;

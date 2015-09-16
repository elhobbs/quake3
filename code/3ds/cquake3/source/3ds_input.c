/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#include "../client/client.h"
#include <3ds.h>
#include <3ds/os.h>

static int buttonMap3ds[] = {
	/* 0*/'a',
	/* 1*/'b',
	/* 2*/K_ESCAPE,
	/* 3*/K_ENTER,
	/* 4*/K_RIGHTARROW,
	/* 5*/K_LEFTARROW,
	/* 6*/K_UPARROW,
	/* 7*/K_DOWNARROW,
	/* 8*/'r',
	/* 9*/'l',
	/*10*/'x',
	/*11*/'y',
	/*12*/0,
	/*13*/0,
	/*14*/'<',
	/*15*/'>',
	/*16*/0,
	/*17*/0,
	/*18*/0,
	/* 0*/0,
	/* 0*/0,
	/* 0*/0,
	/* 0*/0,
	/* 0*/0,
	/* 0*/K_MOUSE1,
	/* 0*/K_MOUSE2,
	/* 0*/K_MOUSE3,
	/* 0*/K_MOUSE4,
	/* 0*/K_JOY1,
	/* 0*/K_JOY2,
	/* 0*/K_JOY3,
	/* 0*/K_JOY4
};

void IN_Init( void ) {
}

touchPosition	g_lastTouch = { 0, 0 };
touchPosition	g_currentTouch = { 0, 0 };

void IN_Frame(void) {
	if (keysDown() & KEY_TOUCH)
	{
		touchRead(&g_lastTouch);// = touchReadXY();
		g_lastTouch.px <<= 7;
		g_lastTouch.py <<= 7;
	}
	if (keysHeld() & KEY_TOUCH)
	{
		int dx, dy;
		touchRead(&g_currentTouch);// = touchReadXY();
		// let's use some fixed point magic to improve touch smoothing accuracy
		g_currentTouch.px <<= 7;
		g_currentTouch.py <<= 7;

		dx = (g_currentTouch.px - g_lastTouch.px) >> 6;
		dy = (g_currentTouch.py - g_lastTouch.py) >> 6;


		dx *= 3;
		dy *= 3;

		Sys_QueEvent(0, SE_MOUSE, dx, dy, 0, NULL);

		// some simple averaging / smoothing through weightened (.5 + .5) accumulation
		g_lastTouch.px = (g_lastTouch.px + g_currentTouch.px) / 2;
		g_lastTouch.py = (g_lastTouch.py + g_currentTouch.py) / 2;
	}
}

void IN_Shutdown( void ) {
}

void Sys_SendKeyEvents (void) {
	hidScanInput();
	u32 down = hidKeysDown();
	u32 up = hidKeysUp();
	int i;
	for (i = 0; i < 32; i++) {
		if (buttonMap3ds[i]) {
			if ((down & BIT(i)) != 0) {
				//events.queue_event(m_msg_time, EV_KEY_DOWN, buttonMap3ds[i], 0);
				Sys_QueEvent(0, SE_KEY, buttonMap3ds[i], qtrue, 0, NULL);
			}
			if ((up & BIT(i)) != 0) {
				//events.queue_event(m_msg_time, EV_KEY_UP, buttonMap3ds[i], 0);
				Sys_QueEvent(0, SE_KEY, buttonMap3ds[i], qfalse, 0, NULL);
			}
		}
	}
	/*
	static int key_last = 0;
	static int key = 0;

	key_last = key;
	key = keyboard_scankeys();
	if (key_last != 0 && key_last != key)
	{
		//printf("key up: %d %c\n", key_last, key_last);
		Sys_QueEvent(0, SE_KEY, key, qfalse, 0, NULL);
		//events.queue_event(m_msg_time, EV_KEY_UP, key_last, 0);
		//event.type = ev_keyup;
		//event.data1 = key_last;
		//D_PostEvent(&event);
	}

	if (key != 0 && key != key_last)
	{
		//printf("key down: %d %c\n", key, key);
		Sys_QueEvent(0, SE_KEY, key, qtrue, 0, NULL);
		//events.queue_event(m_msg_time, EV_KEY_DOWN, key, 0);
		//event.type = ev_keydown;
		//event.data1 = key;
		//D_PostEvent(&event);
	}*/
}

/*
** Sys_ConsoleInput
*/
char *Sys_ConsoleInput(void)
{
	return NULL;
}

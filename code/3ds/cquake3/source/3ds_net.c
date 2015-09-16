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

#include "../game/q_shared.h"
#include "../qcommon/qcommon.h"
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> // bk001204

#define	MAX_IPS		16
static	int		numIP;
static	byte	localIP[MAX_IPS][4];

void NET_GetLocalAddress(void) {
	char				*p;
	int					ip;

	numIP = 0;
	ip = gethostid();
	p = &ip;
	localIP[numIP][0] = p[0];
	localIP[numIP][1] = p[1];
	localIP[numIP][2] = p[2];
	localIP[numIP][3] = p[3];
	Com_Printf("IP: %i.%i.%i.%i\n", (ip >> 24) & 0xff, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
}

void SockadrToNetadr(struct sockaddr_in *s, netadr_t *a)
{
	*(int *)&a->ip = *(int *)&s->sin_addr;
	a->port = s->sin_port;
	a->type = NA_IP;
}

/*
=============
Sys_StringToAdr

idnewt
192.246.40.70
=============
*/
qboolean	Sys_StringToSockaddr(const char *s, struct sockaddr *sadr)
{
	struct hostent	*h;
	//char	*colon; // bk001204 - unused

	memset(sadr, 0, sizeof(*sadr));
	((struct sockaddr_in *)sadr)->sin_family = AF_INET;

	((struct sockaddr_in *)sadr)->sin_port = 0;

	if (s[0] >= '0' && s[0] <= '9')
	{
		*(int *)&((struct sockaddr_in *)sadr)->sin_addr = inet_addr(s);
	}
	else
	{
		if (!(h = gethostbyname(s)))
			return qfalse;
		*(int *)&((struct sockaddr_in *)sadr)->sin_addr = *(int *)h->h_addr_list[0];
	}

	return qtrue;
}

/*
=============
Sys_StringToAdr

localhost
idnewt
idnewt:28000
192.246.40.70
192.246.40.70:28000
=============
*/
qboolean	Sys_StringToAdr(const char *s, netadr_t *a)
{
	struct sockaddr_in sadr;

	if (!Sys_StringToSockaddr(s, (struct sockaddr *)&sadr))
		return qfalse;

	SockadrToNetadr(&sadr, a);

	return qtrue;
}

/*
=============
NET_StringToAdr

localhost
idnewt
idnewt:28000
192.246.40.70
192.246.40.70:28000
=============

qboolean	NET_StringToAdr (char *s, netadr_t *a)
{	
	if (!strcmp (s, "localhost")) {
		memset (a, 0, sizeof(*a));
		a->type = NA_LOOPBACK;
		return true;
	}

	return false;
}
*/

/*
==================
Sys_SendPacket
==================
*/
void Sys_SendPacket( int length, const void *data, netadr_t to ) {
}

/*
==================
Sys_GetPacket

Never called by the game logic, just the system event queing
==================
*/
qboolean	Sys_GetPacket ( netadr_t *net_from, msg_t *net_message ) {
	return qfalse;
}

/*
==================
Sys_IsLANAddress

LAN clients will have their rate var ignored
==================
*/
qboolean Sys_IsLANAddress(netadr_t adr) {
	int		i;

	if (adr.type == NA_LOOPBACK) {
		return qtrue;
	}

	if (adr.type == NA_IPX) {
		return qtrue;
	}

	if (adr.type != NA_IP) {
		return qfalse;
	}

	// choose which comparison to use based on the class of the address being tested
	// any local adresses of a different class than the address being tested will fail based on the first byte

	if (adr.ip[0] == 127 && adr.ip[1] == 0 && adr.ip[2] == 0 && adr.ip[3] == 1) {
		return qtrue;
	}

	// Class A
	if ((adr.ip[0] & 0x80) == 0x00) {
		for (i = 0; i < numIP; i++) {
			if (adr.ip[0] == localIP[i][0]) {
				return qtrue;
			}
		}
		// the RFC1918 class a block will pass the above test
		return qfalse;
	}

	// Class B
	if ((adr.ip[0] & 0xc0) == 0x80) {
		for (i = 0; i < numIP; i++) {
			if (adr.ip[0] == localIP[i][0] && adr.ip[1] == localIP[i][1]) {
				return qtrue;
			}
			// also check against the RFC1918 class b blocks
			if (adr.ip[0] == 172 && localIP[i][0] == 172 && (adr.ip[1] & 0xf0) == 16 && (localIP[i][1] & 0xf0) == 16) {
				return qtrue;
			}
		}
		return qfalse;
	}

	// Class C
	for (i = 0; i < numIP; i++) {
		if (adr.ip[0] == localIP[i][0] && adr.ip[1] == localIP[i][1] && adr.ip[2] == localIP[i][2]) {
			return qtrue;
		}
		// also check against the RFC1918 class c blocks
		if (adr.ip[0] == 192 && localIP[i][0] == 192 && adr.ip[1] == 168 && localIP[i][1] == 168) {
			return qtrue;
		}
	}
	return qfalse;
}

/*
==================
Sys_ShowIP
==================
*/
void Sys_ShowIP(void) {
	int i;

	for (i = 0; i < numIP; i++) {
		Com_Printf("IP: %i.%i.%i.%i\n", localIP[i][0], localIP[i][1], localIP[i][2], localIP[i][3]);
	}
}

/*
====================
NET_Sleep

sleeps msec or until net socket is ready
====================
*/
void NET_Sleep(int msec) {
}

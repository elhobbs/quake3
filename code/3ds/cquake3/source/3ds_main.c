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
// sys_null.h -- null system driver to aid porting efforts

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include "../../../game/q_shared.h"
#include "../../../qcommon/qcommon.h"
#include <sys/dirent.h>
#include <3ds.h>
#include <3ds/svc.h>
#include <malloc.h>
#include <unistd.h>

int			sys_curtime;


//===================================================================

void Sys_BeginStreamedFile(fileHandle_t f, int readAhead) {
}

void Sys_EndStreamedFile(fileHandle_t f) {
}

int Sys_StreamedRead(void *buffer, int size, int count, fileHandle_t f) {
	return FS_Read(buffer, size * count, f);
}

void Sys_StreamSeek(fileHandle_t f, int offset, int origin) {
	FS_Seek(f, offset, origin);
}

//===================================================================

void Sys_ShowConsole(int visLevel, qboolean quitOnClose)
{
}

void Sys_mkdir ( const char *path ) {
	mkdir(path, 0777);
}

extern u8 __end__[];        // end of static code and data
extern u8* fake_heap_start;
extern u8* fake_heap_end;

u8 *getHeapStart() {
	return __end__;
}

u8 *getHeapEnd() {
	return (u8 *)sbrk(0);
}

u8 *getHeapLimit() {
	return fake_heap_end;
}

size_t latestUsed, latestFree;

size_t getMemUsed() {
	struct mallinfo mi = mallinfo();
	latestUsed = mi.uordblks;
	return latestUsed;
}

size_t getMemFree() {
	struct mallinfo mi = mallinfo();
	latestFree = mi.fordblks + (getHeapLimit() - getHeapEnd());
	return latestFree;
}

void Sys_Error (const char *error, ...) {
	va_list		argptr;

	printf ("Sys_Error: ");	
	va_start (argptr,error);
	vprintf (error,argptr);
	va_end (argptr);
	printf ("\n");

	printf("LINEAR  free: %dKB\n", (int)linearSpaceFree() / 1024);
	printf("REGULAR free: %dKB\n", (int)getMemFree() / 1024);

	printf("press A to exit...");
	do {
		scanKeys();
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	} while ((keysDown() & KEY_A) == 0);
	do {
		scanKeys();
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	} while ((keysDown() & KEY_A) == KEY_A);
	printf("done\n");

	do {
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	} while (1);

	exit(1);
}

void Sys_Print(const char *msg) {
	printf(msg);
}

void Sys_Quit (void) {
	exit (0);
}

void	Sys_UnloadGame (void) {
}

void	*Sys_GetGameAPI (void *parms) {
	return NULL;
}

char *Sys_GetClipboardData( void ) {
	return NULL;
}

#define TICKS_PER_MSEC 268123.480

int		Sys_Milliseconds (void) {
	u64 ms = svcGetSystemTick() / TICKS_PER_MSEC;
	return ms;
}

void	Sys_Mkdir (const char *path) {
	mkdir(path, 0777);
}

int is_n3ds = 0;
int detect_n3ds(void)
{
	if (is_n3ds != -1) {
		return is_n3ds;
	}
	else {
		if (ptmSysmInit() == 0) {
			printf("ptmSysmInit: success\n");
			is_n3ds = 1;
			ptmSysmExit();
		}
		else {
			is_n3ds = 0;
		}
	}
	return is_n3ds;
}

void	Sys_Init (void) {
}


void	Sys_EarlyOutput( char *string ) {
	printf( "%s", string );
}

/*
================
Sys_CheckCD

Return true if the proper CD is in the drive
================
*/
qboolean	Sys_CheckCD(void) {
	// FIXME: mission pack
	return qtrue;
	//return Sys_ScanForCD();
}

void Sys_SnapVector(float *v) { // bk001213 - see win32/win_shared.c
	// bk001213 - old linux
	v[0] = rint(v[0]);
	v[1] = rint(v[1]);
	v[2] = rint(v[2]);
}

/*
==================
Sys_LowPhysicalMemory()
==================
*/
qboolean Sys_LowPhysicalMemory() {
	//MEMORYSTATUS stat;
	//GlobalMemoryStatus (&stat);
	//return (stat.dwTotalPhys <= MEM_THRESHOLD) ? qtrue : qfalse;
	return qfalse; // bk001207 - FIXME
}

/*
==============
Sys_Cwd
==============
*/
char *Sys_Cwd(void) {
	static char cwd[MAX_OSPATH];

	getcwd(cwd, sizeof(cwd) - 1);
	cwd[MAX_OSPATH - 1] = 0;

	return cwd;
}

/*
==============
Sys_DefaultCDPath
==============
*/
char *Sys_DefaultCDPath(void) {
	return "";
}

char *Sys_DefaultInstallPath(void)
{
	return Sys_Cwd();
}

char	*Sys_DefaultHomePath(void) {
	return NULL;
}


/*
==============================================================

DIRECTORY SCANNING

==============================================================
*/

#define	MAX_FOUND_FILES	0x1000

void Sys_ListFilteredFiles(const char *basedir, char *subdirs, char *filter, char **list, int *numfiles) {
	char		search[MAX_OSPATH], newsubdirs[MAX_OSPATH];
	char		filename[MAX_OSPATH];
	struct stat st;
	DIR *dir;
	struct dirent *pent;
	int ret;

	if (*numfiles >= MAX_FOUND_FILES - 1) {
		return;
	}

	if (strlen(subdirs)) {
		Com_sprintf(search, sizeof(search), "%s\\%s\\*", basedir, subdirs);
	}
	else {
		Com_sprintf(search, sizeof(search), "%s\\*", basedir);
	}

	dir = opendir(search);
	if (dir == 0) {
		return;
	}

	while ((pent = readdir(dir)) != 0) {
		ret = stat(pent->d_name, &st);
		if (S_ISDIR(st.st_mode)) {
			if (Q_stricmp(pent->d_name, ".") && Q_stricmp(pent->d_name, "..")) {
				if (strlen(subdirs)) {
					Com_sprintf(newsubdirs, sizeof(newsubdirs), "%s\\%s", subdirs, pent->d_name);
				}
				else {
					Com_sprintf(newsubdirs, sizeof(newsubdirs), "%s", pent->d_name);
				}
				Sys_ListFilteredFiles(basedir, newsubdirs, filter, list, numfiles);
			}
		}
		if (*numfiles >= MAX_FOUND_FILES - 1) {
			break;
		}
		Com_sprintf(filename, sizeof(filename), "%s\\%s", subdirs, pent->d_name);
		if (!Com_FilterPath(filter, filename, qfalse))
			continue;
		list[*numfiles] = CopyString(filename);
		(*numfiles)++;
	}

	closedir(dir);
}

static qboolean strgtr(const char *s0, const char *s1) {
	int l0, l1, i;

	l0 = strlen(s0);
	l1 = strlen(s1);

	if (l1<l0) {
		l0 = l1;
	}

	for (i = 0; i<l0; i++) {
		if (s1[i] > s0[i]) {
			return qtrue;
		}
		if (s1[i] < s0[i]) {
			return qfalse;
		}
	}
	return qfalse;
}

char **Sys_ListFiles(const char *directory, const char *extension, char *filter, int *numfiles, qboolean wantsubs) {
	char		*search;// [MAX_OSPATH];
	int			nfiles;
	char		**listCopy;
	char		**list;// [MAX_FOUND_FILES];
	//struct _finddata_t findinfo;
	//int			findhandle;
	int			flag;
	int			i;
	struct stat st;
	DIR *dir;
	struct dirent *d;
	int ret;
	qboolean dironly = wantsubs;

	//printf("Sys_ListFiles %s %s\n", directory, extension);

	search = (char *)malloc(MAX_OSPATH);
	list = (char **)malloc(MAX_FOUND_FILES*sizeof(char *));
	*search = 0;
	*list = 0;

	if (filter) {

		nfiles = 0;
		Sys_ListFilteredFiles(directory, "", filter, list, &nfiles);

		list[nfiles] = 0;
		*numfiles = nfiles;

		if (!nfiles)
			return NULL;

		listCopy = Z_Malloc((nfiles + 1) * sizeof(*listCopy));
		for (i = 0; i < nfiles; i++) {
			listCopy[i] = list[i];
		}
		listCopy[i] = NULL;

		free(search);
		free(list);

		return listCopy;
	}

	if (!extension) {
		extension = "";
	}

	// passing a slash as extension will find directories
	if (extension[0] == '/' && extension[1] == 0) {
		extension = "";
		flag = 0;
	}
	else {
		flag = _IFDIR;
	}

	// search
	nfiles = 0;

	dir = opendir(directory);
	if (dir == 0) {
		*numfiles = 0;
		free(search);
		free(list);
		return NULL;
	}

	while ((d = readdir(dir)) != 0) {
		//printf("%s %c\n", d->d_name, d->d_type);
		Com_sprintf(search, MAX_OSPATH, "%s/%s", directory, d->d_name);
		//printf("%s\n", search);
		if (stat(search, &st) == -1)
			continue;
		if ((dironly && !(st.st_mode & S_IFDIR)) ||
			(!dironly && (st.st_mode & S_IFDIR)))
			continue;

		if (*extension) {
			if (strlen(d->d_name) < strlen(extension) ||
				Q_stricmp(
				d->d_name + strlen(d->d_name) - strlen(extension),
				extension)) {
				continue; // didn't match
			}
		}
		//printf("match\n");

		if (nfiles == MAX_FOUND_FILES - 1)
			break;
		list[nfiles] = CopyString(d->d_name);
		nfiles++;
	};

	list[nfiles] = 0;

	closedir(dir);

	// return a copy of the list
	*numfiles = nfiles;

	if (!nfiles) {
		free(search);
		free(list);
		return NULL;
	}

	listCopy = Z_Malloc((nfiles + 1) * sizeof(*listCopy));
	for (i = 0; i < nfiles; i++) {
		listCopy[i] = list[i];
	}
	listCopy[i] = NULL;

	//printf("list %d\n", nfiles);
	do {
		flag = 0;
		for (i = 1; i<nfiles; i++) {
			if (strgtr(listCopy[i - 1], listCopy[i])) {
				char *temp = listCopy[i];
				listCopy[i] = listCopy[i - 1];
				listCopy[i - 1] = temp;
				flag = 1;
			}
		}
	} while (flag);

	//printf("list %08x\n", listCopy);
	free(search);
	free(list);
	return listCopy;
}

void	Sys_FreeFileList(char **list) {
	int		i;

	if (!list) {
		return;
	}

	for (i = 0; list[i]; i++) {
		Z_Free(list[i]);
	}

	Z_Free(list);
}

/*
========================================================================

EVENT LOOP

========================================================================
*/

#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

sysEvent_t	eventQue[MAX_QUED_EVENTS];
int			eventHead, eventTail;
byte		sys_packetReceived[MAX_MSGLEN];

/*
================
Sys_QueEvent

A time of 0 will get the current time
Ptr should either be null, or point to a block of data that can
be freed by the game later.
================
*/
void Sys_QueEvent(int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr) {
	sysEvent_t	*ev;

	ev = &eventQue[eventHead & MASK_QUED_EVENTS];
	if (eventHead - eventTail >= MAX_QUED_EVENTS) {
		Com_Printf("Sys_QueEvent: overflow\n");
		// we are discarding an event, but don't leak memory
		if (ev->evPtr) {
			Z_Free(ev->evPtr);
		}
		eventTail++;
	}

	eventHead++;

	if (time == 0) {
		time = Sys_Milliseconds();
	}

	ev->evTime = time;
	ev->evType = type;
	ev->evValue = value;
	ev->evValue2 = value2;
	ev->evPtrLength = ptrLength;
	ev->evPtr = ptr;
}

/*
================
Sys_GetEvent

================
*/
sysEvent_t Sys_GetEvent(void) {
	sysEvent_t  ev;
	char    *s;
	msg_t   netmsg;
	netadr_t  adr;

	// return if we have data
	if (eventHead > eventTail)
	{
		eventTail++;
		return eventQue[(eventTail - 1) & MASK_QUED_EVENTS];
	}

	// pump the message loop
	// in vga this calls KBD_Update, under X, it calls GetEvent
	Sys_SendKeyEvents();

	// check for console commands
	s = Sys_ConsoleInput();
	if (s)
	{
		char  *b;
		int   len;

		len = strlen(s) + 1;
		b = Z_Malloc(len);
		strcpy(b, s);
		Sys_QueEvent(0, SE_CONSOLE, 0, 0, len, b);
	}

	// check for other input devices
	IN_Frame();

	// check for network packets
	MSG_Init(&netmsg, sys_packetReceived, sizeof(sys_packetReceived));
	if (Sys_GetPacket(&adr, &netmsg))
	{
		netadr_t    *buf;
		int       len;

		// copy out to a seperate buffer for qeueing
		len = sizeof(netadr_t) + netmsg.cursize;
		buf = Z_Malloc(len);
		*buf = adr;
		memcpy(buf + 1, netmsg.data, netmsg.cursize);
		Sys_QueEvent(0, SE_PACKET, 0, 0, len, buf);
	}

	// return if we have data
	if (eventHead > eventTail)
	{
		eventTail++;
		return eventQue[(eventTail - 1) & MASK_QUED_EVENTS];
	}

	// create an empty event to return

	memset(&ev, 0, sizeof(ev));
	ev.evTime = Sys_Milliseconds();

	return ev;
}

/*
==================
Sys_BeginProfiling
==================
*/
void Sys_BeginProfiling(void) {
	// this is just used on the mac build
}

/*
=================
Sys_UnloadDll

=================
*/
void Sys_UnloadDll(void *dllHandle) {
	Com_Printf("Sys_UnloadDll(%08x)\n", dllHandle);
	svcSleepThread(5000000000LL);
}

int game_vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);
void game_dllEntry(int (QDECL *syscallptr)(int arg, ...));

int cgame_vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);
void cgame_dllEntry(int (QDECL *syscallptr)(int arg, ...));

int q3ui_vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);
void q3ui_dllEntry(int (QDECL *syscallptr)(int arg, ...));

void Sys_Print_dllentry() {
	printf("game : %08x\n", game_dllEntry);
	printf("cgame: %08x\n", cgame_dllEntry);
	printf("ui   : %08x\n", q3ui_dllEntry);
}

void * QDECL Sys_LoadDll(const char *name, char *fqpath, int (QDECL **entryPoint)(int, ...), int (QDECL *systemcalls)(int, ...)) {
	Com_Printf("Sys_LoadDll\n  %s\n  %s\n", name, fqpath);
	svcSleepThread(5000000000LL);
	if (!strncasecmp(name, "ui", 2)) {
		q3ui_dllEntry(systemcalls);
		*entryPoint = q3ui_vmMain;
		return "ui";
	}
	if (!strncasecmp(name, "cgame", 5)) {
		cgame_dllEntry(systemcalls);
		*entryPoint = cgame_vmMain;
		return "cgame";
	}
	if (!strncasecmp(name, "qagame", 6)) {
		game_dllEntry(systemcalls);
		*entryPoint = game_vmMain;
		return "qagame";
	}
	printf("failed to load %s\n", name);
	while (1);
	return 0;
}

int main (int argc, char **argv) {
	char *cmdline;
	int i, len;

	gfxInitDefault();
	consoleInit(GFX_BOTTOM, 0);
	//gfxSwapBuffers();

	GPU_Init(NULL);

	gfxSet3D(false);

	// merge the command line, this is kinda silly
	for (len = 1, i = 1; i < argc; i++)
		len += strlen(argv[i]) + 1;
	cmdline = malloc(len);
	*cmdline = 0;
	for (i = 1; i < argc; i++)
	{
		if (i > 1)
			strcat(cmdline, " ");
		strcat(cmdline, argv[i]);
	}
	//strcat(cmdline, " +cinematic idlogo.roq");

	printf("LINEAR  free: %dKB\n", (int)linearSpaceFree() / 1024);
	printf("REGULAR free: %dKB\n", (int)getMemFree() / 1024);
	//while (aptMainLoop()) {
	//}
	//return 0;
	Com_Init(cmdline);
	printf("LINEAR  free: %dKB\n", (int)linearSpaceFree() / 1024);
	printf("REGULAR free: %dKB\n", (int)getMemFree() / 1024);

	svcSleepThread(3000000000LL);

	while (aptMainLoop()) {
		Com_Frame( );
	}

	return 0;
}



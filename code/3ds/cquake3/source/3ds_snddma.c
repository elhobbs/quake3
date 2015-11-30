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

#define SOUND_COMPILE

// snddma_null.c
// all other sound mixing is portable

#include "../client/client.h"
#include "../client/snd_local.h"

#ifdef SOUND_COMPILE
#include <3ds/svc.h>
#include <3ds/services/csnd.h>
#include <3ds/types.h>

#define SND_BUFFER_SIZE 16384
#define SND_SPEED 11025

static u64 g_snd_start = 0;
static int g_snd_channel = 8;
static int sount_initialized = 0;

Result play_sound(int chn, u32 flags, u32 sampleRate, float vol, float pan, void* data0, void* data1, u32 size)
{
	if (!sount_initialized) return;

	if (!(csndChannels & BIT(chn)))
		return 1;

	u32 paddr0 = 0, paddr1 = 0;

	int encoding = (flags >> 12) & 3;
	int loopMode = (flags >> 10) & 3;

	if (!loopMode) flags |= SOUND_ONE_SHOT;

	if (data0) paddr0 = osConvertVirtToPhys((u32)data0);
	if (data1) paddr1 = osConvertVirtToPhys((u32)data1);

	u32 timer = CSND_TIMER(sampleRate);
	if (timer < 0x0042) timer = 0x0042;
	else if (timer > 0xFFFF) timer = 0xFFFF;
	flags &= ~0xFFFF001F;
	flags |= SOUND_ENABLE | SOUND_CHANNEL(chn) | (timer << 16);

	u32 volumes = CSND_VOL(vol, pan);
	CSND_SetChnRegs(flags, paddr0, paddr1, size, volumes, volumes);

	if (loopMode == CSND_LOOPMODE_NORMAL && paddr1 > paddr0)
	{
		// Now that the first block is playing, configure the size of the subsequent blocks
		size -= paddr1 - paddr0;
		CSND_SetBlock(chn, 1, paddr1, size);
	}

	return 0;
}
#endif

qboolean SNDDMA_Init(void) {

#ifdef SOUND_COMPILE
	if (sount_initialized) return;
	Result ret = 0;
	float pan[] = { -1, 1 };

#if 1
	if (csndInit() == 0) {
		printf("Sound init ok!\n");
	}
	else {
		printf("Sound init failed!\n");
		return qfalse;
	}

#endif // 0
	sount_initialized = 1;

	dma.channels = 1;
	dma.samplebits = 8;
	dma.submission_chunk = 0;
	dma.speed = SND_SPEED;
	dma.buffer = (byte *)linearAlloc(SND_BUFFER_SIZE);
	if (dma.buffer == 0) {
		printf("sound dma buffer alloc failed!\n");
		return qfalse;
	}

	g_snd_start = svcGetSystemTick();
	ret = play_sound(g_snd_channel, SOUND_REPEAT | SOUND_FORMAT_8BIT, SND_SPEED, 1.0f, 0/*pan[i]*/, (u32*)(dma.buffer), (u32*)(dma.buffer), SND_BUFFER_SIZE);
	CSND_UpdateInfo(true);

	u8 playing = 0;
	csndIsPlaying(g_snd_channel, &playing);
	if (!playing) {
		printf("forcing: %d\n", g_snd_channel);
		CSND_SetPlayState(g_snd_channel, 1);
	}
	//flush csnd command buffers
	CSND_UpdateInfo(true);

	CSND_SetVol(g_snd_channel, CSND_VOL(1.0, 0/*pan[i]*/), CSND_VOL(1.0, 0/*pan[i]*/));
	CSND_UpdateInfo(true);

	return qtrue;
#else
	return qfalse;
#endif
}

#define TICKS_PER_SEC_LL 268111856LL

int	SNDDMA_GetDMAPos(void)
{
#ifdef SOUND_COMPILE
	if (!sount_initialized) return 0;
	u64 delta = (svcGetSystemTick() - g_snd_start);
	u64 samples = delta * SND_SPEED / TICKS_PER_SEC_LL;

	return samples;
#else
	return 0;
#endif
}

void SNDDMA_Shutdown(void) {
#ifdef SOUND_COMPILE
	if (!sount_initialized) return;
	//stop sound
	CSND_SetPlayState(g_snd_channel, 0);
	CSND_UpdateInfo(true);

	//delete buffer
	if (dma.buffer) {
		linearFree(dma.buffer);
		dma.buffer = 0;
	}

	csndExit();
#endif
}

void SNDDMA_BeginPainting (void)
{
}

void SNDDMA_Submit(void)
{
}

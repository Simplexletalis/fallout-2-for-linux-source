#include "stdafx.h"
#include "sdl/SDL.h"

static int _stdcall FakeGetKeyState(int n) {
	return 0;
}

typedef void (_stdcall *foCallback)(void* uTimerID, uint32 uMsg, uint32 dwUser, void* dw1, void* dw2);
struct EventState {
	SDL_TimerID id;
	foCallback callback;
	int delay;
	uint32 data;
	int repeat;
};
uint32 EventWrapper(uint32 interval, EventState* state) {
	state->callback(state, 0, state->data, 0, 0);
	return state->repeat;
}

static EventState* _stdcall FakeSetEvent(uint32 uDelay, uint32 uResolution, foCallback lpTimeProc, uint32 dwUser, uint32 fuEvent) {
	EventState* state=new EventState();
	state->callback=lpTimeProc;
	state->delay=uDelay;
	state->id=SDL_AddTimer(uDelay, (SDL_NewTimerCallback)&EventWrapper, state);
	state->data=dwUser;
	state->repeat=(fuEvent&1)?uDelay:0;
	return state;
}

static int _stdcall FakeKillEvent(EventState* state) {
	SDL_RemoveTimer(state->id);
	delete state;
	return 0;
}

//FO can't have any handles open anyway; we nicked them all. Can't be bothered to edit out the calls to this though, since they've both been inlined.
static int _stdcall FakeCloseHandle(int) {
	return 0;
}

static int _stdcall FakeCopyFile(const char* from, const char* to, int bFailIfExists) {
	if(bFailIfExists) {
		FILE* f=fopen(to, "rb");
		if(f) {
			fclose(f);
			return 0;
		}
	}
	FILE* ffrom=fopen(from, "rb");
	if(!ffrom) return 0;
	FILE* fto=fopen(to, "wb");
	if(!fto) {
		fclose(ffrom);
		return 0;
	}

	char buf[4096];
	while(true) {
		int read=fread(buf, 1, 4096, ffrom);
		if(!read) break;
		fwrite(buf, read, 1, fto);
	}

	fclose(ffrom);
	fclose(fto);

	return 1;
}


extern "C" {
const void* _6c010c=&FakeGetKeyState; //&GetKeyState;
const void* _6c0164=&SDL_GetTicks;    //&timeGetTime;
const void* _6c0168=&FakeKillEvent;   //timeKillEvent; // (4ae614)
const void* _6c016c=&FakeSetEvent;    //timeSetEvent;  // (4aeab6)
const void* _6c01d0=&FakeCloseHandle; //CloseHandle;   // (4d5c30, 4d61f4)
const void* _6c01d4=&FakeCopyFile;    //CopyFileA;     // (4527F7, 45282e, 4528b8)
const void* _6c0200=&SDL_GetTicks;    //&GetTickCount;
}

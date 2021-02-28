#include "stdafx.h"
#include <SDL/SDL.h>
#include "events.h"
#include <stdlib.h>
#include <string.h>
#include "EvDiKeys.h"

using namespace std;

extern "C" {
	void WantToQuit();
}


uint32 keyRemap(SDL_keysym &keysym) {
  uint32 newCode;
  switch(keysym.sym) {
    case SDLK_ESCAPE:
      newCode = DIK_ESCAPE;
      break;
    case SDLK_SPACE:
      newCode = DIK_SPACE;
      break;
    case SDLK_RETURN:
      newCode = DIK_RETURN;
      break;
    case SDLK_HOME:
      newCode = DIK_HOME;
      break;
    case SDLK_RIGHT:
      newCode = DIK_RIGHT;
      break;
    case SDLK_LEFT:
      newCode = DIK_LEFT;
      break;
    case SDLK_DOWN:
      newCode = DIK_DOWN;
      break;
    case SDLK_UP:
      newCode = DIK_UP;
      break;
    case SDLK_F1:
      newCode = DIK_F1;
      break;
    case SDLK_F2:
      newCode = DIK_F2;
      break;
    case SDLK_F3:
      newCode = DIK_F3;
      break;
    case SDLK_F4:
      newCode = DIK_F4;
      break;
    case SDLK_F5:
      newCode = DIK_F5;
      break;
    case SDLK_F6:
      newCode = DIK_F6;
      break;
    case SDLK_F7:
      newCode = DIK_F7;
      break;
    case SDLK_F8:
      newCode = DIK_F8;
      break;
    case SDLK_F9:
      newCode = DIK_F9;
      break;
    case SDLK_F10:
      newCode = DIK_F10;
      break;
    case SDLK_F11:
      newCode = DIK_F11;
      break;
    case SDLK_F12:
      newCode = DIK_F12;
      break;
    case SDLK_LCTRL:
      newCode = DIK_LCONTROL;
      break;
    case SDLK_q:
      newCode = DIK_Q;
      break;

    default:
       newCode = keysym.scancode;
       break;
  }
  return newCode;
}


#define MAX_KEY_QUEUE (64)

SDL_Surface *mainSurface;

void SdlInit() {
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) exit(-1);
	if(0) {
		mainSurface=SDL_SetVideoMode(640, 480, 8, SDL_FULLSCREEN|SDL_HWSURFACE|SDL_HWPALETTE);
	} else {
		mainSurface=SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE | SDL_DOUBLEBUF);
	}
	//SDL_ShowCursor(SDL_DISABLE);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void SdlExit() {
	SDL_CloseAudio();
	SDL_Quit();
}

struct DIDEVICEOBJECTDATA {
    uint32       dwOfs;
    uint32       dwData;
    uint32       dwTimeStamp;
    uint32       dwSequence;
};

DIDEVICEOBJECTDATA keyqueue[MAX_KEY_QUEUE];
uint32 keysequence;
int keystart, keyend;
uint8 lastarray[1024];

static void LostFocus() {
    	SDL_Event e;
	SDL_PauseAudio(1);
	while(true) {
		while(!SDL_PollEvent(&e)) SDL_Delay(10);
		if(e.type==SDL_ACTIVEEVENT&&e.active.gain==1) break;

		if(e.type==SDL_QUIT) {
		  WantToQuit();
		  std::exit(0);
		}

	}
	SDL_PauseAudio(0);
}
void RunMessageQueue() {
	//TODO: Might be best to handle caps lock manually. Shift seems fine though.
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_ACTIVEEVENT:
		if(e.active.gain==0) LostFocus();
		break;
	    case SDL_KEYDOWN:
		if((keyend!=keystart-1) && (keyend!=31||keystart!=0)) {
		    keyqueue[keyend].dwTimeStamp=SDL_GetTicks();
		    keyqueue[keyend].dwSequence=keysequence++;
		    keyqueue[keyend].dwData=0x80;
		    keyqueue[keyend].dwOfs=keyRemap(e.key.keysym);
		    if(++keyend==MAX_KEY_QUEUE) keyend=0;
		}
		break;
	    case SDL_KEYUP:
		if((keyend!=keystart-1) && (keyend!=31||keystart!=0)) {
		    keyqueue[keyend].dwTimeStamp=SDL_GetTicks();
		    keyqueue[keyend].dwSequence=keysequence++;
		    keyqueue[keyend].dwData=0;
		    keyqueue[keyend].dwOfs=keyRemap(e.key.keysym);
		    if(++keyend==MAX_KEY_QUEUE) keyend=0;
		}
		break;
	    case SDL_QUIT:
		WantToQuit();
		std::exit(0);
		break;
	    case SDL_VIDEOEXPOSE:
		break;
	    default:
		break;
        }
    }
}

void EmptyKeyQueue() {
	RunMessageQueue();
	keystart=keyend=0;
}

#define DIGDD_PEEK 1
int GetNextKey(uint32, DIDEVICEOBJECTDATA* data, int* count, uint32 flags) {
	int keylen=(keystart>keyend)?((keyend+MAX_KEY_QUEUE)-keystart):(keyend-keystart);
	int oldkeystart=keystart;
	if(keylen==0) {
		*count=0;
		return 0;
	}

	int tocopy=(*count>keylen)?keylen:*count;
	*count=tocopy;

	for(int i=0;i<tocopy;i++) {
		data[i]=keyqueue[keystart];
		if(++keystart==MAX_KEY_QUEUE) keystart=0;
	}

	if(flags&1) keystart=oldkeystart;
	else if(keystart!=keyend) return 1;
	return 0;
}

struct INTERNALMOUSESTATE {
	int x;
	int y;
	uint8 left;
	uint8 right;
};

int MouseState(INTERNALMOUSESTATE *mouseState) {
	int buttons=SDL_GetRelativeMouseState(&mouseState->x, &mouseState->y);
	if(buttons&1) mouseState->left=1;
	else mouseState->left=0;
	if(buttons&4) mouseState->right=1;
	else mouseState->right=0;
	return 1;
}

void SetWindowTitle(const char* title) {
	SDL_WM_SetCaption(title, title);
}

void showRect(char *data, int sWidth, int sHeight, int sX, int sY, int dWidth, int dHeight, int destx, int desty) {
	SDL_LockSurface(mainSurface);
	if(destx+dWidth>=640) dWidth=640-destx;
	if(desty+dHeight>=480) dHeight=480-desty;
	char* dest=&((char*)mainSurface->pixels)[desty*mainSurface->pitch+destx];
	for(int y=0;y<dHeight;y++) {
		memcpy(&dest[y*mainSurface->pitch], &data[(sY+y)*sWidth+sX], dWidth);
	}
	SDL_UnlockSurface(mainSurface);
	SDL_UpdateRect(mainSurface, 0, 0, 0, 0);
}

void clearScreen() {
	SDL_FillRect(mainSurface, 0, 0);
}

int setPaletteEntries(uint8* entries, int pos, int count) {
	SDL_Color colours[256];
	for(int i=0;i<count;i++) {
		colours[i].r=entries[i*3+0]*4;
		colours[i].g=entries[i*3+1]*4;
		colours[i].b=entries[i*3+2]*4;
		colours[i].unused=0;
	}
	SDL_SetPalette(mainSurface, 3, colours, pos, count);
	return 0;
}

void bltToScreen(int target[4], SDL_Surface* surf, int source[4]) {
	//SDL_Rect srect = { source[0], source[1], source[2]-source[0], source[3]-source[1] };
	SDL_Rect trect = { static_cast<Sint16>(target[0]), static_cast<Sint16>(target[1]), static_cast<Uint16>(target[2]-target[0]), static_cast<Uint16>(target[3]-target[1]) };

	//blitting here results in a black screen after the first frame... Probably doing something wrong
	//SDL_BlitSurface(surf, &srect, mainSurface, &trect);
        //SDL_Flip(surf);
	memcpy(&((char*)mainSurface->pixels)[trect.y*mainSurface->pitch], surf->pixels, surf->h*surf->pitch);
	SDL_UpdateRect(mainSurface, trect.x, trect.y, trect.w, trect.h);
}

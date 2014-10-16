struct DIDEVICEOBJECTDATA;
struct INTERNALMOUSESTATE;
struct SDL_Surface;

extern "C" {

void SdlInit();
void  SdlExit();
void CCONV1(cdecl) RunMessageQueue() CCONV2(cdecl);
void CCONV1(cdecl) EmptyKeyQueue() CCONV2(cdecl);
int CCONV1(cdecl) GetNextKey(uint32, DIDEVICEOBJECTDATA* data, int* count, uint32 flags) CCONV2(cdecl);
int CCONV1(cdecl) MouseState(INTERNALMOUSESTATE *mouseState) CCONV2(cdecl);
void CCONV1(cdecl) SetWindowTitle(const char* title) CCONV2(cdecl);
void CCONV1(cdecl) showRect(char *data, int sWidth, int sHeight, int sX, int sY, int dWidth, int dHeight, int destx, int desty) CCONV2(cdecl);
void CCONV1(cdecl) clearScreen() CCONV2(cdecl);
int CCONV1(cdecl) setPaletteEntries(uint8* entries, int pos, int count) CCONV2(cdecl);
void CCONV1(cdecl) bltToScreen(int target[4], SDL_Surface* surf, int source[4]) CCONV2(cdecl);

}

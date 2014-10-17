#include "stdafx.h"
#include "sdl/SDL.h"
#include "sounds.h"
#include <cstring>

#define UNUSEDFUNCTION { crash; return 0; }

static WAVEFORMATEX pFmt;
static int lockDepth;

static const DSCAPS dscaps = { sizeof(DSCAPS), 0 };

class FakeBuffer;

#define MAX_PLAYING_SAMPLES (32)

static FakeBuffer* playingSamples[MAX_PLAYING_SAMPLES];

FakePrimaryBuffer::FakePrimaryBuffer(const DSBUFFERDESC* _desc) {
    refs=1;
}
// IUnknown methods
int FakePrimaryBuffer::QueryInterface(void*, void**)  { UNUSEDFUNCTION; }
uint32 FakePrimaryBuffer::AddRef()  { return ++refs; }
uint32 FakePrimaryBuffer::Release()  {
    if(!--refs) {
        delete this;
        return 0;
    } else return refs;
}

// IDirectSoundBuffer methods
int FakePrimaryBuffer::GetCaps(void*)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::GetCurrentPosition(uint32*, uint32*)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::GetFormat(WAVEFORMATEX* _fmt, uint32 isize, uint32* osize)  {
	*_fmt=pFmt;
	return 0;
}
int FakePrimaryBuffer::GetVolume(int*)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::GetPan(int*)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::GetFrequency(uint32*)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::GetStatus(uint32*)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::Initialize(void*, void*)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::Lock(uint32, uint32, void**, uint32*, void**, uint32*, uint32)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::Play(uint32, uint32, uint32)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::SetCurrentPosition(uint32)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::SetFormat(const WAVEFORMATEX* _fmt)  {
	pFmt=*_fmt;
	return 0;
}
int FakePrimaryBuffer::SetVolume(int)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::SetPan(int)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::SetFrequency(uint32)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::Stop()  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::Unlock(void*, uint32, void*, uint32)  { UNUSEDFUNCTION; }
int FakePrimaryBuffer::Restore()  { UNUSEDFUNCTION; }

FakeBuffer::FakeBuffer(const DSBUFFERDESC* _desc) {
    refs=1;
    desc=*_desc;
    fmt=*desc.lpwfxFormat;
    desc.lpwfxFormat=&fmt;
	buffer=new char[desc.dwBufferBytes];

	caps.dwFlags=desc.dwFlags;
	caps.dwBufferBytes=desc.dwBufferBytes;
	caps.dwPlayCpuOverhead=0;
	caps.dwSize=sizeof(caps);
	caps.dwUnlockTransferRate=0;

	playposition=writeposition=0;
	loop=false;
	playing=false;
	attenuate=false;
	volume=0;
	lpan=false;
	rpan=false;
	pan=0;

	if(desc.lpwfxFormat->nSamplesPerSec!=22050) crash;
	if(fmt.nChannels!=1 && fmt.nChannels!=2) crash;
	if(fmt.wBitsPerSample!=8 && fmt.wBitsPerSample!=16) crash;
}
// IUnknown methods
int FakeBuffer::QueryInterface(void*, void**)  { UNUSEDFUNCTION; }
uint32 FakeBuffer::AddRef()  { return ++refs;; }
uint32 FakeBuffer::Release()  {
    if(!--refs) {
		if(playing) {
			for(int i=0;i<MAX_PLAYING_SAMPLES;i++) {
				if(playingSamples[i]==this) playingSamples[i]=0;
			}
		}
		delete[] buffer;
        delete this;
        return 0;
    } else return refs;
}

// IDirectSoundBuffer methods
int FakeBuffer::GetCaps(DSBCAPS* _caps)  {
	*_caps=caps;
	return 0;
}
int FakeBuffer::GetCurrentPosition(uint32* play, uint32* write)  {
	if(play) *play=playposition;
	if(write) *write=writeposition;
	return 0;
}
int FakeBuffer::GetFormat(WAVEFORMATEX* _fmt, uint32 isize, uint32* osize)  {
	*_fmt=fmt;
	if(osize) *osize=sizeof(WAVEFORMATEX);
	return 0;
}
int FakeBuffer::GetVolume(int* plVolume)  { UNUSEDFUNCTION; }
int FakeBuffer::GetPan(int* plPan)  { UNUSEDFUNCTION; }
int FakeBuffer::GetFrequency(uint32* pdwFrequency)  { UNUSEDFUNCTION; }
int FakeBuffer::GetStatus(uint32* status)  {
	if(playing) {
		if(loop) *status=DSBSTATUS_PLAYING|DSBSTATUS_LOOPING;
		else *status=DSBSTATUS_PLAYING;
	} else *status=0;
	return 0;
}
int FakeBuffer::Initialize(void* pDirectSound, void* pcDSBufferDesc)  { UNUSEDFUNCTION; }
int FakeBuffer::Lock(uint32 offset, uint32 bytes, void** ptr1, uint32* bytes1, void** ptr2, uint32* bytes2, uint32 flags)  {
	if(flags&DSBLOCK_FROMWRITECURSOR) offset=writeposition;
	if(flags&DSBLOCK_ENTIREBUFFER || bytes>desc.dwBufferBytes) bytes=desc.dwBufferBytes;
	*ptr1=&buffer[writeposition];
	if(writeposition+bytes>desc.dwBufferBytes) {
		*ptr2=buffer;
		*bytes1=desc.dwBufferBytes-writeposition;
		*bytes2=bytes-*bytes1;
	} else {
		*ptr2=0;
		*bytes1=bytes;
		*bytes2=0;
	}
	if(!lockDepth++) SDL_LockAudio();
	return 0;
}
int FakeBuffer::Play(uint32, uint32, uint32 flags)  {
	if(flags&DSBPLAY_LOOPING) loop=true; else loop=false;
	if(playing) return 0;
	for(int i=0;i<MAX_PLAYING_SAMPLES;i++) {
		if(playingSamples[i]==0) {
			playing=true;
			playingSamples[i]=this;
			return 0;
		}
	}
	return -1;
}
int FakeBuffer::SetCurrentPosition(uint32 dwNewPosition)  {
	return 0;
}
int FakeBuffer::SetFormat(const WAVEFORMATEX* pcfxFormat)  { UNUSEDFUNCTION; }
int FakeBuffer::SetVolume(int lVolume)  {
	if(lVolume==DSBVOLUME_MAX) attenuate=false;
	else {
		//TODO: This isn't correct; it's a logorithmic scale, not linear
		volume=(double)(lVolume-DSBVOLUME_MIN)/(double)(DSBVOLUME_MAX-DSBVOLUME_MIN);
		attenuate=true;
	}
	return 0;
}
int FakeBuffer::SetPan(int Pan)  {
	if(Pan==0) {
		lpan=false;
		rpan=false;
	} else if(((Pan>0)?Pan:-Pan)>10000) {
		return -1;
	} else if(Pan>0) {
		lpan=true;
		rpan=false;
	} else {
		lpan=false;
		rpan=true;
	}
	if(Pan<0) Pan=-Pan;
	pan=Pan/10000.0;
	return 0;
}
int FakeBuffer::SetFrequency(uint32 dwFrequency)  { UNUSEDFUNCTION; }
int FakeBuffer::Stop()  {
	if(!playing) return 0;
	playing=false;
	for(int i=0;i<MAX_PLAYING_SAMPLES;i++) {
		if(playingSamples[i]==this) playingSamples[i]=0;
	}
	return 0;
}
int FakeBuffer::Unlock(void* ptr1, uint32 bytes1, void* ptr2, uint32 bytes2)  {
	if(!--lockDepth) SDL_UnlockAudio();
	writeposition+=bytes1;
	if(ptr2) writeposition+=bytes2;
	if(writeposition>=desc.dwBufferBytes) writeposition-=desc.dwBufferBytes;
	return 0;
}
int FakeBuffer::Restore()  { UNUSEDFUNCTION; }

void FakeBuffer::Reset() {
	playing=false;
	for(int i=0;i<MAX_PLAYING_SAMPLES;i++) {
		if(playingSamples[i]==this) playingSamples[i]=0;
	}
	playposition=0;
}

void FakeBuffer::GetNextSample(int* _c1, int* _c2) {
	if(fmt.nChannels==1) {
		int c1;
		if(fmt.wBitsPerSample==8) {
			c1=buffer[playposition]*8;
			playposition++;
		} else {
			c1=*(short*)&buffer[playposition];
			playposition+=2;
		}
		if(attenuate) {
			c1=(int)(c1*volume);
		}
		*_c1+=lpan?(int)(c1*pan):c1;
		*_c2+=rpan?(int)(c1*pan):c1;
	} else {
		int c1, c2;
		if(fmt.wBitsPerSample==8) {
			c1=buffer[playposition]*8;
			c2=buffer[playposition+1]*8;
			playposition+=2;
		} else {
			c1=*(short*)&buffer[playposition];
			c2=*(short*)&buffer[playposition+2];
			playposition+=4;
		}
		if(attenuate) {
			c1=(int)(c1*volume);
			c2=(int)(c2*volume);
		}
		if(lpan) c1=(int)(c1*pan);
		if(rpan) c2=(int)(c2*pan);
		*_c1+=c1;
		*_c2+=c2;
	}
	if(playposition>=desc.dwBufferBytes) {
		if(loop) playposition-=desc.dwBufferBytes;
		else Reset();
	}
}

FakeDirectSound::FakeDirectSound() {
    refs=1;
}
// IUnknown methods
int FakeDirectSound::QueryInterface(void* a, void** b)  { UNUSEDFUNCTION; }
uint32 FakeDirectSound::AddRef()  { return ++refs;; }
uint32 FakeDirectSound::Release()  {
    if(!--refs) {
        delete this;
        return 0;
    } else return refs;
}

// IDirectSound methods
int FakeDirectSound::CreateSoundBuffer(const DSBUFFERDESC* desc, void** buffer, void*)  {
    if(!desc->lpwfxFormat) *buffer=new FakePrimaryBuffer(desc);
    else *buffer=new FakeBuffer(desc);
    return 0;
}
int FakeDirectSound::GetCaps(DSCAPS* pDSCaps)  {
	*pDSCaps=dscaps;
	return 0;
}
int FakeDirectSound::DuplicateSoundBuffer(void* pDSBufferOriginal, void** ppDSBufferDuplicate)  { UNUSEDFUNCTION; }
int FakeDirectSound::SetCooperativeLevel(int hwnd, uint32 dwLevel)  { return 0; }
int FakeDirectSound::Compact()  { UNUSEDFUNCTION; }
int FakeDirectSound::GetSpeakerConfig(uint32* pdwSpeakerConfig)  { UNUSEDFUNCTION; }
int FakeDirectSound::SetSpeakerConfig(uint32 dwSpeakerConfig)  { UNUSEDFUNCTION; }
int FakeDirectSound::Initialize(void* pcGuidDevice)  { UNUSEDFUNCTION; }

struct stream_t {
	short c1, c2;
};

static void mixerCallback(void *userdata, Uint8 *_stream, int len) {
	int samples=len/4; // /2 for two channels, and /2 for 16 bit
	stream_t *stream=(stream_t*)_stream;

    for(int i=0;i<samples;i++) {
		int channel1=0;
		int channel2=0;

		for(int j=0;j<MAX_PLAYING_SAMPLES;j++) {
			if(!playingSamples[j]) continue;
			playingSamples[j]->GetNextSample(&channel1, &channel2);
		}

		if(channel1>0x7fff) stream[i].c1=0x7fff;
		else if(channel1<-0x8000) stream[i].c1=-0x8000;
		else stream[i].c1=(short)channel1;
		if(channel2>0x7fff) stream[i].c2=0x7fff;
		else if(channel2<-0x8000) stream[i].c2=-0x8000;
		else stream[i].c2=(short)channel2;
    }
}

int CCONV1(stdcall) FakeDSoundCreate(uint32, FakeDirectSound** iface,uint32) {
	SDL_AudioSpec spec;
	std::memset(&spec, 0, sizeof(spec));
	spec.callback=mixerCallback;
	spec.channels=2;
	spec.freq=22050;
	spec.format=AUDIO_S16SYS;
	SDL_OpenAudio(&spec, 0);
	SDL_PauseAudio(0);

    *iface=new FakeDirectSound();
    return 0;
}

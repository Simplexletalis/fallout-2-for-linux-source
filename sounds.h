struct WAVEFORMATEX
{
    uint16        wFormatTag;        /* format type */
    uint16        nChannels;         /* number of channels (i.e. mono, stereo...) */
    uint32       nSamplesPerSec;     /* sample rate */
    uint32       nAvgBytesPerSec;    /* for buffer estimation */
    uint16        nBlockAlign;       /* block size of data */
    uint16        wBitsPerSample;    /* number of bits per sample of mono data */
    uint16        cbSize;            /* the count in bytes of the size of */
                                     /* extra information (after cbSize) */
};

struct DSBUFFERDESC
{
    uint32           dwSize;
    uint32           dwFlags;
    uint32           dwBufferBytes;
    uint32           dwReserved;
    WAVEFORMATEX*    lpwfxFormat;
};

struct DSCAPS
{
    uint32           dwSize;
    uint32           dwFlags;
    uint32           dwMinSecondarySampleRate;
    uint32           dwMaxSecondarySampleRate;
    uint32           dwPrimaryBuffers;
    uint32           dwMaxHwMixingAllBuffers;
    uint32           dwMaxHwMixingStaticBuffers;
    uint32           dwMaxHwMixingStreamingBuffers;
    uint32           dwFreeHwMixingAllBuffers;
    uint32           dwFreeHwMixingStaticBuffers;
    uint32           dwFreeHwMixingStreamingBuffers;
    uint32           dwMaxHw3DAllBuffers;
    uint32           dwMaxHw3DStaticBuffers;
    uint32           dwMaxHw3DStreamingBuffers;
    uint32           dwFreeHw3DAllBuffers;
    uint32           dwFreeHw3DStaticBuffers;
    uint32           dwFreeHw3DStreamingBuffers;
    uint32           dwTotalHwMemBytes;
    uint32           dwFreeHwMemBytes;
    uint32           dwMaxContigFreeHwMemBytes;
    uint32           dwUnlockTransferRateHwBuffers;
    uint32           dwPlayCpuOverheadSwBuffers;
    uint32           dwReserved1;
    uint32           dwReserved2;
};

struct DSBCAPS
{
    uint32           dwSize;
    uint32           dwFlags;
    uint32           dwBufferBytes;
    uint32           dwUnlockTransferRate;
    uint32           dwPlayCpuOverhead;
};

#define DSBSTATUS_PLAYING           0x00000001
#define DSBSTATUS_BUFFERLOST        0x00000002
#define DSBSTATUS_LOOPING           0x00000004
#define DSBSTATUS_LOCHARDWARE       0x00000008
#define DSBSTATUS_LOCSOFTWARE       0x00000010
#define DSBSTATUS_TERMINATED        0x00000020

#define DSBLOCK_FROMWRITECURSOR     0x00000001
#define DSBLOCK_ENTIREBUFFER        0x00000002

#define DSBPLAY_LOOPING             0x00000001

#define DSBVOLUME_MIN               -10000
#define DSBVOLUME_MAX               0

class FakePrimaryBuffer {
private:
    uint32 refs;
public:
    FakePrimaryBuffer(const DSBUFFERDESC* _desc);
    // IUnknown methods
    virtual int CCONV1(stdcall) QueryInterface(void*, void**) CCONV2(stdcall);
    virtual uint32 CCONV1(stdcall) AddRef() CCONV2(stdcall);
    virtual uint32 CCONV1(stdcall) Release() CCONV2(stdcall);

    // IDirectSoundBuffer methods
    virtual int CCONV1(stdcall) GetCaps(void*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetCurrentPosition(uint32*, uint32*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetFormat(WAVEFORMATEX* _fmt, uint32 isize, uint32* osize) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetVolume(int*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetPan(int*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetFrequency(uint32*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetStatus(uint32*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Initialize(void*, void*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Lock(uint32, uint32, void**, uint32*, void**, uint32*, uint32) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Play(uint32, uint32, uint32) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetCurrentPosition(uint32) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetFormat(const WAVEFORMATEX* _fmt) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetVolume(int) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetPan(int) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetFrequency(uint32) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Stop() CCONV2(stdcall);
    virtual int CCONV1(stdcall) Unlock(void*, uint32, void*, uint32) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Restore() CCONV2(stdcall);
};

class FakeBuffer
{
private:
    uint32 refs;
	DSBUFFERDESC desc;
	WAVEFORMATEX fmt;
	DSBCAPS caps;
	uint32 playposition;
	uint32 writeposition;
	bool loop;
	bool playing;
	char* buffer;
	bool attenuate;
	bool lpan;
	bool rpan;
	double volume;
	double pan;
public:
    FakeBuffer(const DSBUFFERDESC* _desc);
    // IUnknown methods
    virtual int CCONV1(stdcall) QueryInterface(void*, void**) CCONV2(stdcall);
    virtual uint32 CCONV1(stdcall) AddRef() CCONV2(stdcall);
    virtual uint32 CCONV1(stdcall) Release() CCONV2(stdcall);

    // IDirectSoundBuffer methods
    virtual int CCONV1(stdcall) GetCaps(DSBCAPS* _caps) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetCurrentPosition(uint32* play, uint32* write) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetFormat(WAVEFORMATEX* _fmt, uint32 isize, uint32* osize) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetVolume(int* plVolume) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetPan(int* plPan) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetFrequency(uint32* pdwFrequency) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetStatus(uint32* status) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Initialize(void* pDirectSound, void* pcDSBufferDesc) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Lock(uint32 offset, uint32 bytes, void** ptr1, uint32* bytes1, void** ptr2, uint32* bytes2, uint32 flags) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Play(uint32, uint32, uint32 flags) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetCurrentPosition(uint32 dwNewPosition) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetFormat(const WAVEFORMATEX* pcfxFormat) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetVolume(int lVolume) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetPan(int Pan) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetFrequency(uint32 dwFrequency) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Stop() CCONV2(stdcall);
    virtual int CCONV1(stdcall) Unlock(void* ptr1, uint32 bytes1, void* ptr2, uint32 bytes2)  CCONV2(stdcall);
    virtual int CCONV1(stdcall) Restore() CCONV2(stdcall);

	void Reset();
	void GetNextSample(int* _c1, int* _c2);
};

class FakeDirectSound
{
private:
    uint32 refs;
public:
    FakeDirectSound();

    // IUnknown methods
    virtual int CCONV1(stdcall) QueryInterface(void* a, void** b) CCONV2(stdcall);
    virtual uint32 CCONV1(stdcall) AddRef() CCONV2(stdcall);
    virtual uint32 CCONV1(stdcall) Release() CCONV2(stdcall);

    // IDirectSound methods
    virtual int CCONV1(stdcall) CreateSoundBuffer(const DSBUFFERDESC* desc, void** buffer, void*) CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetCaps(DSCAPS* pDSCaps) CCONV2(stdcall);
    virtual int CCONV1(stdcall) DuplicateSoundBuffer(void* pDSBufferOriginal, void** ppDSBufferDuplicate) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetCooperativeLevel(int hwnd, uint32 dwLevel) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Compact() CCONV2(stdcall);
    virtual int CCONV1(stdcall) GetSpeakerConfig(uint32* pdwSpeakerConfig) CCONV2(stdcall);
    virtual int CCONV1(stdcall) SetSpeakerConfig(uint32 dwSpeakerConfig) CCONV2(stdcall);
    virtual int CCONV1(stdcall) Initialize(void* pcGuidDevice) CCONV2(stdcall);
};

extern "C" {
int CCONV1(stdcall) FakeDSoundCreate(uint32, FakeDirectSound** iface,uint32) CCONV2(stdcall);
}
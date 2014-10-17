#include "stdafx.h"
#include "sdl/SDL.h"
#include <cstring>
using namespace std;

struct PixelFormat_t { 
    uint32 dwSize; 
    uint32 dwFlags; 
    uint32 dwFourCC; 
    uint32 dwRGBBitCount; 
    uint32 dwRBitMask; 
    uint32 dwGBitMask; 
    uint32 dwBBitMask; 
    uint32 dwRGBAlphaBitMask; 
};

struct ColorKey_t { 
    uint32 dwColorSpaceLowValue; 
    uint32 dwColorSpaceHighValue; 
};

struct SurfaceDesc_t {
    uint32          dwSize;
    uint32          dwFlags;
    uint32          dwHeight;
    uint32          dwWidth;
    int             lPitch;
    uint32          dwBackBufferCount;
    uint32          dwMipMapCount;
    uint32          dwAlphaBitDepth;
    uint32          dwReserved;
    void*           lpSurface;
    ColorKey_t      ddckCKDestOverlay;
    ColorKey_t      ddckCKDestBlt;
    ColorKey_t      ddckCKSrcOverlay;
    ColorKey_t      ddckCKSrcBlt;
    PixelFormat_t   ddpfPixelFormat;
    uint32          ddsCaps;
};

struct stream_t {
  uint16 width;
  uint16 height;
  uint8 *code_map;
  uint8 *back_buf1;
  uint8 *back_buf2;
};

static stream_t stream;

extern "C" void _cdecl FreeSurface(SDL_Surface* surf) {
	SDL_FreeSurface(surf);
}

extern "C" int _cdecl CreateSurface(SurfaceDesc_t *desc, SDL_Surface** surf) {
	*surf=SDL_CreateRGBSurface(SDL_SWSURFACE, desc->dwWidth, desc->dwHeight, desc->ddpfPixelFormat.dwRGBBitCount, desc->ddpfPixelFormat.dwRBitMask,
		desc->ddpfPixelFormat.dwGBitMask, desc->ddpfPixelFormat.dwBBitMask, desc->ddpfPixelFormat.dwRGBAlphaBitMask);
	if(!*surf) return -1;
	desc->lPitch=(*surf)->pitch;
	stream.width=desc->dwWidth;
	stream.height=desc->dwHeight;
	return 0;
}

extern "C" int _cdecl LockSurfaces(SDL_Surface* surf1, SDL_Surface* surf2, void** ptr1, void** ptr2) {
	SDL_LockSurface(surf1);
	SDL_LockSurface(surf2);
	*ptr1=surf1->pixels;
	*ptr2=surf2->pixels;
	return 1;
}

extern "C" void _cdecl UnlockSurfaces(SDL_Surface* surf1, SDL_Surface* surf2) {
	SDL_UnlockSurface(surf1);
	SDL_UnlockSurface(surf2);
}

extern "C" void _cdecl LockSingleSurface(SDL_Surface* surf, int rect[4], SurfaceDesc_t *desc) {
	SDL_LockSurface(surf);
	desc->lpSurface=surf->pixels;
	desc->lPitch=surf->pitch;
}

extern "C" void _cdecl UnlockSingleSurface(SDL_Surface* surf) {
	SDL_UnlockSurface(surf);
}

static int ipvideo_copy_block (const stream_t * s, unsigned char *frame, const unsigned char *src, int offset)
{
	int i;
	long frame_offset;

	frame_offset = frame - (uint8 *) s->back_buf1 + offset;

	for (i = 0; i < 8; ++i) {
		memcpy (frame, src, 8);
		frame += s->width;
		src += s->width;
	}

	return 0;
}

static int ipvideo_decode_0x2 (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	unsigned char B;
	int x, y;
	int offset;

	/* copy block from 2 frames ago using a motion vector */
	B = *(*data)++;

	if (B < 56) {
		x = 8 + (B % 7);
		y = B / 7;
	} else {
		x = -14 + ((B - 56) % 29);
		y = 8 + ((B - 56) / 29);
	}
	offset = y * s->width + x;

	return ipvideo_copy_block (s, frame, frame + offset, offset);
}

static int ipvideo_decode_0x3 (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	unsigned char B;
	int x, y;
	int offset;

	/* copy 8x8 block from current frame from an up/left block */
	B = *(*data)++;

	if (B < 56) {
		x = -(8 + (B % 7));
		y = -(B / 7);
	} else {
		x = -(-14 + ((B - 56) % 29));
		y = -(8 + ((B - 56) / 29));
	}
	offset = y * s->width + x;

	return ipvideo_copy_block (s, frame, frame + offset, offset);
}

static int ipvideo_decode_0x4 (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	unsigned char B;
	int x, y;
	int offset;

	/* copy a block from the previous frame */
	B = *(*data)++;
	x = -8 + (B & 0x0F);
	y = -8 + (B >> 4);
	offset = y * s->width + x;

	return ipvideo_copy_block (s, frame,
			frame + ((uint8 *) s->back_buf2 - (uint8 *) s->back_buf1) + offset, offset);
}

static int ipvideo_decode_0x5 (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	signed char x, y;
	int offset;

	/* copy a block from the previous frame using an expanded range */

	x = (signed char) *(*data)++;
	y = (signed char) *(*data)++;
	offset = y * s->width + x;

	return ipvideo_copy_block (s, frame,
			frame + ((uint8 *) s->back_buf2 - (uint8 *) s->back_buf1) + offset, offset);
}

static int ipvideo_decode_0x7 (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int x, y;
	unsigned char P0, P1;
	unsigned int flags;
	int bitmask;

	/* 2-color encoding */

	P0 = *(*data)++;
	P1 = *(*data)++;

	if (P0 <= P1) {

		/* need 8 more bytes from the stream */

		for (y = 0; y < 8; ++y) {
			flags = *(*data)++;
			for (x = 0x01; x <= 0x80; x <<= 1) {
				if (flags & x)
					*frame++ = P1;
				else
					*frame++ = P0;
			}
			frame += s->width - 8;
		}

	} else {

		/* need 2 more bytes from the stream */
		flags = ((*data)[1] << 8) | (*data)[0];
		(*data) += 2;
		bitmask = 0x0001;
		for (y = 0; y < 8; y += 2) {
			for (x = 0; x < 8; x += 2, bitmask <<= 1) {
				if (flags & bitmask) {
					*(frame + x) = P1;
					*(frame + x + 1) = P1;
					*(frame + s->width + x) = P1;
					*(frame + s->width + x + 1) = P1;
				} else {
					*(frame + x) = P0;
					*(frame + x + 1) = P0;
					*(frame + s->width + x) = P0;
					*(frame + s->width + x + 1) = P0;
				}
			}
			frame += s->width * 2;
		}
	}

	return 0;
}

static int ipvideo_decode_0x8 (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int x, y;
	unsigned char P[8];
	unsigned char B[8];
	unsigned int flags = 0;
	unsigned int bitmask = 0;
	unsigned char P0 = 0, P1 = 0;
	int lower_half = 0;

	/* 2-color encoding for each 4x4 quadrant, or 2-color encoding on
	 * either top and bottom or left and right halves */

	P[0] = (*data)[0];
	P[1] = (*data)[1];
	B[0] = (*data)[2];
	B[1] = (*data)[3];
	(*data) += 4;

	if (P[0] <= P[1]) {

		/* need 12 more bytes */

		P[2] = (*data)[0];
		P[3] = (*data)[1];
		B[2] = (*data)[2];
		B[3] = (*data)[3];
		P[4] = (*data)[4];
		P[5] = (*data)[5];
		B[4] = (*data)[6];
		B[5] = (*data)[7];
		P[6] = (*data)[8];
		P[7] = (*data)[9];
		B[6] = (*data)[10];
		B[7] = (*data)[11];
		(*data) += 12;

		flags =
				((B[0] & 0xF0) << 4) | ((B[4] & 0xF0) << 8) |
				((B[0] & 0x0F)) | ((B[4] & 0x0F) << 4) |
				((B[1] & 0xF0) << 20) | ((B[5] & 0xF0) << 24) |
				((B[1] & 0x0F) << 16) | ((B[5] & 0x0F) << 20);
		bitmask = 0x00000001;
		lower_half = 0; /* still on top half */

		for (y = 0; y < 8; ++y) {

			/* time to reload flags? */
			if (y == 4) {
				flags =
						((B[2] & 0xF0) << 4) | ((B[6] & 0xF0) << 8) |
						((B[2] & 0x0F)) | ((B[6] & 0x0F) << 4) |
						((B[3] & 0xF0) << 20) | ((B[7] & 0xF0) << 24) |
						((B[3] & 0x0F) << 16) | ((B[7] & 0x0F) << 20);
				bitmask = 0x00000001;
				lower_half = 2;
			}

			/* get the pixel values ready for this quadrant */
			P0 = P[lower_half + 0];
			P1 = P[lower_half + 1];

			for (x = 0; x < 8; ++x, bitmask <<= 1) {
				if (x == 4) {
					P0 = P[lower_half + 4];
					P1 = P[lower_half + 5];
				}

				if (flags & bitmask)
					*frame++ = P1;
				else
					*frame++ = P0;
			}
			frame += s->width - 8;
		}

	} else {

		/* need 8 more bytes */
		B[2] = (*data)[0];
		B[3] = (*data)[1];
		P[2] = (*data)[2];
		P[3] = (*data)[3];
		B[4] = (*data)[4];
		B[5] = (*data)[5];
		B[6] = (*data)[6];
		B[7] = (*data)[7];
		(*data) += 8;

		if (P[2] <= P[3]) {

			/* vertical split; left & right halves are 2-color encoded */

			flags = ((B[0] & 0xF0) << 4) | ((B[4] & 0xF0) << 8) |
				((B[0] & 0x0F)) | ((B[4] & 0x0F) << 4) |
				((B[1] & 0xF0) << 20) | ((B[5] & 0xF0) << 24) |
				((B[1] & 0x0F) << 16) | ((B[5] & 0x0F) << 20);
			bitmask = 0x00000001;

			for (y = 0; y < 8; ++y) {

				/* time to reload flags? */
				if (y == 4) {
					flags = ((B[2] & 0xF0) << 4) | ((B[6] & 0xF0) << 8) |
						((B[2] & 0x0F)) | ((B[6] & 0x0F) << 4) |
						((B[3] & 0xF0) << 20) | ((B[7] & 0xF0) << 24) |
						((B[3] & 0x0F) << 16) | ((B[7] & 0x0F) << 20);
					bitmask = 0x00000001;
				}

				/* get the pixel values ready for this half */
				P0 = P[0];
				P1 = P[1];

				for (x = 0; x < 8; ++x, bitmask <<= 1) {
					if (x == 4) {
						P0 = P[2];
						P1 = P[3];
					}

					if (flags & bitmask)
						*frame++ = P1;
					else
						*frame++ = P0;
				}
				frame += s->width - 8;
			}

		} else {

			/* horizontal split; top & bottom halves are 2-color encoded */

			P0 = P[0];
			P1 = P[1];

			for (y = 0; y < 8; ++y) {

				flags = B[y];
				if (y == 4) {
					P0 = P[2];
					P1 = P[3];
				}

				for (bitmask = 0x01; bitmask <= 0x80; bitmask <<= 1) {

					if (flags & bitmask)
						*frame++ = P1;
					else
						*frame++ = P0;
				}
				frame += s->width - 8;
			}
		}
	}

	return 0;
}

static int ipvideo_decode_0x9 (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int x, y;
	unsigned char P[4];
	unsigned char B[4];
	unsigned long flags = 0;
	int shifter = 0;
	unsigned char pix;

	/* 4-color encoding */

	P[0] = (*data)[0];
	P[1] = (*data)[1];
	P[2] = (*data)[2];
	P[3] = (*data)[3];
	(*data) += 4;

	if ((P[0] <= P[1]) && (P[2] <= P[3])) {

		/* 1 of 4 colors for each pixel, need 16 more bytes */

		for (y = 0; y < 8; ++y) {
			/* get the next set of 8 2-bit flags */
			flags = ((*data)[1] << 8) | (*data)[0];
			(*data) += 2;
			for (x = 0, shifter = 0; x < 8; ++x, shifter += 2) {
				*frame++ = P[(flags >> shifter) & 0x03];
			}
			frame += s->width - 8;
		}

	} else if ((P[0] <= P[1]) && (P[2] > P[3])) {

		/* 1 of 4 colors for each 2x2 block, need 4 more bytes */
		B[0] = (*data)[0];
		B[1] = (*data)[1];
		B[2] = (*data)[2];
		B[3] = (*data)[3];
		(*data) += 4;
		flags = (B[3] << 24) | (B[2] << 16) | (B[1] << 8) | B[0];
		shifter = 0;

		for (y = 0; y < 8; y += 2) {
			for (x = 0; x < 8; x += 2, shifter += 2) {
				pix = P[(flags >> shifter) & 0x03];
				*(frame + x) = pix;
				*(frame + x + 1) = pix;
				*(frame + s->width + x) = pix;
				*(frame + s->width + x + 1) = pix;
			}
			frame += s->width * 2;
		}

	} else if ((P[0] > P[1]) && (P[2] <= P[3])) {

		/* 1 of 4 colors for each 2x1 block, need 8 more bytes */

		for (y = 0; y < 8; ++y) {
			/* time to reload flags? */
			if ((y == 0) || (y == 4)) {
				B[0] = (*data)[0];
				B[1] = (*data)[1];
				B[2] = (*data)[2];
				B[3] = (*data)[3];
				(*data) += 4;
				flags = (B[3] << 24) | (B[2] << 16) | (B[1] << 8) | B[0];
				shifter = 0;
			}
			for (x = 0; x < 8; x += 2, shifter += 2) {
				pix = P[(flags >> shifter) & 0x03];
				*(frame + x) = pix;
				*(frame + x + 1) = pix;
			}
			frame += s->width;
		}

	} else {

		/* 1 of 4 colors for each 1x2 block, need 8 more bytes */

		for (y = 0; y < 8; y += 2) {
			/* time to reload flags? */
			if ((y == 0) || (y == 4)) {
				B[0] = (*data)[0];
				B[1] = (*data)[1];
				B[2] = (*data)[2];
				B[3] = (*data)[3];
				(*data) += 4;
				flags = (B[3] << 24) | (B[2] << 16) | (B[1] << 8) | B[0];
				shifter = 0;
			}
			for (x = 0; x < 8; ++x, shifter += 2) {
				pix = P[(flags >> shifter) & 0x03];
				*(frame + x) = pix;
				*(frame + s->width + x) = pix;
			}
			frame += s->width * 2;
		}
	}

	return 0;
}

static int ipvideo_decode_0xa (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int x, y;
	unsigned char P[16];
	unsigned char B[16];
	int flags = 0;
	int shifter = 0;
	int index;
	int split;
	int lower_half;

	/* 4-color encoding for each 4x4 quadrant, or 4-color encoding on
	 * either top and bottom or left and right halves */

	P[0] = (*data)[0];
	P[1] = (*data)[1];
	P[2] = (*data)[2];
	P[3] = (*data)[3];
	B[0] = (*data)[4];
	B[1] = (*data)[5];
	B[2] = (*data)[6];
	B[3] = (*data)[7];
	(*data) += 8;

	if (P[0] <= P[1]) {

		/* 4-color encoding for each quadrant; need 24 more bytes */

		for (y = 4; y < 16; y += 4) {
			for (x = y; x < y + 4; ++x)
				P[x] = *(*data)++;
			for (x = y; x < y + 4; ++x)
				B[x] = *(*data)++;
		}

		for (y = 0; y < 8; ++y) {

			lower_half = (y >= 4) ? 4 : 0;
			flags = (B[y + 8] << 8) | B[y];

			for (x = 0, shifter = 0; x < 8; ++x, shifter += 2) {
				split = (x >= 4) ? 8 : 0;
				index = split + lower_half + ((flags >> shifter) & 0x03);
				*frame++ = P[index];
			}

			frame += s->width - 8;
		}

	} else {

		/* 4-color encoding for either left and right or top and bottom
		 * halves; need 16 more bytes */

		B[4] = (*data)[0];
		B[5] = (*data)[1];
		B[6] = (*data)[2];
		B[7] = (*data)[3];
		P[4] = (*data)[4];
		P[5] = (*data)[5];
		P[6] = (*data)[6];
		P[7] = (*data)[7];
		(*data) += 8;
		memcpy (&B[8], *data, 8);
		(*data) += 8;

		if (P[4] <= P[5]) {

			/* block is divided into left and right halves */
			for (y = 0; y < 8; ++y) {

				flags = (B[y + 8] << 8) | B[y];
				split = 0;

				for (x = 0, shifter = 0; x < 8; ++x, shifter += 2) {
					if (x == 4)
						split = 4;
					*frame++ = P[split + ((flags >> shifter) & 0x03)];
				}

				frame += s->width - 8;
			}

		} else {

			/* block is divided into top and bottom halves */
			split = 0;
			for (y = 0; y < 8; ++y) {

				flags = (B[y * 2 + 1] << 8) | B[y * 2];
				if (y == 4)
					split = 4;

				for (x = 0, shifter = 0; x < 8; ++x, shifter += 2)
					*frame++ = P[split + ((flags >> shifter) & 0x03)];

				frame += s->width - 8;
			}
		}
	}

	return 0;
}

static int ipvideo_decode_0xb (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int y;

	/* 64-color encoding (each pixel in block is a different color) */

	for (y = 0; y < 8; ++y) {
		memcpy (frame, *data, 8);
		frame += s->width;
		(*data) += 8;
	}

	return 0;
}

static int ipvideo_decode_0xc (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int x, y;
	unsigned char pix;

	/* 16-color block encoding: each 2x2 block is a different color */

	for (y = 0; y < 8; y += 2) {
		for (x = 0; x < 8; x += 2) {
			pix = *(*data)++;
			*(frame + x) = pix;
			*(frame + x + 1) = pix;
			*(frame + s->width + x) = pix;
			*(frame + s->width + x + 1) = pix;
		}
		frame += s->width * 2;
	}

	return 0;
}

static int ipvideo_decode_0xd (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int x, y;
	unsigned char P[4];
	unsigned char index = 0;

	/* 4-color block encoding: each 4x4 block is a different color */

	P[0] = (*data)[0];
	P[1] = (*data)[1];
	P[2] = (*data)[2];
	P[3] = (*data)[3];
	(*data) += 4;

	for (y = 0; y < 8; ++y) {
		if (y < 4)
			index = 0;
		else
			index = 2;

		for (x = 0; x < 8; ++x) {
			if (x == 4)
				++index;
			*frame++ = P[index];
		}
		frame += s->width - 8;
	}

	return 0;
}

static int ipvideo_decode_0xe (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int y;
	unsigned char pix;

	/* 1-color encoding: the whole block is 1 solid color */
	pix = *(*data)++;

	for (y = 0; y < 8; ++y) {
		memset (frame, pix, 8);
		frame += s->width;
	}

	return 0;
}

static int ipvideo_decode_0xf (const stream_t * s, unsigned char *frame, const unsigned char **data)
{
	int x, y;
	unsigned char P[2];

	/* dithered encoding */

	P[0] = *(*data)++;
	P[1] = *(*data)++;

	for (y = 0; y < 8; ++y) {
		for (x = 0; x < 4; ++x) {
			*frame++ = P[y & 1];
			*frame++ = P[(y & 1) ^ 1];
		}
		frame += s->width - 8;
	}

	return 0;
}

static int
ipvideo_decode_frame8 (const stream_t * s, const unsigned char *data)
{
	int rc = 0;
	int x, y, xx, yy;
	int index = 0;
	unsigned char opcode;
	unsigned char *frame;

	frame = (uint8 *) s->back_buf1;

	/* decoding is done in 8x8 blocks */
	xx = s->width >> 3;
	yy = s->height >> 3;

	for (y = 0; y < yy; ++y) {
		for (x = 0; x < xx; ++x) {
			/* decoding map contains 4 bits of information per 8x8 block */
			/* bottom nibble first, then top nibble */
			if (index & 1)
				opcode = s->code_map[index >> 1] >> 4;
			else
				opcode = s->code_map[index >> 1] & 0x0F;
			++index;

			switch (opcode) {
				case 0x00:
					/* copy a block from the previous frame */
					rc = ipvideo_copy_block (s, frame,
							frame + ((uint8 *) s->back_buf2 - (uint8 *) s->back_buf1), 0);
					break;
				case 0x01:
					/* copy block from 2 frames ago; since we switched the back
					 * buffers we don't actually have to do anything here */
					break;
				case 0x02:
					rc = ipvideo_decode_0x2 (s, frame, &data);
					break;
				case 0x03:
					rc = ipvideo_decode_0x3 (s, frame, &data);
					break;
				case 0x04:
					rc = ipvideo_decode_0x4 (s, frame, &data);
					break;
				case 0x05:
					rc = ipvideo_decode_0x5 (s, frame, &data);
					break;
				case 0x06:
					/* mystery opcode? skip multiple blocks? */
					crash;
					break;
				case 0x07:
					rc = ipvideo_decode_0x7 (s, frame, &data);
					break;
				case 0x08:
					rc = ipvideo_decode_0x8 (s, frame, &data);
					break;
				case 0x09:
					rc = ipvideo_decode_0x9 (s, frame, &data);
					break;
				case 0x0a:
					rc = ipvideo_decode_0xa (s, frame, &data);
					break;
				case 0x0b:
					rc = ipvideo_decode_0xb (s, frame, &data);
					break;
				case 0x0c:
					rc = ipvideo_decode_0xc (s, frame, &data);
					break;
				case 0x0d:
					rc = ipvideo_decode_0xd (s, frame, &data);
					break;
				case 0x0e:
					rc = ipvideo_decode_0xe (s, frame, &data);
					break;
				case 0x0f:
					rc = ipvideo_decode_0xf (s, frame, &data);
					break;
			}

			if (rc != 0)
				return rc;

			frame += 8;
		}
		frame += 7 * s->width;
	}

	return 0;
}


extern "C" uint8 *_6b4033, *_6b4037;
extern "C" int _6b401b, _6b4017, _6b401f, _6b4023;
extern "C" void _cdecl _4f7359(uint8* blocktypes, uint8* data, short xoffset, short yoffset, short xsize, short ysize) {
	stream.back_buf1=_6b4033;
	stream.back_buf2=_6b4037;
	stream.code_map=blocktypes;
	_6b401b = xoffset*8;
	_6b4017 = xsize*8;
	_6b401f = yoffset*8;
	_6b4023 = ysize*8;
	ipvideo_decode_frame8(&stream, data);
}

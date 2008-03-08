/*
Copyright (C) 1994-1995 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Modifications for JonoF's port by Jonathon Fowler (jonof@edgenetwk.com)
*/
/**********************************************************************
   file:   _MULTIVC.H

   author: James R. Dose
   date:   December 20, 1993

   Private header for MULTIVOC.C

   (c) Copyright 1993 James R. Dose.  All Rights Reserved.
**********************************************************************/

#ifndef ___MULTIVC_H
#define ___MULTIVC_H

#include "openal.h"

#ifndef TRUE
#define TRUE  ( 1 == 1 )
#define FALSE ( !TRUE )
#endif

#define VOC_8BIT            0x0
#define VOC_CT4_ADPCM       0x1
#define VOC_CT3_ADPCM       0x2
#define VOC_CT2_ADPCM       0x3
#define VOC_16BIT           0x4
#define VOC_ALAW            0x6
#define VOC_MULAW           0x7
#define VOC_CREATIVE_ADPCM  0x200

#define T_SIXTEENBIT_STEREO 0
#define T_8BITS       1
#define T_MONO        2
#define T_16BITSOURCE 4
#define T_LEFTQUIET   8
#define T_RIGHTQUIET  16
#define T_DEFAULT     T_SIXTEENBIT_STEREO

#define MV_MaxPanPosition  31
#define MV_NumPanPositions ( MV_MaxPanPosition + 1 )
#define MV_MaxTotalVolume  255
//#define MV_MaxVolume       63
#define MV_NumVoices       8

#define MIX_VOLUME( volume ) \
   ( ( max( 0, min( ( volume ), 255 ) ) * ( MV_MaxVolume + 1 ) ) >> 8 )
//   ( ( max( 0, min( ( volume ), 255 ) ) ) >> 2 )

//#define SILENCE_16BIT     0x80008000
#define SILENCE_16BIT     0
#define SILENCE_8BIT      0x80808080
//#define SILENCE_16BIT_PAS 0

#if defined(_WIN32)
#define MixBufferSize     (MV_GetBufferSize(MV_RequestedMixRate))
#else
#define MixBufferSize     512
#endif

#define NumberOfBuffers   16
#define TotalBufferSize   ( MixBufferSize * NumberOfBuffers )

#define PI                3.1415926536

typedef enum
   {
   Raw,
   VOC,
   DemandFeed,
   WAV,
#ifdef USE_OPENAL
   OGG
#endif
   } wavedata;

typedef enum
   {
   NoMoreData,
   KeepPlaying
   } playbackstatus;


typedef struct VoiceNode
   {
   struct VoiceNode *next;
   struct VoiceNode *prev;

   wavedata      wavetype;
   char          bits;

   playbackstatus ( *GetSound )( struct VoiceNode *voice );

   void ( *mix )( unsigned int position, unsigned int rate,
      char *start, unsigned int length );

   char         *NextBlock;
   char         *LoopStart;
   char         *LoopEnd;
   unsigned      LoopCount;
   unsigned int LoopSize;
   unsigned int BlockLength;

   unsigned int PitchScale;
   unsigned int FixedPointBufferSize;

   char         *sound;
   unsigned int length;
   unsigned int SamplingRate;
   unsigned int RateScale;
   unsigned int position;
   int           Playing;

   int           handle;
   int           priority;

   void          ( *DemandFeed )( char **ptr, unsigned int *length );
#ifdef USE_OPENAL
   struct       sounddef OGGstream;
   char         *bufsnd;
   int          downsample;
#endif
   short        *LeftVolume;
   short        *RightVolume;

   unsigned int callbackval;

   } VoiceNode;

typedef struct
   {
   VoiceNode *start;
   VoiceNode *end;
   } VList;

typedef struct
   {
   char left;
   char right;
   } Pan;

typedef signed short MONO16;
typedef signed char  MONO8;

typedef struct
   {
   MONO16 left;
   MONO16 right;
//   unsigned short left;
//   unsigned short right;
   } STEREO16;

typedef struct
   {
   MONO16 left;
   MONO16 right;
   } SIGNEDSTEREO16;

typedef struct
   {
//   MONO8 left;
//   MONO8 right;
   char left;
   char right;
   } STEREO8;

typedef struct
   {
   char          RIFF[ 4 ];
   unsigned int file_size;
   char          WAVE[ 4 ];
   char          fmt[ 4 ];
   unsigned int format_size;
   } riff_header;

typedef struct
   {
   unsigned short wFormatTag;
   unsigned short nChannels;
   unsigned int  nSamplesPerSec;
   unsigned int  nAvgBytesPerSec;
   unsigned short nBlockAlign;
   unsigned short nBitsPerSample;
   } format_header;

typedef struct
   {
   unsigned char DATA[ 4 ];
   unsigned int size;
   } data_header;

typedef MONO8  VOLUME8[ 256 ];
typedef MONO16 VOLUME16[ 256 ];

typedef char HARSH_CLIP_TABLE_8[ MV_NumVoices * 256 ];

static unsigned MV_GetBufferSize(unsigned);

static void MV_Mix( VoiceNode *voice, int buffer );
static void MV_PlayVoice( VoiceNode *voice );
static void MV_StopVoice( VoiceNode *voice );
static int  MV_ServiceVoc( int );

static playbackstatus MV_GetNextVOCBlock( VoiceNode *voice );
static playbackstatus MV_GetNextDemandFeedBlock( VoiceNode *voice );
static playbackstatus MV_GetNextRawBlock( VoiceNode *voice );
static playbackstatus MV_GetNextWAVBlock( VoiceNode *voice );

// static void       MV_ServiceRecord( void );
static VoiceNode *MV_GetVoice( int handle );
static VoiceNode *MV_AllocVoice( int priority );

static short     *MV_GetVolumeTable( int vol );

static void       MV_SetVoiceMixMode( VoiceNode *voice );

static void       MV_SetVoicePitch( VoiceNode *voice, unsigned int rate, int pitchoffset );
static void       MV_CalcVolume( int MaxLevel );
static void       MV_CalcPanTable( void );

static void ClearBuffer_DW(void *ptr, int data, int length);

/*
#define ClearBuffer_DW( ptr, data, length ) \
	({ void *__ptr=(ptr); unsigned __data=(data); int __length=(length); \
	__asm__ __volatile__ ("rep; stosl" \
		: "+c" (__length), "+D" (__ptr) : "a" (__data) : "memory", "cc"); \
	0; })
*/
/*
#pragma aux ClearBuffer_DW = \
   "cld",                    \
   "push   es",              \
   "push   ds",              \
   "pop    es",              \
   "rep    stosd",           \
   "pop    es",              \
parm [ edi ] [ eax ] [ ecx ] modify exact [ ecx edi ];
*/

#if defined(__WATCOMC__)

#pragma aux MV_Mix8BitMono parm [eax] [edx] [ebx] [ecx]
#pragma aux MV_Mix8BitStereo parm [eax] [edx] [ebx] [ecx]
#pragma aux MV_Mix16BitMono parm [eax] [edx] [ebx] [ecx]
#pragma aux MV_Mix16BitStereo parm [eax] [edx] [ebx] [ecx]
#pragma aux MV_Mix16BitMono16 parm [eax] [edx] [ebx] [ecx]
#pragma aux MV_Mix8BitMono16 parm [eax] [edx] [ebx] [ecx]
#pragma aux MV_Mix8BitStereo16 parm [eax] [edx] [ebx] [ecx]
#pragma aux MV_Mix16BitStereo16 parm [eax] [edx] [ebx] [ecx]

#pragma aux MV_16BitReverb parm [eax] [edx] [ebx] [ecx] modify exact [eax ebx ecx edx esi edi]
#pragma aux MV_8BitReverb parm [eax] [edx] [ebx] [ecx] modify exact [eax ebx ecx edx esi edi]
#pragma aux MV_16BitReverbFast parm [eax] [edx] [ebx] [ecx] modify exact [eax ebx ecx edx esi edi]
#pragma aux MV_8BitReverbFast parm [eax] [edx] [ebx] [ecx] modify exact [eax ebx ecx edx esi edi]

#define CDEC

#elif defined(_MSC_VER)

#define CDEC _cdecl

#elif defined(_WIN32)

#define CDEC __cdecl

#else

#define CDEC

#endif


void CDEC MV_Mix8BitMono( unsigned int position, unsigned int rate,
   char *start, unsigned int length );

void CDEC MV_Mix8BitStereo( unsigned int position,
   unsigned int rate, char *start, unsigned int length );

void CDEC MV_Mix16BitMono( unsigned int position,
   unsigned int rate, char *start, unsigned int length );

void CDEC MV_Mix16BitStereo( unsigned int position,
   unsigned int rate, char *start, unsigned int length );

void CDEC MV_Mix16BitMono16( unsigned int position,
   unsigned int rate, char *start, unsigned int length );

void CDEC MV_Mix8BitMono16( unsigned int position, unsigned int rate,
   char *start, unsigned int length );

void CDEC MV_Mix8BitStereo16( unsigned int position,
   unsigned int rate, char *start, unsigned int length );

void CDEC MV_Mix16BitStereo16( unsigned int position,
   unsigned int rate, char *start, unsigned int length );

void CDEC MV_16BitReverb( char *src, char *dest, VOLUME16 *volume, int count );
void CDEC MV_8BitReverb( signed char *src, signed char *dest, VOLUME16 *volume, int count );
void CDEC MV_16BitReverbFast( char *src, char *dest, int count, int shift );
void CDEC MV_8BitReverbFast( signed char *src, signed char *dest, int count, int shift );

#undef CDEC

#endif

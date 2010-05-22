//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment
Copyright (C) 2000, 2003 - Matt Saettler (EDuke Enhancements)
Copyright (C) 2004, 2007 - EDuke32 developers

This file is part of EDuke32

EDuke32 is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
//-------------------------------------------------------------------------

#ifndef _m32script_h_
#define _m32script_h_

#include "compat.h"
#include "baselayer.h"
#include "build.h"
#include "editor.h"

#define MAXQUOTES 2048
#define MAXQUOTELEN 128

// Some misc #defines
#define NO       0
#define YES      1

typedef int32_t instype;
typedef int32_t ofstype;

extern char *ScriptQuotes[MAXQUOTES+1], *ScriptQuoteRedefinitions[MAXQUOTES+1];
extern int32_t g_numQuoteRedefinitions;

//extern int16_t neartagsector, neartagwall, neartagsprite;
//extern int32_t neartaghitdist;

extern int32_t VM_Execute(int32_t once);
extern void VM_OnEvent(register int32_t iEventID, register int32_t iActor);

extern void VM_ScriptInfo(void);
extern void VM_Disasm(ofstype beg, int32_t size);
extern void C_ReportError(int32_t iError);

extern int32_t Gv_NewVar(const char *pszLabel, intptr_t lValue, uint32_t dwFlags);
extern int32_t Gv_NewArray(const char *pszLabel, void *arrayptr, intptr_t asize, uint32_t dwFlags);
extern void Gv_Init(void);

extern int32_t __fastcall Gv_GetVarX(register int32_t id);
extern void __fastcall Gv_SetVarX(register int32_t id, register int32_t lValue);
extern int32_t __fastcall Gv_GetVarN(register int32_t id);  // 'N' for "no side-effects"... vars only!

extern void SetGAMEPalette(void);
extern void SetWATERPalette(void);
extern void SetSLIMEPalette(void);
extern void SetBOSS1Palette(void);

extern int32_t *constants, constants_allocsize;
extern int32_t g_numSavedConstants;

extern instype *script ,*insptr;
extern int32_t *labelval;
extern uint8_t *labeltype;
extern int32_t g_numLabels, g_numDefaultLabels;
extern int32_t g_scriptSize;
extern char *label;
//extern int32_t label_allocsize;

extern hashtable_t h_labels;

#define MAXLABELLEN 32

//extern uint8_t waterpal[768],slimepal[768],titlepal[768],drealms[768],endingpal[768],animpal[768];
//extern char currentboardfilename[BMAVM_PATH];


enum GameEvent_t {
    EVENT_ENTER3DMODE,
    EVENT_ANALYZESPRITES,
    EVENT_INSERTSPRITE2D,
    EVENT_INSERTSPRITE3D,
    EVENT_DRAW2DSCREEN,
    EVENT_KEYS2D,
    EVENT_KEYS3D,
    EVENT_PREKEYS2D,
    EVENT_PREKEYS3D,
    MAXEVENTS
};

extern ofstype aEventOffsets[MAXEVENTS];
extern int32_t aEventSizes[MAXEVENTS];
extern uint8_t aEventEnabled[MAXEVENTS];


enum GamevarFlags_t {
    MAXGAMEVARS        = 1024,       // must be a power of two between 256 and 4096, inclusive
    LOG2MAXGV          = 10,
    MAXVARLABEL        = MAXLABELLEN, //26,

    GAMEVAR_PERBLOCK   = 0x00000001, // per-block (state, event, or top-level) variable
    GAMEVAR_USER_MASK  = (0x00000001),

    GAMEVAR_RESET      = 0x00000008, // marks var for to default
    GAMEVAR_DEFAULT    = 0x00000100, // allow override
    GAMEVAR_SECRET     = 0x00000200, // don't dump...

    GAMEVAR_SYSTEM     = 0x00000800, // cannot change mode flags...(only default value)
    GAMEVAR_READONLY   = 0x00001000, // values are read-only (no setvar allowed)
    GAMEVAR_INTPTR     = 0x00002000, // plValues is a pointer to an int32_t
    GAMEVAR_FLOATPTR   = 0x00004000, // plValues is a pointer to a float
    GAMEVAR_SHORTPTR   = 0x00008000, // plValues is a pointer to a short
    GAMEVAR_CHARPTR    = 0x00010000, // plValues is a pointer to a char
//    GAMEVAR_NORESET    = 0x00020000, // var values are not reset when restoring map state
    GAMEVAR_SPECIAL    = 0x00040000, // flag for structure member shortcut vars
};

enum GamearrayFlags_t {
    MAXGAMEARRAYS      = (MAXGAMEVARS>>2), // must be lower than MAXGAMEVARS
    MAXARRAYLABEL      = MAXVARLABEL,
    GAMEARRAY_NORMAL   = 0,
    GAMEARRAY_READONLY = 0x00001000,

    GAMEARRAY_OFCHAR   = 0x00000001,
    GAMEARRAY_OFSHORT  = 0x00000002,
    GAMEARRAY_OFINT    = 0x00000004,
    GAMEARRAY_TYPEMASK = 0x00000007,

    GAMEARRAY_VARSIZE = 0x00000020,

    GAMEARRAY_RESET    = 0x00000008,
///    GAMEARRAY_NORESET  = 0x00000001,
};

typedef struct {
    union {
        intptr_t lValue;   // pointer when (dwFlags & GAMEVAR_*PTR)
        int32_t *plValues;     // array of values when (dwFlags & GAMEVAR_PERBLOCK)
    } val;
    intptr_t lDefault;
    char *szLabel;
    uint32_t dwFlags;
} gamevar_t;

typedef struct {
    char *szLabel;
    void *vals;     // array of values, type determined by (dwFlags & GAMEARRAY_TYPEMASK)
    uint32_t dwFlags;
    int32_t size;  // id to size gamevar when (dwFlags & GAMEARRAY_VARSIZE)
} gamearray_t;

extern gamevar_t aGameVars[MAXGAMEVARS];
extern gamearray_t aGameArrays[MAXGAMEARRAYS];
extern int32_t g_gameVarCount, g_systemVarCount;
extern int32_t g_gameArrayCount, g_systemArrayCount;

extern uint32_t m32_drawlinepat;


extern int32_t g_iReturnVar;
extern int32_t m32_sortvar1, m32_sortvar2;

//extern int32_t g_numRealPalettes;
//extern int32_t g_scriptDebug;

extern int32_t g_numQuoteRedefinitions;

extern hashtable_t h_gamevars;
extern hashtable_t h_arrays;
//extern hashtable_t h_keywords;
extern hashtable_t h_gamefuncs;


extern int16_t asksave;

extern vec3_t pos;
extern int16_t ang;
extern int32_t horiz;
extern int16_t cursectnum;
extern int32_t searchx, searchy;
/*
extern int16_t searchstat;
extern int16_t searchwall;
extern int16_t searchsector;
extern int16_t searchbottomwall;
*/
extern int16_t pointhighlight;
extern int16_t linehighlight;
extern int16_t highlightcnt;
extern int16_t highlightsectorcnt;
/*
extern int32_t temppicnum;
extern int32_t tempcstat;
extern int32_t templotag;
extern int32_t temphitag;
extern int32_t tempextra;
extern int32_t tempshade;
extern uint32_t temppal;
extern uint32_t tempvis;
extern uint32_t tempxrepeat;
extern uint32_t tempyrepeat;
*/
extern int32_t startposx;
extern int32_t startposy;
extern int32_t startposz;
extern int16_t startang;
extern int16_t startsectnum;

extern int32_t mousxplc;
extern int32_t mousyplc;

extern int16_t highlight[];
extern int16_t highlightsector[];

extern int32_t numsprites;

extern int32_t zoom;
extern int32_t halfxdim16, midydim16;


#define M32_FLAG_NEGATE (MAXGAMEVARS<<1)
#define M32_FLAG_ARRAY (MAXGAMEVARS<<2)
#define M32_FLAG_SPECIAL (MAXGAMEVARS<<3)

// IDs of special vars
#define M32_SPRITE_VAR_ID 0
#define M32_SECTOR_VAR_ID 1
#define M32_WALL_VAR_ID 2
#define M32_TSPRITE_VAR_ID 3

#define M32_THISACTOR_VAR_ID 4
#define M32_RETURN_VAR_ID 5
#define M32_LOTAG_VAR_ID 6
#define M32_HITAG_VAR_ID 7
#define M32_TEXTURE_VAR_ID 8

#define M32_PRINTERROR(Text, ...) OSD_Printf(OSD_ERROR "Line %d, %s: " Text "\n", g_errorLineNum, keyw[g_tw], ## __VA_ARGS__)

#endif

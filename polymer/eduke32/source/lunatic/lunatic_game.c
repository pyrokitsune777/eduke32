/* The Lunatic Interpreter, part of EDuke32. Game-side stuff. */

#include <stdint.h>

#include <lualib.h>
#include <lauxlib.h>

#include "build.h"  // printext256

#include "lunatic_game.h"

#include "osd.h"
#include "gamedef.h"  // EventNames[]


L_State g_ElState;


// this serves two purposes:
// the values as booleans and the addresses as keys to the Lua registry
uint8_t g_elEvents[MAXEVENTS];

// same thing for actors:
el_actor_t g_elActors[MAXTILES];

int32_t g_elCallDepth = 0;

// for timing events and actors
uint32_t g_eventCalls[MAXEVENTS], g_actorCalls[MAXTILES];
double g_eventTotalMs[MAXEVENTS], g_actorTotalMs[MAXTILES];


// forward-decls...
static int32_t SetEvent_luacf(lua_State *L);
static int32_t SetActor_luacf(lua_State *L);

// in lpeg.o
extern int luaopen_lpeg(lua_State *L);


typedef struct {
    uint32_t x, y, z, c;
} rng_jkiss_t;

// See: Good Practice in (Pseudo) Random Number Generation for
//      Bioinformatics Applications, by David Jones
ATTRIBUTE_OPTIMIZE("O2")
uint32_t rand_jkiss_u32(rng_jkiss_t *s)
{
    uint64_t t;
    s->x = 314527869 * s->x + 1234567;
    s->y ^= s->y << 5; s->y ^= s->y >> 7; s->y ^= s->y << 22;
    t = 4294584393ULL * s->z + s->c; s->c = t >> 32; s->z = t;
    return s->x + s->y + s->z;
}

ATTRIBUTE_OPTIMIZE("O2")
double rand_jkiss_dbl(rng_jkiss_t *s)
{
    double x;
    unsigned int a, b;
    a = rand_jkiss_u32(s) >> 6; /* Upper 26 bits */
    b = rand_jkiss_u32(s) >> 5; /* Upper 27 bits */
    x = (a * 134217728.0 + b) / 9007199254740992.0;
    return x;
}


void El_PrintTimes(void)
{
    int32_t i, maxlen=0;
    char buf[32];
    const char nn = Bstrlen("EVENT_");

    for (i=0; i<MAXEVENTS; i++)
    {
        int32_t len = Bstrlen(EventNames[i]+nn);
        Bassert(len < (int32_t)sizeof(buf));
        maxlen = max(len, maxlen);
    }

    OSD_Printf("{\n {\n");
    OSD_Printf("  -- event times, [event]={ total calls, total time [ms], mean time/call [us] }\n");
    for (i=0; i<MAXEVENTS; i++)
        if (g_eventCalls[i])
        {
            int32_t n=Bsprintf(buf, "%s", EventNames[i]+nn);

            for (; n<maxlen; n++)
                buf[n] = ' ';
            buf[maxlen] = 0;

            OSD_Printf("  [%s]={ %8d, %9.3f, %9.3f },\n",
                       buf, g_eventCalls[i], g_eventTotalMs[i],
                       1000*g_eventTotalMs[i]/g_eventCalls[i]);
        }

    OSD_Printf(" },\n\n {\n");
    OSD_Printf("  -- actor times, [tile]={ total calls, total time [ms], mean time/call [us] }\n");
    for (i=0; i<MAXTILES; i++)
        if (g_actorCalls[i])
            OSD_Printf("  [%5d]={ %8d, %9.3f, %9.3f },\n",
                       i, g_actorCalls[i], g_actorTotalMs[i],
                       1000*g_actorTotalMs[i]/g_actorCalls[i]);
    OSD_Printf(" },\n}\n");
}

////////// ERROR REPORTING //////////
#define EL_MAXERRORS 20
static int32_t el_numErrors=0, el_tooMuchErrors;
static char *el_errorMsgs[EL_MAXERRORS];

// Compare against all other error messages.
// Strictly seen, this is quadratic-time, but EL_MAXERRORS is small and
// errors should be fixed anyway.
static int32_t cmp_against_others(const char *str, int32_t slen)
{
    int32_t i;
    for (i=0; i<el_numErrors; i++)
        if (!Bstrncmp(str, el_errorMsgs[i], slen))
            return 1;
    return 0;
}

static void El_OnError(const char *str)
{
    if (!el_tooMuchErrors)
    {
        char *errstr = NULL;
        const char *nl = Bstrchr(str, '\n');

        // First, check whether the error message matches an already saved one
        if (nl)
        {
            // cut off string after the newline
            if (cmp_against_others(str, nl-str))
                return;
        }
        else
        {
            // save string fully
            if (cmp_against_others(str, Bstrlen(str)))
                return;
        }

        // If the (EL_MAXERRORS+1)'th distinct error appeared, we have too many.
        if (el_numErrors==EL_MAXERRORS)
        {
            el_tooMuchErrors = 1;
            return;
        }

        // Otherwise, allocate storage for the potentially clipped error string...
        if (nl)
        {
            errstr = Bmalloc(nl-str+1);
            if (errstr)
            {
                Bmemcpy(errstr, str, nl-str);
                errstr[nl-str] = 0;
            }
        }
        else
        {
            errstr = Bstrdup(str);
        }

        // ...and save it:
        if (errstr)
            el_errorMsgs[el_numErrors++] = errstr;
    }
}

void El_ClearErrors(void)
{
    int32_t i;
    for (i=0; i<EL_MAXERRORS; i++)
    {
        Bfree(el_errorMsgs[i]);
        el_errorMsgs[i] = NULL;
    }
    el_numErrors = el_tooMuchErrors = 0;
}

void El_DisplayErrors(void)
{
    int32_t i;
    for (i=0; i<el_numErrors; i++)
        printext256(8, 8+8*i, 242, 0, el_errorMsgs[i], 0);
    if (el_tooMuchErrors)
        printext256(8, 8+8*EL_MAXERRORS, 242, 0, "(more distinct errors ...)", 0);
}


////////// STATE CREATION/DESTRUCTIION //////////

static void El_StateSetup(lua_State *L)
{
    luaL_openlibs(L);  // NOTE: we set up the sandbox in defs.ilua
    luaopen_lpeg(L);
    lua_pop(L, lua_gettop(L));  // pop off whatever lpeg leaves on the stack

    L_SetupDebugTraceback(L);

    // create misc. global functions in the Lua state
    lua_pushcfunction(L, SetEvent_luacf);
    lua_setglobal(L, "gameevent_internal");
    lua_pushcfunction(L, SetActor_luacf);
    lua_setglobal(L, "gameactor_internal");

    Bassert(lua_gettop(L)==0);
}

// 0: success, <0: failure
int32_t El_CreateState(L_State *estate, const char *name)
{
    L_ErrorFunc = El_OnError;

    return L_CreateState(estate, name, &El_StateSetup);
}

void El_DestroyState(L_State *estate)
{
    L_DestroyState(estate);
}


////////// Lua_CFunctions //////////

// gameevent(EVENT_..., lua_function)
static int32_t SetEvent_luacf(lua_State *L)
{
    int32_t eventidx;

    Bassert(lua_gettop(L) == 2);
    eventidx = luaL_checkint(L, 1);
    Bassert((unsigned)eventidx < MAXEVENTS);

    L_CheckAndRegisterFunction(L, &g_elEvents[eventidx]);
    g_elEvents[eventidx] = 1;

    return 0;
}

// gameactor(actortile, strength, act, mov, movflags, lua_function)
static int32_t SetActor_luacf(lua_State *L)
{
    int32_t actortile, strength, movflags;
    const con_action_t *act;
    const con_move_t *mov;

    el_actor_t *a;

    Bassert(lua_gettop(L) == 6);

    actortile = luaL_checkint(L, 1);
    Bassert((unsigned)actortile < MAXTILES);

    strength = luaL_checkint(L, 2);
    movflags = luaL_checkint(L, 5);

    act = lua_topointer(L, 3);
    mov = lua_topointer(L, 4);

    a = &g_elActors[actortile];
    L_CheckAndRegisterFunction(L, a);
    a->haveit = 1;

    a->strength = strength;
    a->movflags = movflags;

    Bmemcpy(&a->act, act, sizeof(con_action_t));
    Bmemcpy(&a->mov, mov, sizeof(con_move_t));

    return 0;
}

//////////////////////////////

static int32_t call_regd_function3(lua_State *L, void *keyaddr,
                                   int32_t iActor, int32_t iPlayer, int32_t lDist)
{
    int32_t i;

    // get the Lua function from the registry
    lua_pushlightuserdata(L, keyaddr);
    lua_gettable(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, iActor);
    lua_pushinteger(L, iPlayer);
    lua_pushinteger(L, lDist);

    // -- call it! --

    i = lua_pcall(L, 3, 0, 0);
    if (i == LUA_ERRMEM)
    {
        lua_pop(L, 1);
        // XXX: should be more sophisticated.  Clean up stack? Do GC?
    }

    return i;
}

int32_t El_CallEvent(L_State *estate, int32_t eventidx, int32_t iActor, int32_t iPlayer, int32_t lDist)
{
    // XXX: estate must be the one where the events were registered...
    //      make a global?

    lua_State *const L = estate->L;
    int32_t i;

    g_elCallDepth++;
    i = call_regd_function3(L, &g_elEvents[eventidx], iActor, iPlayer, lDist);
    g_elCallDepth--;

    if (i == LUA_ERRRUN)
    {
        const char *errstr;

        if (lua_isboolean(L, -1))
        {
            lua_pop(L, 1);
            return 0;
        }

        errstr = lua_tostring(L, -1);
        Bassert(lua_type(L, -1)==LUA_TSTRING);
        OSD_Printf(OSD_ERROR "event \"%s\" (state \"%s\") runtime error: %s\n",
                   EventNames[eventidx], estate->name, errstr);
        if (L_ErrorFunc)
            L_ErrorFunc(errstr);
        lua_pop(L, 1);
        return -1;
    }

    return 0;
}

int32_t El_CallActor(L_State *estate, int32_t actortile, int32_t iActor, int32_t iPlayer, int32_t lDist)
{
    lua_State *const L = estate->L;
    int32_t i;

    g_elCallDepth++;
    i = call_regd_function3(L, &g_elActors[actortile], iActor, iPlayer, lDist);
    g_elCallDepth--;

    if (i == LUA_ERRRUN)
    {
        const char *errstr;

        if (lua_isboolean(L, -1))
        {
            int32_t killit = lua_toboolean(L, -1);
            lua_pop(L, 1);
            return killit;
        }

        Bassert(lua_type(L, -1)==LUA_TSTRING);
        errstr = lua_tostring(L, -1);
        OSD_Printf(OSD_ERROR "actor %d (sprite %d, state \"%s\") runtime error: %s\n",
                   actortile, iActor, estate->name, errstr);
        if (L_ErrorFunc)
            L_ErrorFunc(errstr);
        lua_pop(L, 1);
        return -1;
    }

    return 0;
}

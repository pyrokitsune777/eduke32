//------------------------------------- MD2/MD3 LIBRARY BEGINS -------------------------------------

#ifdef __POWERPC__
#define SHIFTMOD32(a) ((a)&31)
#else
#define SHIFTMOD32(a) (a)
#endif

typedef struct
{
    long mdnum; //VOX=1, MD2=2, MD3=3. NOTE: must be first in structure!
    long shadeoff;
    float scale, bscale, zadd;
    GLuint *texid;	// skins
} mdmodel;

typedef struct _mdanim_t
{
    int startframe, endframe;
    int fpssc, flags;
    struct _mdanim_t *next;
} mdanim_t;
#define MDANIM_LOOP 0
#define MDANIM_ONESHOT 1

typedef struct _mdskinmap_t
{
    unsigned char palette, filler[3]; // Build palette number
    int skinnum, surfnum;   // Skin identifier, surface number
    char *fn;   // Skin filename
    GLuint texid[HICEFFECTMASK+1];   // OpenGL texture numbers for effect variations
    struct _mdskinmap_t *next;
    float param;
} mdskinmap_t;


//This MD2 code is based on the source code from David Henry (tfc_duke(at)hotmail.com)
//   Was at http://tfc.duke.free.fr/us/tutorials/models/md2.htm
//   Available from http://web.archive.org/web/20030816010242/http://tfc.duke.free.fr/us/tutorials/models/md2.htm
//   Now at http://tfc.duke.free.fr/coding/md2.html (in French)
//He probably wouldn't recognize it if he looked at it though :)
typedef struct { float x, y, z; } point3d;

typedef struct
{
    long id, vers, skinxsiz, skinysiz, framebytes; //id:"IPD2", vers:8
    long numskins, numverts, numuv, numtris, numglcmds, numframes;
    long ofsskins, ofsuv, ofstris, ofsframes, ofsglcmds, ofseof; //ofsskins: skin names (64 bytes each)
} md2head_t;

typedef struct { unsigned char v[3], ni; } md2vert_t; //compressed vertex coords (x,y,z)
typedef struct
{
    point3d mul, add; //scale&translation vector
    char name[16];    //frame name
    md2vert_t verts[1]; //first vertex of this frame
} md2frame_t;

typedef struct { short u, v; } md2uv_t;
typedef struct
{
    unsigned short v[3];
    unsigned short u[3];
} md2tri_t;

typedef struct
{
    //WARNING: This top block is a union between md2model&md3model: Make sure it matches!
    long mdnum; //VOX=1, MD2=2, MD3=3. NOTE: must be first in structure!
    long shadeoff;
    float scale, bscale, zadd;
    GLuint *texid;   // texture ids for base skin if no mappings defined

    long numframes, cframe, nframe, fpssc, usesalpha;
    float oldtime, curtime, interpol;
    mdanim_t *animations;
    mdskinmap_t *skinmap;
    long numskins, skinloaded;   // set to 1+numofskin when a skin is loaded and the tex coords are modified,

    //MD2 specific stuff:
    long numverts, numglcmds, framebytes, *glcmds;
    char *frames;
    char *basepath;   // pointer to string of base path
    char *skinfn;   // pointer to first of numskins 64-char strings
    md2uv_t *uv;
    md2tri_t* tris;
} md2model;


typedef struct { char nam[64]; long i; } md3shader_t; //ascz path of shader, shader index
typedef struct { long i[3]; } md3tri_t; //indices of tri
typedef struct { float u, v; } md3uv_t;
typedef struct { signed short x, y, z; unsigned char nlat, nlng; } md3xyzn_t; //xyz are [10:6] ints

typedef struct
{
    point3d min, max, cen; //bounding box&origin
    float r; //radius of bounding sphere
    char nam[16]; //ascz frame name
} md3frame_t;

typedef struct
{
    char nam[64]; //ascz tag name
    point3d p, x, y, z; //tag object pos&orient
} md3tag_t;

typedef struct
{
    long id; //IDP3(0x33806873)
    char nam[64]; //ascz surface name
    long flags; //?
    long numframes, numshaders, numverts, numtris; //numframes same as md3head,max shade=~256,vert=~4096,tri=~8192
    md3tri_t *tris;       //file format: rel offs from md3surf
    md3shader_t *shaders; //file format: rel offs from md3surf
    md3uv_t *uv;          //file format: rel offs from md3surf
    md3xyzn_t *xyzn;      //file format: rel offs from md3surf
    long ofsend;
} md3surf_t;

typedef struct
{
    long id, vers; //id=IDP3(0x33806873), vers=15
    char nam[64]; //ascz path in PK3
    long flags; //?
    long numframes, numtags, numsurfs, numskins; //max=~1024,~16,~32,numskins=artifact of MD2; use shader field instead
    md3frame_t *frames; //file format: abs offs
    md3tag_t *tags;     //file format: abs offs
    md3surf_t *surfs;   //file format: abs offs
    long eof;           //file format: abs offs
} md3head_t;

typedef struct
{
    //WARNING: This top block is a union between md2model&md3model: Make sure it matches!
    long mdnum; //VOX=1, MD2=2, MD3=3. NOTE: must be first in structure!
    long shadeoff;
    float scale, bscale, zadd;
    unsigned int *texid;   // texture ids for base skin if no mappings defined

    long numframes, cframe, nframe, fpssc, usesalpha;
    float oldtime, curtime, interpol;
    mdanim_t *animations;
    mdskinmap_t *skinmap;
    long numskins, skinloaded;   // set to 1+numofskin when a skin is loaded and the tex coords are modified,

    //MD3 specific
    md3head_t head;
    point3d *muladdframes;
} md3model;

#define VOXBORDWIDTH 1 //use 0 to save memory, but has texture artifacts; 1 looks better...
#define VOXUSECHAR 0
#if (VOXUSECHAR != 0)
typedef struct { unsigned char x, y, z, u, v; } vert_t;
#else
typedef struct { unsigned short x, y, z, u, v; } vert_t;
#endif
typedef struct { vert_t v[4]; } voxrect_t;
typedef struct
{
    //WARNING: This top block is a union of md2model,md3model,voxmodel: Make sure it matches!
    long mdnum; //VOX=1, MD2=2, MD3=3. NOTE: must be first in structure!
    long shadeoff;
    float scale, bscale, zadd;
    unsigned int *texid;    // skins for palettes

    //VOX specific stuff:
    voxrect_t *quad; long qcnt, qfacind[7];
    long *mytex, mytexx, mytexy;
    long xsiz, ysiz, zsiz;
    float xpiv, ypiv, zpiv;
    long is8bit;
} voxmodel;
static voxmodel *voxmodels[MAXVOXELS];

typedef struct
{ // maps build tiles to particular animation frames of a model
    int modelid;
    int skinnum;
    int framenum;   // calculate the number from the name when declaring
} tile2model_t;
static tile2model_t tile2model[MAXTILES];

//Move this to appropriate place!
typedef struct { float xadd, yadd, zadd; short angadd, flags; } hudtyp;
hudtyp hudmem[2][MAXTILES]; //~320KB ... ok for now ... could replace with dynamic alloc

static char mdinited=0;

#define MODELALLOCGROUP 256
static long nummodelsalloced = 0, nextmodelid = 0;
static mdmodel **models = NULL;

static long maxmodelverts = 0, allocmodelverts = 0;
static point3d *vertlist = NULL; //temp array to store interpolated vertices for drawing

mdmodel *mdload (const char *);
int mddraw (spritetype *);
void mdfree (mdmodel *);

extern int timerticspersec;

static void freeallmodels ()
{
    int i;

    if (models)
    {
        for (i=0;i<nextmodelid;i++) mdfree(models[i]);
        free(models); models = NULL;
        nummodelsalloced = 0;
        nextmodelid = 0;
    }

    memset(tile2model,-1,sizeof(tile2model));

    if (vertlist)
    {
        free(vertlist);
        vertlist = NULL;
        allocmodelverts = maxmodelverts = 0;
    }
}

static void clearskins ()
{
    mdmodel *m;
    int i, j;

    for (i=0;i<nextmodelid;i++)
    {
        m = models[i];
        if (m->mdnum == 1) {
            voxmodel *v = (voxmodel*)m;
            for (j=0;j<MAXPALOOKUPS;j++) {
                if (v->texid[j]) bglDeleteTextures(1,(GLuint*)&v->texid[j]);
                v->texid[j] = 0;
            }
        } else if (m->mdnum == 2 || m->mdnum == 3) {
            md2model *m2 = (md2model*)m;
            mdskinmap_t *sk;
            for (j=0;j<m2->numskins*(HICEFFECTMASK+1);j++)
            {
                if (m2->texid[j]) bglDeleteTextures(1,(GLuint*)&m2->texid[j]);
                m2->texid[j] = 0;
            }

            for (sk=m2->skinmap;sk;sk=sk->next)
                for (j=0;j<(HICEFFECTMASK+1);j++)
                {
                    if (sk->texid[j]) bglDeleteTextures(1,(GLuint*)&sk->texid[j]);
                    sk->texid[j] = 0;
                }
        }
    }

    for (i=0;i<MAXVOXELS;i++)
    {
        voxmodel *v = (voxmodel*)voxmodels[i]; if (!v) continue;
        for (j=0;j<MAXPALOOKUPS;j++) {
            if (v->texid[j]) bglDeleteTextures(1,(GLuint*)&v->texid[j]);
            v->texid[j] = 0;
        }
    }
}

static void mdinit ()
{
    memset(hudmem,0,sizeof(hudmem));
    freeallmodels();
    mdinited = 1;
}

int md_loadmodel (const char *fn)
{
    mdmodel *vm, **ml;

    if (!mdinited) mdinit();

    if (nextmodelid >= nummodelsalloced)
    {
        ml = (mdmodel **)realloc(models,(nummodelsalloced+MODELALLOCGROUP)*4); if (!ml) return(-1);
        models = ml; nummodelsalloced += MODELALLOCGROUP;
    }

    vm = mdload(fn); if (!vm) return(-1);
    models[nextmodelid++] = vm;
    return(nextmodelid-1);
}

int md_setmisc (int modelid, float scale, int shadeoff, float zadd)
{
    mdmodel *m;

    if (!mdinited) mdinit();

    if ((unsigned long)modelid >= (unsigned long)nextmodelid) return -1;
    m = models[modelid];
    m->bscale = scale;
    m->shadeoff = shadeoff;
    m->zadd = zadd;

    return 0;
}

int md_tilehasmodel (int tilenume)
{
    if (!mdinited) return -1;
    return tile2model[tilenume].modelid;
}

static long framename2index (mdmodel *vm, const char *nam)
{
    int i = 0;

    switch (vm->mdnum)
    {
    case 2:
    {
        md2model *m = (md2model *)vm;
        md2frame_t *fr;
        for (i=0;i<m->numframes;i++)
        {
            fr = (md2frame_t *)&m->frames[i*m->framebytes];
            if (!Bstrcmp(fr->name, nam)) break;
        }
    }
    break;
    case 3:
    {
        md3model *m = (md3model *)vm;
        for (i=0;i<m->numframes;i++)
            if (!Bstrcmp(m->head.frames[i].nam,nam)) break;
    }
    break;
    }
    return(i);
}

int md_defineframe (int modelid, const char *framename, int tilenume, int skinnum)
{
    void *vm;
    md2model *m;
    int i;

    if (!mdinited) mdinit();

    if ((unsigned long)modelid >= (unsigned long)nextmodelid) return(-1);
    if ((unsigned long)tilenume >= (unsigned long)MAXTILES) return(-2);
    if (!framename) return(-3);

    m = (md2model *)models[modelid];
    if (m->mdnum == 1) {
        tile2model[tilenume].modelid = modelid;
        tile2model[tilenume].framenum = tile2model[tilenume].skinnum = 0;
        return 0;
    }

    i = framename2index((mdmodel*)m,framename);
    if (i == m->numframes) return(-3);   // frame name invalid

    tile2model[tilenume].modelid = modelid;
    tile2model[tilenume].framenum = i;
    tile2model[tilenume].skinnum = skinnum;

    return 0;
}

int md_defineanimation (int modelid, const char *framestart, const char *frameend, int fpssc, int flags)
{
    md2model *m;
    mdanim_t ma, *map;
    int i;

    if (!mdinited) mdinit();

    if ((unsigned long)modelid >= (unsigned long)nextmodelid) return(-1);

    memset(&ma, 0, sizeof(ma));
    m = (md2model *)models[modelid];
    if (m->mdnum < 2) return 0;

    //find index of start frame
    i = framename2index((mdmodel*)m,framestart);
    if (i == m->numframes) return -2;
    ma.startframe = i;

    //find index of finish frame which must trail start frame
    i = framename2index((mdmodel*)m,frameend);
    if (i == m->numframes) return -3;
    ma.endframe = i;

    ma.fpssc = fpssc;
    ma.flags = flags;

    map = (mdanim_t*)calloc(1,sizeof(mdanim_t));
    if (!map) return(-4);
    memcpy(map, &ma, sizeof(ma));

    map->next = m->animations;
    m->animations = map;

    return(0);
}

int md_defineskin (int modelid, const char *skinfn, int palnum, int skinnum, int surfnum, float param)
{
    mdskinmap_t *sk, *skl;
    md2model *m;

    if (!mdinited) mdinit();

    if ((unsigned long)modelid >= (unsigned long)nextmodelid) return -1;
    if (!skinfn) return -2;
    if ((unsigned)palnum >= (unsigned)MAXPALOOKUPS) return -3;

    m = (md2model *)models[modelid];
    if (m->mdnum < 2) return 0;
    if (m->mdnum == 2) surfnum = 0;

    skl = NULL;
    for (sk = m->skinmap; sk; skl = sk, sk = sk->next)
        if (sk->palette == (unsigned char)palnum && skinnum == sk->skinnum && surfnum == sk->surfnum) break;
    if (!sk) {
        sk = (mdskinmap_t *)calloc(1,sizeof(mdskinmap_t));
        if (!sk) return -4;

        if (!skl) m->skinmap = sk;
        else skl->next = sk;
    } else if (sk->fn) free(sk->fn);

    sk->palette = (unsigned char)palnum;
    sk->skinnum = skinnum;
    sk->surfnum = surfnum;
    sk->param = param;
    sk->fn = (char *)malloc(strlen(skinfn)+1);
    if (!sk->fn) return(-4);
    strcpy(sk->fn, skinfn);

    return 0;
}

int md_definehud (int modelid, int tilex, double xadd, double yadd, double zadd, double angadd, int flags)
{
    if (!mdinited) mdinit();

    if ((unsigned long)modelid >= (unsigned long)nextmodelid) return -1;
    if ((unsigned long)tilex >= (unsigned long)MAXTILES) return -2;

    hudmem[(flags>>2)&1][tilex].xadd = xadd;
    hudmem[(flags>>2)&1][tilex].yadd = yadd;
    hudmem[(flags>>2)&1][tilex].zadd = zadd;
    hudmem[(flags>>2)&1][tilex].angadd = ((short)angadd)|2048;
    hudmem[(flags>>2)&1][tilex].flags = (short)flags;

    return 0;
}

int md_undefinetile(int tile)
{
    if (!mdinited) return 0;
    if ((unsigned)tile >= (unsigned)MAXTILES) return -1;

    tile2model[tile].modelid = -1;
    return 0;
}

int md_undefinemodel(int modelid)
{
    int i;
    if (!mdinited) return 0;
    if ((unsigned long)modelid >= (unsigned long)nextmodelid) return -1;

    for (i=MAXTILES-1; i>=0; i--)
        if (tile2model[i].modelid == modelid)
            tile2model[i].modelid = -1;

    if (models) {
        mdfree(models[modelid]);
        models[modelid] = NULL;
    }

    return 0;
}

static int daskinloader (long filh, long *fptr, long *bpl, long *sizx, long *sizy, long *osizx, long *osizy, char *hasalpha, char effect)
{
    long picfillen, j,y,x;
    char *picfil,*cptr,al=255;
    coltype *pic;
    long xsiz, ysiz, tsizx, tsizy;

    picfillen = kfilelength(filh);
    picfil = (char *)malloc(picfillen); if (!picfil) { return -1; }
    kread(filh, picfil, picfillen);

    // tsizx/y = replacement texture's natural size
    // xsiz/y = 2^x size of replacement

    kpgetdim(picfil,picfillen,&tsizx,&tsizy);
    if (tsizx == 0 || tsizy == 0) { free(picfil); return -1; }

    if (!glinfo.texnpot) {
        for (xsiz=1;xsiz<tsizx;xsiz+=xsiz);
        for (ysiz=1;ysiz<tsizy;ysiz+=ysiz);
    } else {
        xsiz = tsizx;
        ysiz = tsizy;
    }
    *osizx = tsizx; *osizy = tsizy;
    pic = (coltype *)malloc(xsiz*ysiz*sizeof(coltype));
    if (!pic) { free(picfil); return -1; }
    memset(pic,0,xsiz*ysiz*sizeof(coltype));

    if (kprender(picfil,picfillen,(long)pic,xsiz*sizeof(coltype),xsiz,ysiz,0,0))
    { free(picfil); free(pic); return -1; }
    free(picfil);

    cptr = &britable[gammabrightness ? 0 : curbrightness][0];
    for (y=0,j=0;y<tsizy;y++,j+=xsiz)
    {
        coltype *rpptr = &pic[j], tcol;

        for (x=0;x<tsizx;x++)
        {
            tcol.b = cptr[rpptr[x].b];
            tcol.g = cptr[rpptr[x].g];
            tcol.r = cptr[rpptr[x].r];

            if (effect & 1) {
                // greyscale
                tcol.b = max(tcol.b, max(tcol.g, tcol.r));
                tcol.g = tcol.r = tcol.b;
            }
            if (effect & 2) {
                // invert
                tcol.b = 255-tcol.b;
                tcol.g = 255-tcol.g;
                tcol.r = 255-tcol.r;
            }

            rpptr[x].b = tcol.b;
            rpptr[x].g = tcol.g;
            rpptr[x].r = tcol.r;
            al &= rpptr[x].a;
        }
    }
    if (!glinfo.bgra) {
        for (j=xsiz*ysiz-1;j>=0;j--) {
            swapchar(&pic[j].r, &pic[j].b);
        }
    }

    *sizx = xsiz;
    *sizy = ysiz;
    *bpl = xsiz;
    *fptr = (long)pic;
    *hasalpha = (al != 255);
    return 0;
}

// JONOF'S COMPRESSED TEXTURE CACHE STUFF ---------------------------------------------------
long mdloadskin_trytexcache(char *fn, long len, char effect, texcacheheader *head)
{
    long fil, fp;
    char cachefn[BMAX_PATH], *cp;
    unsigned char mdsum[16];

    if (!glinfo.texcompr || !glusetexcompr || !glusetexcache) return -1;
    if (!bglCompressedTexImage2DARB || !bglGetCompressedTexImageARB) {
        // lacking the necessary extensions to do this
        initprintf("Warning: the GL driver lacks necessary functions to use caching\n");
        glusetexcache = 0;
        return -1;
    }

    md4once((unsigned char *)fn, strlen(fn), mdsum);
    for (cp = cachefn, fp = 0; (*cp = TEXCACHEDIR[fp]); cp++,fp++);
    *(cp++) = '/';
    for (fp = 0; fp < 16; phex(mdsum[fp++], cp), cp+=2);
    sprintf(cp, "-%lx-0%x", len, effect);

    fil = kopen4load(cachefn, 0);
    if (fil < 0) return -1;

    /* initprintf("Loading cached skin: %s\n", cachefn); */

    if (kread(fil, head, sizeof(texcacheheader)) < (int)sizeof(texcacheheader)) goto failure;
    if (memcmp(head->magic, "Polymost", 8)) goto failure;

    head->xdim = B_LITTLE32(head->xdim);
    head->ydim = B_LITTLE32(head->ydim);
    head->flags = B_LITTLE32(head->flags);

    if (!glinfo.texnpot && (head->flags & 1)) goto failure;

    return fil;
failure:
    kclose(fil);
    return -1;
}

static long mdloadskin_cached(long fil, texcacheheader *head, long *doalloc, GLuint *glpic, long *xsiz, long *ysiz)
{
    int level, r;
    texcachepicture pict;
    void *pic = NULL, *packbuf = NULL;
    void *midbuf = NULL;
    long alloclen=0;

    if (*doalloc&1) {
        bglGenTextures(1,glpic);  //# of textures (make OpenGL allocate structure)
        *doalloc |= 2;	// prevents bglGenTextures being called again if we fail in here
    }
    bglBindTexture(GL_TEXTURE_2D,*glpic);

    bglGetError();

    // load the mipmaps
    for (level = 0; level==0 || (pict.xdim > 1 || pict.ydim > 1); level++) {
        r = kread(fil, &pict, sizeof(texcachepicture));
        if (r < (int)sizeof(texcachepicture)) goto failure;

        pict.size = B_LITTLE32(pict.size);
        pict.format = B_LITTLE32(pict.format);
        pict.xdim = B_LITTLE32(pict.xdim);
        pict.ydim = B_LITTLE32(pict.ydim);
        pict.border = B_LITTLE32(pict.border);
        pict.depth = B_LITTLE32(pict.depth);

    if (level == 0) { *xsiz = pict.xdim; *ysiz = pict.ydim; }

        if (alloclen < pict.size) {
            void *picc = realloc(pic, pict.size);
            if (!picc) goto failure; else pic = picc;
            alloclen = pict.size;

            picc = realloc(packbuf, alloclen+16);
            if (!picc) goto failure; else packbuf = picc;

            picc = realloc(midbuf, pict.size);
            if (!picc) goto failure; else midbuf = picc;
        }

        if (dedxtfilter(fil, &pict, pic, midbuf, packbuf, (head->flags&4)==4)) goto failure;

        bglCompressedTexImage2DARB(GL_TEXTURE_2D,level,pict.format,pict.xdim,pict.ydim,pict.border,
                                   pict.size,pic);
        if (bglGetError() != GL_NO_ERROR) goto failure;
    }

    if (midbuf) free(midbuf);
    if (pic) free(pic);
    if (packbuf) free(packbuf);
    return 0;
failure:
    if (midbuf) free(midbuf);
    if (pic) free(pic);
    if (packbuf) free(packbuf);
    return -1;
}
// --------------------------------------------------- JONOF'S COMPRESSED TEXTURE CACHE STUFF

//Note: even though it says md2model, it works for both md2model&md3model
static long mdloadskin (md2model *m, int number, int pal, int surf)
{
    long i,j, fptr=0, bpl, xsiz=0, ysiz=0, osizx, osizy, texfmt = GL_RGBA, intexfmt = GL_RGBA;
    char *skinfile, hasalpha, fn[BMAX_PATH+65];
    GLuint *texidx = NULL;
    mdskinmap_t *sk, *skzero = NULL;
    long doalloc = 1, filh;

    long cachefil = -1, picfillen;
    texcacheheader cachead;

    if (m->mdnum == 2) surf = 0;

    if ((unsigned)pal >= (unsigned)MAXPALOOKUPS) return 0;
    i = -1;
    for (sk = m->skinmap; sk; sk = sk->next)
    {
        if ((int)sk->palette == pal && sk->skinnum == number && sk->surfnum == surf)
        {
            skinfile = sk->fn;
            texidx = &sk->texid[ hictinting[pal].f ];
            strcpy(fn,skinfile);
            //OSD_Printf("Using exact match skin (pal=%d,skinnum=%d,surfnum=%d) %s\n",pal,number,surf,skinfile);
            break;
        }
        //If no match, give highest priority to number, then pal.. (Parkar's request, 02/27/2005)
        else if (((int)sk->palette ==   0) && (sk->skinnum == number) && (sk->surfnum == surf) && (i < 5)) { i = 5; skzero = sk; }
        else if (((int)sk->palette == pal) && (sk->skinnum ==      0) && (sk->surfnum == surf) && (i < 4)) { i = 4; skzero = sk; }
        else if (((int)sk->palette ==   0) && (sk->skinnum ==      0) && (sk->surfnum == surf) && (i < 3)) { i = 3; skzero = sk; }
        else if (((int)sk->palette ==   0) && (sk->skinnum == number) && (i < 2)) { i = 2; skzero = sk; }
        else if (((int)sk->palette == pal) && (sk->skinnum ==      0) && (i < 1)) { i = 1; skzero = sk; }
        else if (((int)sk->palette ==   0) && (sk->skinnum ==      0) && (i < 0)) { i = 0; skzero = sk; }
    }
    if (!sk)
    {
        if (pal >= (MAXPALOOKUPS - RESERVEDPALS))
            return (0);
        if (skzero)
        {
            skinfile = skzero->fn;
            texidx = &skzero->texid[ hictinting[pal].f ];
            strcpy(fn,skinfile);
            //OSD_Printf("Using def skin 0,0 as fallback, pal=%d\n", pal);
        }
        else
        {
            if ((unsigned)number >= (unsigned)m->numskins) number = 0;
            skinfile = m->skinfn + number*64;
            texidx = &m->texid[ number * (HICEFFECTMASK+1) + hictinting[pal].f ];
            strcpy(fn,m->basepath); strcat(fn,skinfile);
            //OSD_Printf("Using MD2/MD3 skin (%d) %s, pal=%d\n",number,skinfile,pal);
        }
    }
    if (!skinfile[0]) return 0;

    if (*texidx) return *texidx;

    // possibly fetch an already loaded multitexture :_)
    if (pal >= (MAXPALOOKUPS - RESERVEDPALS))
        for (i=0;i<nextmodelid;i++)
            for (skzero = ((md2model *)models[i])->skinmap; skzero; skzero = skzero->next)
                if (!Bstrcasecmp(skzero->fn, sk->fn) && skzero->texid[hictinting[pal].f])
                {
                    sk->texid[hictinting[pal].f] = skzero->texid[hictinting[pal].f];
                    return sk->texid[hictinting[pal].f];
                }

    *texidx = 0;

    if ((filh = kopen4load(fn, 0)) < 0) {
        initprintf("Skin %s not found.\n",fn);
        skinfile[0] = 0;
        return 0;
    }

    picfillen = kfilelength(filh);
    kclose(filh);	// FIXME: shouldn't have to do this. bug in cache1d.c

    cachefil = mdloadskin_trytexcache(fn, picfillen, hictinting[pal].f, &cachead);
    if (cachefil >= 0 && !mdloadskin_cached(cachefil, &cachead, &doalloc, texidx, &xsiz, &ysiz)) {
        osizx = cachead.xdim;
        osizy = cachead.ydim;
        m->usesalpha = hasalpha = (cachead.flags & 2) ? 1 : 0;
        kclose(cachefil);
        //kclose(filh);	// FIXME: uncomment when cache1d.c is fixed
        // cachefil >= 0, so it won't be rewritten
    } else {
        if (cachefil >= 0) kclose(cachefil);
        cachefil = -1;	// the compressed version will be saved to disk

        if ((filh = kopen4load(fn, 0)) < 0) return -1;
        if (daskinloader(filh,&fptr,&bpl,&xsiz,&ysiz,&osizx,&osizy,&hasalpha,hictinting[pal].f))
        {
            kclose(filh);
            initprintf("Failed loading skin file \"%s\"\n", fn);
            skinfile[0] = 0;
            return(0);
        } else kclose(filh);
        m->usesalpha = hasalpha;

        if ((doalloc&3)==1) bglGenTextures(1,(GLuint*)texidx);
        bglBindTexture(GL_TEXTURE_2D,*texidx);

        //gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,xsiz,ysiz,GL_BGRA_EXT,GL_UNSIGNED_BYTE,(char *)fptr);
        if (glinfo.texcompr && glusetexcompr) intexfmt = hasalpha ? GL_COMPRESSED_RGBA_ARB : GL_COMPRESSED_RGB_ARB;
        else if (!hasalpha) intexfmt = GL_RGB;
        if (glinfo.bgra) texfmt = GL_BGRA;
        uploadtexture((doalloc&1), xsiz, ysiz, intexfmt, texfmt, (coltype*)fptr, xsiz, ysiz, 0);
        free((void*)fptr);
    }

    if (!m->skinloaded)
    {
        if (xsiz != osizx || ysiz != osizy)
        {
            float fx, fy;
            fx = ((float)osizx)/((float)xsiz);
            fy = ((float)osizy)/((float)ysiz);
            if (m->mdnum == 2)
            {
                long *lptr;
                for (lptr=m->glcmds;(i=*lptr++);)
                    for (i=labs(i);i>0;i--,lptr+=3)
                    {
                        ((float *)lptr)[0] *= fx;
                        ((float *)lptr)[1] *= fy;
                    }
            }
            else if (m->mdnum == 3)
            {
                md3model *m3 = (md3model *)m;
                md3surf_t *s;
                long surfi;
                for (surfi=0;surfi<m3->head.numsurfs;surfi++)
                {
                    s = &m3->head.surfs[surfi];
                    for (i=s->numverts-1;i>=0;i--)
                    {
                        s->uv[i].u *= fx;
                        s->uv[i].v *= fy;
                    }
                }
            }
        }
        m->skinloaded = 1+number;
    }

    bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,glfiltermodes[gltexfiltermode].mag);
    bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,glfiltermodes[gltexfiltermode].min);
    if (glinfo.maxanisotropy > 1.0)
        bglTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,glanisotropy);
    bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    if (cachefil < 0) {
        // save off the compressed version
        cachead.xdim = osizx;
        cachead.ydim = osizy;
        i = 0;
        for (j=0;j<31;j++) {
            if (xsiz == pow2long[j]) { i |= 1; }
            if (ysiz == pow2long[j]) { i |= 2; }
        }
        cachead.flags = (i!=3) | (hasalpha ? 2 : 0);
        writexcache(fn, picfillen, 0, hictinting[pal].f, &cachead);
    }

    return(*texidx);
}

char mdpause;

//Note: even though it says md2model, it works for both md2model&md3model
static void updateanimation (md2model *m, spritetype *tspr)
{
    mdanim_t *anim;
    long i, j;

    if (mdpause)
    {
//        spriteext[tspr->owner].mdanimtims = mdtims;
        m->interpol = 0;
        m->nframe = m->cframe;
        return;
    }

    m->cframe = m->nframe = tile2model[tspr->picnum].framenum;

    for (anim = m->animations;
            anim && anim->startframe != m->cframe;
            anim = anim->next) ;
if (!anim) { m->interpol = 0; return; }

    if (((long)spriteext[tspr->owner].mdanimcur) != anim->startframe ||
            (spriteext[tspr->owner].flags & SPREXT_NOMDANIM))
    {
        spriteext[tspr->owner].mdanimcur = (short)anim->startframe;
        spriteext[tspr->owner].mdanimtims = mdtims;
        m->cframe = m->nframe = anim->startframe;
        m->interpol = 0;
        return;
    }

    i = (mdtims-spriteext[tspr->owner].mdanimtims)*((anim->fpssc*timerticspersec)/120);
    j = ((anim->endframe+1-anim->startframe)<<16);
    //Just in case you play the game for a VERY long time...
    if (i < 0) { i = 0; spriteext[tspr->owner].mdanimtims = mdtims; }
    //compare with j*2 instead of j to ensure i stays > j-65536 for MDANIM_ONESHOT
    if ((i >= j+j) && (anim->fpssc)) //Keep mdanimtims close to mdtims to avoid the use of MOD
        spriteext[tspr->owner].mdanimtims += j/((anim->fpssc*timerticspersec)/120);

    if (anim->flags&MDANIM_ONESHOT)
    { if (i > j-65536) i = j-65536; }
else { if (i >= j) { i -= j; if (i >= j) i %= j; } }

    m->cframe = (i>>16)+anim->startframe;
    m->nframe = m->cframe+1; if (m->nframe > anim->endframe) m->nframe = anim->startframe;
    m->interpol = ((float)(i&65535))/65536.f;
}

//--------------------------------------- MD2 LIBRARY BEGINS ---------------------------------------
static long long memoryusage = 0;

static md2model *md2load (int fil, const char *filnam)
{
    md2model *m;
    md3model *m3;
    md3surf_t *s;
    md2frame_t *f;
    md2head_t head;
    char *buf, st[BMAX_PATH];
    long i, j, k;

    m = (md2model *)calloc(1,sizeof(md2model)); if (!m) return(0);
    m->mdnum = 2; m->scale = .01;

    kread(fil,(char *)&head,sizeof(md2head_t));
    head.id = B_LITTLE32(head.id);                 head.vers = B_LITTLE32(head.vers);
    head.skinxsiz = B_LITTLE32(head.skinxsiz);     head.skinysiz = B_LITTLE32(head.skinysiz);
    head.framebytes = B_LITTLE32(head.framebytes); head.numskins = B_LITTLE32(head.numskins);
    head.numverts = B_LITTLE32(head.numverts);     head.numuv = B_LITTLE32(head.numuv);
    head.numtris = B_LITTLE32(head.numtris);       head.numglcmds = B_LITTLE32(head.numglcmds);
    head.numframes = B_LITTLE32(head.numframes);   head.ofsskins = B_LITTLE32(head.ofsskins);
    head.ofsuv = B_LITTLE32(head.ofsuv);           head.ofstris = B_LITTLE32(head.ofstris);
    head.ofsframes = B_LITTLE32(head.ofsframes);   head.ofsglcmds = B_LITTLE32(head.ofsglcmds);
    head.ofseof = B_LITTLE32(head.ofseof);

if ((head.id != 0x32504449) || (head.vers != 8)) { free(m); return(0); } //"IDP2"

    m->numskins = head.numskins;
    m->numframes = head.numframes;
    m->numverts = head.numverts;
    m->numglcmds = head.numglcmds;
    m->framebytes = head.framebytes;

    m->frames = (char *)calloc(m->numframes,m->framebytes); if (!m->frames) { free(m); return(0); }
    m->glcmds = (long *)calloc(m->numglcmds,sizeof(long)); if (!m->glcmds) { free(m->frames); free(m); return(0); }
    m->tris = (md2tri_t *)calloc(head.numtris, sizeof(md2tri_t)); if (!m->tris) { free(m->glcmds); free(m->frames); free(m); return(0); }
    m->uv = (md2uv_t *)calloc(head.numuv, sizeof(md2uv_t)); if (!m->uv) { free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }

    klseek(fil,head.ofsframes,SEEK_SET);
    if (kread(fil,(char *)m->frames,m->numframes*m->framebytes) != m->numframes*m->framebytes)
    { free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }

    klseek(fil,head.ofsglcmds,SEEK_SET);
    if (kread(fil,(char *)m->glcmds,m->numglcmds*sizeof(long)) != (long)(m->numglcmds*sizeof(long)))
    { free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }

    klseek(fil,head.ofstris,SEEK_SET);
    if (kread(fil,(char *)m->tris,head.numtris*sizeof(md2tri_t)) != (long)(head.numtris*sizeof(md2tri_t)))
    { free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }

    klseek(fil,head.ofsuv,SEEK_SET);
    if (kread(fil,(char *)m->uv,head.numuv*sizeof(md2uv_t)) != (long)(head.numuv*sizeof(md2uv_t)))
    { free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }

#if B_BIG_ENDIAN != 0
    {
        char *f = (char *)m->frames;
        long *l,j;
        md2frame_t *fr;

        for (i = m->numframes-1; i>=0; i--) {
            fr = (md2frame_t *)f;
            l = (long *)&fr->mul;
            for (j=5;j>=0;j--) l[j] = B_LITTLE32(l[j]);
            f += m->framebytes;
        }

        for (i = m->numglcmds-1; i>=0; i--) {
            m->glcmds[i] = B_LITTLE32(m->glcmds[i]);
        }
    }
#endif

    strcpy(st,filnam);
    for (i=strlen(st)-1;i>0;i--)
    if ((st[i] == '/') || (st[i] == '\\')) { i++; break; }
    if (i<0) i=0;
    st[i] = 0;
m->basepath = (char *)malloc(i+1); if (!m->basepath) { free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }
    strcpy(m->basepath, st);

    m->skinfn = (char *)calloc(m->numskins,64); if (!m->skinfn) { free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }
    klseek(fil,head.ofsskins,SEEK_SET);
    if (kread(fil,m->skinfn,64*m->numskins) != 64*m->numskins)
    { free(m->glcmds); free(m->frames); free(m); return(0); }

    m->texid = (GLuint *)calloc(m->numskins, sizeof(GLuint) * (HICEFFECTMASK+1));
    if (!m->texid) { free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }

    maxmodelverts = max(maxmodelverts, m->numverts);

    //return(m);

    // the MD2 is now loaded internally - let's begin the MD3 conversion process
    //OSD_Printf("Beginning md3 conversion.\n");
    m3 = (md3model *)calloc(1, sizeof(md3model)); if (!m3) { free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }
    m3->mdnum = 3; m3->texid = 0; m3->scale = m->scale;
    m3->head.id = 0x33504449; m3->head.vers = 15;
    m3->head.flags = 1337; m3->head.numframes = m->numframes;
    m3->head.numtags = 0; m3->head.numsurfs = 1;
    m3->head.numskins = 0;

    m3->numskins = m3->head.numskins;
    m3->numframes = m3->head.numframes;

    m3->head.frames = (md3frame_t *)calloc(m3->head.numframes, sizeof(md3frame_t)); if (!m3->head.frames) { free(m3); free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }
    m3->muladdframes = (point3d *)calloc(m->numframes * 2, sizeof(point3d));

    f = (md2frame_t *)(m->frames);

    // frames converting
    i = 0;
    while (i < m->numframes)
    {
        f = (md2frame_t *)&m->frames[i*m->framebytes];
        strcpy(m3->head.frames[i].nam, f->name);
        //OSD_Printf("Copied frame %s.\n", m3->head.frames[i].nam);
        m3->muladdframes[i*2] = f->mul;
        m3->muladdframes[i*2+1] = f->add;
        i++;
    }

    m3->head.tags = NULL;

    m3->head.surfs = (md3surf_t *)calloc(1, sizeof(md3surf_t)); if (!m3->head.surfs) { free(m3->head.frames); free(m3); free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }
    s = m3->head.surfs;

    // model converting
    s->id = 0x33504449; s->flags = 0;
    s->numframes = m->numframes; s->numshaders = 0;
    s->numtris = head.numtris;
    s->numverts = head.numtris * 3; // oh man talk about memory effectiveness :((((
    // MD2 is actually more accurate than MD3 in term of uv-mapping, because each triangle has a triangle counterpart on the UV-map.
    // In MD3, each vertex unique UV coordinates, meaning that you have to duplicate vertices if you need non-seamless UV-mapping.

    maxmodelverts = max(maxmodelverts, s->numverts);

    strcpy(s->nam, "Dummy surface from MD2");

    s->shaders = NULL;

    s->tris = (md3tri_t *)calloc(head.numtris, sizeof(md3tri_t)); if (!s->tris) { free(s); free(m3->head.frames); free(m3); free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }
    s->uv = (md3uv_t *)calloc(s->numverts, sizeof(md3uv_t)); if (!s->uv) { free(s->tris); free(s); free(m3->head.frames); free(m3); free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }
    s->xyzn = (md3xyzn_t *)calloc(s->numverts * m->numframes, sizeof(md3xyzn_t)); if (!s->xyzn) { free(s->uv); free(s->tris); free(s); free(m3->head.frames); free(m3); free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m); return(0); }

    //memoryusage += (s->numverts * m->numframes * sizeof(md3xyzn_t));
    //OSD_Printf("Current model geometry memory usage : %i.\n", memoryusage);

    //OSD_Printf("Number of frames : %i\n", m->numframes);
    //OSD_Printf("Number of triangles : %i\n", head.numtris);
    //OSD_Printf("Number of vertices : %i\n", s->numverts);

    // triangle converting
    i = 0;
    while (i < head.numtris)
    {
        j = 0;
        //OSD_Printf("Triangle : %i\n", i);
        while (j < 3)
        {
            // triangle vertex indexes
            s->tris[i].i[j] = i*3 + j;

            // uv coords
            s->uv[i*3+j].u = (float)(m->uv[m->tris[i].u[j]].u) / (float)(head.skinxsiz);
            s->uv[i*3+j].v = (float)(m->uv[m->tris[i].u[j]].v) / (float)(head.skinysiz);

            // vertices for each frame
            k = 0;
            while (k < m->numframes)
            {
                f = (md2frame_t *)&m->frames[k*m->framebytes];
                //s->xyzn[(k*s->numverts) + (i*3) + j].x = ((f->verts[m->tris[i].v[j]].v[0] * f->mul.x) + f->add.x);
                //s->xyzn[(k*s->numverts) + (i*3) + j].y = ((f->verts[m->tris[i].v[j]].v[1] * f->mul.y) + f->add.y);
                //s->xyzn[(k*s->numverts) + (i*3) + j].z = ((f->verts[m->tris[i].v[j]].v[2] * f->mul.z) + f->add.z);
                s->xyzn[(k*s->numverts) + (i*3) + j].x = f->verts[m->tris[i].v[j]].v[0];
                s->xyzn[(k*s->numverts) + (i*3) + j].y = f->verts[m->tris[i].v[j]].v[1];
                s->xyzn[(k*s->numverts) + (i*3) + j].z = f->verts[m->tris[i].v[j]].v[2];
                k++;
            }
            j++;
        }
        //OSD_Printf("End triangle.\n");
        i++;
    }
    //OSD_Printf("Finished md3 conversion.\n");

    {
        mdskinmap_t *sk;

        sk = (mdskinmap_t *)calloc(1,sizeof(mdskinmap_t));
        sk->palette = 0;
        sk->skinnum = 0;
        sk->surfnum = 0;

        if(m->numskins > 0)
        {        
            sk->fn = (char *)malloc(strlen(m->basepath)+strlen(m->skinfn)+1);
            strcpy(sk->fn, m->basepath);
            strcat(sk->fn, m->skinfn);
        }
        m3->skinmap = sk;
    }

    // die MD2 ! DIE !
    free(m->texid); free(m->skinfn); free(m->basepath); free(m->uv); free(m->tris); free(m->glcmds); free(m->frames); free(m);

    return((md2model *)m3);
}
//---------------------------------------- MD2 LIBRARY ENDS ----------------------------------------

// DICHOTOMIC RECURSIVE SORTING - USED BY MD3DRAW - MAY PUT IT IN ITS OWN SOURCE FILE LATER
int partition(unsigned short *indexes, float *depths, int f, int l) {
    int up,down,temp;
    float tempf;
    unsigned short tempus;
    float piv = depths[f];
    unsigned short piv2 = indexes[f];
    up = f;
    down = l;
    do {
        while ((depths[up] <= piv) && (up < l))
            up++;
        while ((depths[down] > piv)  && (down > f))
            down--;
        if (up < down )
        {
            tempf = depths[up];
            depths[up] = depths[down];
            depths[down] = tempf;
            tempus = indexes[up];
            indexes[up] = indexes[down];
            indexes[down] = tempus;
        }
    } while (down > up);
    depths[f] = depths[down];
    depths[down] = piv;
    indexes[f] = indexes[down];
    indexes[down] = piv2;
    return down;
}

void quicksort(unsigned short *indexes, float *depths, int first, int last) {
    int pivIndex = 0;
    if (first < last) {
        pivIndex = partition(indexes,depths,first, last);
        quicksort(indexes,depths,first,(pivIndex-1));
        quicksort(indexes,depths,(pivIndex+1),last);
    }
}
// END OF QUICKSORT LIB

//--------------------------------------- MD3 LIBRARY BEGINS ---------------------------------------

static md3model *md3load (int fil)
{
    char *buf, st[BMAX_PATH+2], bst[BMAX_PATH+2];
    long i, j, surfi, ofsurf, bsc, offs[4], leng[4];
    md3model *m;
    md3surf_t *s;

    m = (md3model *)calloc(1,sizeof(md3model)); if (!m) return(0);
    m->mdnum = 3; m->texid = 0; m->scale = .01;

    m->muladdframes = NULL;

    kread(fil,&m->head,sizeof(md3head_t));
    m->head.id = B_LITTLE32(m->head.id);             m->head.vers = B_LITTLE32(m->head.vers);
    m->head.flags = B_LITTLE32(m->head.flags);       m->head.numframes = B_LITTLE32(m->head.numframes);
    m->head.numtags = B_LITTLE32(m->head.numtags);   m->head.numsurfs = B_LITTLE32(m->head.numsurfs);
    m->head.numskins = B_LITTLE32(m->head.numskins); m->head.frames = (md3frame_t*)B_LITTLE32((long)m->head.frames);
    m->head.tags = (md3tag_t*)B_LITTLE32((long)m->head.tags); m->head.surfs = (md3surf_t*)B_LITTLE32((long)m->head.surfs);
    m->head.eof = B_LITTLE32(m->head.eof);

if ((m->head.id != 0x33504449) && (m->head.vers != 15)) { free(m); return(0); } //"IDP3"

    m->numskins = m->head.numskins; //<- dead code?
    m->numframes = m->head.numframes;

    ofsurf = (long)m->head.surfs;

    klseek(fil,(long)m->head.frames,SEEK_SET); i = m->head.numframes*sizeof(md3frame_t);
    m->head.frames = (md3frame_t *)malloc(i); if (!m->head.frames) { free(m); return(0); }
    kread(fil,m->head.frames,i);

    if (m->head.numtags == 0) m->head.tags = NULL;
    else {
        klseek(fil,(long)m->head.tags,SEEK_SET); i = m->head.numtags*sizeof(md3tag_t);
        m->head.tags = (md3tag_t *)malloc(i); if (!m->head.tags) { free(m->head.frames); free(m); return(0); }
        kread(fil,m->head.tags,i);
    }

    klseek(fil,(long)m->head.surfs,SEEK_SET); i = m->head.numsurfs*sizeof(md3surf_t);
    m->head.surfs = (md3surf_t *)malloc(i); if (!m->head.surfs) { if (m->head.tags) free(m->head.tags); free(m->head.frames); free(m); return(0); }

#if B_BIG_ENDIAN != 0
    {
        long *l;

        for (i = m->head.numframes-1; i>=0; i--) {
            l = (long *)&m->head.frames[i].min;
            for (j=3+3+3+1-1;j>=0;j--) l[j] = B_LITTLE32(l[j]);
        }

        for (i = m->head.numtags-1; i>=0; i--) {
            l = (long *)&m->head.tags[i].p;
            for (j=3+3+3+3-1;j>=0;j--) l[j] = B_LITTLE32(l[j]);
        }
    }
#endif

    for (surfi=0;surfi<m->head.numsurfs;surfi++)
    {
        s = &m->head.surfs[surfi];
        klseek(fil,ofsurf,SEEK_SET); kread(fil,s,sizeof(md3surf_t));

#if B_BIG_ENDIAN != 0
        {
            long *l;
            s->id = B_LITTLE32(s->id);
            l =	(long *)&s->flags;
            for	(j=1+1+1+1+1+1+1+1+1+1-1;j>=0;j--) l[j] = B_LITTLE32(l[j]);
        }
#endif

        offs[0] = ofsurf+((long)(s->tris   )); leng[0] = s->numtris*sizeof(md3tri_t);
        offs[1] = ofsurf+((long)(s->shaders)); leng[1] = s->numshaders*sizeof(md3shader_t);
        offs[2] = ofsurf+((long)(s->uv     )); leng[2] = s->numverts*sizeof(md3uv_t);
        offs[3] = ofsurf+((long)(s->xyzn   )); leng[3] = s->numframes*s->numverts*sizeof(md3xyzn_t);
        //memoryusage += (s->numverts * s->numframes * sizeof(md3xyzn_t));
        //OSD_Printf("Current model geometry memory usage : %i.\n", memoryusage);


        s->tris = (md3tri_t *)malloc(leng[0]+leng[1]+leng[2]+leng[3]);
        if (!s->tris)
        {
            for (surfi--;surfi>=0;surfi--) free(m->head.surfs[surfi].tris);
            if (m->head.tags) free(m->head.tags); free(m->head.frames); free(m); return(0);
        }
        s->shaders = (md3shader_t *)(((long)s->tris   )+leng[0]);
        s->uv      = (md3uv_t     *)(((long)s->shaders)+leng[1]);
        s->xyzn    = (md3xyzn_t   *)(((long)s->uv     )+leng[2]);

        klseek(fil,offs[0],SEEK_SET); kread(fil,s->tris   ,leng[0]);
        klseek(fil,offs[1],SEEK_SET); kread(fil,s->shaders,leng[1]);
        klseek(fil,offs[2],SEEK_SET); kread(fil,s->uv     ,leng[2]);
        klseek(fil,offs[3],SEEK_SET); kread(fil,s->xyzn   ,leng[3]);

#if B_BIG_ENDIAN != 0
        {
            long *l;

            for (i=s->numtris-1;i>=0;i--) {
                for (j=2;j>=0;j--) s->tris[i].i[j] = B_LITTLE32(s->tris[i].i[j]);
            }
            for (i=s->numshaders-1;i>=0;i--) {
                s->shaders[i].i = B_LITTLE32(s->shaders[i].i);
            }
            for (i=s->numverts-1;i>=0;i--) {
                l = (long*)&s->uv[i].u;
                l[0] = B_LITTLE32(l[0]);
                l[1] = B_LITTLE32(l[1]);
            }
            for (i=s->numframes*s->numverts-1;i>=0;i--) {
                s->xyzn[i].x = (signed short)B_LITTLE16((unsigned short)s->xyzn[i].x);
                s->xyzn[i].y = (signed short)B_LITTLE16((unsigned short)s->xyzn[i].y);
                s->xyzn[i].z = (signed short)B_LITTLE16((unsigned short)s->xyzn[i].z);
            }
        }
#endif
        maxmodelverts = max(maxmodelverts, s->numverts);
        ofsurf += s->ofsend;
    }

#if 0
    strcpy(st,filnam);
    for (i=0,j=0;st[i];i++) if ((st[i] == '/') || (st[i] == '\\')) j = i+1;
    st[j] = '*'; st[j+1] = 0;
    kzfindfilestart(st); bsc = -1;
    while (kzfindfile(st))
    {
        if (st[0] == '\\') continue;

        for (i=0,j=0;st[i];i++) if (st[i] == '.') j = i+1;
        if ((!stricmp(&st[j],"JPG")) || (!stricmp(&st[j],"PNG")) || (!stricmp(&st[j],"GIF")) ||
                (!stricmp(&st[j],"PCX")) || (!stricmp(&st[j],"TGA")) || (!stricmp(&st[j],"BMP")) ||
                (!stricmp(&st[j],"CEL")))
        {
            for (i=0;st[i];i++) if (st[i] != filnam[i]) break;
        if (i > bsc) { bsc = i; strcpy(bst,st); }
        }
    }
    if (!mdloadskin(&m->texid,&m->usesalpha,bst)) ;//bad!
#endif

    return(m);
}

static int md3draw (md3model *m, spritetype *tspr)
{
    point3d fp, fp1, fp2, m0, m1, a0, a1;
    md3xyzn_t *v0, *v1;
    long i, j, k, l, surfi, *lptr;
    float f, g, k0, k1, k2, k3, k4, k5, k6, k7, mat[16], pc[4], mult;
    md3surf_t *s;
    int                 texunits = GL_TEXTURE0_ARB;
    mdskinmap_t *sk;
    //PLAG : sorting stuff
    unsigned short      *indexes;
    float               *maxdepths;
    unsigned short      tempus;


    //    if ((tspr->cstat&48) == 32) return 0;

    updateanimation((md2model *)m,tspr);

    //create current&next frame's vertex list from whole list

    f = m->interpol; g = 1-f;

    if (m->head.flags == 1337)
    { // md2
        /*m0.x = m->scale * g * m->muladdframes[m->cframe*2].x; m1.x = m->scale * f * m->muladdframes[m->nframe*2].x;
        m0.y = m->scale * g * m->muladdframes[m->cframe*2].y; m1.y = m->scale * f * m->muladdframes[m->nframe*2].y;
        m0.z = m->scale * g * m->muladdframes[m->cframe*2].z; m1.z = m->scale * f * m->muladdframes[m->nframe*2].z;
        a0.x = m->muladdframes[m->cframe*2+1].x * m->scale; a0.x = (m->muladdframes[m->nframe*2+1].x * m->scale - a0.x)*f+a0.x;
        a0.y = m->muladdframes[m->cframe*2+1].y * m->scale; a0.y = (m->muladdframes[m->nframe*2+1].y * m->scale - a0.y)*f+a0.y;
        a0.z = m->muladdframes[m->cframe*2+1].z * m->scale; a0.z = (m->muladdframes[m->nframe*2+1].z * m->scale - a0.z)*f+a0.z + m->zadd*m->scale;*/
        m0.x = m->scale * g; m1.x = m->scale *f;
        m0.y = m->scale * g; m1.y = m->scale *f;
        m0.z = m->scale * g; m1.z = m->scale *f;
        a0.x = a0.y = 0; a0.z = m->zadd*m->scale;
    }
    else
    {
        m0.x = (1.0/64.0) * m->scale * g; m1.x = (1.0/64.0) * m->scale *f;
        m0.y = (1.0/64.0) * m->scale * g; m1.y = (1.0/64.0) * m->scale *f;
        m0.z = (1.0/64.0) * m->scale * g; m1.z = (1.0/64.0) * m->scale *f;
        a0.x = a0.y = 0; a0.z = m->zadd*m->scale;
    }


    // Parkar: Moved up to be able to use k0 for the y-flipping code
    k0 = tspr->z;
    if ((globalorientation&128) && !((globalorientation&48)==32)) k0 += (float)((tilesizy[tspr->picnum]*tspr->yrepeat)<<1);

    // Parkar: Changed to use the same method as centeroriented sprites
    if (globalorientation&8) //y-flipping
    {
        m0.z = -m0.z; m1.z = -m1.z; a0.z = -a0.z;
        k0 -= (float)((tilesizy[tspr->picnum]*tspr->yrepeat)<<2);
    }
    if (globalorientation&4) { m0.y = -m0.y; m1.y = -m1.y; a0.y = -a0.y; } //x-flipping

    f = ((float)tspr->xrepeat)/64*m->bscale;
    m0.x *= f; m1.x *= f; a0.x *= f; f = -f;   // 20040610: backwards models aren't cool
    m0.y *= f; m1.y *= f; a0.y *= f;
    f = ((float)tspr->yrepeat)/64*m->bscale;
    m0.z *= f; m1.z *= f; a0.z *= f;

    // floor aligned
    k1 = tspr->y;
    if ((globalorientation&48)==32)
    {
        m0.z = -m0.z; m1.z = -m1.z; a0.z = -a0.z;
        m0.y = -m0.y; m1.y = -m1.y; a0.y = -a0.y;
        f = a0.x; a0.x = a0.z; a0.z = f;
        k1 += (float)((tilesizy[tspr->picnum]*tspr->yrepeat)>>3);
    }

    f = (65536.0*512.0)/((float)xdimen*viewingrange);
    g = 32.0/((float)xdimen*gxyaspect);
    m0.y *= f; m1.y *= f; a0.y = (((float)(tspr->x-globalposx))/  1024.0 + a0.y)*f;
    m0.x *=-f; m1.x *=-f; a0.x = (((float)(k1     -globalposy))/ -1024.0 + a0.x)*-f;
    m0.z *= g; m1.z *= g; a0.z = (((float)(k0     -globalposz))/-16384.0 + a0.z)*g;

    k0 = ((float)(tspr->x-globalposx))*f/1024.0;
    k1 = ((float)(tspr->y-globalposy))*f/1024.0;
    f = gcosang2*gshang;
    g = gsinang2*gshang;
    k4 = (float)sintable[(tspr->ang+spriteext[tspr->owner].angoff+1024)&2047] / 16384.0;
    k5 = (float)sintable[(tspr->ang+spriteext[tspr->owner].angoff+ 512)&2047] / 16384.0;
    k2 = k0*(1-k4)+k1*k5;
    k3 = k1*(1-k4)-k0*k5;
    k6 = f*gstang - gsinang*gctang; k7 = g*gstang + gcosang*gctang;
    mat[0] = k4*k6 + k5*k7; mat[4] = gchang*gstang; mat[ 8] = k4*k7 - k5*k6; mat[12] = k2*k6 + k3*k7;
    k6 = f*gctang + gsinang*gstang; k7 = g*gctang - gcosang*gstang;
    mat[1] = k4*k6 + k5*k7; mat[5] = gchang*gctang; mat[ 9] = k4*k7 - k5*k6; mat[13] = k2*k6 + k3*k7;
    k6 =           gcosang2*gchang; k7 =           gsinang2*gchang;
    mat[2] = k4*k6 + k5*k7; mat[6] =-gshang;        mat[10] = k4*k7 - k5*k6; mat[14] = k2*k6 + k3*k7;

    mat[12] += a0.y*mat[0] + a0.z*mat[4] + a0.x*mat[ 8];
    mat[13] += a0.y*mat[1] + a0.z*mat[5] + a0.x*mat[ 9];
    mat[14] += a0.y*mat[2] + a0.z*mat[6] + a0.x*mat[10];

    // floor aligned
    if ((globalorientation&48)==32)
    {
        f = mat[4]; mat[4] = mat[8]*16.0; mat[8] = -f*(1.0/16.0);
        f = mat[5]; mat[5] = mat[9]*16.0; mat[9] = -f*(1.0/16.0);
        f = mat[6]; mat[6] = mat[10]*16.0; mat[10] = -f*(1.0/16.0);
    }

    //Mirrors
    if (grhalfxdown10x < 0) { mat[0] = -mat[0]; mat[4] = -mat[4]; mat[8] = -mat[8]; mat[12] = -mat[12]; }

    //------------
    //bit 10 is an ugly hack in game.c\animatesprites telling MD2SPRITE
    //to use Z-buffer hacks to hide overdraw problems with the shadows
    if (tspr->cstat&1024)
    {
        bglDepthFunc(GL_LESS); //NEVER,LESS,(,L)EQUAL,GREATER,(NOT,G)EQUAL,ALWAYS
        bglDepthRange(0.0,0.9999);
    }
    bglPushAttrib(GL_POLYGON_BIT);
    if ((grhalfxdown10x >= 0) ^ ((globalorientation&8) != 0) ^ ((globalorientation&4) != 0)) bglFrontFace(GL_CW); else bglFrontFace(GL_CCW);
    bglEnable(GL_CULL_FACE);
    bglCullFace(GL_BACK);

    bglEnable(GL_TEXTURE_2D);

    pc[0] = pc[1] = pc[2] = ((float)(numpalookups-min(max((globalshade * shadescale)+m->shadeoff,0),numpalookups)))/((float)numpalookups);
    pc[0] *= (float)hictinting[globalpal].r / 255.0;
    pc[1] *= (float)hictinting[globalpal].g / 255.0;
    pc[2] *= (float)hictinting[globalpal].b / 255.0;
    if (hictinting[MAXPALOOKUPS-1].r != 255 || hictinting[MAXPALOOKUPS-1].g != 255 || hictinting[MAXPALOOKUPS-1].b != 255)
    {
        pc[0] *= (float)hictinting[MAXPALOOKUPS-1].r / 255.0;
        pc[1] *= (float)hictinting[MAXPALOOKUPS-1].g / 255.0;
        pc[2] *= (float)hictinting[MAXPALOOKUPS-1].b / 255.0;
    }

if (tspr->cstat&2) { if (!(tspr->cstat&512)) pc[3] = 0.66; else pc[3] = 0.33; } else pc[3] = 1.0;
    if (m->usesalpha) //Sprites with alpha in texture
    {
        //      bglEnable(GL_BLEND);// bglBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        //      bglEnable(GL_ALPHA_TEST); bglAlphaFunc(GL_GREATER,0.32);
        //      float al = 0.32;
        // PLAG : default cutoff removed
        float al = 0.0;
        if (alphahackarray[globalpicnum] != 0)
            al=alphahackarray[globalpicnum];
        if (!peelcompiling)
            bglEnable(GL_BLEND);
        bglEnable(GL_ALPHA_TEST);
        bglAlphaFunc(GL_GREATER,al);
    }
    else
    {
        if (tspr->cstat&2 && (!peelcompiling)) bglEnable(GL_BLEND); //else bglDisable(GL_BLEND);
    }
    bglColor4f(pc[0],pc[1],pc[2],pc[3]);
    //if (m->head.flags == 1337)
    //    bglColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    //------------

    // PLAG: Cleaner model rotation code
    if (spriteext[tspr->owner].pitch || spriteext[tspr->owner].roll || m->head.flags == 1337)
    {
        if (spriteext[tspr->owner].xoff)
            a0.x = (int)(spriteext[tspr->owner].xoff / (2048 * (m0.x+m1.x)));
        else
            a0.x = 0;
        if (spriteext[tspr->owner].yoff)
            a0.y = (int)(spriteext[tspr->owner].yoff / (2048 * (m0.x+m1.x)));
        else
            a0.y = 0;
        if ((spriteext[tspr->owner].zoff) && !(tspr->cstat&1024))
            a0.z = (int)(spriteext[tspr->owner].zoff / (524288 * (m0.z+m1.z)));
        else
            a0.z = 0;
        k0 = (float)sintable[(spriteext[tspr->owner].pitch+512)&2047] / 16384.0;
        k1 = (float)sintable[spriteext[tspr->owner].pitch&2047] / 16384.0;
        k2 = (float)sintable[(spriteext[tspr->owner].roll+512)&2047] / 16384.0;
        k3 = (float)sintable[spriteext[tspr->owner].roll&2047] / 16384.0;
    }
    for (surfi=0;surfi<m->head.numsurfs;surfi++)
    {
        s = &m->head.surfs[surfi];
        v0 = &s->xyzn[m->cframe*s->numverts];
        v1 = &s->xyzn[m->nframe*s->numverts];

        for (i=s->numverts-1;i>=0;i--)
        {
            if (spriteext[tspr->owner].pitch || spriteext[tspr->owner].roll || m->head.flags == 1337)
            {
                fp.z = ((m->head.flags == 1337) ? (v0[i].x * m->muladdframes[m->cframe*2].x) + m->muladdframes[m->cframe*2+1].x : v0[i].x) + a0.x;
                fp.x = ((m->head.flags == 1337) ? (v0[i].y * m->muladdframes[m->cframe*2].y) + m->muladdframes[m->cframe*2+1].y : v0[i].y) + a0.y;
                fp.y = ((m->head.flags == 1337) ? (v0[i].z * m->muladdframes[m->cframe*2].z) + m->muladdframes[m->cframe*2+1].z : v0[i].z) + a0.z;
                fp1.x = fp.x*k2 +       fp.y*k3;
                fp1.y = fp.x*k0*(-k3) + fp.y*k0*k2 + fp.z*(-k1);
                fp1.z = fp.x*k1*(-k3) + fp.y*k1*k2 + fp.z*k0;
                fp.z = ((m->head.flags == 1337) ? (v1[i].x * m->muladdframes[m->nframe*2].x) + m->muladdframes[m->nframe*2+1].x : v1[i].x) + a0.x;
                fp.x = ((m->head.flags == 1337) ? (v1[i].y * m->muladdframes[m->nframe*2].y) + m->muladdframes[m->nframe*2+1].y : v1[i].y) + a0.y;
                fp.y = ((m->head.flags == 1337) ? (v1[i].z * m->muladdframes[m->nframe*2].z) + m->muladdframes[m->nframe*2+1].z : v1[i].z) + a0.z;
                fp2.x = fp.x*k2 +       fp.y*k3;
                fp2.y = fp.x*k0*(-k3) + fp.y*k0*k2 + fp.z*(-k1);
                fp2.z = fp.x*k1*(-k3) + fp.y*k1*k2 + fp.z*k0;
                fp.z = (fp1.z - a0.x)*m0.x + (fp2.z - a0.x)*m1.x;
                fp.x = (fp1.x - a0.y)*m0.y + (fp2.x - a0.y)*m1.y;
                fp.y = (fp1.y - a0.z)*m0.z + (fp2.y - a0.z)*m1.z;
            }
            else
            {
                fp.z = v0[i].x*m0.x + v1[i].x*m1.x;
                fp.y = v0[i].z*m0.z + v1[i].z*m1.z;
                fp.x = v0[i].y*m0.y + v1[i].y*m1.y;
            }
            vertlist[i].x = fp.x;
            vertlist[i].y = fp.y;
            vertlist[i].z = fp.z;
        }
        bglMatrixMode(GL_MODELVIEW); //Let OpenGL (and perhaps hardware :) handle the matrix rotation
        mat[3] = mat[7] = mat[11] = 0.f; mat[15] = 1.f; bglLoadMatrixf(mat);
        // PLAG: End

        i = mdloadskin((md2model *)m,tile2model[tspr->picnum].skinnum,globalpal,surfi); if (!i) continue;
        //i = mdloadskin((md2model *)m,tile2model[tspr->picnum].skinnum,surfi); //hack for testing multiple surfaces per MD3
        bglBindTexture(GL_TEXTURE_2D, i);

        if (r_detailmapping && !r_depthpeeling && indrawroomsandmasks&& !(tspr->cstat&1024))
            i = mdloadskin((md2model *)m,tile2model[tspr->picnum].skinnum,DETAILPAL,surfi);
        else
            i = 0;

        if (i)
        {
            bglActiveTextureARB(++texunits);

            bglEnable(GL_TEXTURE_2D);
            bglBindTexture(GL_TEXTURE_2D, i);

            bglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
            bglTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);

            bglTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
            bglTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

            bglTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
            bglTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
  
            bglTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
            bglTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
            bglTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);

            bglTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2.0f);

            bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

            for (sk = m->skinmap; sk; sk = sk->next)
                if ((int)sk->palette == DETAILPAL && sk->skinnum == tile2model[tspr->picnum].skinnum && sk->surfnum == surfi)
                    f = sk->param;

            bglMatrixMode(GL_TEXTURE);
            bglLoadIdentity();
            bglScalef(f, f, 1.0f);
            bglMatrixMode(GL_MODELVIEW);
        }

        if (r_glowmapping && !r_depthpeeling && indrawroomsandmasks && !(tspr->cstat&1024))
            i = mdloadskin((md2model *)m,tile2model[tspr->picnum].skinnum,GLOWPAL,surfi);
        else
            i = 0;

        if (i)
        {
            bglActiveTextureARB(++texunits);

            bglEnable(GL_TEXTURE_2D);
            bglBindTexture(GL_TEXTURE_2D, i);

            bglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
            bglTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);

            bglTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
            bglTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

            bglTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
            bglTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);

            bglTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_TEXTURE);
            bglTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_ONE_MINUS_SRC_ALPHA);

            bglTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
            bglTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
            bglTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);

            bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        }

        //PLAG: delayed polygon-level sorted rendering
        if (m->usesalpha && !(tspr->cstat & 1024) && !r_depthpeeling)
        {
            indexes = malloc(sizeof(unsigned short) * s->numtris);
            maxdepths = malloc(sizeof(float) * s->numtris);

            // old sorting methods - dead code
            /*for(i=s->numtris-1;i>=0;i--)
            {
                tempf = (vertlist[s->tris[i].i[0]].x * mat[2]) + (vertlist[s->tris[i].i[0]].y * mat[6]) + (vertlist[s->tris[i].i[0]].z * mat[10]) + mat[14];
                if (tempf < ((vertlist[s->tris[i].i[1]].x * mat[2]) + (vertlist[s->tris[i].i[1]].y * mat[6]) + (vertlist[s->tris[i].i[1]].z * mat[10]) + mat[14]))
                    tempf = (vertlist[s->tris[i].i[1]].x * mat[2]) + (vertlist[s->tris[i].i[1]].y * mat[6]) + (vertlist[s->tris[i].i[1]].z * mat[10]) + mat[14];
                if (tempf < ((vertlist[s->tris[i].i[2]].x * mat[2]) + (vertlist[s->tris[i].i[2]].y * mat[6]) + (vertlist[s->tris[i].i[2]].z * mat[10]) + mat[14]))
                    tempf = (vertlist[s->tris[i].i[2]].x * mat[2]) + (vertlist[s->tris[i].i[2]].y * mat[6]) + (vertlist[s->tris[i].i[2]].z * mat[10]) + mat[14];
                maxdepths[i] = tempf;
                indexes[i] = i;
            }            

            for(i=s->numtris-1;i>=0;i--)
            {
                tempf = (vertlist[s->tris[i].i[0]].x * mat[2]) + (vertlist[s->tris[i].i[0]].y * mat[6]) + (vertlist[s->tris[i].i[0]].z * mat[10]) + mat[14];
                tempf += (vertlist[s->tris[i].i[1]].x * mat[2]) + (vertlist[s->tris[i].i[1]].y * mat[6]) + (vertlist[s->tris[i].i[1]].z * mat[10]) + mat[14];
                tempf += (vertlist[s->tris[i].i[2]].x * mat[2]) + (vertlist[s->tris[i].i[2]].y * mat[6]) + (vertlist[s->tris[i].i[2]].z * mat[10]) + mat[14];
                tempf /= 3.0f;
                maxdepths[i] = tempf;
                indexes[i] = i;
            }

            for(i=s->numtris-1;i>=0;i--)
            {
                tempvec[0].x = (vertlist[s->tris[i].i[0]].x * mat[0]) + (vertlist[s->tris[i].i[0]].y * mat[4]) + (vertlist[s->tris[i].i[0]].z * mat[8]) + mat[12];
                tempvec[0].y = (vertlist[s->tris[i].i[0]].x * mat[1]) + (vertlist[s->tris[i].i[0]].y * mat[5]) + (vertlist[s->tris[i].i[0]].z * mat[9]) + mat[13];
                tempvec[0].z = (vertlist[s->tris[i].i[0]].x * mat[2]) + (vertlist[s->tris[i].i[0]].y * mat[6]) + (vertlist[s->tris[i].i[0]].z * mat[10]) + mat[14];
                
                tempvec[1].x = (vertlist[s->tris[i].i[1]].x * mat[0]) + (vertlist[s->tris[i].i[1]].y * mat[4]) + (vertlist[s->tris[i].i[1]].z * mat[8]) + mat[12];
                tempvec[1].y = (vertlist[s->tris[i].i[1]].x * mat[1]) + (vertlist[s->tris[i].i[1]].y * mat[5]) + (vertlist[s->tris[i].i[1]].z * mat[9]) + mat[13];
                tempvec[1].z = (vertlist[s->tris[i].i[1]].x * mat[2]) + (vertlist[s->tris[i].i[1]].y * mat[6]) + (vertlist[s->tris[i].i[1]].z * mat[10]) + mat[14];
                
                tempvec[2].x = (vertlist[s->tris[i].i[2]].x * mat[0]) + (vertlist[s->tris[i].i[2]].y * mat[4]) + (vertlist[s->tris[i].i[2]].z * mat[8]) + mat[12];
                tempvec[2].y = (vertlist[s->tris[i].i[2]].x * mat[1]) + (vertlist[s->tris[i].i[2]].y * mat[5]) + (vertlist[s->tris[i].i[2]].z * mat[9]) + mat[13];
                tempvec[2].z = (vertlist[s->tris[i].i[2]].x * mat[2]) + (vertlist[s->tris[i].i[2]].y * mat[6]) + (vertlist[s->tris[i].i[2]].z * mat[10]) + mat[14];

                tempf = (tempvec[0].x * tempvec[0].x) + (tempvec[0].y * tempvec[0].y) + (tempvec[0].z * tempvec[0].z);
                tempf += (tempvec[1].x * tempvec[1].x) + (tempvec[1].y * tempvec[1].y) + (tempvec[1].z * tempvec[1].z);
                tempf += (tempvec[2].x * tempvec[2].x) + (tempvec[2].y * tempvec[2].y) + (tempvec[2].z * tempvec[2].z);
                
                maxdepths[i] = tempf;
                indexes[i] = i;
            }*/

            for (i=s->numtris-1;i>=0;i--)
            {
                // Matrix multiplication - ugly but clear
                fp.x = (vertlist[s->tris[i].i[0]].x * mat[0]) + (vertlist[s->tris[i].i[0]].y * mat[4]) + (vertlist[s->tris[i].i[0]].z * mat[8]) + mat[12];
                fp.y = (vertlist[s->tris[i].i[0]].x * mat[1]) + (vertlist[s->tris[i].i[0]].y * mat[5]) + (vertlist[s->tris[i].i[0]].z * mat[9]) + mat[13];
                fp.z = (vertlist[s->tris[i].i[0]].x * mat[2]) + (vertlist[s->tris[i].i[0]].y * mat[6]) + (vertlist[s->tris[i].i[0]].z * mat[10]) + mat[14];

                fp1.x = (vertlist[s->tris[i].i[1]].x * mat[0]) + (vertlist[s->tris[i].i[1]].y * mat[4]) + (vertlist[s->tris[i].i[1]].z * mat[8]) + mat[12];
                fp1.y = (vertlist[s->tris[i].i[1]].x * mat[1]) + (vertlist[s->tris[i].i[1]].y * mat[5]) + (vertlist[s->tris[i].i[1]].z * mat[9]) + mat[13];
                fp1.z = (vertlist[s->tris[i].i[1]].x * mat[2]) + (vertlist[s->tris[i].i[1]].y * mat[6]) + (vertlist[s->tris[i].i[1]].z * mat[10]) + mat[14];

                fp2.x = (vertlist[s->tris[i].i[2]].x * mat[0]) + (vertlist[s->tris[i].i[2]].y * mat[4]) + (vertlist[s->tris[i].i[2]].z * mat[8]) + mat[12];
                fp2.y = (vertlist[s->tris[i].i[2]].x * mat[1]) + (vertlist[s->tris[i].i[2]].y * mat[5]) + (vertlist[s->tris[i].i[2]].z * mat[9]) + mat[13];
                fp2.z = (vertlist[s->tris[i].i[2]].x * mat[2]) + (vertlist[s->tris[i].i[2]].y * mat[6]) + (vertlist[s->tris[i].i[2]].z * mat[10]) + mat[14];

                f = (fp.x * fp.x) + (fp.y * fp.y) + (fp.z * fp.z);

                g = (fp1.x * fp1.x) + (fp1.y * fp1.y) + (fp1.z * fp1.z);
                if (f > g)
                    f = g;
                g = (fp2.x * fp2.x) + (fp2.y * fp2.y) + (fp2.z * fp2.z);
                if (f > g)
                    f = g;

                maxdepths[i] = f;
                indexes[i] = i;
            }

            //bubble sort - dead code
            /*test = 0;
            j = 0;
            while (j == 0)
            {
                j = 1;
                for(i=s->numtris-1;i>0;i--)
                    if (maxdepths[i] < maxdepths[i-1])
                    {
                        f = maxdepths[i];
                        maxdepths[i] = maxdepths[i-1];
                        maxdepths[i-1] = f;
                        k = indexes[i];
                        indexes[i] = indexes[i-1];
                        indexes[i-1] = k;
                        test++;
                        j = 0;
                    }
            }*/

            // dichotomic recursive sorting - about 100x less iterations than bubblesort
            quicksort(indexes, maxdepths, 0, s->numtris - 1);

            bglBegin(GL_TRIANGLES);
            for (i=s->numtris-1;i>=0;i--)
                for (j=0;j<3;j++)
                {
                    k = s->tris[indexes[i]].i[j];
                    if (texunits > GL_TEXTURE0_ARB)
                    {
                        l = GL_TEXTURE0_ARB;
                        while (l <= texunits)
                            bglMultiTexCoord2fARB(l++, s->uv[k].u,s->uv[k].v);
                    }
                    else
                        bglTexCoord2f(s->uv[k].u,s->uv[k].v);
                    bglVertex3fv((float *)&vertlist[k]);
                }
            bglEnd();

            free(indexes);
            free(maxdepths);
        }
        else
        {
            bglBegin(GL_TRIANGLES);
            for (i=s->numtris-1;i>=0;i--)
                for (j=0;j<3;j++)
                {
                    k = s->tris[i].i[j];
                    if (texunits > GL_TEXTURE0_ARB)
                    {
                        l = GL_TEXTURE0_ARB;
                        while (l <= texunits)
                            bglMultiTexCoord2fARB(l++, s->uv[k].u,s->uv[k].v);
                    }
                    else
                        bglTexCoord2f(s->uv[k].u,s->uv[k].v);
                    bglVertex3fv((float *)&vertlist[k]);
                }
            bglEnd();
        }

        if (texunits > GL_TEXTURE0_ARB)
        {
            while (texunits > GL_TEXTURE0_ARB)
            {
                bglMatrixMode(GL_TEXTURE);
                bglLoadIdentity();
                bglMatrixMode(GL_MODELVIEW);
                bglTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.0f);
                bglDisable(GL_TEXTURE_2D);
                bglActiveTextureARB(--texunits);
            }
        }
    }
    //------------

    if (m->usesalpha) bglDisable(GL_ALPHA_TEST);
    bglDisable(GL_CULL_FACE);
    bglPopAttrib();
    if (tspr->cstat&1024)
    {
        bglDepthFunc(GL_LESS); //NEVER,LESS,(,L)EQUAL,GREATER,(NOT,G)EQUAL,ALWAYS
        bglDepthRange(0.0,0.99999);
    }
    bglLoadIdentity();

    return 1;
}

static void md3free (md3model *m)
{
    mdanim_t *anim, *nanim = NULL;
    mdskinmap_t *sk, *nsk = NULL;
    md3surf_t *s;
    long surfi;

    if (!m) return;

    for (anim=m->animations; anim; anim=nanim)
    {
        nanim = anim->next;
        free(anim);
    }
    for (sk=m->skinmap; sk; sk=nsk)
    {
        nsk = sk->next;
        free(sk->fn);
        free(sk);
    }

    if (m->head.surfs)
    {
        for (surfi=m->head.numsurfs-1;surfi>=0;surfi--)
        {
            s = &m->head.surfs[surfi];
            if (s->tris) free(s->tris);
            if (m->head.flags == 1337)
            {
                if (s->shaders) free(s->shaders);
                if (s->uv) free(s->uv);
                if (s->xyzn) free(s->xyzn);
            }
        }
        free(m->head.surfs);
    }
    if (m->head.tags) free(m->head.tags);
    if (m->head.frames) free(m->head.frames);

    if (m->texid) free(m->texid);

    if (m->muladdframes) free(m->muladdframes);

    free(m);
}

//---------------------------------------- MD3 LIBRARY ENDS ----------------------------------------
//--------------------------------------- VOX LIBRARY BEGINS ---------------------------------------

//For loading/conversion only
static long xsiz, ysiz, zsiz, yzsiz, *vbit = 0; //vbit: 1 bit per voxel: 0=air,1=solid
static float xpiv, ypiv, zpiv; //Might want to use more complex/unique names!
static long *vcolhashead = 0, vcolhashsizm1;
typedef struct { long p, c, n; } voxcol_t;
static voxcol_t *vcol = 0; long vnum = 0, vmax = 0;
typedef struct { short x, y; } spoint2d;
static spoint2d *shp;
static long *shcntmal, *shcnt = 0, shcntp;
static long mytexo5, *zbit, gmaxx, gmaxy, garea, pow2m1[33];
static voxmodel *gvox;

//pitch must equal xsiz*4
unsigned gloadtex (long *picbuf, long xsiz, long ysiz, long is8bit, long dapal)
{
    unsigned rtexid;
    coltype *pic, *pic2;
    unsigned char *cptr;
    long i;

    pic = (coltype *)picbuf; //Correct for GL's RGB order; also apply gamma here..
    pic2 = (coltype *)malloc(xsiz*ysiz*sizeof(long)); if (!pic2) return((unsigned)-1);
    cptr = (unsigned char*)&britable[gammabrightness ? 0 : curbrightness][0];
    if (!is8bit)
    {
        for (i=xsiz*ysiz-1;i>=0;i--)
        {
            pic2[i].b = cptr[pic[i].r];
            pic2[i].g = cptr[pic[i].g];
            pic2[i].r = cptr[pic[i].b];
            pic2[i].a = 255;
        }
    }
    else
    {
        if (palookup[dapal] == NULL) dapal = 0;
        for (i=xsiz*ysiz-1;i>=0;i--)
        {
            pic2[i].b = cptr[palette[(long)palookup[dapal][pic[i].a]*3+2]*4];
            pic2[i].g = cptr[palette[(long)palookup[dapal][pic[i].a]*3+1]*4];
            pic2[i].r = cptr[palette[(long)palookup[dapal][pic[i].a]*3+0]*4];
            pic2[i].a = 255;
        }
    }

    bglGenTextures(1,(GLuint*)&rtexid);
    bglBindTexture(GL_TEXTURE_2D,rtexid);
    bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    bglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    bglTexImage2D(GL_TEXTURE_2D,0,4,xsiz,ysiz,0,GL_RGBA,GL_UNSIGNED_BYTE,(unsigned char *)pic2);
    free(pic2);
    return(rtexid);
}

static long getvox (long x, long y, long z)
{
    z += x*yzsiz + y*zsiz;
    for (x=vcolhashead[(z*214013)&vcolhashsizm1];x>=0;x=vcol[x].n)
        if (vcol[x].p == z) return(vcol[x].c);
    return(0x808080);
}

static void putvox (long x, long y, long z, long col)
{
    if (vnum >= vmax) { vmax = max(vmax<<1,4096); vcol = (voxcol_t *)realloc(vcol,vmax*sizeof(voxcol_t)); }

    z += x*yzsiz + y*zsiz;
    vcol[vnum].p = z; z = ((z*214013)&vcolhashsizm1);
    vcol[vnum].c = col;
    vcol[vnum].n = vcolhashead[z]; vcolhashead[z] = vnum++;
}

//Set all bits in vbit from (x,y,z0) to (x,y,z1-1) to 0's
static void setzrange0 (long *lptr, long z0, long z1)
{
    long z, ze;
    if (!((z0^z1)&~31)) { lptr[z0>>5] &= ((~(-1<<SHIFTMOD32(z0)))|(-1<<SHIFTMOD32(z1))); return; }
    z = (z0>>5); ze = (z1>>5);
    lptr[z] &=~(-1<<SHIFTMOD32(z0)); for (z++;z<ze;z++) lptr[z] = 0;
    lptr[z] &= (-1<<SHIFTMOD32(z1));
}

//Set all bits in vbit from (x,y,z0) to (x,y,z1-1) to 1's
static void setzrange1 (long *lptr, long z0, long z1)
{
    long z, ze;
    if (!((z0^z1)&~31)) { lptr[z0>>5] |= ((~(-1<<SHIFTMOD32(z1)))&(-1<<SHIFTMOD32(z0))); return; }
    z = (z0>>5); ze = (z1>>5);
    lptr[z] |= (-1<<SHIFTMOD32(z0)); for (z++;z<ze;z++) lptr[z] = -1;
    lptr[z] |=~(-1<<SHIFTMOD32(z1));
}

static long isrectfree (long x0, long y0, long dx, long dy)
{
#if 0
    long i, j, x;
    i = y0*gvox->mytexx + x0;
    for (dy=0;dy;dy--,i+=gvox->mytexx)
    for (x=0;x<dx;x++) { j = i+x; if (zbit[j>>5]&(1<<SHIFTMOD32(j))) return(0); }
#else
    long i, c, m, m1, x;

    i = y0*mytexo5 + (x0>>5); dx += x0-1; c = (dx>>5) - (x0>>5);
    m = ~pow2m1[x0&31]; m1 = pow2m1[(dx&31)+1];
    if (!c) { for (m&=m1;dy;dy--,i+=mytexo5) if (zbit[i]&m) return(0); }
    else
        {  for (;dy;dy--,i+=mytexo5)
        {
            if (zbit[i]&m) return(0);
            for (x=1;x<c;x++) if (zbit[i+x]) return(0);
            if (zbit[i+x]&m1) return(0);
        }
    }
#endif
    return(1);
}

static void setrect (long x0, long y0, long dx, long dy)
{
#if 0
    long i, j, y;
    i = y0*gvox->mytexx + x0;
    for (y=0;y<dy;y++,i+=gvox->mytexx)
    for (x=0;x<dx;x++) { j = i+x; zbit[j>>5] |= (1<<SHIFTMOD32(j)); }
#else
    long i, c, m, m1, x;

    i = y0*mytexo5 + (x0>>5); dx += x0-1; c = (dx>>5) - (x0>>5);
    m = ~pow2m1[x0&31]; m1 = pow2m1[(dx&31)+1];
    if (!c) { for (m&=m1;dy;dy--,i+=mytexo5) zbit[i] |= m; }
    else
        {  for (;dy;dy--,i+=mytexo5)
        {
            zbit[i] |= m;
            for (x=1;x<c;x++) zbit[i+x] = -1;
            zbit[i+x] |= m1;
        }
    }
#endif
}

static void cntquad (long x0, long y0, long z0, long x1, long y1, long z1, long x2, long y2, long z2, long face)
{
    long x, y, z;

    x = labs(x2-x0); y = labs(y2-y0); z = labs(z2-z0);
    if (!x) x = z; else if (!y) y = z;
if (x < y) { z = x; x = y; y = z; }
    shcnt[y*shcntp+x]++;
    if (x > gmaxx) gmaxx = x;
    if (y > gmaxy) gmaxy = y;
    garea += (x+(VOXBORDWIDTH<<1))*(y+(VOXBORDWIDTH<<1));
    gvox->qcnt++;
}

static void addquad (long x0, long y0, long z0, long x1, long y1, long z1, long x2, long y2, long z2, long face)
{
    long i, j, x, y, z, xx, yy, nx = 0, ny = 0, nz = 0, *lptr;
    voxrect_t *qptr;

    x = labs(x2-x0); y = labs(y2-y0); z = labs(z2-z0);
    if (!x) { x = y; y = z; i = 0; } else if (!y) { y = z; i = 1; } else i = 2;
if (x < y) { z = x; x = y; y = z; i += 3; }
    z = shcnt[y*shcntp+x]++;
    lptr = &gvox->mytex[(shp[z].y+VOXBORDWIDTH)*gvox->mytexx+(shp[z].x+VOXBORDWIDTH)];
    switch (face)
    {
    case 0:
        ny = y1; x2 = x0; x0 = x1; x1 = x2; break;
    case 1:
        ny = y0; y0++; y1++; y2++; break;
    case 2:
        nz = z1; y0 = y2; y2 = y1; y1 = y0; z0++; z1++; z2++; break;
    case 3:
        nz = z0; break;
    case 4:
        nx = x1; y2 = y0; y0 = y1; y1 = y2; x0++; x1++; x2++; break;
    case 5:
        nx = x0; break;
    }
    for (yy=0;yy<y;yy++,lptr+=gvox->mytexx)
        for (xx=0;xx<x;xx++)
        {
            switch (face)
            {
            case 0:
            if (i < 3) { nx = x1+x-1-xx; nz = z1+yy;   } //back
                else { nx = x1+y-1-yy; nz = z1+xx;   } break;
            case 1:
            if (i < 3) { nx = x0+xx;     nz = z0+yy;   } //front
                else { nx = x0+yy;     nz = z0+xx;   } break;
            case 2:
            if (i < 3) { nx = x1-x+xx;   ny = y1-1-yy; } //bot
                else { nx = x1-1-yy;   ny = y1-1-xx; } break;
            case 3:
            if (i < 3) { nx = x0+xx;     ny = y0+yy;   } //top
                else { nx = x0+yy;     ny = y0+xx;   } break;
            case 4:
            if (i < 3) { ny = y1+x-1-xx; nz = z1+yy;   } //right
                else { ny = y1+y-1-yy; nz = z1+xx;   } break;
            case 5:
            if (i < 3) { ny = y0+xx;     nz = z0+yy;   } //left
                else { ny = y0+yy;     nz = z0+xx;   } break;
            }
            lptr[xx] = getvox(nx,ny,nz);
        }

    //Extend borders horizontally
    for (yy=VOXBORDWIDTH;yy<y+VOXBORDWIDTH;yy++)
        for (xx=0;xx<VOXBORDWIDTH;xx++)
        {
            lptr = &gvox->mytex[(shp[z].y+yy)*gvox->mytexx+shp[z].x];
            lptr[xx] = lptr[VOXBORDWIDTH]; lptr[xx+x+VOXBORDWIDTH] = lptr[x-1+VOXBORDWIDTH];
        }
    //Extend borders vertically
    for (yy=0;yy<VOXBORDWIDTH;yy++)
    {
        memcpy(&gvox->mytex[(shp[z].y+yy)*gvox->mytexx+shp[z].x],
               &gvox->mytex[(shp[z].y+VOXBORDWIDTH)*gvox->mytexx+shp[z].x],
               (x+(VOXBORDWIDTH<<1))<<2);
        memcpy(&gvox->mytex[(shp[z].y+y+yy+VOXBORDWIDTH)*gvox->mytexx+shp[z].x],
               &gvox->mytex[(shp[z].y+y-1+VOXBORDWIDTH)*gvox->mytexx+shp[z].x],
               (x+(VOXBORDWIDTH<<1))<<2);
    }

    qptr = &gvox->quad[gvox->qcnt];
    qptr->v[0].x = x0; qptr->v[0].y = y0; qptr->v[0].z = z0;
    qptr->v[1].x = x1; qptr->v[1].y = y1; qptr->v[1].z = z1;
    qptr->v[2].x = x2; qptr->v[2].y = y2; qptr->v[2].z = z2;
    for (j=0;j<3;j++) { qptr->v[j].u = shp[z].x+VOXBORDWIDTH; qptr->v[j].v = shp[z].y+VOXBORDWIDTH; }
    if (i < 3) qptr->v[1].u += x; else qptr->v[1].v += y;
    qptr->v[2].u += x; qptr->v[2].v += y;

    qptr->v[3].u = qptr->v[0].u - qptr->v[1].u + qptr->v[2].u;
    qptr->v[3].v = qptr->v[0].v - qptr->v[1].v + qptr->v[2].v;
    qptr->v[3].x = qptr->v[0].x - qptr->v[1].x + qptr->v[2].x;
    qptr->v[3].y = qptr->v[0].y - qptr->v[1].y + qptr->v[2].y;
    qptr->v[3].z = qptr->v[0].z - qptr->v[1].z + qptr->v[2].z;
    if (gvox->qfacind[face] < 0) gvox->qfacind[face] = gvox->qcnt;
    gvox->qcnt++;

}

static long isolid (long x, long y, long z)
{
    if ((unsigned long)x >= (unsigned long)xsiz) return(0);
    if ((unsigned long)y >= (unsigned long)ysiz) return(0);
    if ((unsigned long)z >= (unsigned long)zsiz) return(0);
    z += x*yzsiz + y*zsiz; return(vbit[z>>5]&(1<<SHIFTMOD32(z)));
}

static voxmodel *vox2poly ()
{
    long i, j, x, y, z, v, ov, oz = 0, cnt, sc, x0, y0, dx, dy, i0, i1, *bx0, *by0;
    void (*daquad)(long, long, long, long, long, long, long, long, long, long);
    coltype *pic;
    unsigned char *cptr, ch;

    gvox = (voxmodel *)malloc(sizeof(voxmodel)); if (!gvox) return(0);
    memset(gvox,0,sizeof(voxmodel));

    //x is largest dimension, y is 2nd largest dimension
    x = xsiz; y = ysiz; z = zsiz;
    if ((x < y) && (x < z)) x = z; else if (y < z) y = z;
if (x < y) { z = x; x = y; y = z; }
    shcntp = x; i = x*y*sizeof(long);
    shcntmal = (long *)malloc(i); if (!shcntmal) { free(gvox); return(0); }
    memset(shcntmal,0,i); shcnt = &shcntmal[-shcntp-1];
    gmaxx = gmaxy = garea = 0;

    if (pow2m1[32] != -1) { for (i=0;i<32;i++) pow2m1[i] = (1<<i)-1; pow2m1[32] = -1; }
    for (i=0;i<7;i++) gvox->qfacind[i] = -1;

    i = ((max(ysiz,zsiz)+1)<<2);
bx0 = (long *)malloc(i<<1); if (!bx0) { free(gvox); return(0); }
    by0 = (long *)(((long)bx0)+i);

    for (cnt=0;cnt<2;cnt++)
    {
        if (!cnt) daquad = cntquad;
        else daquad = addquad;
        gvox->qcnt = 0;

        memset(by0,-1,(max(ysiz,zsiz)+1)<<2); v = 0;

        for (i=-1;i<=1;i+=2)
            for (y=0;y<ysiz;y++)
                for (x=0;x<=xsiz;x++)
                    for (z=0;z<=zsiz;z++)
                    {
                        ov = v; v = (isolid(x,y,z) && (!isolid(x,y+i,z)));
                        if ((by0[z] >= 0) && ((by0[z] != oz) || (v >= ov)))
                        { daquad(bx0[z],y,by0[z],x,y,by0[z],x,y,z,i>=0); by0[z] = -1; }
                        if (v > ov) oz = z; else if ((v < ov) && (by0[z] != oz)) { bx0[z] = x; by0[z] = oz; }
                    }

        for (i=-1;i<=1;i+=2)
            for (z=0;z<zsiz;z++)
                for (x=0;x<=xsiz;x++)
                    for (y=0;y<=ysiz;y++)
                    {
                        ov = v; v = (isolid(x,y,z) && (!isolid(x,y,z-i)));
                        if ((by0[y] >= 0) && ((by0[y] != oz) || (v >= ov)))
                        { daquad(bx0[y],by0[y],z,x,by0[y],z,x,y,z,(i>=0)+2); by0[y] = -1; }
                        if (v > ov) oz = y; else if ((v < ov) && (by0[y] != oz)) { bx0[y] = x; by0[y] = oz; }
                    }

        for (i=-1;i<=1;i+=2)
            for (x=0;x<xsiz;x++)
                for (y=0;y<=ysiz;y++)
                    for (z=0;z<=zsiz;z++)
                    {
                        ov = v; v = (isolid(x,y,z) && (!isolid(x-i,y,z)));
                        if ((by0[z] >= 0) && ((by0[z] != oz) || (v >= ov)))
                        { daquad(x,bx0[z],by0[z],x,y,by0[z],x,y,z,(i>=0)+4); by0[z] = -1; }
                        if (v > ov) oz = z; else if ((v < ov) && (by0[z] != oz)) { bx0[z] = y; by0[z] = oz; }
                    }

        if (!cnt)
        {
            shp = (spoint2d *)malloc(gvox->qcnt*sizeof(spoint2d));
            if (!shp) { free(bx0); free(gvox); return(0); }

            sc = 0;
            for (y=gmaxy;y;y--)
                for (x=gmaxx;x>=y;x--)
                {
                    i = shcnt[y*shcntp+x]; shcnt[y*shcntp+x] = sc; //shcnt changes from counter to head index
                    for (;i>0;i--) { shp[sc].x = x; shp[sc].y = y; sc++; }
                }

            for (gvox->mytexx=32;gvox->mytexx<(gmaxx+(VOXBORDWIDTH<<1));gvox->mytexx<<=1);
            for (gvox->mytexy=32;gvox->mytexy<(gmaxy+(VOXBORDWIDTH<<1));gvox->mytexy<<=1);
            while (gvox->mytexx*gvox->mytexy*8 < garea*9) //This should be sufficient to fit most skins...
            {
skindidntfit:
                ;
                if (gvox->mytexx <= gvox->mytexy) gvox->mytexx <<= 1; else gvox->mytexy <<= 1;
            }
            mytexo5 = (gvox->mytexx>>5);

            i = (((gvox->mytexx*gvox->mytexy+31)>>5)<<2);
        zbit = (long *)malloc(i); if (!zbit) { free(bx0); free(gvox); free(shp); return(0); }
            memset(zbit,0,i);

            v = gvox->mytexx*gvox->mytexy;
            for (z=0;z<sc;z++)
            {
                dx = shp[z].x+(VOXBORDWIDTH<<1); dy = shp[z].y+(VOXBORDWIDTH<<1); i = v;
                do
                {
#if (VOXUSECHAR != 0)
                    x0 = (((rand()&32767)*(min(gvox->mytexx,255)-dx))>>15);
                    y0 = (((rand()&32767)*(min(gvox->mytexy,255)-dy))>>15);
#else
                    x0 = (((rand()&32767)*(gvox->mytexx+1-dx))>>15);
                    y0 = (((rand()&32767)*(gvox->mytexy+1-dy))>>15);
#endif
                    i--;
                    if (i < 0) //Time-out! Very slow if this happens... but at least it still works :P
                    {
                        free(zbit);

                        //Re-generate shp[].x/y (box sizes) from shcnt (now head indices) for next pass :/
                        j = 0;
                        for (y=gmaxy;y;y--)
                            for (x=gmaxx;x>=y;x--)
                            {
                                i = shcnt[y*shcntp+x];
                                for (;j<i;j++) { shp[j].x = x0; shp[j].y = y0; }
                                x0 = x; y0 = y;
                            }
                        for (;j<sc;j++) { shp[j].x = x0; shp[j].y = y0; }

                        goto skindidntfit;
                    }
                } while (!isrectfree(x0,y0,dx,dy));
                while ((y0) && (isrectfree(x0,y0-1,dx,1))) y0--;
                while ((x0) && (isrectfree(x0-1,y0,1,dy))) x0--;
                setrect(x0,y0,dx,dy);
                shp[z].x = x0; shp[z].y = y0; //Overwrite size with top-left location
            }

            gvox->quad = (voxrect_t *)malloc(gvox->qcnt*sizeof(voxrect_t));
        if (!gvox->quad) { free(zbit); free(shp); free(bx0); free(gvox); return(0); }

            gvox->mytex = (long *)malloc(gvox->mytexx*gvox->mytexy*sizeof(long));
            if (!gvox->mytex) { free(gvox->quad); free(zbit); free(shp); free(bx0); free(gvox); return(0); }
        }
    }
    free(shp); free(zbit); free(bx0);
    return(gvox);
}

static long loadvox (const char *filnam)
{
    long i, j, k, x, y, z, pal[256], fil;
    unsigned char c[3], *tbuf;

    fil = kopen4load((char *)filnam,0); if (fil < 0) return(-1);
    kread(fil,&xsiz,4); xsiz = B_LITTLE32(xsiz);
    kread(fil,&ysiz,4); ysiz = B_LITTLE32(ysiz);
    kread(fil,&zsiz,4); zsiz = B_LITTLE32(zsiz);
    xpiv = ((float)xsiz)*.5;
    ypiv = ((float)ysiz)*.5;
    zpiv = ((float)zsiz)*.5;

    klseek(fil,-768,SEEK_END);
    for (i=0;i<256;i++)
    { kread(fil,c,3); pal[i] = (((long)c[0])<<18)+(((long)c[1])<<10)+(((long)c[2])<<2)+(i<<24); }
    pal[255] = -1;

    vcolhashsizm1 = 8192-1;
    vcolhashead = (long *)malloc((vcolhashsizm1+1)*sizeof(long)); if (!vcolhashead) { kclose(fil); return(-1); }
    memset(vcolhashead,-1,(vcolhashsizm1+1)*sizeof(long));

    yzsiz = ysiz*zsiz; i = ((xsiz*yzsiz+31)>>3);
    vbit = (long *)malloc(i); if (!vbit) { kclose(fil); return(-1); }
    memset(vbit,0,i);

    tbuf = (unsigned char *)malloc(zsiz*sizeof(char)); if (!tbuf) { kclose(fil); return(-1); }

    klseek(fil,12,SEEK_SET);
    for (x=0;x<xsiz;x++)
        for (y=0,j=x*yzsiz;y<ysiz;y++,j+=zsiz)
        {
            kread(fil,tbuf,zsiz);
            for (z=zsiz-1;z>=0;z--)
            { if (tbuf[z] != 255) { i = j+z; vbit[i>>5] |= (1<<SHIFTMOD32(i)); } }
        }

    klseek(fil,12,SEEK_SET);
    for (x=0;x<xsiz;x++)
        for (y=0,j=x*yzsiz;y<ysiz;y++,j+=zsiz)
        {
            kread(fil,tbuf,zsiz);
            for (z=0;z<zsiz;z++)
            {
                if (tbuf[z] == 255) continue;
                if ((!x) || (!y) || (!z) || (x == xsiz-1) || (y == ysiz-1) || (z == zsiz-1))
                { putvox(x,y,z,pal[tbuf[z]]); continue; }
                k = j+z;
                if ((!(vbit[(k-yzsiz)>>5]&(1<<SHIFTMOD32(k-yzsiz)))) ||
                        (!(vbit[(k+yzsiz)>>5]&(1<<SHIFTMOD32(k+yzsiz)))) ||
                        (!(vbit[(k- zsiz)>>5]&(1<<SHIFTMOD32(k- zsiz)))) ||
                        (!(vbit[(k+ zsiz)>>5]&(1<<SHIFTMOD32(k+ zsiz)))) ||
                        (!(vbit[(k-    1)>>5]&(1<<SHIFTMOD32(k-    1)))) ||
                        (!(vbit[(k+    1)>>5]&(1<<SHIFTMOD32(k+    1)))))
                { putvox(x,y,z,pal[tbuf[z]]); continue; }
            }
        }

    free(tbuf); kclose(fil); return(0);
}

static long loadkvx (const char *filnam)
{
    long i, j, k, x, y, z, pal[256], z0, z1, mip1leng, ysizp1, fil;
    unsigned short *xyoffs;
    unsigned char c[3], *tbuf, *cptr;

    fil = kopen4load((char *)filnam,0); if (fil < 0) return(-1);
    kread(fil,&mip1leng,4); mip1leng = B_LITTLE32(mip1leng);
    kread(fil,&xsiz,4);     xsiz = B_LITTLE32(xsiz);
    kread(fil,&ysiz,4);     ysiz = B_LITTLE32(ysiz);
    kread(fil,&zsiz,4);     zsiz = B_LITTLE32(zsiz);
    kread(fil,&i,4); xpiv = ((float)B_LITTLE32(i))/256.0;
    kread(fil,&i,4); ypiv = ((float)B_LITTLE32(i))/256.0;
    kread(fil,&i,4); zpiv = ((float)B_LITTLE32(i))/256.0;
    klseek(fil,(xsiz+1)<<2,SEEK_CUR);
    ysizp1 = ysiz+1;
    i = xsiz*ysizp1*sizeof(short);
xyoffs = (unsigned short *)malloc(i); if (!xyoffs) { kclose(fil); return(-1); }
    kread(fil,xyoffs,i); for (i=i/sizeof(short)-1; i>=0; i--) xyoffs[i] = B_LITTLE16(xyoffs[i]);

    klseek(fil,-768,SEEK_END);
    for (i=0;i<256;i++)
    { kread(fil,c,3); pal[i] = B_LITTLE32((((long)c[0])<<18)+(((long)c[1])<<10)+(((long)c[2])<<2)+(i<<24)); }

    yzsiz = ysiz*zsiz; i = ((xsiz*yzsiz+31)>>3);
    vbit = (long *)malloc(i); if (!vbit) { free(xyoffs); kclose(fil); return(-1); }
    memset(vbit,0,i);

    for (vcolhashsizm1=4096;vcolhashsizm1<(mip1leng>>1);vcolhashsizm1<<=1); vcolhashsizm1--; //approx to numvoxs!
vcolhashead = (long *)malloc((vcolhashsizm1+1)*sizeof(long)); if (!vcolhashead) { free(xyoffs); kclose(fil); return(-1); }
    memset(vcolhashead,-1,(vcolhashsizm1+1)*sizeof(long));

    klseek(fil,28+((xsiz+1)<<2)+((ysizp1*xsiz)<<1),SEEK_SET);

    i = kfilelength(fil)-ktell(fil);
    tbuf = (unsigned char *)malloc(i); if (!tbuf) { free(xyoffs); kclose(fil); return(-1); }
    kread(fil,tbuf,i); kclose(fil);

    cptr = tbuf;
    for (x=0;x<xsiz;x++) //Set surface voxels to 1 else 0
        for (y=0,j=x*yzsiz;y<ysiz;y++,j+=zsiz)
        {
            i = xyoffs[x*ysizp1+y+1] - xyoffs[x*ysizp1+y]; if (!i) continue;
            z1 = 0;
            while (i)
            {
                z0 = (long)cptr[0]; k = (long)cptr[1]; cptr += 3;
                if (!(cptr[-1]&16)) setzrange1(vbit,j+z1,j+z0);
                i -= k+3; z1 = z0+k;
                setzrange1(vbit,j+z0,j+z1);
                for (z=z0;z<z1;z++) putvox(x,y,z,pal[*cptr++]);
            }
        }

    free(tbuf); free(xyoffs); return(0);
}

static long loadkv6 (const char *filnam)
{
    long i, j, x, y, z, numvoxs, z0, z1, fil;
    unsigned short *ylen;
    unsigned char c[8];

    fil = kopen4load((char *)filnam,0); if (fil < 0) return(-1);
kread(fil,&i,4); if (B_LITTLE32(i) != 0x6c78764b) { kclose(fil); return(-1); } //Kvxl
    kread(fil,&xsiz,4);    xsiz = B_LITTLE32(xsiz);
    kread(fil,&ysiz,4);    ysiz = B_LITTLE32(ysiz);
    kread(fil,&zsiz,4);    zsiz = B_LITTLE32(zsiz);
    kread(fil,&i,4);       xpiv = (float)(B_LITTLE32(i));
    kread(fil,&i,4);       ypiv = (float)(B_LITTLE32(i));
    kread(fil,&i,4);       zpiv = (float)(B_LITTLE32(i));
    kread(fil,&numvoxs,4); numvoxs = B_LITTLE32(numvoxs);

    ylen = (unsigned short *)malloc(xsiz*ysiz*sizeof(short));
    if (!ylen) { kclose(fil); return(-1); }

    klseek(fil,32+(numvoxs<<3)+(xsiz<<2),SEEK_SET);
    kread(fil,ylen,xsiz*ysiz*sizeof(short)); for (i=xsiz*ysiz-1; i>=0; i--) ylen[i] = B_LITTLE16(ylen[i]);
    klseek(fil,32,SEEK_SET);

    yzsiz = ysiz*zsiz; i = ((xsiz*yzsiz+31)>>3);
vbit = (long *)malloc(i); if (!vbit) { free(ylen); kclose(fil); return(-1); }
    memset(vbit,0,i);

    for (vcolhashsizm1=4096;vcolhashsizm1<numvoxs;vcolhashsizm1<<=1); vcolhashsizm1--;
vcolhashead = (long *)malloc((vcolhashsizm1+1)*sizeof(long)); if (!vcolhashead) { free(ylen); kclose(fil); return(-1); }
    memset(vcolhashead,-1,(vcolhashsizm1+1)*sizeof(long));

    for (x=0;x<xsiz;x++)
        for (y=0,j=x*yzsiz;y<ysiz;y++,j+=zsiz)
        {
            z1 = zsiz;
            for (i=ylen[x*ysiz+y];i>0;i--)
            {
                kread(fil,c,8); //b,g,r,a,z_lo,z_hi,vis,dir
                z0 = B_LITTLE16(*(unsigned short *)&c[4]);
                if (!(c[6]&16)) setzrange1(vbit,j+z1,j+z0);
                vbit[(j+z0)>>5] |= (1<<SHIFTMOD32(j+z0));
                putvox(x,y,z0,B_LITTLE32(*(long *)&c[0])&0xffffff);
                z1 = z0+1;
            }
        }
    free(ylen); kclose(fil); return(0);
}

#if 0
//While this code works, it's way too slow and can only cause trouble.
static long loadvxl (const char *filnam)
{
    long i, j, x, y, z, fil;
    unsigned char *v, *vbuf;

    fil = kopen4load((char *)filnam,0); if (fil < 0) return(-1);
    kread(fil,&i,4);
    kread(fil,&xsiz,4);
    kread(fil,&ysiz,4);
if ((i != 0x09072000) || (xsiz != 1024) || (ysiz != 1024)) { kclose(fil); return(-1); }
    zsiz = 256;
    klseek(fil,96,SEEK_CUR); //skip pos&orient
    xpiv = ((float)xsiz)*.5;
    ypiv = ((float)ysiz)*.5;
    zpiv = ((float)zsiz)*.5;

    yzsiz = ysiz*zsiz; i = ((xsiz*yzsiz+31)>>3);
    vbit = (long *)malloc(i); if (!vbit) { kclose(fil); return(-1); }
    memset(vbit,-1,i);

    vcolhashsizm1 = 1048576-1;
    vcolhashead = (long *)malloc((vcolhashsizm1+1)*sizeof(long)); if (!vcolhashead) { kclose(fil); return(-1); }
    memset(vcolhashead,-1,(vcolhashsizm1+1)*sizeof(long));

    //Allocate huge buffer and load rest of file into it...
    i = kfilelength(fil)-ktell(fil);
    vbuf = (unsigned char *)malloc(i); if (!vbuf) { kclose(fil); return(-1); }
    kread(fil,vbuf,i);
    kclose(fil);

    v = vbuf;
    for (y=0;y<ysiz;y++)
        for (x=0,j=y*zsiz;x<xsiz;x++,j+=yzsiz)
        {
            z = 0;
            while (1)
            {
                setzrange0(vbit,j+z,j+v[1]);
                for (z=v[1];z<=v[2];z++) putvox(x,y,z,(*(long *)&v[(z-v[1]+1)<<2])&0xffffff);
                if (!v[0]) break; z = v[2]-v[1]-v[0]+2; v += v[0]*4;
                for (z+=v[3];z<v[3];z++) putvox(x,y,z,(*(long *)&v[(z-v[3])<<2])&0xffffff);
            }
            v += ((((long)v[2])-((long)v[1])+2)<<2);
        }
    free(vbuf); return(0);
}
#endif

static void voxfree (voxmodel *m)
{
    if (!m) return;
    if (m->mytex) free(m->mytex);
    if (m->quad) free(m->quad);
    if (m->texid) free(m->texid);
    free(m);
}

static voxmodel *voxload (const char *filnam)
{
    long i, is8bit, ret;
    voxmodel *vm;

    i = strlen(filnam)-4; if (i < 0) return(0);
if (!Bstrcasecmp(&filnam[i],".vox")) { ret = loadvox(filnam); is8bit = 1; }
    else if (!Bstrcasecmp(&filnam[i],".kvx")) { ret = loadkvx(filnam); is8bit = 1; }
    else if (!Bstrcasecmp(&filnam[i],".kv6")) { ret = loadkv6(filnam); is8bit = 0; }
    //else if (!Bstrcasecmp(&filnam[i],".vxl")) { ret = loadvxl(filnam); is8bit = 0; }
    else return(0);
    if (ret >= 0) vm = vox2poly(); else vm = 0;
    if (vm)
    {
        vm->mdnum = 1; //VOXel model id
        vm->scale = vm->bscale = 1.0;
        vm->xsiz = xsiz; vm->ysiz = ysiz; vm->zsiz = zsiz;
        vm->xpiv = xpiv; vm->ypiv = ypiv; vm->zpiv = zpiv;
        vm->is8bit = is8bit;

        vm->texid = (unsigned int *)calloc(MAXPALOOKUPS,sizeof(unsigned int));
        if (!vm->texid) { voxfree(vm); vm = 0; }
    }
    if (shcntmal) { free(shcntmal); shcntmal = 0; }
    if (vbit) { free(vbit); vbit = 0; }
    if (vcol) { free(vcol); vcol = 0; vnum = 0; vmax = 0; }
    if (vcolhashead) { free(vcolhashead); vcolhashead = 0; }
    return(vm);
}

//Draw voxel model as perfect cubes
static int voxdraw (voxmodel *m, spritetype *tspr)
{
    point3d fp, m0, a0;
    long i, j, k, fi, *lptr, xx, yy, zz;
    float ru, rv, uhack[2], vhack[2], phack[2], clut[6] = {1,1,1,1,1,1}; //1.02,1.02,0.94,1.06,0.98,0.98};
    float f, g, k0, k1, k2, k3, k4, k5, k6, k7, mat[16], omat[16], pc[4];
    vert_t *vptr;

    if ((int)m == (int)0xffffffff) // hackhackhack
        return 0;
    if ((tspr->cstat&48)==32) return 0;

    //updateanimation((md2model *)m,tspr);

    m0.x = m->scale;
    m0.y = m->scale;
    m0.z = m->scale;
    a0.x = a0.y = 0; a0.z = m->zadd*m->scale;

    //if (globalorientation&8) //y-flipping
    //{
    //   m0.z = -m0.z; a0.z = -a0.z;
    //      //Add height of 1st frame (use same frame to prevent animation bounce)
    //   a0.z += m->zsiz*m->scale;
    //}
    //if (globalorientation&4) { m0.y = -m0.y; a0.y = -a0.y; } //x-flipping

    f = ((float)tspr->xrepeat)*(256.0/320.0)/64.0*m->bscale;
    m0.x *= f; a0.x *= f; f = -f;
    m0.y *= f; a0.y *= f;
    f = ((float)tspr->yrepeat)/64.0*m->bscale;
    m0.z *= f; a0.z *= f;

    k0 = tspr->z;
    if (globalorientation&128) k0 += (float)((tilesizy[tspr->picnum]*tspr->yrepeat)<<1);

    f = (65536.0*512.0)/((float)xdimen*viewingrange);
    g = 32.0/((float)xdimen*gxyaspect);
    m0.y *= f; a0.y = (((float)(tspr->x-globalposx))/  1024.0 + a0.y)*f;
    m0.x *=-f; a0.x = (((float)(tspr->y-globalposy))/ -1024.0 + a0.x)*-f;
    m0.z *= g; a0.z = (((float)(k0     -globalposz))/-16384.0 + a0.z)*g;

    k0 = ((float)(tspr->x-globalposx))*f/1024.0;
    k1 = ((float)(tspr->y-globalposy))*f/1024.0;
    f = gcosang2*gshang;
    g = gsinang2*gshang;
    k4 = (float)sintable[(tspr->ang+spriteext[tspr->owner].angoff+1024)&2047] / 16384.0;
    k5 = (float)sintable[(tspr->ang+spriteext[tspr->owner].angoff+ 512)&2047] / 16384.0;
    k2 = k0*(1-k4)+k1*k5;
    k3 = k1*(1-k4)-k0*k5;
    k6 = f*gstang - gsinang*gctang; k7 = g*gstang + gcosang*gctang;
    mat[0] = k4*k6 + k5*k7; mat[4] = gchang*gstang; mat[ 8] = k4*k7 - k5*k6; mat[12] = k2*k6 + k3*k7;
    k6 = f*gctang + gsinang*gstang; k7 = g*gctang - gcosang*gstang;
    mat[1] = k4*k6 + k5*k7; mat[5] = gchang*gctang; mat[ 9] = k4*k7 - k5*k6; mat[13] = k2*k6 + k3*k7;
    k6 =           gcosang2*gchang; k7 =           gsinang2*gchang;
    mat[2] = k4*k6 + k5*k7; mat[6] =-gshang;        mat[10] = k4*k7 - k5*k6; mat[14] = k2*k6 + k3*k7;

    mat[12] += a0.y*mat[0] + a0.z*mat[4] + a0.x*mat[ 8];
    mat[13] += a0.y*mat[1] + a0.z*mat[5] + a0.x*mat[ 9];
    mat[14] += a0.y*mat[2] + a0.z*mat[6] + a0.x*mat[10];

    //Mirrors
if (grhalfxdown10x < 0) { mat[0] = -mat[0]; mat[4] = -mat[4]; mat[8] = -mat[8]; mat[12] = -mat[12]; }

    //------------
    //bit 10 is an ugly hack in game.c\animatesprites telling MD2SPRITE
    //to use Z-buffer hacks to hide overdraw problems with the shadows
    if (tspr->cstat&1024)
    {
        bglDepthFunc(GL_LESS); //NEVER,LESS,(,L)EQUAL,GREATER,(NOT,G)EQUAL,ALWAYS
        bglDepthRange(0.0,0.9999);
    }
    bglPushAttrib(GL_POLYGON_BIT);
    if ((grhalfxdown10x >= 0) /*^ ((globalorientation&8) != 0) ^ ((globalorientation&4) != 0)*/) bglFrontFace(GL_CW); else bglFrontFace(GL_CCW);
    bglEnable(GL_CULL_FACE);
    bglCullFace(GL_BACK);

    bglEnable(GL_TEXTURE_2D);

    pc[0] = pc[1] = pc[2] = ((float)(numpalookups-min(max((globalshade * shadescale)+m->shadeoff,0),numpalookups)))/((float)numpalookups);
    pc[0] *= (float)hictinting[globalpal].r / 255.0;
    pc[1] *= (float)hictinting[globalpal].g / 255.0;
    pc[2] *= (float)hictinting[globalpal].b / 255.0;
if (tspr->cstat&2) { if (!(tspr->cstat&512)) pc[3] = 0.66; else pc[3] = 0.33; } else pc[3] = 1.0;
    if (tspr->cstat&2 && (!peelcompiling)) bglEnable(GL_BLEND); //else bglDisable(GL_BLEND);
    //------------

    //transform to Build coords
    memcpy(omat,mat,sizeof(omat));
    f = 1.f/64.f;
    g = m0.x*f; mat[0] *= g; mat[1] *= g; mat[2] *= g;
    g = m0.y*f; mat[4] = omat[8]*g; mat[5] = omat[9]*g; mat[6] = omat[10]*g;
    g =-m0.z*f; mat[8] = omat[4]*g; mat[9] = omat[5]*g; mat[10] = omat[6]*g;
    mat[12] -= (m->xpiv*mat[0] + m->ypiv*mat[4] + (m->zpiv+m->zsiz*.5)*mat[ 8]);
    mat[13] -= (m->xpiv*mat[1] + m->ypiv*mat[5] + (m->zpiv+m->zsiz*.5)*mat[ 9]);
    mat[14] -= (m->xpiv*mat[2] + m->ypiv*mat[6] + (m->zpiv+m->zsiz*.5)*mat[10]);
    bglMatrixMode(GL_MODELVIEW); //Let OpenGL (and perhaps hardware :) handle the matrix rotation
    mat[3] = mat[7] = mat[11] = 0.f; mat[15] = 1.f;

    bglLoadMatrixf(mat);

    ru = 1.f/((float)m->mytexx);
    rv = 1.f/((float)m->mytexy);
#if (VOXBORDWIDTH == 0)
    uhack[0] = ru*.125; uhack[1] = -uhack[0];
    vhack[0] = rv*.125; vhack[1] = -vhack[0];
#endif
    phack[0] = 0; phack[1] = 1.f/256.f;

    if (!m->texid[globalpal]) m->texid[globalpal] = gloadtex(m->mytex,m->mytexx,m->mytexy,m->is8bit,globalpal);
    else bglBindTexture(GL_TEXTURE_2D,m->texid[globalpal]);
    bglBegin(GL_QUADS);
    for (i=0,fi=0;i<m->qcnt;i++)
    {
        if (i == m->qfacind[fi]) { f = clut[fi++]; bglColor4f(pc[0]*f,pc[1]*f,pc[2]*f,pc[3]*f); }
        vptr = &m->quad[i].v[0];

        xx = vptr[0].x+vptr[2].x;
        yy = vptr[0].y+vptr[2].y;
        zz = vptr[0].z+vptr[2].z;

        for (j=0;j<4;j++)
        {
#if (VOXBORDWIDTH == 0)
            bglTexCoord2f(((float)vptr[j].u)*ru+uhack[vptr[j].u!=vptr[0].u],
                          ((float)vptr[j].v)*rv+vhack[vptr[j].v!=vptr[0].v]);
#else
            bglTexCoord2f(((float)vptr[j].u)*ru,((float)vptr[j].v)*rv);
#endif
            fp.x = ((float)vptr[j].x) - phack[xx>vptr[j].x*2] + phack[xx<vptr[j].x*2];
            fp.y = ((float)vptr[j].y) - phack[yy>vptr[j].y*2] + phack[yy<vptr[j].y*2];
            fp.z = ((float)vptr[j].z) - phack[zz>vptr[j].z*2] + phack[zz<vptr[j].z*2];
            bglVertex3fv((float *)&fp);
        }
    }
    bglEnd();

    //------------
    bglDisable(GL_CULL_FACE);
    bglPopAttrib();
    if (tspr->cstat&1024)
    {
        bglDepthFunc(GL_LESS); //NEVER,LESS,(,L)EQUAL,GREATER,(NOT,G)EQUAL,ALWAYS
        bglDepthRange(0.0,0.99999);
    }
    bglLoadIdentity();
    return 1;
}

//---------------------------------------- VOX LIBRARY ENDS ----------------------------------------
//--------------------------------------- MD LIBRARY BEGINS  ---------------------------------------

mdmodel *mdload (const char *filnam)
{
    mdmodel *vm;
    int fil;
    long i;

    vm = (mdmodel*)voxload(filnam); if (vm) return(vm);

    fil = kopen4load((char *)filnam,0); if (fil < 0) return(0);
    kread(fil,&i,4); klseek(fil,0,SEEK_SET);
    switch (B_LITTLE32(i))
    {
    case 0x32504449:
//        initprintf("Warning: model '%s' is version IDP2; wanted version IDP3\n",filnam);
        vm = (mdmodel*)md2load(fil,filnam); break; //IDP2
    case 0x33504449:
        vm = (mdmodel*)md3load(fil); break; //IDP3
    default:
        vm = (mdmodel*)0; break;
    }
    kclose(fil);
    return(vm);
}

int mddraw (spritetype *tspr)
{
    mdanim_t *anim;
    mdmodel *vm;

    if (maxmodelverts > allocmodelverts)
    {
        point3d *vl = (point3d *)realloc(vertlist,sizeof(point3d)*maxmodelverts);
        if (!vl) { OSD_Printf("ERROR: Not enough memory to allocate %d vertices!\n",maxmodelverts); return 0; }
        vertlist = vl; allocmodelverts = maxmodelverts;
    }

    vm = models[tile2model[tspr->picnum].modelid];
    if (vm->mdnum == 1) { return voxdraw((voxmodel *)vm,tspr); }
    if (vm->mdnum == 3) { return md3draw((md3model *)vm,tspr); }
    return 0;
}

void mdfree (mdmodel *vm)
{
    if (vm->mdnum == 1) { voxfree((voxmodel *)vm); return; }
    if (vm->mdnum == 3) { md3free((md3model *)vm); return; }
}

//---------------------------------------- MD LIBRARY ENDS  ----------------------------------------

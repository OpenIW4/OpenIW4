#pragma once

enum XAssetType : std::uint32_t
{
    ASSET_TYPE_PHYSPRESET = 0x0,
    ASSET_TYPE_PHYSCOLLMAP = 0x1,
    ASSET_TYPE_XANIMPARTS = 0x2,
    ASSET_TYPE_XMODEL_SURFS = 0x3,
    ASSET_TYPE_XMODEL = 0x4,
    ASSET_TYPE_MATERIAL = 0x5,
    ASSET_TYPE_PIXELSHADER = 0x6,
    ASSET_TYPE_VERTEXSHADER = 0x7,
    ASSET_TYPE_VERTEXDECL = 0x8,
    ASSET_TYPE_TECHNIQUE_SET = 0x9,
    ASSET_TYPE_IMAGE = 0xA,
    ASSET_TYPE_SOUND = 0xB,
    ASSET_TYPE_SOUND_CURVE = 0xC,
    ASSET_TYPE_LOADED_SOUND = 0xD,
    ASSET_TYPE_CLIPMAP_SP = 0xE,
    ASSET_TYPE_CLIPMAP_MP = 0xF,
    ASSET_TYPE_COMWORLD = 0x10,
    ASSET_TYPE_GAMEWORLD_SP = 0x11,
    ASSET_TYPE_GAMEWORLD_MP = 0x12,
    ASSET_TYPE_MAP_ENTS = 0x13,
    ASSET_TYPE_FXWORLD = 0x14,
    ASSET_TYPE_GFXWORLD = 0x15,
    ASSET_TYPE_LIGHT_DEF = 0x16,
    ASSET_TYPE_UI_MAP = 0x17,
    ASSET_TYPE_FONT = 0x18,
    ASSET_TYPE_MENULIST = 0x19,
    ASSET_TYPE_MENU = 0x1A,
    ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
    ASSET_TYPE_WEAPON = 0x1C,
    ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
    ASSET_TYPE_FX = 0x1E,
    ASSET_TYPE_IMPACT_FX = 0x1F,
    ASSET_TYPE_AITYPE = 0x20,
    ASSET_TYPE_MPTYPE = 0x21,
    ASSET_TYPE_CHARACTER = 0x22,
    ASSET_TYPE_XMODELALIAS = 0x23,
    ASSET_TYPE_RAWFILE = 0x24,
    ASSET_TYPE_STRINGTABLE = 0x25,
    ASSET_TYPE_LEADERBOARD = 0x26,
    ASSET_TYPE_STRUCTURED_DATA_DEF = 0x27,
    ASSET_TYPE_TRACER = 0x28,
    ASSET_TYPE_VEHICLE = 0x29,
    ASSET_TYPE_ADDON_MAP_ENTS = 0x2A,
    ASSET_TYPE_COUNT = 0x2B,
    ASSET_TYPE_STRING = 0x2B,
    ASSET_TYPE_ASSETLIST = 0x2C,
};

typedef struct XZoneInfo
{
    const char* name;
    std::uint32_t allocFlags;
    std::uint32_t unloadFlags;
} XZoneInfo;

struct cmd_function_s
{
    cmd_function_s* next;
    const char* name;
    const char* autoCompleteDir;
    const char* autoCompleteExt;
    void(__cdecl* function)();
    int flags;
};

enum errorParm_t
{
    ERR_FATAL = 0x0,
    ERR_DROP = 0x1,
    ERR_SERVERDISCONNECT = 0x2,
    ERR_DISCONNECT = 0x3,
    ERR_SCRIPT = 0x4,
    ERR_SCRIPT_DROP = 0x5,
    ERR_LOCALIZATION = 0x6,
    ERR_MAPLOADERRORSUMMARY = 0x7,
};

struct msg_t
{
    bool overflowed;
    bool readOnly;
    char* data;
    char* splitData;
    std::int32_t maxSize;
    std::int32_t curSize;
    std::int32_t splitSize;
    std::int32_t readCount;
    std::int32_t bit; //depending on use, size could be smaller than an int as we may not need all fields
    std::int32_t lastEntityRef;
};

struct language_t
{
    char* loc_string;
};

struct localization_t
{
    char* language;
    char* strings;
};

struct TempPriority
{
    void* threadHandle;
    int oldPriority;
};

struct FastCriticalSection
{
    volatile long readCount;
    volatile long writeCount;
    TempPriority tempPriority;
};

enum $5C7603243A829E5BC6BA7F928C72347C
{
    THREAD_VALUE_PROF_STACK = 0x0,
    THREAD_VALUE_VA = 0x1,
    THREAD_VALUE_COM_ERROR = 0x2,
    THREAD_VALUE_TRACE = 0x3,
    THREAD_VALUE_COUNT = 0x4,
};

struct va_info_t
{
    char va_string[2][1024];
    int index;
};

struct nodetype
{
    nodetype* left;
    nodetype* right;
    nodetype* parent;
    std::int32_t weight;
    std::int32_t symbol;
};

struct huff_t
{
    std::int32_t blocNode;
    std::int32_t blocPtrs;
    nodetype* tree;
    nodetype* loc[257];
    nodetype** freelist;
    nodetype nodeList[768];
    nodetype* nodePtrs[768];
};

struct huffman_t
{
    huff_t compressDecompress;
};

enum PMem_Source
{
    PMEM_SOURCE_EXTERNAL,
    PMEM_SOURCE_DATABASE,
    PMEM_SOURCE_MOVIE,
};

struct PhysicalMemoryAllocation
{
    const char* name;
    unsigned int pos;
};

struct PhysicalMemoryPrim
{
    const char* allocName;
    unsigned int allocListCount;
    unsigned int pos;
    PhysicalMemoryAllocation allocList[32];
};

struct PhysicalMemory
{
    char* buf;
    PhysicalMemoryPrim prim[2];
};

union DvarValue
{
    bool enabled;
    int integer;
    unsigned int unsignedInt;
    float value;
    float vector[4];
    const char* string;
    unsigned char color[4];
};

struct $BFBB53559BEAC4289F32B924847E59CB
{
    int stringCount;
    const char** strings;
};

struct $9CA192F9DB66A3CB7E01DE78A0DEA53D
{
    int min;
    int max;
};

struct $251C2428A496074035CACA7AAF3D55BD
{
    float min;
    float max;
};

union DvarLimits
{
    $BFBB53559BEAC4289F32B924847E59CB enumeration;
    $9CA192F9DB66A3CB7E01DE78A0DEA53D integer;
    $251C2428A496074035CACA7AAF3D55BD value;
    $251C2428A496074035CACA7AAF3D55BD vector;
};

enum dvar_type : char
{
    DVAR_TYPE_BOOL = 0x0,
    DVAR_TYPE_FLOAT = 0x1,
    DVAR_TYPE_FLOAT_2 = 0x2,
    DVAR_TYPE_FLOAT_3 = 0x3,
    DVAR_TYPE_FLOAT_4 = 0x4,
    DVAR_TYPE_INT = 0x5,
    DVAR_TYPE_ENUM = 0x6,
    DVAR_TYPE_STRING = 0x7,
    DVAR_TYPE_COLOR = 0x8,
    DVAR_TYPE_FLOAT_3_COLOR = 0x9,
    DVAR_TYPE_COUNT = 0xA,
};

struct dvar_t
{
    const char* name;
    const char* description;
    unsigned int flags;
    dvar_type type;
    bool modified;
    DvarValue current;
    DvarValue latched;
    DvarValue reset;
    DvarLimits domain;
    bool(__cdecl* domainFunc)(dvar_t*, DvarValue);
    dvar_t* hashNext;
};

#include "DB.hpp"
#include "../Sys/Sys.hpp"

#include <utils/memory/memory.hpp>

//DONE : 0x00440EC0
void DB_Update()
{
    if (!Sys_IsDatabaseReady2())
    {
        if (Sys_IsDatabaseReady())
        {
            DB_PostLoadXZone();
        }
    }
}

//THUNK : 0x005BC6A0
void DB_PostLoadXZone()
{
    memory::call<void()>(0x005BC6A0)();
}

//May need more asset cases
//TODO : 0x0049196B
int DB_GetXAssetTypeSize(XAssetType a1)
{
    int result;

    switch (a1)
    {
    case ASSET_TYPE_PHYSPRESET:
    case ASSET_TYPE_LOADED_SOUND:
    case ASSET_TYPE_MAP_ENTS:
        result = 44;
        break;
    case ASSET_TYPE_PHYSCOLLMAP:
        result = 72;
        break;
    case ASSET_TYPE_XANIMPARTS:
        result = 88;
        break;
    case ASSET_TYPE_XMODEL_SURFS:
        result = 36;
        break;
    case ASSET_TYPE_XMODEL:
        result = 304;
        break;
    case ASSET_TYPE_MATERIAL:
        result = 96;
        break;
    case ASSET_TYPE_PIXELSHADER:
    case ASSET_TYPE_VERTEXSHADER:
    case ASSET_TYPE_COMWORLD:
    case ASSET_TYPE_LIGHT_DEF:
        result = 16;
        break;
    case ASSET_TYPE_VERTEXDECL:
        result = 100;
        break;
    case ASSET_TYPE_TECHNIQUE_SET:
        result = 204;
        break;
    case ASSET_TYPE_IMAGE:
        result = 32;
        break;
    case ASSET_TYPE_SOUND:
        result = 12;
        break;
    case ASSET_TYPE_SOUND_CURVE:
        result = 136;
        break;
    case ASSET_TYPE_CLIPMAP_SP:
    case ASSET_TYPE_CLIPMAP_MP:
        result = 256;
        break;
    case ASSET_TYPE_GAMEWORLD_SP:
        result = 56;
        break;
    case ASSET_TYPE_GAMEWORLD_MP:
        result = 8;
        break;
    case ASSET_TYPE_FXWORLD:
        result = 116;
        break;
    case ASSET_TYPE_GFXWORLD:
        result = 628;
        break;
    case ASSET_TYPE_WEAPON:
        result = 1688;
    }
    return result;
}

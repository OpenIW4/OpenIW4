#include "Images.hpp"
#include "../Com/Com.hpp"

//DONE : 0x53A430
/*
* this will crash the game if we use this version of the function
* the vanilla function has no prologue or epilogue (unsure why)
* there was limited success with turning on optimizations for the project and using __fastcall but it still crashes
*/
bool Image_VerifyHeader(GfxImageFileHeader* imageFile, const char* filepath)
{
	if (imageFile->tag[0] == 'I' && imageFile->tag[1] == 'W' && imageFile->tag[2] == 'i')
	{
		std::uint8_t version = 8;
		if (imageFile->version == version)
		{
			//Com_Printf(10, "Image '%s' reports version %i\n", filepath, imageFile->version);
			return true;
		}
		else
		{
			Com_PrintError(10, "ERROR: image '%s' is version %i but should be version %i\n", filepath, imageFile->version, version);
			return false;
		}
	}
	else
	{
		Com_PrintError(10, "ERROR: image '%s' is not an IW image\n", filepath);
		return false;
	}
}

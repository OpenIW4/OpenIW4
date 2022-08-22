#include "Unsorted.hpp"

//check header for description


char* getBuildNumber()
{
    char* buffer = *(char**)0x7014640;
    sprintf(buffer, "%s %s %s %s", "177", "latest", __DATE__, __TIME__);
    return buffer;
}

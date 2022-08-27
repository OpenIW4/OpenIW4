#include "NET.hpp"

//DONE : 0x4E7720
const char* NET_ErrorString()
{
    std::uint32_t error = WSAGetLastError();
    std::uint32_t v3;
    const char* result;

    if (error > 11001)
    {
        std::uint32_t v2 = error - 11002;
        if (v2)
        {
            v3 = v2 - 1;
            if (v3)
            {
                if (v3 == 1)
                {
                    return "WSANO_DATA";
                }
                else
                {
                    return "UNKNOWN ERROR";
                }
            }
            else
            {
                return "WSANO_RECOVERY";
            }
        }
        else
        {
            return "WSATRY_AGAIN";
        }
    }
    else if (error == 11001)
    {
        return "WSAHOST_NOT_FOUND";
    }
    else
    {
        switch (error)
        {
        case 10004:
            result = "WSAEINTR";
            break;
        case 10009:
            result = "WSAEBADF";
            break;
        case 10013:
            result = "WSAEACCES";
            break;
        case 10014:
            result = "WSAEFAULT";
            break;
        case 10022:
            result = "WSAEINVAL";
            break;
        case 10024:
            result = "WSAEMFILE";
            break;
        case 10035:
            result = "WSAEWOULDBLOCK";
            break;
        case 10036:
            result = "WSAEINPROGRESS";
            break;
        case 10037:
            result = "WSAEALREADY";
            break;
        case 10038:
            result = "WSAENOTSOCK";
            break;
        case 10039:
            result = "WSAEDESTADDRREQ";
            break;
        case 10040:
            result = "WSAEMSGSIZE";
            break;
        case 10041:
            result = "WSAEPROTOTYPE";
            break;
        case 10042:
            result = "WSAENOPROTOOPT";
            break;
        case 10043:
            result = "WSAEPROTONOSUPPORT";
            break;
        case 10044:
            result = "WSAESOCKTNOSUPPORT";
            break;
        case 10045:
            result = "WSAEOPNOTSUPP";
            break;
        case 10046:
            result = "WSAEPFNOSUPPORT";
            break;
        case 10047:
            result = "WSAEAFNOSUPPORT";
            break;
        case 10048:
            result = "WSAEADDRINUSE";
            break;
        case 10049:
            result = "WSAEADDRNOTAVAIL";
            break;
        case 10050:
            result = "WSAENETDOWN";
            break;
        case 10051:
            result = "WSAENETUNREACH";
            break;
        case 10052:
            result = "WSAENETRESET";
            break;
        case 10053:
            result = "WSWSAECONNABORTEDAEINTR";
            break;
        case 10054:
            result = "WSAECONNRESET";
            break;
        case 10055:
            result = "WSAENOBUFS";
            break;
        case 10056:
            result = "WSAEISCONN";
            break;
        case 10057:
            result = "WSAENOTCONN";
            break;
        case 10058:
            result = "WSAESHUTDOWN";
            break;
        case 10059:
            result = "WSAETOOMANYREFS";
            break;
        case 10060:
            result = "WSAETIMEDOUT";
            break;
        case 10061:
            result = "WSAECONNREFUSED";
            break;
        case 10062:
            result = "WSAELOOP";
            break;
        case 10063:
            result = "WSAENAMETOOLONG";
            break;
        case 10064:
            result = "WSAEHOSTDOWN";
            break;
        case 10065:
            result = "WSAEHOSTUNREACHABLE";
            break;
        case 10091:
            result = "WSASYSNOTREADY";
            break;
        case 10092:
            result = "WSAVERNOTSUPPORTED";
            break;
        case 10093:
            result = "WSANOTINITIALISED";
            break;
        case 10101:
            result = "WSAEDISCON";
            break;
        default:
            return "UNKNOWN ERROR";
        }
    }

    return result;
}

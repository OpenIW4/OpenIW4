#include "loader/loader.hpp"

#include "Sys/Sys.hpp"

#include <utils/memory/memory.hpp>

void main()
{

}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	loader::load("iw4mp.exe");
	//memory::replace(0x005C93C0, main);	//Will replace when main is fully reimped
	return memory::call<int()>(0x0066250F)();
}
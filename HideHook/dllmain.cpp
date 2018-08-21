// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

HINSTANCE hInstance; //钩子函数所在模块的句柄  

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInstance = (HINSTANCE)hModule;
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


// HideHook.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


// Hook.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "HideHook.h"

using namespace std;


extern HINSTANCE hInstance; //钩子函数所在模块的句柄  

#pragma data_seg("SHARE")
static HHOOK hHook = NULL;
#pragma data_seg

BOOL IsChildWindow(HWND hwnd, std::string parentName)
{
	HWND currHwnd = hwnd;
	while (currHwnd != NULL)
	{
		currHwnd = GetParent(currHwnd);

		char test[1024];
		memset(test, 0, 1024);
		GetWindowTextA(currHwnd, (LPSTR)test, 1024);

		if (string(test).find(parentName) != string::npos)
		{
			return true;
		}

	}

	return false;
}

void GetProcessName(HWND hwnd, char* p)
{
	DWORD dwId = 0;
	GetWindowThreadProcessId(hwnd, &dwId);
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwId);

	DWORD dwLen = MAX_PATH;
	QueryFullProcessImageNameA(handle, 0, p, &dwLen);

}

BOOL IsSameProcess(HWND hwnd, std::string processName)
{
	DWORD dwId = 0;
	GetWindowThreadProcessId(hwnd, &dwId);
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwId);

	TCHAR szFileName[MAX_PATH] = { 0 };
	DWORD dwLen = MAX_PATH;
	GetModuleFileNameEx(handle, 0, szFileName, dwLen);

	if (string((char*)szFileName).find(processName) != string::npos)
	{
		return true;
	}

	return false;
}

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PCWPSTRUCT msg = (PCWPSTRUCT)lParam;
	LPWINDOWPOS POS = (LPWINDOWPOS)msg->lParam;

	if (nCode == HC_ACTION)
	{
		//if (IsSameProcess(msg->hwnd, "wConvert"))
		//{
		//	ShowWindow(msg->hwnd, SW_HIDE);
		//}

		if (msg->message != WM_WINDOWPOSCHANGED)
		{
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		}

		LPWINDOWPOS lp = (LPWINDOWPOS)lParam;
		if ((lp->flags | SWP_SHOWWINDOW) == 0)
		{
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		}

		CHAR szFileName[MAX_PATH];
		memset(szFileName, 0x00, sizeof(szFileName));

		GetProcessName(msg->hwnd, szFileName);

		if (strstr(szFileName, "wConvert") != NULL
			|| strstr(szFileName, "wGpsIns") != NULL
			|| strstr(szFileName, "Download") != NULL)
		{
			//UINT u = SWP_SHOWWINDOW;
			//lp->flags = lp->flags & ~u;

			//u = SWP_HIDEWINDOW;
			//lp->flags = lp->flags | u;


			ShowWindow(msg->hwnd, SW_HIDE);
			SetActiveWindow(msg->hwnd);
			return 0;
		}

		//string processName = "aaa"; GetProcessName(msg->hwnd);
		//if (processName.find("wConvert") != string::npos 
		//  ||processName.find("wGpsIns")  != string::npos)
		//{
		//	ShowWindow(msg->hwnd, SW_HIDE);
		//	SetActiveWindow(msg->hwnd);

		//	return 0;
		//}

		//GetWindowTextA(msg->hwnd, (LPSTR)test, 1024);

		//if (strcmp("Waypoint - Inertial Explorer 8.60", test) == 0
		//	|| IsChildWindow(msg->hwnd, "Inertial Explorer")
		//	|| IsChildWindow(msg->hwnd, "Convert Raw GNSS data to GPB")
		//	|| strcmp("Project Wizard", test) == 0
		//	|| strcmp("Convert Raw GNSS data to GPB", test) == 0
		//	|| strcmp("Converting RINEX to GPB (1/1)", test) == 0
		//	|| strcmp("Conversion Complete (1/1 files succeeded)", test) == 0
		//	|| strcmp("Converting NovAtel OEM/SPAN to GPB (1/2)", test) == 0
		//	|| strcmp("RINEX Options", test) == 0
		//	|| strcmp("Version 8.60", test) == 0
		//	|| strcmp("Enter Project File", test) == 0
		//	|| strcmp("Select GNSS File", test) == 0
		//	|| strcmp("Select IMU File (Waypoint Format)", test) == 0
		//	|| strcmp("Add DMR File", test) == 0
		//	|| strcmp("Process GNSS", test) == 0
		//	|| strcmp("Differential GNSS Preprocessing ...", test) == 0
		//	|| strstr(test, "Differential GPS 1") != NULL
		//	|| strstr(test, "Differential GPS 2") != NULL
		//	|| strcmp("Process Tightly Coupled", test) == 0
		//	|| strcmp("TC GNSS Settings", test) == 0
		//	|| strcmp("IMU Processing Settings", test) == 0
		//	|| strcmp("Alignment Options", test) == 0
		//	|| strcmp("Tightly Coupled Differential Preprocessing ...", test) == 0
		//	|| strcmp("Processing GPS-IMU TC 1", test) == 0
		//	|| strcmp("Processing GPS-IMU TC 2", test) == 0
		//	|| strstr(test, "GPS-IMU TC 1") != NULL
		//	|| strcmp("Combining Forward and Reverse IMU", test) == 0
		//	|| strcmp("Smoothing ...", test) == 0
		//	|| strcmp("Export Coordinates Wizard", test) == 0)
		//{
		//	string name = GetProcessName(msg->hwnd);
		//	MessageBoxA(NULL, "ddd", name.c_str(), MB_OK);
		//	//ShowWindow(msg->hwnd, SW_HIDE);
		//	//SetActiveWindow(msg->hwnd);
		//				
		//	return 0;
		//}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}



BOOL SetHook()
{
	hHook = SetWindowsHookEx(WH_CALLWNDPROC, HookProc, hInstance, NULL);
	return hHook != NULL;
}

BOOL EndHook()
{
	return UnhookWindowsHookEx(hHook);
}
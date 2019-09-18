#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <algorithm>

#include <Windows.h>

HHOOK hhook_preview = 0;
HHOOK hhook_slobs = 0;
HMODULE dll_handle = 0;

void print_window_info(HWND window_hwnd)
{
	WINDOWINFO wi;
	GetWindowInfo(window_hwnd, &wi);

	std::cout << std::dec << "window pos [" << wi.rcWindow.left << "," << wi.rcWindow.top << "], size [" << wi.rcWindow.right - wi.rcWindow.left << "," << wi.rcWindow.bottom - wi.rcWindow.top << "]" << std::endl;

	std::cout << "window info dwExStyle " << std::hex << (DWORD)wi.dwExStyle << std::endl;
	std::cout << "window info dwStyle " << std::hex << (DWORD)wi.dwStyle << std::dec << std::endl;
}

int main(int argc, char* argv[])
{
	HWND first_window = 0;
	HWND preview_window = 0;

	while (first_window = FindWindowEx(0, first_window, L"Chrome_WidgetWin_1", NULL))
	{
		std::cout <<  "Electron window found " << (DWORD)first_window << std::endl;

		if (preview_window = FindWindowEx(first_window, 0, L"Win32DisplayClass", NULL))
		{
			std::cout <<  "Preview window found " << (DWORD)preview_window << std::endl;
			print_window_info(preview_window);

			DWORD processID = 0;
			DWORD actualProcId = GetWindowThreadProcessId(preview_window, &processID);
			DWORD slobsProcId = GetWindowThreadProcessId(first_window, &processID);

			if (actualProcId) {
				std::cout << "Process ID is: " << processID << std::endl;
				dll_handle = LoadLibraryEx(L"msg_interceptor.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);

				if (dll_handle)
				{
					HOOKPROC hook_addr = (HOOKPROC)GetProcAddress(dll_handle, "NextMsgHook"); 
					if (hook_addr)
					{
						hhook_preview = SetWindowsHookEx(WH_CALLWNDPROC, hook_addr, dll_handle, actualProcId);
						if (hhook_preview)
						{
							std::cout << "Successfully hooked to messages preview window " << std::endl;
						}
						else {
							std::cout <<  "Failed to hook to messages preview window " << std::endl;
						}
						
						//hhook_slobs = SetWindowsHookEx(WH_CALLWNDPROC, hook_addr, dll_handle, slobsProcId);

						//PostThreadMessage(actualProcId, WM_NULL, NULL, NULL);
						//PostThreadMessage(actualProcId, WM_PAINT, NULL, NULL);
					} else {
						std::cout << "Failed to get hook procedure !" << std::endl;
					}
				} else {
					std::cout << "Failed to load dll!" << std::endl;
				}
			} else {
				std::cout << "Unable to find the process id !" << std::endl;
			}
			
		}
	}
	
	std::cout << "Window found and hook installed " << std::endl;
	std::cout.flush();

	if (hhook_preview)
	{
		int i = 10;
		while (i--)
		{
			std::cout << "exit after " << i << "sec" << std::endl;
			Sleep(1000);
			std::cout.flush();
		}
	}

	if (hhook_preview)
	{
		UnhookWindowsHookEx(hhook_preview);
	}
	
	if (hhook_slobs)
	{
		UnhookWindowsHookEx(hhook_slobs);
	}
	
	if (dll_handle)
	{
		FreeLibrary(dll_handle);
	}
	return 0;
}
#include <windows.h>
#include <iostream>
#include <mmsystem.h>

#pragma comment(lib, "winmm")

void BatteryKeyPress()
{

}

void MSIKeyPress()
{
	LPCWSTR mediaPath = L"C:\\Program Files\\Microsoft Office\\root\\Office16\\ONENOTE.EXE";
	ShellExecute(NULL, NULL, mediaPath, NULL, NULL, SW_SHOWDEFAULT);
}

void MediaKeyPress()
{
	LPCWSTR mediaPath = L"C:\\Users\\Amares8\\AppData\\Roaming\\Spotify\\Spotify.exe";
	ShellExecute(NULL, NULL, mediaPath, NULL, NULL, SW_SHOWDEFAULT);
}


void ControlCdTray(TCHAR drive, DWORD command)
{
	// Not used here, only for debug
	MCIERROR mciError = 0;

	// Flags for MCI command
	DWORD mciFlags = MCI_WAIT | MCI_OPEN_SHAREABLE |
		MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT;

	// Open drive device and get device ID
	TCHAR elementName[] = { drive };
	MCI_OPEN_PARMS mciOpenParms = { 0 };
	mciOpenParms.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;
	mciOpenParms.lpstrElementName = elementName;
	mciError = mciSendCommand(0,
		MCI_OPEN, mciFlags, (DWORD_PTR)&mciOpenParms);

	// Eject or close tray using device ID
	MCI_SET_PARMS mciSetParms = { 0 };
	mciFlags = MCI_WAIT | command; // command is sent by caller
	mciError = mciSendCommand(mciOpenParms.wDeviceID,
		MCI_SET, mciFlags, (DWORD_PTR)&mciSetParms);

	// Close device ID
	mciFlags = MCI_WAIT;
	MCI_GENERIC_PARMS mciGenericParms = { 0 };
	mciError = mciSendCommand(mciOpenParms.wDeviceID,
		MCI_CLOSE, mciFlags, (DWORD_PTR)&mciGenericParms);
}

void EjectCDKeyPress()
{
	ControlCdTray('D', MCI_SET_DOOR_OPEN);
}

void ECOKeyPress()
{

}

void P1KeyPress()
{
	LPCWSTR p1Path = L"C:\\Windows\\System32\\cmd.exe";
	//LPCWSTR params = L"/k cd /d d:\your\path";
	ShellExecute(NULL, L"runas", p1Path, NULL, NULL, SW_SHOWDEFAULT);
	
}




LRESULT CALLBACK KBDHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	
	if (s->flags == 129)
	{
		switch (s->scanCode)
		{
			case 43:
				//std::cout << "BATTERY! " << std::endl;
				break;
			case 57:
				//std::cout << "MSI! " << std::endl;
				MSIKeyPress();
				break;
			case 59:
				//std::cout << "MEDIA! " << std::endl;
				MediaKeyPress();
				break;
			case 66:
				//std::cout << "EJECT CD! " << std::endl;
				EjectCDKeyPress();
				break;
			case 78:
				//std::cout << "ECO! " << std::endl;
				break;
			case 111:
				//std::cout << "P1! " << std::endl;
				P1KeyPress();
				break;
		}
	}
		/*
	if (true)
	{
		std::cout << "nCode: " << nCode << std::endl;
		std::cout << "scan code: " << s->scanCode << std::endl;
		std::cout << "extra info: " << s->dwExtraInfo << std::endl;
		std::cout << "flags: " << s->flags << std::endl;
		std::cout << std::endl;
	}
		*/
	
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}






int main()
{
	//std::cout << "hello" << std::endl;

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	HHOOK kbd = SetWindowsHookEx(WH_KEYBOARD_LL, &KBDHook, 0, 0);

	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(kbd);
	return 0;
}



#include "Memory.h"

HANDLE tekkenHandle;
int tekkenPid;
DWORD_PTR tekkenModulePointer;


HANDLE getProcessHandle(DWORD pid) {
	HANDLE processHandle;
	DWORD errorCode;
	processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
	if (processHandle == NULL) {
		errorCode = GetLastError();
		printf("Error! OpenProcess failed! Code: %d. Closing program.\n", errorCode);
		system("PAUSE");
		exit(0);
	}
	return processHandle;
}

DWORD getProcessId(const std::wstring& programNameExe) {
	HANDLE processesSnapshot;   //will hold a snapshot of all processes...
	PROCESSENTRY32 processInfo; //this struct will contain the process id
	DWORD errorCode;
	ZeroMemory(&processInfo, sizeof(PROCESSENTRY32)); // set all the struct's members equal to zero
	processInfo.dwSize = sizeof(processInfo); // must set the size before using it / filling it
	processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); // get snapshot of all processes
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		errorCode = GetLastError();
		printf("Error! CreateToolhelp32Snapshot failed! Code: %d. Closing program.\n", errorCode);
		system("PAUSE");
		exit(0);
	}
	Process32First(processesSnapshot, &processInfo);
	if (!programNameExe.compare(processInfo.szExeFile)) { //if names are equal
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}
	while (Process32Next(processesSnapshot, &processInfo)) {
		if (!programNameExe.compare(processInfo.szExeFile)) {
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}
	CloseHandle(processesSnapshot);
	return 0;
}

// purpose: finding the base address of a module (example: "steam_api64.dll")
DWORD_PTR getModuleBaseAddressDWORD(DWORD pid, const wchar_t* moduleName) {
	DWORD_PTR result = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, moduleName)) {
					result = (DWORD_PTR)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return result;
}


void initTekkenHandle() {
	DWORD pid, secondsDelay;
	pid = getProcessId(TEXT(TEKKEN_EXE_NAME));
	secondsDelay = 2;
	while (pid == 0) {
		std::cout << "Tekken not found. Trying again in " << secondsDelay << " seconds..." << std::endl;
		Sleep(secondsDelay * 1000); // milliseconds
		pid = getProcessId(TEXT(TEKKEN_EXE_NAME));
	}
	tekkenPid = pid; // global variable
	std::cout << "Tekken found! pid = (" << pid << ")" << std::endl;
	std::cout << "Opening Tekken process..." << std::endl;
	tekkenHandle = getProcessHandle(pid);  // global variable
	std::cout << "Opening Tekken process success!" << std::endl;
}

void initModuleAddresses() {
	tekkenModulePointer = getModuleBaseAddressDWORD(tekkenPid, TEKKEN_MODULE_NAME);
	if (tekkenModulePointer == 0) {
		std::cout << "Error finding the Tekken module base address." << std::endl;
	}
	else {
		std::cout << "Tekken module base address found." << std::endl;
	}
}
DWORD64 readDwordFromMemory(HANDLE processHandle, DWORD_PTR address) {
	DWORD64 result, errorCode;
	errorCode = ReadProcessMemory(processHandle, (void*)address, &result, sizeof(result), NULL);
	if (errorCode == 0) {
		errorCode = GetLastError();
		// make sure this is compiled into 64bit or you'll get the 299 error: trying to read x64 with x86 exe
		printf("Error! ReadProcessMemory failed in readDwordFromMemory. Address: %p, Code: %d .\n", address, errorCode);
	
		//system("PAUSE");
	}
	return result;
}

DWORD readDwordWithOffset(HANDLE processHandle, DWORD_PTR baseAddress, DWORD offset) {
	DWORD result, errorCode;

	errorCode = ReadProcessMemory(processHandle, (void*)(baseAddress + offset), &result, sizeof(result), NULL);

	if (errorCode == 0) {
		errorCode = GetLastError();

		// make sure this is compiled into 64bit or you'll get the 299 error: trying to read x64 with x86 exe
		printf("Error! ReadProcessMemory failed in readDwordWithOffset. Code: %d .\n", errorCode);
		//system("PAUSE");
	}


	return result;


}


DWORD readDwordWithOffsets(HANDLE processHandle, DWORD_PTR baseAddress, int numOffsets, DWORD* offsets) {
	DWORD_PTR result;
	DWORD errorCode;
	result = baseAddress;
	for (int i = 0; i < numOffsets; i++) {
		errorCode = ReadProcessMemory(processHandle, (void*)result, &result, sizeof(result), NULL);

		if (errorCode == 0) {
			errorCode = GetLastError();

			// make sure this is compiled into 64bit or you'll get the 299 error: trying to read x64 with x86 exe
			printf("Error! ReadProcessMemory failed in readDwordWithOffsets. Code: %d .\n", errorCode);
			//system("PAUSE");
		}
	}

	return result;
	
}

/* commented out to avoid detection when not using gui

void injectDLL() {

	if (!tekkenHandle || tekkenHandle == INVALID_HANDLE_VALUE) {
		printf("Unable to inject DLL! Handle not found or invalid!\n");
		return;
	}


	void* loc = VirtualAllocEx(tekkenHandle, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	printf("Allocated memory for storing dll path.\n");

	if (loc == 0) {
		printf("Unable to inject DLL! loc is zero\n");
		return;
	}

	WriteProcessMemory(tekkenHandle, loc, INJECTION_DLL_ABSOLUTE_PATH, strlen(INJECTION_DLL_ABSOLUTE_PATH) + 1, 0);
	printf("Wrote address of DLL to allocated memory\n");

	HANDLE injectionThread = CreateRemoteThread(tekkenHandle, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
	printf("Created injection thread\n");

	if (injectionThread) {
		CloseHandle(injectionThread);
		printf("Closed injection thread, successful injection!\n");
	}


}


//debug function to list active dlls
void listDLLs() {

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, tekkenPid);

	MODULEENTRY32 moduleEntry;

	moduleEntry.dwSize = sizeof(moduleEntry);

	HMODULE dllModule = NULL;

	if (Module32FirstW(hSnapshot, &moduleEntry))
	{
		do {
			std::wcout << "Module name: " << moduleEntry.szModule << " injectiondllname: " << INJECTION_DLL_NAME << std::endl;

			// compare just the module filename (not full path)
			if (_wcsicmp(moduleEntry.szModule, INJECTION_DLL_NAME_WCHAR) == 0)
			{
				dllModule = moduleEntry.hModule;   // base address in remote
			}
		} while (Module32NextW(hSnapshot, &moduleEntry));
	}

	if (dllModule) {
		std::cout << "Injected DLL still in target process!" << std::endl;
	}
	else {
		printf("ERROR: could not find dll in target process to uninject!");
	}

	CloseHandle(hSnapshot);
}



void uninjectDLL() {
	

	printf("Attempting to uninject DLL.\n");

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, tekkenPid);

	MODULEENTRY32 moduleEntry;

	moduleEntry.dwSize = sizeof(moduleEntry);

	HMODULE dllModule = NULL;

	if (Module32FirstW(hSnapshot, &moduleEntry))
	{
		do {
		
			// compare just the module filename (not full path)
			if (_wcsicmp(moduleEntry.szModule, INJECTION_DLL_NAME_WCHAR) == 0)
			{
				dllModule =  moduleEntry.hModule;   // base address in remote
				printf("DLL module found when uninjecting: %s\n", moduleEntry.szModule);
				break;
			}
		} while (Module32NextW(hSnapshot, &moduleEntry));
	}

	if (dllModule) {
		//code from humza1400's ISP injector on github

		// Load the DLL locally (without running its DllMain) to compute the function offset.
		HMODULE hLocalModule = LoadLibraryExA(INJECTION_DLL_ABSOLUTE_PATH, NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (!hLocalModule) {
			printf("Could not load dll locally\n");
			return;
		}

		FARPROC pLocalUnload = GetProcAddress(hLocalModule, "UnloadDLL");

		if (!pLocalUnload) {
			FreeLibrary(hLocalModule);
			printf("Could not get UnloadDLL function!\n");
			return;
		}

		DWORD_PTR offset = (DWORD_PTR)pLocalUnload - (DWORD_PTR)hLocalModule;
		FARPROC pRemoteUnload = (FARPROC)((DWORD_PTR)dllModule + offset);
		FreeLibrary(hLocalModule);

		HANDLE hThread = CreateRemoteThread(tekkenHandle, nullptr, 0,
			(LPTHREAD_START_ROUTINE)pRemoteUnload, nullptr, 0, nullptr);
		if (!hThread) {
			printf("could not create uninjection thread!\n");
			return;
		}

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);


	}
	else {
		printf("ERROR: could not find dll in target process to uninject!");
	}

	CloseHandle(hSnapshot);

	listDLLs();
	
}

end of comment out to avoid AV */
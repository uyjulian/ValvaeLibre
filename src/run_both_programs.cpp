
#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>

static int create_process(PROCESS_INFORMATION *pi, WCHAR *executable_name)
{
	// process info
	STARTUPINFOW si;

	// first create process as suspended
	memset(&si, 0, sizeof(si));
	memset(pi, 0, sizeof(*pi));
	si.cb = sizeof(si);
	if (CreateProcessW(executable_name, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, pi) == FALSE)
	{
		return 1;
	}
	return 0;
}

static int start_process(PROCESS_INFORMATION *pi)
{
	// resume our suspended processes
	if (ResumeThread(pi->hThread) == (DWORD) -1)
	{
		return 1;
	}
	return 0;
}

static int wait_process(PROCESS_INFORMATION *pi)
{
	// Wait for process, then close related resources
	WaitForSingleObject(pi->hProcess, INFINITE);
    CloseHandle(pi->hProcess);
    pi->hProcess = INVALID_HANDLE_VALUE;
    CloseHandle(pi->hThread);
    pi->hThread = INVALID_HANDLE_VALUE;
	return 0;
}

int main(int ac, char **av)
{
	PROCESS_INFORMATION pi[2];
	WCHAR modnamebuf[32768];
	WCHAR newprocessname1[32768];
	WCHAR newprocessname2[32768];

	memset(&modnamebuf, 0, sizeof(modnamebuf));
	memset(&newprocessname1, 0, sizeof(newprocessname1));
	memset(&newprocessname2, 0, sizeof(newprocessname2));
	DWORD ret_len = GetModuleFileNameW(GetModuleHandle(NULL), modnamebuf, sizeof(modnamebuf) / sizeof(modnamebuf[0]));
	PathRemoveFileSpecW(modnamebuf);
	wcsncpy(newprocessname1, modnamebuf, sizeof(newprocessname1) / sizeof(newprocessname1[0]));
	wcsncpy(newprocessname2, modnamebuf, sizeof(newprocessname2) / sizeof(newprocessname2[0]));
	wcsncat(newprocessname1, L"/engineSim.exe", sizeof(modnamebuf) / sizeof(modnamebuf[0]));
	wcsncat(newprocessname2, L"/VCP.exe", sizeof(modnamebuf) / sizeof(modnamebuf[0]));

	create_process(&pi[0], newprocessname1);
	create_process(&pi[1], newprocessname2);

	start_process(&pi[0]);
	start_process(&pi[1]);

	wait_process(&pi[0]);
	wait_process(&pi[1]);

	return 0;
}

#else

int main(int ac, char **av)
{
	return 0;
}

#endif

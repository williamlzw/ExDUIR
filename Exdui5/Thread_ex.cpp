#include "Thread_ex.h"

void* Thread_InitializeCriticalSection()
{
	auto addr = LocalAlloc(64, sizeof(CRITICAL_SECTION));
	InitializeCriticalSection((LPCRITICAL_SECTION)addr);
	return addr;
}

void Thread_DeleteCriticalSection(void* addr)
{
	DeleteCriticalSection((LPCRITICAL_SECTION)addr);
	LocalFree(addr);
}

void Thread_EnterCriticalSection(void* addr)
{
	EnterCriticalSection((LPCRITICAL_SECTION)addr);
}

void Thread_LeaveCriticalSection(void* addr)
{
	LeaveCriticalSection((LPCRITICAL_SECTION)addr);
}

void Thread_Create(LPTHREAD_START_ROUTINE fun, LPVOID parameter)
{
	auto hThread = CreateThread(NULL, 0, fun, parameter, 0, 0);
	if (hThread != NULL) {
		CloseHandle(hThread);
	}
}
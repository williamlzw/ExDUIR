#pragma once
#include "help_ex.h"

void* Thread_InitializeCriticalSection();
void Thread_DeleteCriticalSection(void* addr);
void Thread_EnterCriticalSection(void* addr);
void Thread_LeaveCriticalSection(void* addr);
void Thread_Create(LPTHREAD_START_ROUTINE fun, LPVOID parameter);
#pragma once

LPVOID Thread_InitializeCriticalSection();
void   Thread_DeleteCriticalSection(LPVOID addr);
void   Thread_EnterCriticalSection(LPVOID addr);
void   Thread_LeaveCriticalSection(LPVOID addr);
void   Thread_Create(LPTHREAD_START_ROUTINE fun, LPVOID parameter);

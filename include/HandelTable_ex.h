#pragma once
#include "help_ex.h"

mempool_s* _handle_init();
BOOL _handle_uninit(mempool_s* hTable);
EXHANDLE _handle_create(INT nType, LPVOID dwData, INT* nError);
BOOL _handle_destroy(EXHANDLE handle, INT* nError);
BOOL _handle_validate(EXHANDLE handle, INT type, LPVOID* dwData = NULL, INT* nError = NULL);

#pragma once
#include "help_ex.h"

void* _handle_init();
bool _handle_uninit(void* hTable);
EXHANDLE _handle_create(int nType, void* dwData, int* nError);
bool _handle_destroy(EXHANDLE handle, int* nError);
bool _handle_validate(EXHANDLE handle, int type, void** dwData = NULL, int* nError = NULL);

#pragma once
#include "help_ex.h"

void* _handle_init();
bool _handle_uninit(void* hTable);
ExHandle _handle_create(int nType, void* dwData, int* nError);
bool _handle_destroy(ExHandle handle, int* nError);
bool _handle_validate(ExHandle handle, int type, void** dwData = NULL, int* nError = NULL);

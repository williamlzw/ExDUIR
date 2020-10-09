#pragma once
#include "help_ex.h"

void* _handle_init();
bool _handle_uninit(void* hTable);
size_t _handle_create(int nType, void* dwData, int* nError);
bool _handle_destroy(size_t handle, int* nError);
bool _handle_validate(size_t handle, int type, void** dwData=NULL, int* nError=NULL);

#pragma once
#include "help_ex.h"

EX_DEFINE_API(RtlGetNtVersionNumbers, int, (int*, int*, int*));


void 加载NTDLL();
int 取系统主版本号(int* dwMinor=0, int* Optional=0);

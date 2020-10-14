#pragma once
#include "help_ex.h"

struct mbp_s
{
	size_t handle_;
	void* lpText_;
	void* lpCaption_;
	int uType_;
	void* lpCheckBox_;
	void* lpCheckBoxChecked_;
	int dwMilliseconds_;
	int dwFlags_;
	void* lpfnNotifyCallback_;
};

void _msgbox_drawinfo(void* pWnd, size_t cvBkg);

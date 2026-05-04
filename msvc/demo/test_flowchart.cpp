#include "test_flowchart.h"

HWND hWnd_flowchart;
HEXDUI hExDui_flowchart;
HEXOBJ hFlowChart;

void CALLBACK FlowchartTimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	KillTimer(hWnd, idEvent);

	//执行图片改变尺寸流程
	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_EXECUTE_NODE, 1001, 0);

	//执行数据拼接输出流程
	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_EXECUTE_NODE, 1003, 0);
}

void AddTestNode(EX_FLOWCHART_NODE& node) {
	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_NODE, 0, (LPARAM)&node);
	for (INT i = 0; i < node.portCount; i++) {
		if (node.ports[i].widgetType == FLOWCHART_NODEDATA_TYPE_COMBO && node.ports[i].widgetData) {
			EX_FLOWCHART_NODE_COMBO_DATA* combo = (EX_FLOWCHART_NODE_COMBO_DATA*)node.ports[i].widgetData;
			for (INT k = 0; k < combo->count; k++) Ex_MemFree((void*)combo->options[k]);
			Ex_MemFree(combo->options); Ex_MemFree(combo);
		}
	}
	Ex_MemFree(node.ports);
}

LRESULT CALLBACK OnFlowChartButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 10001)
	{
		// 导出流程图到YAML
		Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_EXPORT_YAML, 0, (LPARAM)L"res/flowchart.yaml");
	}
	else if (nID == 10002)
	{
		// 从YAML加载流程图, 无需手动创建节点
		Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_IMPORT_YAML, 0, (LPARAM)L"res/flowchart.yaml");
		// 延时演示节点执行流程
		SetTimer(hWnd_flowchart, 1, 1000, FlowchartTimerProc);
	}
	return 0;
}

LRESULT CALLBACK FlowChartNotifyProc(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam) {
	if (nID == 200 && nCode == FLOWCHART_EVENT_EXECUTE_NODE) {
		EX_FLOWCHART_EXECUTE_PARAMS* params = (EX_FLOWCHART_EXECUTE_PARAMS*)lParam;

		if (params->nodeId == 1001) { // Load Image
			LPCWSTR path = (LPCWSTR)params->inputs[0].data;
			HEXIMAGE hImage = NULL;
			if (path && _img_createfromfile(path, &hImage)) {
				params->outputs[0].data = (LPVOID)hImage;
				EX_FLOWCHART_PORT newData; newData.id = 101; newData.widgetType = FLOWCHART_NODEDATA_TYPE_IMAGE; newData.widgetData = (LPVOID)hImage;
				Ex_ObjSendMessage(hObj, FLOWCHART_MESSAGE_UPDATE_NODEDATA, 1001, (LPARAM)&newData);
			}
		}
		else if (params->nodeId == 1002) { // Resize Image
			HEXIMAGE hSrc = (HEXIMAGE)params->inputs[0].data;
			HEXIMAGE hDst = NULL;
			// 新图像，防止组件销毁重复释放图像
			if (hSrc && _img_scale(hSrc, 512, 512, &hDst)) {
				params->outputs[0].data = (LPVOID)hDst;
				EX_FLOWCHART_PORT newData; newData.id = 201; newData.widgetType = FLOWCHART_NODEDATA_TYPE_IMAGE; newData.widgetData = (LPVOID)hDst;
				Ex_ObjSendMessage(hObj, FLOWCHART_MESSAGE_UPDATE_NODEDATA, 1002, (LPARAM)&newData);
			}
		}
		else if (params->nodeId == 1003) { // Text Config
			LPCWSTR prefix = (LPCWSTR)params->inputs[0].data;
			EX_FLOWCHART_NODE_COMBO_DATA* combo = (EX_FLOWCHART_NODE_COMBO_DATA*)params->inputs[1].data;
			EX_FLOWCHART_NODE_COMBO_DATA* combo1 = (EX_FLOWCHART_NODE_COMBO_DATA*)params->inputs[2].data;
			std::wstring format = L"PNG";
			std::wstring data = L"数据";
			
			if (combo && combo->count > 0) format = combo->options[combo->current];
			if (combo1 && combo1->count > 0) data = combo1->options[combo1->current];
			std::wstring result = (prefix ? std::wstring(prefix) : L"") + L"_" + format + L"_" + data;
			// 文本数据传送 拷贝
			params->outputs[0].data = (LPVOID)StrDupW(result.c_str());
		}
		else if (params->nodeId == 1004) { // Save Image
			HEXIMAGE hSrc = (HEXIMAGE)params->inputs[0].data;
			LPCWSTR config = (LPCWSTR)params->inputs[1].data;
			std::wstring savePath = config ? L"res/" + std::wstring(config) + L".png" : L"output.png";
			std::wstring result = L"Saved as [" + (config ? std::wstring(config) : L"null") + L"].png";
			
			if (config)
			{
				_img_savetofile(hSrc, savePath.c_str());
			}
			
			EX_FLOWCHART_PORT newData; newData.id = 402; newData.widgetType = FLOWCHART_NODEDATA_TYPE_EDIT; newData.widgetData = (LPVOID)StrDupW(result.c_str());
			Ex_ObjSendMessage(hObj, FLOWCHART_MESSAGE_UPDATE_NODEDATA, 1004, (LPARAM)&newData);
			// 释放StrDupW 拷贝的数据
			if (config) Ex_MemFree((void*)config);
		}
		else if (params->nodeId == 1005) { // Preview
			HEXIMAGE hSrc = (HEXIMAGE)params->inputs[0].data;
			HEXIMAGE hDst = NULL;
			// 拷贝图像，防止组件销毁重复释放图像
			if (hSrc && _img_copy(hSrc, &hDst)) {
				EX_FLOWCHART_PORT newData; newData.id = 501; newData.widgetType = FLOWCHART_NODEDATA_TYPE_IMAGE; newData.widgetData = (LPVOID)hDst;
				Ex_ObjSendMessage(hObj, FLOWCHART_MESSAGE_UPDATE_NODEDATA, 1005, (LPARAM)&newData);
			}
		}
	}
	return 0;
}

void test_flowchart(HWND hWnd)
{
	hWnd_flowchart = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"流程图演示", 0, 0, 1400, 900, 0, 0);
	hExDui_flowchart = Ex_DUIBindWindowEx(hWnd_flowchart, 0,
		WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_flowchart, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

	hFlowChart = Ex_ObjCreateEx(-1, L"FlowChart", NULL, -1, 0, 50, 1380, 780, hExDui_flowchart, 200, -1, 0, 0, NULL);

	// ================= 1. Load Image =================
	EX_FLOWCHART_NODE node1 = { 0 };
	node1.id = 1001; node1.x = 50; node1.y = 50; node1.title = L"Load Image";
	node1.portCount = 3; node1.ports = (EX_FLOWCHART_PORT*)Ex_MemAlloc(sizeof(EX_FLOWCHART_PORT) * 3);
	node1.ports[0] = { 100, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_STRING, L"path", {0}, FLOWCHART_NODEDATA_TYPE_EDIT, 0, 180, 25, {0}, (LPVOID)L"res/rotateimgbox.jpg", FALSE };
	node1.ports[1] = { 101, FLOWCHART_PORTTYPE_INTERMEDIATE, FLOWCHART_DATATYPE_IMAGE, L"preview", {0}, FLOWCHART_NODEDATA_TYPE_IMAGE, 0, 180, 180, {0}, NULL, FALSE };
	node1.ports[2] = { 102, FLOWCHART_PORTTYPE_OUTPUT, FLOWCHART_DATATYPE_IMAGE, L"IMAGE", {0}, 0, 0, 0, 0, {0}, NULL, FALSE };
	AddTestNode(node1);

	// ================= 2. Resize Image =================
	EX_FLOWCHART_NODE node2 = { 0 };
	node2.id = 1002; node2.x = 350; node2.y = 50; node2.title = L"Resize Image";
	node2.portCount = 3; node2.ports = (EX_FLOWCHART_PORT*)Ex_MemAlloc(sizeof(EX_FLOWCHART_PORT) * 3);
	node2.ports[0] = { 200, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_IMAGE, L"image", {0}, 0, 0, 0, 0, {0}, NULL, FALSE };
	node2.ports[1] = { 201, FLOWCHART_PORTTYPE_INTERMEDIATE, FLOWCHART_DATATYPE_IMAGE, L"preview", {0}, FLOWCHART_NODEDATA_TYPE_IMAGE, 0, 180, 180, {0}, NULL, FALSE };
	node2.ports[2] = { 202, FLOWCHART_PORTTYPE_OUTPUT, FLOWCHART_DATATYPE_IMAGE, L"IMAGE", {0}, 0, 0, 0, 0, {0}, NULL, FALSE };
	AddTestNode(node2);

	// ================= 3. Text Config (Input Card) =================
	EX_FLOWCHART_NODE node3 = { 0 };
	node3.id = 1003; node3.x = 50; node3.y = 450; node3.title = L"Text Config";
	node3.portCount = 4; node3.ports = (EX_FLOWCHART_PORT*)Ex_MemAlloc(sizeof(EX_FLOWCHART_PORT) * 4);
	node3.ports[0] = { 300, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_STRING, L"prefix", {0}, FLOWCHART_NODEDATA_TYPE_EDIT, 0, 180, 25, {0}, (LPVOID)L"output_", FALSE };
	LPCWSTR* fmtOpts = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * 2); fmtOpts[0] = StrDupW(L"PNG"); fmtOpts[1] = StrDupW(L"JPG");
	EX_FLOWCHART_NODE_COMBO_DATA* fmtData = (EX_FLOWCHART_NODE_COMBO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_COMBO_DATA));
	fmtData->options = fmtOpts; fmtData->count = 2; fmtData->current = 0;
	node3.ports[1] = { 301, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_COMBO, L"format", {0}, FLOWCHART_NODEDATA_TYPE_COMBO, 0, 180, 25, {0}, (LPVOID)fmtData, FALSE };
	
	LPCWSTR* fmtOpts1 = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * 3); fmtOpts1[0] = StrDupW(L"数据1"); fmtOpts1[1] = StrDupW(L"数据2"); fmtOpts1[2] = StrDupW(L"数据3");
	EX_FLOWCHART_NODE_COMBO_DATA* fmtData1 = (EX_FLOWCHART_NODE_COMBO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_COMBO_DATA));
	fmtData1->options = fmtOpts1; fmtData1->count = 3; fmtData1->current = 0;
	node3.ports[2] = { 302, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_COMBO, L"type", {0}, FLOWCHART_NODEDATA_TYPE_COMBO, 0, 180, 25, {0}, (LPVOID)fmtData1, FALSE };
	
	node3.ports[3] = { 303, FLOWCHART_PORTTYPE_OUTPUT, FLOWCHART_DATATYPE_STRING, L"CONFIG", {0}, 0, 0, 0, 0, {0}, NULL, FALSE };
	
	AddTestNode(node3);

	// ================= 4. Save Image (Output Card) =================
	EX_FLOWCHART_NODE node4 = { 0 };
	node4.id = 1004; node4.x = 350; node4.y = 450; node4.title = L"Save Image";
	node4.portCount = 3; node4.ports = (EX_FLOWCHART_PORT*)Ex_MemAlloc(sizeof(EX_FLOWCHART_PORT) * 3);
	node4.ports[0] = { 400, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_IMAGE, L"image", {0}, 0, 0, 0, 0, {0}, NULL, FALSE };
	node4.ports[1] = { 401, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_STRING, L"config", {0}, 0, 0, 0, 0, {0}, NULL, FALSE };
	node4.ports[2] = { 402, FLOWCHART_PORTTYPE_INTERMEDIATE, FLOWCHART_DATATYPE_STRING, L"result", {0}, FLOWCHART_NODEDATA_TYPE_EDIT, 0, 180, 50, {0}, NULL, FALSE };
	AddTestNode(node4);

	// ================= 5. Preview =================
	EX_FLOWCHART_NODE node5 = { 0 };
	node5.id = 1005; node5.x = 650; node5.y = 50; node5.title = L"Preview";
	node5.portCount = 2; node5.ports = (EX_FLOWCHART_PORT*)Ex_MemAlloc(sizeof(EX_FLOWCHART_PORT) * 2);
	node5.ports[0] = { 500, FLOWCHART_PORTTYPE_INPUT, FLOWCHART_DATATYPE_IMAGE, L"image", {0}, 0, 0, 0, 0, {0}, NULL, FALSE };
	node5.ports[1] = { 501, FLOWCHART_PORTTYPE_INTERMEDIATE, FLOWCHART_DATATYPE_IMAGE, L"preview", {0}, FLOWCHART_NODEDATA_TYPE_IMAGE, 0, 250, 250, {0}, NULL, FALSE };
	AddTestNode(node5);

	// ================= 连线演示 =================
	EX_FLOWCHART_CONNECTION conn = { 0 };
	conn.fromNode = 1001; conn.fromSlot = 2; conn.toNode = 1002; conn.toSlot = 0;
	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_CONNECTION, 0, (LPARAM)&conn);
	conn.fromNode = 1002; conn.fromSlot = 2; conn.toNode = 1005; conn.toSlot = 0;
	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_CONNECTION, 0, (LPARAM)&conn);
	conn.fromNode = 1002; conn.fromSlot = 2; conn.toNode = 1004; conn.toSlot = 0;
	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_CONNECTION, 0, (LPARAM)&conn);
	conn.fromNode = 1003; conn.fromSlot = 3; conn.toNode = 1004; conn.toSlot = 1;
	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_CONNECTION, 0, (LPARAM)&conn);

	Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_SET_BACKGROUNDCOLOR, 0, ExARGB(120, 120, 120, 255));
	
	Ex_ObjHandleEvent(hFlowChart, FLOWCHART_EVENT_EXECUTE_NODE, FlowChartNotifyProc);

	// 延时演示节点执行流程
	SetTimer(hWnd_flowchart, 1, 1000, FlowchartTimerProc);

	auto button1 = Ex_ObjCreateEx(-1, L"button", L"导出流程图到YAML", -1, 10, 850, 200, 30,
		hExDui_flowchart, 10001, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	auto button2 = Ex_ObjCreateEx(-1, L"button", L"从YAML加载流程图", -1, 240, 850, 200, 30,
		hExDui_flowchart, 10002, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(button1, NM_CLICK, OnFlowChartButtonEvent);
	Ex_ObjHandleEvent(button2, NM_CLICK, OnFlowChartButtonEvent);

	HEXOBJ hObj_label1 = Ex_ObjCreateEx(-1, L"static", L"1.选中连接线按下【DELETE】键可以删除连接线\n2.按住【SHIFT】键可以从输出插槽拉出第二条乃至更多连接线", -1,
		500, 850, 480, 60, hExDui_flowchart, 0, DT_WORDBREAK, 0, 0, NULL);
	Ex_ObjSetColor(hObj_label1, COLOR_EX_TEXT_NORMAL, ExARGB(133, 33, 53, 255), TRUE);
	Ex_DUIShowWindow(hExDui_flowchart, SW_SHOWNORMAL);
}
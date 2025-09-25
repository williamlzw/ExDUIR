#include "test_flowchart.h"

HEXDUI hExDui_flowchart;

void UpdateNodeData(HEXOBJ hFlowchart, INT nodeId, INT dataId)
{
    // 创建新的节点数据
    EX_FLOWCHART_NODE_DATA newData;
    newData.id = dataId;  // 数据项ID
    newData.type = FLOWCHART_NODEDATA_TYPE_EDIT;
    newData.data = (LPVOID)L"更新的节点内容";

    // 发送消息更新节点数据
    Ex_ObjSendMessage(hFlowchart, FLOWCHART_MESSAGE_UPDATE_NODEDATA,
        (WPARAM)nodeId, (LPARAM)&newData);
}

void test_flowchart(HWND hWnd)
{
    HWND hWnd_flowchart = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试流程图", 0, 0, 1100, 1000, 0, 0);
    hExDui_flowchart = Ex_DUIBindWindowEx(hWnd_flowchart, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_flowchart, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    auto hFlowChart = Ex_ObjCreateEx(-1, L"FlowChart", NULL, -1, 50, 50, 1000, 750, hExDui_flowchart, 200, -1, 0, 0, NULL);

    HEXIMAGE hImage;
    _img_createfromfile(L"res/rotateimgbox.jpg", &hImage);
    // 创建节点数据项
    EX_FLOWCHART_NODE_DATA dataItems[3] = { 0 };

    // 编辑框项
    dataItems[0].type = FLOWCHART_NODEDATA_TYPE_EDIT;
    dataItems[0].id = 1000;
    dataItems[0].data = (LPVOID)L"可编辑文本可编辑文本可编辑文本可编辑文本\r\n可编辑文本";

    // 图片项（假设hImage是已加载的图片句柄）
    dataItems[1].id = 1001;
    dataItems[1].type = FLOWCHART_NODEDATA_TYPE_IMAGE;
    dataItems[1].data = (LPVOID)hImage;

    // 选项卡项
    dataItems[2].id = 1002;
    dataItems[2].type = FLOWCHART_NODEDATA_TYPE_COMBO;
    EX_FLOWCHART_NODE_COMBO_DATA comboData;
    comboData.count = 2;
    comboData.current = 0;
    comboData.options = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * comboData.count);
    comboData.options[0] = L"选项1"; 
    comboData.options[1] = L"选项2";
    dataItems[2].data = (LPVOID)&comboData;

    EX_FLOWCHART_NODE node;
    memset(&node, 0, sizeof(EX_FLOWCHART_NODE));

    // 设置节点基本属性
    node.id = 1001;  // 唯一ID

    node.x = 200;    // X坐标
    node.y = 150;    // Y坐标
    node.width = 200; // 宽度
    node.height = 120; // 高度


    // 设置标题和描述
    node.title = L"处理节点";
    node.nodeDataList = dataItems;
    node.nodeDataCount = 3;

    // 添加输入插槽
    node.inputCount = 2;
    node.inputSlots = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * node.inputCount);

    node.inputSlots[0] = L"输入A";
    node.inputSlots[1] = L"输入B";

    // 添加输出插槽
    node.outputCount = 2;
    node.outputSlots = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * node.outputCount);

    node.outputSlots[0] = L"结果一";
    node.outputSlots[1] = L"结果二";
    Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_NODE, 0, (LPARAM)&node);

    // 创建节点数据项
    EX_FLOWCHART_NODE_DATA dataItems2[1] = { 0 };

    // 编辑框项
    dataItems2[0].id = 2001;
    dataItems2[0].type = FLOWCHART_NODEDATA_TYPE_EDIT;
    dataItems2[0].data = (LPVOID)L"可编辑文本";

    EX_FLOWCHART_NODE node2;
    memset(&node2, 0, sizeof(EX_FLOWCHART_NODE));

    // 设置节点基本属性
    node2.id = 2000;  // 唯一ID
    node2.x = 500;    // X坐标
    node2.y = 150;    // Y坐标
    node2.width = 200; // 宽度
    node2.height = 120; // 高度


    // 设置标题和描述
    node2.title = L"处理节点2";
    node2.nodeDataList = dataItems2;
    node2.nodeDataCount = 1;

    // 添加输入插槽
    node2.inputCount = 2;
    node2.inputSlots = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * node2.inputCount);

    node2.inputSlots[0] = L"输入A";
    node2.inputSlots[1] = L"输入B";


    // 添加输出插槽
    node2.outputCount = 1;
    node2.outputSlots = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * node2.outputCount);

    node2.outputSlots[0] = L"结果";

    Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_NODE, 0, (LPARAM)&node2);

    EX_FLOWCHART_CONNECTION connect;
    connect.fromNode = 1001;
    connect.toNode = 2000;
    connect.fromSlot = 0;
    connect.toSlot = 0;
    connect.controlPoint1.x = 200;
    connect.controlPoint1.y = 50;
    connect.controlPoint2.x = 400;
    connect.controlPoint2.y = 50;
    Ex_ObjSendMessage(hFlowChart, FLOWCHART_MESSAGE_ADD_CONNECTION, 0, (LPARAM)&connect);

    UpdateNodeData(hFlowChart, 2000, 2001);

    Ex_MemFree(comboData.options);
    // 释放输入插槽
    Ex_MemFree(node.inputSlots);

    // 释放输出插槽
    Ex_MemFree(node.outputSlots);

    // 释放输入插槽
    Ex_MemFree(node2.inputSlots);

    // 释放输出插槽
    Ex_MemFree(node2.outputSlots);

    Ex_DUIShowWindow(hExDui_flowchart, SW_SHOWNORMAL);
}
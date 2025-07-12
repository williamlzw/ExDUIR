ExDUIR
=======
[English](./README_EN.md)

## 简介

Windows平台轻量DirectUI框架。

本项目基于ExDirectUI 4.1.18开源版重写。

## 相对于原版的改进

请查阅[更新日志](CHANGELOG.txt)

## 问题反馈&帮助
* 交流QQ群: 214406227
* 提交反馈：[Gitee issues](https://gitee.com/william_lzw/ExduiR/issues) 或者 [GitHub issues](https://github.com/laizewei/ExduiR/issues)

## 运行系统
win10+

win7需要安装KB2670838补丁

虚拟机VMware win7运行ExDUIR，先安装kb2670838和kb4474419补丁，然后安装VMware Tools。

## 帮助文档
* [帮助文档](doc/SUMMARY.md)

## 编译
* Visual Studio 2022。
  工程文件ExDUIR.sln。包含以下项目：
  * dll: 可编译兼容原版ExDirectUI的动态库。 -> libexdui.dll
  * libexdui: 编译为静态库。 -> libexdui.lib
  * libexduiCPP: c++类封装和调用演示
  * test: 测试工程。

## 特点
* 使用DirectX进行硬件加速的2D绘制,比GDI绘制效率更高。
* 支持Unicode多语言。
* 提供Win32风格的API,支持生成lib和dll。生成的dll可以被其他语言调用。
* 扩展组件自由灵活，引擎处理底层逻辑。用户只需编写绘制代码，不需要考虑复杂的消息处理。
* 目前支持组件有按钮、开关、单选框、选择框、编辑框、富文本编辑框、列表框、报表列表、模板列表、分组框、组合框、菜单、树形框、滑块、单选框、选择框、选项卡、加载动画框、旋转图片框、页面、图片框、消息框、图标列表、列表按钮、工具条、状态条、日期框、调色板、颜色选择器、标题框、月历框、分数按钮、cef3浏览框、鼠标绘制板、媒体播放器、卷帘菜单，标注画板,可以在这些组件基础上超类化扩展组件。
* 支持布局，布局可以在窗口尺寸改变情况下自动更新组件位置。目前支持的布局有绝对布局，相对布局、线性布局、流式布局、表格布局。用户可以扩展布局。
* 支持GIF格式图片自动播放动画，支持图片格式PNG、JPEG、BMP、GIF、DDS、ICON。窗口或组件都可以用以上格式图片做背景图。
* 支持34种缓动特效，用户可以自由编写窗口缓动或组件缓动特效。用户可以扩展缓动算法。
* 编辑框支持富文本，支持加载RTF格式文档。
* 支持模态窗口、拖放、透明、异型窗口、图片形状窗口等特性。窗口和组件支持0到255透明度。支持高斯模糊。
* 支持限制区域消息通知。

## 窗口绘画概念
* window图形界面是基于窗口和消息机制的。包括桌面也是个窗口。控件是绘制在窗口上的局部图形，配上诸如鼠标消息,键盘消息即可实现各种控件的效果。如鼠标悬浮在按钮上面，鼠标按下触发按钮被点击事件，同时按钮渲染改变按下的状态，以此实现按钮的作用。而窗口包括控件的绘制分为GDI绘制和DirectX绘制。前者不调用显卡渲染,性能较差。后者调用显卡渲染以此加速。
* 根据上面原理，ExDuiR选择基于DirectX绘制的函数(DirectX11和D2D)，用windows子类化的方式实现自定义绘制和接收Windows窗口消息。

## 总体设计
* 为了编译其他语言调用,ExDuiR以函数形式对外暴露接口，编译为dll供其他语言调用。	
* ExDuiR设计了自己了一套基础类，包括Array数组类（供需要数组的控件使用）；HandleTable组件句柄表类(维护引擎的控件句柄)；HashTable哈希表类(用于保存控件事件、控件属性、资源包、主题包、字体、布局的键值信息)；Theme主题包类(用于读取主题包文件资源图片、主题绘制)；Resource资源包类(主题包文件解包)；Easing缓动算法类(用于缓动)；ImageList图片列表类(便于管理图片资源)；Layout布局类(定义控件间的位置关系)；MemPool内存池类(哈希表缓存信息，控件消息发送时缓存发送内容)；Thread线程类；DropTarget拖曳类。
* ExDuiR用于绘制控件的基础类有Brush画刷类;Canvas画布类；Font字体类；Image图像类；Matrix矩阵类；Path路径类；Region区域类；StokeStyle线条风格类。

## 调用说明
1.Ex_Init初始化引擎。  
2.Ex_WndCreate创建窗口，接着调用Ex_DUIBindWindowEx创建皮肤关联到窗口，接着用Ex_DUISetLong设置窗口背景色。  
3.Ex_DUIShowWindow显示窗口。然后是Ex_WndMsgLoop维持消息循环(收到窗口关闭消息会中止循环)  
4.Ex_UnInit释放引擎。  

## 扩展组件说明
Ex_ObjRegister注册组件类,在回调callback的WM_PAINT消息里绘制组件,其它消息控制组件状态。使用时创建类名为该类的组件即可。组件需要的属性个数在Ex_ObjRegister的cbObjextra设置,默认每个大小都是8字节。使用索引从0开始(负数为基础组件内置索引,勿用)。用Ex_ObjSetLong设置属性,用Ex_ObjGetLong取属性。

* 可以在基础组件回调基础上超类化扩展组件参照Class_RotateImageBox_ex.cpp,回调默认返回Ex_ObjCallProc
* 可以重新自定义组件回调扩展组件参照Class_TitleBar_ex.cpp,回调默认返回Ex_ObjDefProc

绘制组件开始调用Ex_ObjBeginPaint函数，传入绘制结构体，结束绘制记得调用Ex_ObjEndPaint。

## 感谢名单(排名不分先后)
泆寒、壹丁、Sunshine、SupMe、何永

## Demo
### demo_all:  
![image](demo_image/demo_all.png)    
### demo button and switch:  
![image](demo_image/demo_button_switch.png)    
### demo buttonex:  
![image](demo_image/demo_buttonex.png)    
### demo combobox:  
![image](demo_image/demo_combobox.png)       
### demo custom menu:  
![image](demo_image/demo_custommenu.png)    
### demo custom background:  
![image](demo_image/demo_custombackground.png)    
### demo drag obj:  
![image](demo_image/demo_drag_obj.png)    
### demo drop and drag:  
![image](demo_image/demo_dropdrag.png)    
### demo easing: 
![image](demo_image/demo_easing.png)    
### demo edit and richedit:  
![image](demo_image/demo_edit_richedit.png)   
### demo editex:  
![image](demo_image/demo_editex.png)    
### demo groupbox:  
![image](demo_image/demo_groupbox.png)    
### demo iconlistview:  
![image](demo_image/demo_iconlistview.png)      
### demo irregular shape window:  
![image](demo_image/demo_irregular_shape_window.png)    
### demo label and background:  
![image](demo_image/demo_label_background.png)   
### demo layout-absolute:  
![image](demo_image/demo_layout_absolute.png)    
### demo layout-flow:  
![image](demo_image/demo_layout_flow.png)    
### demo layout-linear:  
![image](demo_image/demo_layout_linear.png)    
### demo layout-relative:  
![image](demo_image/demo_layout_relative.png)    
### demo layout-table:  
![image](demo_image/demo_layout_table.png)
### demo listview:  
![image](demo_image/demo_listview.png)
### demo loading:  
![image](demo_image/demo_loading.png)
### demo matrix:  
![image](demo_image/demo_matrix.png)
### demo menubutton:  
![image](demo_image/demo_menubutton.png)
### demo messagebox:  
![image](demo_image/demo_messagebox.png)
### demo navbutton:  
![image](demo_image/demo_navbutton.png)
### demo nchitest:  
![image](demo_image/demo_nchitest.png)
### demo progressbar:  
![image](demo_image/demo_progressbar.png)
### demo radio and checkbox:  
![image](demo_image/demo_radio_checkbox.png)
### demo reportlistview:  
![image](demo_image/demo_reportlistview.png)
### demo rotatebox:  
![image](demo_image/demo_rotatebox.png)
### demo sliderbar:  
![image](demo_image/demo_sliderbar.png)
### demo treeview:  
![image](demo_image/demo_treeview.png)
### demo modal:  
![image](demo_image/demo_modal.png)
### demo titlebar:
![image](demo_image/demo_titlebar.png)
### demo datebox:
![image](demo_image/demo_datebox.png)
### demo palette:
![image](demo_image/demo_palette.png)
### demo color picker:
![image](demo_image/demo_colorpicker.png)
### demo calendar:
![image](demo_image/demo_calendar.png)
### demo cefbrowser:
![image](demo_image/demo_cefbrowser.png)
### demo score button:
![image](demo_image/demo_scorebutton.png)
### demo template listview:
![image](demo_image/demo_template_listview.png)
### demo drawing board:
![image](demo_image/demo_drawingboard.png)
### demo propertygrid:
![image](demo_image/demo_propertygrid.png)
### demo media player:
![image](demo_image/demo_mediaplayer.png)
### demo svg:
![image](demo_image/demo_svg.png)
### demo carousel:
![image](demo_image/demo_carousel.png)
### demo rollmenu:
![image](demo_image/demo_rollmenu.png)
### demo tray:
![image](demo_image/demo_trayicon.png)
### demo mask:
![image](demo_image/demo_mask.png)
### demo taggingboard:
![image](demo_image/demo_taggingboard.png)
### demo effect:
![image](demo_image/demo_effect.png)
### demo pathandrgn:
![image](demo_image/demo_pathandrgn.png)
### demo chatbox:
![image](demo_image/demo_chatbox.png)
### demo flowchart:
![image](demo_image/demo_flowchart.png)
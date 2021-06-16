ExDUIR
=======

## 简介

Windows平台轻量DirectUI框架。

本项目基于ExDirectUI 4.1.18开源版重写。

## 相对于原版的改进

请查阅[更新日志](CHANGELOG.txt)

## 问题反馈&帮助
* 交流QQ群: 214406227
* 提交反馈：[Gitee issues](https://gitee.com/william_lzw/ExduiR/issues) 或者 [GitHub issues](https://github.com/laizewei/ExduiR/issues)

## 编译
目前支持以下两种方式编译：
* Visual Studio 2019(推荐)。
  工程文件ExDUIR.sln。包含以下项目：
  * dll: 可编译兼容原版ExDirectUI的动态库。 -> libexdui.dll
  * libexdui: 编译为静态库。 -> libexdui.lib
  * test: 测试工程。
* CMake。只支持MSVC编译器。

## 特点
* 绘制函数是DirectX11、Direct2D，效率比GDI高上百倍。
* Unicode编码，支持多国语言。
* 仿Windows消息机制，通过发送消息，分发通知给组件，兼容原生Windows消息和常量。win32 api写法，比类写法扩展性更灵活。
* 调用方式采用Win32风格API方式，支持生成lib静态库和dll动态库。生成的动态链接库支持被其它语言如python、java、go、dephi、C#、VB、易语言等调用。
* 扩展组件自由灵活，引擎处理底层逻辑。用户只需编写绘制代码，不需要考虑复杂的消息处理。
* 目前支持组件有按钮、编辑框、富文本编辑框、列表框、报表列表、分组框、组合框、菜单、树形框、滑块、单选框、选择框、选项卡、加载动画框、旋转图片框、页面、图片框、消息框、图标列表，可以在这些组件基础上超类化扩展组件。
* 支持布局，布局可以在窗口尺寸改变情况下自动更新组件位置。目前支持的布局有绝对布局，相对布局、线性布局、流式布局、表格布局。用户可以扩展布局。
* 支持APNG、GIF格式图片自动播放动画，支持图片格式PNG、APNG、JPEG、BMP、GIF、WEBP。窗口或组件都可以用以上格式图片做背景图。
* 支持34种缓动特效，用户可以自由编写窗口缓动或组件缓动特效。用户可以扩展缓动算法。
* 窗口支持异型窗口，图片形状窗口。窗口和组件支持0到255透明度。支持高斯模糊。
* 编辑框支持富文本，支持加载RTF格式文档。
* 窗口或组件支持接收拖曳文件或文本。

## Demo (部分图片为ExDUI原版)
### demo_all:  
![image](https://raw.githubusercontent.com/laizewei/ExduiR/master/demo_image/demo_all.png)    
### demo button and switch:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_button_switch.png)    
### demo buttonex:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_buttonex.png)    
### demo combobox:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_combobox.png)       
### demo custom menu:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_custommenu.png)    
### demo custom background:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_custombackground.png)    
### demo drag obj:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_drag_obj.png)    
### demo drop and drag:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_dropdrag.png)    
### demo easing: 
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_easing.png)    
### demo edit and richedit:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_edit_richedit.png)   
### demo editex:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_editex.png)    
### demo groupbox:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_groupbox.png)    
### demo iconlistview:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_iconlistview.png)      
### demo irregular shape window:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_irregular_shape_window.png)    
### demo label and background:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_label_background.png)   
### demo layout-absolute:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_layout_absolute.png)    
### demo layout-flow:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_layout_flow.png)    
### demo layout-linear:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_layout_linear.png)    
### demo layout-relative:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_layout_relative.png)    
### demo layout-table:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/ddemo_layout_table.png)
### demo listview:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_listview.png)
### demo loading:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_loading.png)
### demo matrix:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_matrix.png)
### demo menubutton:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_menubutton.png)
### demo messagebox:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_messagebox.png)
### demo navbutton:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_navbutton.png)
### demo nchitest:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_nchitest.png)
### demo progressbar:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_progressbar.png)
### demo radio and checkbox:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_radio_checkbox.png)
### demo reportlistview:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_reportlistview.png)
### demo rotatebox:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_rotatebox.png)
### demo sliderbar:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_sliderbar.png)
### demo treeview:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_treeview.png)
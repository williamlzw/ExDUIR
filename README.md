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
### apng_background:  
![image](https://raw.githubusercontent.com/laizewei/ExduiR/master/demo_image/demo_apng_background.png)    
### Animate button:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_Animate%20Button.png)    
### checkbox and radio page:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_checkbox_radio_page.png)    
### Gaussian blur and Transparent text box:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_Gaussian%20Blur%20and%20Transparent%20text%20box.png)    
### combox:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_combox.png)    
### image list:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_image_list.png)    
### line chart and bar chart:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_linechart_barchart.png)    
### list:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_list.png)    
### menubutton:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_menubutton.png)    
### reportlist: 
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_reportlist.png)    
### slider:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_slider.png)   
### switchbutton:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_switchbutton.png)    
### tab:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_tab.png)    
### treelist:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_treelist.png)      
### webview-miniblink:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_webview-miniblink.png)    
### echart1:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_echart1.png)   
### echart2:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_echart2.png)    
### echart3:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_echart3.png)    
### echart4:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_echart4.png)    
### echart5:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_echart5.png)    
### music box:  
![image](https://raw.githubusercontent.com/laizewei/Exdui4.1R/master/demo_image/demo_music_box.png)

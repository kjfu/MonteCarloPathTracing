# Monte-Carlo-Path-Tracing
Zhejiang University Graduate Computer Graphics Coursework（2/2）
本程序是基于Qt 5.11.2 clang 64 bit开发，经测试可以在macOS Mojave 10.14.4上正常运行，编译本程序需在安装QT的前提下，双击MonteCarloPathTracer.pro即可在QT Creator完成build。
本程序运行界面如下图所示，有已经配置好相关信息的四个场景（但是导入*.obj还是需要用户自己完成，以免用户改变*.obj对应文件路径），此外，本程序支持用户导入自己的*.obj文件，自行设置相关参数（光源参数只允许增加新的光源，不允许修改已经配置好的光源），支持用户将渲染好的场景保存为*.png。用户相关操作，都会有右下角的文本浏览器进行反馈。本程序对生成的图像进行了Gamma校正，Gamma值为2.2。

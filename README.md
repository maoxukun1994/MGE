# MGE
Multifunctional GL Engine

The main purpose of this project is building a multifunctional graphics engine (or game engine in the future) for general use.


issues:

mgeQuitApp() ( which then call exit(x) )causes application to quit without cleaning memory.


//uniform组织方式：
MGE_GLUniformManager中存储所有引擎可提供的uniform变量值（引用？指针？）并提供动态注册和删除Uniform变量的函数
其他类线程安全地更新其中的uniform值
MGE_GLShaderProgram中声明Uniform Update List（可能有多个，固定间隔更新，单次更新，随帧重绘更新等）
并把需要更新的uniform location 注册（关联）到MGE_GLUniformManager提供的变量上。在MGE_GLShaderProgram
中更新这些uniform。
MGE_GLUniform保留被引用Uniform的引用计数（被哪些shader使用），并在该Uniform更新（如删除或改变指针指向）时通知shader更新

features to be implemented:

* use Uniform Buffer in MGE_GLUniformManager

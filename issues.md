
issues:

mgeQuitApp() ( which then call exit(x) )causes application to quit without cleaning memory.


//uniform组织方式：
MGE_GLUniformManager中存储所有引擎可提供的uniform变量值（引用？指针？）并提供动态注册和删除Uniform变量的函数
其他类线程安全地更新其中的uniform值
MGE_GLShaderProgram中声明Uniform Update List（可能有多个，固定间隔更新，单次更新，随帧重绘更新等）
并把需要更新的uniform location 注册（关联）到MGE_GLUniformManager提供的变量上。在MGE_GLShaderProgram
中更新这些uniform。
MGE_GLUniform保留被引用Uniform的引用计数，并在该Uniform更新（如删除或改变指针指向）时通知shader更新(需uniform所有者手动调用unregister函数)

计划使用UBO

//uniform组织小记：

目前uniformUpdate类型：
1.每帧更新
2.固定时间间隔更新
3.所属Shader激活时更新一次


一个shaderProgram的构造除了需要源码以外，还需要config file
格式：

#注释内容
<Uniform名称> <类型> <更新策略>（每行）

类型包括以下几种：
INT = 1
FLOAT = 2
DOUBLE = 3
VEC2 = 4
VEC3 = 5
VEC4 = 6
MAT3 = 7
MAT4 = 8

更新策略包括以下几种：
FRAME_UPDATE = 0
FIX_UPDATE = 1
ONCE_UPDATE = 2

shaderProgram初始化时会读取该文件中的信息设置uniform更新策略。如果Uniform名称未在UniformManager中
注册，则会忽略该条声明。


对第三方库有依赖的类或模块：

mge_math.h -> 依赖GLM提供基础向量和矩阵类型
mge_image -> 依赖sfml载入图片
mge_app -> 依赖sfml创建窗口和context，处理事件，依赖GLAD提供GL函数和扩展入口
#在 linux 下std::thread 依赖 pthread
#在 linux 下 glad 依赖 libdl 动态载入函数


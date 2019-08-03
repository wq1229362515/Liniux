/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：
*   创建日期：
*   描    述： gdb工具的使用+零碎的命令知识点
*
================================================================*/

/*
    将程序置为Gdb可以调试的状态
    gcc -o main.c -o main -g  
    gdb ./main -tui
    可以进入tui模式
    0.进来之后按start 进入程序开始运行的步骤
    按r开始运行程序直到结束,按n开始单步调试
    1. 窗口之间的切换 fs src切换到src，和fs cmd切换到cmd。
    2. print() 显示变量的内容
    breadk 14  在14行打断点
    display 变量名 :跟踪一个变量名,每次都停下来显示这个值,undisplay 取消对这些变量值的跟踪
    backtrace 查看运行到了哪一行
    until 跳出本次循环
    info breakpoints 查看设置的断点 delete breakpoints x 删除序号为x的断点
    list 行号/函数名 
    step 进入函数调用
    continue 从当前开始连续并非单步运行程序
    break 函数名 : 在某个函数开头设置断点
    p  变量  打印变量名字 

    info break 查看断点信息
    finish 执行完当前的函数,并不是执行完整个程序
 *
 *
 *
 *
 *
 *
 */
 
 /*
 * ulimit -a 查看系统限制
 * 
 * ps  命令
 * -e 查看所有进程
 *  f 查看程序之间的关系
 * ps -ef 
 * ps -u root 显示这个root用户下的进程
 * ps -l 将你这次登录的进程显示出来
 * ps |grep 显示指定的进程
 * ps aux 查看正在内存中的程序
 *
 *
 * /

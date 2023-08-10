/**
 * Auth :   liubo
 * Date :   2023-08-10 00:00:00
 * Comment: 自动测试
 */

#pragma once

class AutoRegTestFunc
{
public:
    typedef void (*Func)();
    AutoRegTestFunc(Func callback);

    static void RunAll();
    static void Destroy();
};

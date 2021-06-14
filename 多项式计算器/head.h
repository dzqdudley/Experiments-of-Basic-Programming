#include<stdlib.h>
#include <iostream>
#include <vector>
#include<stack>
#include<string>
#include<cstring>
#include<cmath>
using namespace std;
struct func;
void func_loop();//主界面函数
bool get_func();//输入多项式
void mass_calculate(char list[]);//混合运算
void initialize_func(struct func& result);//初始化结构
bool is_legal(char name[]);//判断输入合法性
bool get_root();//求根
bool print_func(string name);//输出多项式
double assign_value(struct func name, double m);//计算函数值
struct func integrate(string name,struct func poly);//计算积分
struct func derive(struct func poly);//计算导数
bool get_reverse(string name1, struct func &temp);//求逆
bool divide();//多项式除法



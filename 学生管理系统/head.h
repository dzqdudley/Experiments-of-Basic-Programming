#ifndef head_h
#define head_h
#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<conio.h>
#include<Windows.h>
using namespace std;
struct lesson
{
	char id[4];//课程id
	char name[31];//课程名
	char tutor[31];//教师名
	int max;//最大人数
	int current;//现有人数
	char type[31];//课程类型
	vector<string> chosen_by;//选课学生
};
struct student
{
	char stu_id[31];//学生id
	int signed_ta;//报名助教数
	int major;//专业课数目
	int minor;//非专业课数目
};
extern std::vector <lesson> info;
extern std::vector <string> chosen_by;
extern student stu;
extern int login();
extern int adminfunc();
extern void stufunc();
extern void notice();
extern void error();
extern void success();
extern void setco(unsigned short forecolor , unsigned short backgroundcolor );
#endif /* head_h */


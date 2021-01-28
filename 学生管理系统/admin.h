#ifndef admin_h
#define admin_h
#include "head.h"
void signout();//登出
bool get_lesson_info();//导入课程
bool view_alllesson_info();//查看全部课程信息
bool add_lesson();//添加课程
bool delete_lesson();//删除课程
bool change_lesson();//修改课程信息
bool view_specific_info();//查看课程具体信息
bool check_duplicate2(char [], char[]);//查看是否重复
bool initial_ta_info();//初始化助教文件
bool view_ta_stu();//查看课程助教对应的学生
extern string star;
extern int bullet;
#endif /* admin_h */


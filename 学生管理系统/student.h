#ifndef student_h
#define student_h
#include "head.h"

bool choose_lesson();//选课
bool view_schedule(int);//查看个人课表
bool quit_lesson();//退课
bool sign_for_ta();//报名助教
bool choose_ta();//选择助教
void clear_assistant(char []);//更新助教信息
void update_cur_cour();//更新课程文件
bool change_pass();//修改密码
extern void signout();//登出
extern bool view_alllesson_info();//查看全部课程信息
extern string star;
extern string starline;
extern string splitline;
#endif /* student_h */

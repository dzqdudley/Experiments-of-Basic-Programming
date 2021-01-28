#define _CRT_SECURE_NO_WARNINGS
#include"head.h"
using namespace std;
bool stu_login();//学生登录
bool stu_signup();//学生注册
bool admin_login();//管理员登录
bool check_duplicate(char[]);//检查是否重复
void initial_vector();//初始化课程容器
bool initial_stu();//初始化学生信息
bool check_ta_status();//检查助教状态
void empty_chosen_ta(char[]);//登录时修改个人助教信息
extern bool change_pass();
lesson initial_chosen_by(lesson p);
vector<lesson> info;
student stu;
string star(12, '*');
 string namesplit(60, '=');
 int bullet = 0;
 int check = 1;
int login()
{
	while (1)
	{
		FILE *fa = fopen("current_course.txt", "r");
		if (fa == 0)
			check = 0;
		else
		{
			fclose(fa);
			check = 1;
		}
		system("cls");
		char i[31] = { '\0' };
		cout << "\033[31m"<<star << "欢迎访问学生信息管理系统！" << star <<"\033[30m"<< endl; 
		cout<<"请选择您的操作:"<<endl;
		cout << "\033[34m"<< namesplit << endl;
		cout <<"1.学生登录\t2.学生注册\t3.管理员登陆\t4.退出系统\n";
		cout<< namesplit<<"\033[30m" << endl << endl;
		 cout << "请输入您的操作:";
		cin >> i;
		if (!strcmp(i, "1"))
		{
			if (stu_login())
			{
				if (check)
					stufunc();
				else
				{
					notice();
					cout << "管理员还未导入课程信息，您无法进行课程相关操作！仅能使用修改密码功能！" << endl;
					cout << "若您想修改密码，请输入1，否则输入0返回登陆界面：";
					int j;
					cin >> j;
					if (j == 1)
					{
						change_pass();
						cout << "现返回登录界面！" << endl << endl;
						Sleep(2000);
					}
				}
			}
			else continue;
		}
		else if (!strcmp(i, "2"))
		{
			if (stu_signup())
			{
				success();
				cout <<star<< "注册成功！登录成功！" <<star<< endl<<endl;
				if (check)
					stufunc();
				else
				{ 
					notice();
					cout << "管理员还未导入课程信息，您无法进行课程相关操作！仅能使用修改密码功能！" << endl;
					cout << "若您想修改密码，请输入1，否则输入0返回登陆界面：";
					int j;
					cin >> j;
					if (j == 1)
					{
						change_pass();
						cout << "现返回登录界面！" << endl << endl;
						Sleep(2000);
					}
				}
			}
			else continue;
		}
		else if (!strcmp(i, "3"))
		{
			if (admin_login())
				adminfunc();
			else continue;
		}
		else if (!strcmp(i, "4"))
			exit(-1);
		else
		{
			error();
			cout << "命令无效！请重新输入！" << endl<<endl; Sleep(2000);
		}
	}
	return 0;
}
bool stu_login()
{
	char input_id[31] = { '\0' };
	char input_pass[31] = { '\0' };
	FILE *fp = fopen("student.txt", "r");
	if (fp == 0)
	{
		error();
		cout << "系统未录入学生账户，无法登录！请先进行注册！" << endl; Sleep(2000);
		return false;
	}
	cout << "请输入账号:";
	cin >> input_id;
	cout << "请输入密码:";
	char ch;
	int i = 0;
	while ((ch = _getch()) != '\r')
	{
		if (ch == '\b'&&i > 0)
		{
			cout << "\b\b";
			i--;
		}
		else if (ch != '\b')
		{
			input_pass[i++] = ch;
			cout << "*";
		}
	}
	cout << endl;
	char id1[31] = { '\0' }, pass[31] = { '\0' };
	fscanf(fp, "%[^,]", id1);
	while (!feof(fp))
	{
		if (!strcmp(id1, input_id))
		{
			fseek(fp, 1, SEEK_CUR);
			fscanf(fp, "%s", pass);
			if (!strcmp(pass, input_pass))
			{
				success();
				cout <<star<<"登录成功！"<<star << endl;
				strcpy(stu.stu_id, id1);
				fclose(fp);
				
				if (check)
				{
					initial_vector();
					initial_stu();
					check_ta_status();
				}
				return true;
			}
			else
			{
				error();
				cout << "账号或密码错误！请重新登录！" << endl; Sleep(2000);
				return false;
			}
		}
		else
		fseek(fp, 1, SEEK_CUR);
		fscanf(fp, "%s", id1);
		fseek(fp, 2, SEEK_CUR);
		fscanf(fp, "%[^,]", id1);
	}
	fclose(fp);
	error();
	cout << "账号或密码错误！请重新登录！" << endl; Sleep(2000);
	return false;
}
bool stu_signup()
{
	char input_id[31] = { '\0' };
	char input_pass[31] = { '\0' };
	FILE *fk = fopen("student.txt", "r");
	if (fk == 0)
	{
		FILE *fw = fopen("student.txt", "w+");
		fclose(fw);
	}
	else fclose(fk);
	cout << "请输入账号:";
	cin >> input_id;
	for (int i = 0; input_id[i]; i++)
		if (!(('0' <= input_id[i] && input_id[i] <= '9') || ('a' <= input_id[i] && input_id[i] <= 'z')))
		{
			error();
			cout << "账号中只能含有小写字母或数字，无法注册！" << endl<<"现返回登录界面！"<<endl<<endl; Sleep(2000);
			return false;
		}
	if (check_duplicate(input_id))
	{
		error();
		cout << "账户已注册！请直接登录!" << endl<<endl; Sleep(2000);
		return false;
	}
	else
	{
		FILE *fp = fopen("student.txt", "r+");
		if (fp == 0)
		{
			error();
			cout << "无法检查账户是否已注册，账户创建失败！"<<endl<<"现返回登录界面" << endl<<endl; Sleep(2000);
			return false;
		}
		again: cout << "请输入密码：";
		cin >> input_pass;
		for (int i = 0; input_pass[i]; i++)
			if (!(('0' <= input_pass[i]&& input_pass[i] <= '9')||( 'a' <= input_pass[i]&& input_pass[i] <= 'z')))
			{
				error();
				cout << "密码中只能含有小写字母或数字，请重新输入密码！"<<endl;
				goto again;
			}
		strcpy(stu.stu_id, input_id);
		fseek(fp, 0, SEEK_END);
		fprintf(fp, "%s", input_id);
		fprintf(fp, "%s", ",");
		fprintf(fp, "%s\n", input_pass);
		fclose(fp);
		strcat(input_id, ".txt");
		FILE *fpp = fopen(input_id, "w+");
		fclose(fpp);
		initial_vector();
		initial_stu();
		return true;
	}
}
bool admin_login()
{
	char input_id[31] = { '\0' };
	char input_pass[31] = { '\0' };
	char ch;
	cout << "请输入账号:";
	cin >> input_id;
	cout << "请输入密码：";
	int i=0;
	while ((ch = _getch()) != '\r')
	{
		if (ch == '\b'&&i > 0)
		{
			cout << "\b\b";
			i--;
		}
		else if (ch != '\b')
		{
			input_pass[i++] = ch;
			cout << "*";
		}
	}
	cout << endl;
	char adminid[6] = "Admin";
	if (!strcmp(adminid,input_id))
	{
		char adminpass[6] = "admin";
		if (!strcmp(adminpass, input_pass))
		{
			success();
			cout<<star<< "登录成功！" <<star<< endl;
			initial_vector();
			return true;
		}
		else
		{
			error();
			cout << "密码错误！请重新登录！\n" << endl; Sleep(2000);
			return false;
		}
	}
	error();
	cout << "该管理员账号不存在！请重新登录！\n" << endl; Sleep(2000);
	return false;
}
bool check_duplicate(char m[])
{
	FILE *fp = fopen("student.txt", "r");
	if (fp == 0)
	{
		error();
		cout << "账户创建失败！" << endl; Sleep(2000);
		return false;
	}
	char id[31] = { '\0' };
	fscanf(fp, "%[^,]", id);
	while (!feof(fp))
	{
		if (!strcmp(m,id))
		{
			fclose(fp);
			return true;
		}
		fseek(fp, 1, SEEK_CUR);
		fscanf(fp, "%s", id);
		fseek(fp, 2, SEEK_CUR);
		fscanf(fp, "%[^,]", id);
	}
	fclose(fp);
	return false;
}
void initial_vector()
{
	if (bullet == 0&&check)
	{
		bullet = 1;
		FILE*fpp = fopen("current_course.txt", "r");
		if (fpp != 0)
		{
			fclose(fpp);
			fstream fp("current_course.txt", ios::in);
			if (!fp.fail())
			{
				char id[4] = { '\0' };
				fp >> id;
				while (!fp.eof())
				{
					lesson p;
					strcpy(p.id, id);
					fp >> p.name;
					fp >> p.tutor;
					fp >> p.max;
					fp >> p.current;
					fp >> p.type;
					p = initial_chosen_by(p);
					info.push_back(p);
					fp >> id;
				}
				fp.close();
			}
		}
	}
}
bool initial_stu()
{
	stu.major = 0, stu.minor = 0,stu.signed_ta=0;
	char file[25] = { '\0' };
	strcpy(file, stu.stu_id);
	strcat(file, ".txt");
	FILE *fp = fopen(file, "r");
	char num[31] = { '\0' };
	fscanf(fp, "%s", num);
	while (!feof(fp))
	{
		vector<lesson>::iterator loc = info.begin(), last = info.end();
		while (loc != last)
		{
			if (!strcmp(num, loc->id))
			{
				if (!strcmp(loc->type, "专业课"))
					stu.major++;
				else stu.minor++;
				break;
			}
			loc++;
		}
		fscanf(fp, "%s", num);
		fscanf(fp, "%s", num);
	}
	int count = strlen(stu.stu_id);
	char temp[31] = { '\0' };
	FILE *fpp = fopen("assistant.txt", "r");
	if (fpp == 0)
		return false;
	fscanf(fpp, "%s", temp);
	while (!feof(fpp))
	{
		char tmp[5000] = { '\0' };
		fscanf(fpp, "%s", tmp);
		int i=0;
		for (int j = 0; tmp[j]; j++)
			if (tmp[j] == ',')
				i++;
		fseek(fpp, -(int)strlen(tmp), SEEK_CUR);
		for (int j = 0; j < i; j++)
		{
			fscanf(fpp, "%[^,]", temp);
			fseek(fpp,1, SEEK_CUR);
			if (!strcmp(temp, stu.stu_id))
				stu.signed_ta++;
		}
		fscanf(fpp, "%s", temp);
		if (!strcmp(temp, stu.stu_id))
			stu.signed_ta++;
		fseek(fpp, 2, SEEK_CUR);
		fscanf(fpp, "%s", temp);
	}
fclose(fp);
return true;
}
lesson initial_chosen_by(lesson p)
{
	FILE *fp = fopen("student.txt", "r");
	if(fp!=0)
	{
		char id[31] = { '\0' };
		fscanf(fp, "%[^,]", id);
		while (!feof(fp))
		{
			char ide[25] = { '\0' };
			strcpy(ide, id);
			strcat(ide, ".txt");
			FILE *fpp = fopen(ide, "r");
			if (fpp == 0)
				cout << "初始化失败！";
			char temp[4] = { '\0' };
			char ta[31] = { '\0' };
			fscanf(fpp, "%s", temp);
			while (!feof(fpp))
			{
				if (!strcmp(temp, p.id))
				{
					p.chosen_by.push_back(id);
					break;
				}
				fscanf(fpp, "%s", ta);
				fscanf(fpp, "%s", temp);
			}
			fclose(fpp);
			fseek(fp, 1, SEEK_CUR);
			fscanf(fp, "%s", id);
			fseek(fp, 2, SEEK_CUR);
			fscanf(fp, "%[^,]", id);
		}
		fclose(fp);
		}
	return p;
}
bool check_ta_status()
{
	char stu_file[25] = { '\0' };
	strcpy(stu_file, stu.stu_id);
	strcat(stu_file, ".txt");
	FILE *fp = fopen("assistant.txt", "r");
	if (fp == 0)
	{
		return false;
	}
loop:
	FILE *fpp = fopen(stu_file, "r");
	if (fpp == 0)
	{
		cout << "助教变动情况查看失败！";
		exit(-1);
	}
	char already[4] = { '\0' };
	fscanf(fpp, "%s", already);
	while (!feof(fpp))
	{
		char temp[31] = { '\0' };
		fscanf(fpp, "%s", temp);
		if (fabs(strcmp(temp, "Null")))
		{
			int flag = 1;
			fseek(fp, 0, SEEK_CUR);
			char id[4] = { '\0' };
			char tmp[5000] = { '\0' };
			fscanf(fp, "%s", id);
			while (!feof(fp))
			{
				fscanf(fp, "%s", tmp);
				if (!strcmp(id, already))
				{
					int i = 0;
					for (int j = 0; tmp[j]; j++)
						if (tmp[j] == ',')
							i++;
					fseek(fp, -(int)strlen(tmp), SEEK_CUR);
					for (int j = 0; j <= i; j++)
					{
						if (j < i)
						{
							fscanf(fp, "%[^,]", tmp);
							fseek(fp, 1, SEEK_CUR);
						}
						else if(j==i)
							fscanf(fp, "%s", tmp);
						if (!strcmp(tmp, temp))
						{
							flag = 0;
							break;
						}
					}
					if (flag)
						{
							fclose(fpp);
							notice();
							cout << "您在课程ID为 " << id << " 的课程中选择的助教 " << temp << " 已退课，您可以重新选择助教！"<<endl;
							empty_chosen_ta(id);
							goto loop;
						}
					}
				else fscanf(fp, "%s", id);
			}
		}
		else fscanf(fpp, "%s", already);
	}
	fclose(fpp);
	return true;
}
void empty_chosen_ta(char id[])
{
	char file[25] = { '\0' };
	strcpy(file, stu.stu_id);
	strcat(file, ".txt");
	FILE *fpp = fopen(file, "r");
	fseek(fpp, 0, SEEK_SET);
	char tmp[5000] = { '\0' }, key[5000] = { '\0' }, already[4] = { '\0' }, temp[31] = { '\0' };
	fscanf(fpp, "%s", already);
	while (!feof(fpp))
	{
		strcat(tmp, already);
		strcat(tmp, " ");
		fscanf(fpp, "%s", temp);
		if (fabs(strcmp(id, already)))
		{
			strcat(tmp, temp);
			strcat(tmp, "\n");
		}
		else if (!strcmp(id, already))
		{
			fscanf(fpp, "%[^;]", key);
			fclose(fpp);
			FILE *fk=fopen(file,"w+");
			fprintf(fk, tmp);
			fprintf(fk, "%s", "Null");
			fprintf(fk, key);
			fclose(fk);
			break;
		}
		fscanf(fpp, "%s", already);
	}
}
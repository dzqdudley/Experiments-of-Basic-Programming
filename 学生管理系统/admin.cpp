#define _CRT_SECURE_NO_WARNINGS
#include "admin.h"
string splitline(82, '=');
string starline(100, '*');
int adminfunc()
{
	char i[31] = { '\0' };
	while (1)
	{
		cout <<endl<< "请从下列选项中选择您的操作:\n" << "\033[34m"<<splitline<<endl;
		cout << "1.注销登录\t2.录入课程信息\t3.查看全部课程信息\t4.增加课程\n5.删除课程\t6.修改课程\t7.查看具体课程  \t8.查看课程助教对应学生名单";
		cout<< endl << splitline <<"\033[30m"<< endl;
		cout << endl<<"请输入您的操作：";
		cin >> i;
		cout << endl;
		if (!strcmp(i, "1"))
			signout();
		else if (!strcmp(i, "2"))
			get_lesson_info();
		else if (!strcmp(i, "3"))
			view_alllesson_info();
		else if (!strcmp(i, "4"))
			add_lesson();
		else if (!strcmp(i, "5"))
			delete_lesson();
		else if (!strcmp(i, "6"))
			change_lesson();
		else if (!strcmp(i, "7"))
			view_specific_info();
		else if (!strcmp(i, "8"))
			view_ta_stu();
		else 
		{ 
			error();
			cout << "指令无效！" << endl; 
		}
		notice();
		cout << "现返回上级页面！" << endl;
	}
	return 0;
}
void signout()
{
	success();
	cout<<star<< "成功退出登录！现返回登录界面！"<<star<<endl<<endl;
	Sleep(2000);
	login();
}
bool get_lesson_info()
{
	FILE *fm = fopen("current_course.txt", "r");
	if(fm!=0)
	{ 
		fclose(fm);
		error();
		cout << "课程此前已录入，无法重复录入！"<<endl;
		return false;
	}
start: cout<<"请输入需要导入的文件名称：";
	char name[50] = { '\0' };
	cin >> name;
	FILE *fk = fopen(name, "r");
	if (fk == 0)
	{
		error();
		cout << "文件打开失败！请重新输入文件名称！" << endl;
		goto start;
	}
	else
	{
		bullet = 1;
		fclose(fk);
		ifstream fp(name, ios::in);
		fstream fpp("current_course.txt", ios::out);
		if (fpp.fail())
		{
			cout << "文件录入失败！"<<endl;
			return false;
		}
		success();
		cout <<star<< "课程导入成功！"<<star<<endl<<"导入课程信息如下：" << endl;
		cout << starline<<endl<<setw(10) << std::left << "课程编号" << setw(30) << std::left << "课程名称" << setw(20) << std::left
			<< "授课教师" << setw(10) << std::left << "上限人数" << setw(10) << std::left << "目前已选" << setw(10) << std::left << "课程类型" << endl;
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
			info.push_back(p);
			cout << setw(10) << std::left << p.id << setw(30) << std::left << p.name
				<< setw(20) << std::left << p.tutor << setw(10) << std::left << p.max << setw(10) << std::left << p.current << setw(10) << std::left << p.type << endl;
			fpp << p.id << " " << p.name << " " << p.tutor << " " << p.max << " " << p.current << " " << p.type << endl;
			fp >> id;
		}
		cout << starline << endl;
		fpp.close();
		fp.close();
		initial_ta_info();
	}
	return true;
}
bool view_alllesson_info()
{
	FILE *fk = fopen("current_course.txt", "r");
	if (fk == 0)
	{
		error();
		cout << "无法查看课程信息！请导入课程后再进行操作！" << endl;
		return false;
	}
	fclose(fk);
	vector<lesson>::iterator loc = info.begin(), last = info.end();
	if (loc == last)
	{
		notice();
		cout << "目前还没有课程！" << endl;
		return true;
	}
	cout << "全部课程信息如下：" << endl<<starline<<endl;
	cout<< setw(10) << std::left<<  "课程编号" << setw(30) << std::left << "课程名称" << setw(20) << std::left
		<< "授课教师" << setw(10) << std::left << "上限人数" << setw(10) << std::left << "目前已选" << setw(10) << std::left << "课程类型" << endl;
	while (loc != last)
	{
		cout <<setw(10) <<std::left << loc->id<<setw(30)<<std::left<< loc->name
			 << setw(20) << std::left << loc->tutor << setw(10) << std::left << loc->max << setw(10) << std::left<< loc->current << setw(10) << std::left << loc->type << endl;
		loc++;
	}
	cout << starline << endl;
	return true;
}
bool add_lesson()
{
	FILE *fk = fopen("current_course.txt", "r");
	if (fk==0)
	{
		error();
		cout << "无法添加课程！请导入课程后再进行操作！" << endl;
		return false;
	}
	fclose(fk);
	
	cout << "请依次输入课程信息（名称、教师、容纳人数、类型，用空格分隔，回车结束）：";
	lesson p;
	cin >> p.name>> p.tutor >> p.max >> p.type;
		p.current = 0;
		if (check_duplicate2(p.name, p.tutor))
		{
			error();
			cout << "输入的课程已存在，添加失败！" << endl;
			return false;
		}
		else if (fabs(strcmp(p.type, "专业课")) && fabs(strcmp(p.type, "非专业课")))
		{
			error();
			cout <<"输入的课程类型不符合规定！添加失败！" << endl;
			return false;
		}
		else if (p.max <= 0)
		{
			error();
			cout << "输入的容纳人数不符合规定！添加失败！" << endl;
			return false;
		}
		p.id[3] = '\0';
		if (info.size() == 0)
		{
			int maxs = p.max, currents = p.current;
			char ids[4] = { '\0' }, names[31] = { '\0' }, tutors[31] = { '\0' }, types[31] = { '\0' };
			strcpy(names, p.name); strcpy(tutors, p.tutor); strcpy(types, p.type);
			strcpy(p.id, "001");
			strcpy(ids, p.id);
			info.push_back(p);
			FILE *fpk = fopen("current_course.txt", "w+");
			fprintf(fpk, "%s\t%s\t%s\t%d\t%d\t%s\n", ids, names, tutors, maxs, currents, types);
			fclose(fpk);
		}
		else
		{
			vector<lesson>::iterator loc = info.begin(), last = info.end();
			while (loc != last - 1)
				loc++;
			int i = (loc->id[0] - '0') * 100 + (loc->id[1] - '0') * 10 + (loc->id[2] - '0') + 1;
			if (i <= 9)
			{
				p.id[0] = '0';
				p.id[1] = '0';
				p.id[2] = '0' + i;
			}
			else if (i <= 99)
			{
				p.id[0] = '0';
				p.id[1] = '0' + i / 10;
				p.id[2] = '0' + i % 10;
			}
			else if (i <= 999)
			{
				p.id[0] = '0' + i / 100;
				p.id[1] = '0' + (i % 100) / 10;
				p.id[2] = '0' + i % 10;
			}
			info.push_back(p);
		ofstream fp("current_course.txt", ios::app);
		fp << p.id << "\t" << p.name << "\t" << p.tutor << "\t" << p.max << "\t" << p.current << "\t" << p.type << endl;
		fp.close();
		}
		FILE *fpp = fopen("assistant.txt", "a+");
		if (fpp == 0)
		{
			cout << "课程添加成功，但是所添加课程无法导入助教列表！" << endl;
			return false;
		}
		fprintf(fpp, "%s", p.id);
		fprintf(fpp, "%s\n", " Null");
		fclose(fpp);
		success();
		cout << star <<"课程添加成功！" << star << endl;
	return true;
}
bool check_duplicate2(char name[], char tutor[])
{
	vector<lesson>::iterator first = info.begin(), last = info.end();
	while (first != last)
	{
		if ((!strcmp(name, first->name))&&(!strcmp(tutor,first->tutor)))
			return true;
		else first++;
	}
	return false;
}
bool delete_lesson()
{
	FILE *fk = fopen("current_course.txt", "r");
	if (fk==0)
	{
		error();
		cout << "无法删除课程！请导入课程后再进行操作！" << endl;
		return false;
	}
	fclose(fk);
	vector<lesson>::iterator first1 = info.begin(), last1 = info.end();
	if (first1 == last1)
	{
		error();
		cout << "目前还没有课程，无法删除！" << endl;
		return true;
	}
	view_alllesson_info();
	char num[4] = { '\0' };
	cout << "请输入待删除的课程编号：";
	while (1)
	{
		vector<lesson>::iterator first = info.begin(), last = info.end();
		cin >> num;
		int flag1 = 1, flag2 = 0;
		while (first != last)
		{
			if (!strcmp(num, first->id))
				flag1 = 0;
			if (flag1 == 0 && first->current == 0)
			{
				info.erase(first);
				break;
			}
			else if (flag1 == 0 && first->current != 0)
			{
				flag2 = 1;
				break;
			}
			first++;
		}
		if (flag1)
		{
			error();
			cout << "ID为"<<num<<"的课程不存在，删除失败！" << endl;
			if (getchar() == '\n')
				break;
			else continue;
		}
		else if (flag2)
		{
			error();
			cout << "已有人选择ID为"<<num<<"的课程，删除失败！" << endl;
			if (getchar() == '\n')
				break;
			else continue;
		}
		else
		{
			ofstream fp("current_course.txt", ios::out);
			vector<lesson>::iterator loc = info.begin(), end = info.end();
			while (loc != end)
			{
				fp << loc->id << "\t" << loc->name << "\t" << loc->tutor << "\t" << loc->max << "\t" << loc->current << "\t" << loc->type << endl;
				loc++;
			}
			fp.close();
			FILE *fpp = fopen("assistant.txt", "r");
			if (fpp == 0)
			{
				cout << "无法修改该课程助教信息！" << endl;
				return false;
			}
			char already[4] = { '\0' }, temp[5000] = { '\0' }, tmp[5000] = { '\0' }, key[5000] = { '\0' };
			fscanf(fpp, "%s", already);
			while (!feof(fpp))
			{
				fscanf(fpp, "%s", temp);
				if (fabs(strcmp(already, num)))
				{
					strcat(tmp, already);
					strcat(tmp, " ");
					strcat(tmp, temp);
					strcat(tmp, "\n");
				}
				else if (!strcmp(already, num))
				{
					fseek(fpp, 2, SEEK_CUR);
					fscanf(fpp, "%[^;]", key);
					fclose(fpp);
					FILE *fk = fopen("assistant.txt", "w+");
					if (fk == 0)
					{
						cout << "无法修改课程助教信息！";
						return false;
					}
					fprintf(fk, tmp);
					fprintf(fk, key);
					fclose(fk);
					break;
				}
				fscanf(fpp, "%s", already);
			}
		}
		success();
		cout << star <<"ID为"<<num<< "的课程删除成功！" << star << endl;
		if (getchar() == '\n')
				break;
	}
	cout << "删除后，";
	view_alllesson_info();
	return true;
}
bool change_lesson()
{
	FILE *fk = fopen("current_course.txt", "r");
	if (fk==0)
	{
		error();
		cout << "无法修改！请导入课程后再进行操作！"<<endl;
		return false;
	}
	fclose(fk);
	char num[4] = { '\0' };
	cout << "请输入需要修改的课程号：";
	cin >> num;
	vector<lesson>::iterator first = info.begin(), last = info.end();
	int flag=1;
	while (first != last)
	{
		if (!strcmp(num, first->id))
		{
			flag = 0;
			break;
		}
		first++;
	}
	if (flag)
	{ 
		error();
		cout << "所选择的课程不存在！"<<endl;
	}
	else
	{
		int choice;
	cout << "请选择需要修改的内容 （1.教师	2.容纳人数）：";
	cin >> choice;
	cout << "具体修改为：";
	if (choice == 1)
	{
		char tutor[31] = { '\0' };
		cin >> tutor;
		strcpy(first->tutor, tutor);
		success();
		cout << star<<"修改课程教师成功！"<<star<<endl;
	}
	else if (choice == 2)
	{
		int temp;
		cin >> temp;
		if (temp < first->max)
		{
			error();
			cout << "修改后容纳人数小于当前容纳人数，修改失败！" << endl;
			return false;
		}
		else
		{
			first->max = temp;
			success();
			cout <<star<< "修改课程容纳人数成功！"<<star<<endl;
		}
	}
	else
	{
		error();
		cout << "操作无效！";
		return false;
	}
		ofstream fp("current_course.txt", ios::out);
		vector<lesson>::iterator loc = info.begin(), end = info.end();
		while (loc != end)
		{
			fp << loc->id << "\t" << loc->name << "\t" << loc->tutor << "\t" << loc->max << "\t" << loc->current << "\t" << loc->type << endl;
			loc++;
		}
		fp.close();
	}
	return true;
	}
bool view_specific_info()
{
	FILE *fk = fopen("current_course.txt", "r");
	if (fk==0)
	{
		error();
		cout << "无法查看课程信息！请导入课程后再进行操作！" << endl;
		return false;
	}
	fclose(fk);
	char temp[31] = { '\0' };
	cout << "请输入需要查看的课程号或课程名称：";
	cin >> temp;
	vector<lesson>::iterator first = info.begin(), last = info.end();
	int flag = 1;
	while (first != last)
	{
		if (!(strcmp(temp, first->id)&&strcmp(temp,first->name)))
		{
			flag = 0;
			break;
		}
		first++;
	}
	if (flag)
	{
		error();
		cout << "所选择的课程不存在！" << endl;
	}
	else
	{
		cout << "课程信息如下："<<endl<<starline<<endl;
		cout << first->id << "\t" << first->name << "\t" << first->tutor << "\t" << first->max << "\t" << first->current << "\t" << first->type << endl<<starline<<endl;
		cout << "请选择还需查看的内容（1.选课学生名单	2.报名助教名单）：";
		int choice;
		cin >> choice;
		if (choice == 1)
		{
			if (first->chosen_by.size() == 0)
				cout << "该课程目前没有学生选择！";
			else
			{
				cout << "目前选择该课程的学生有：";
				for (auto beg = first->chosen_by.begin(); beg < first->chosen_by.end(); beg++)
					cout << *beg << " ";
			}
				cout << endl;
		}
		else if (choice == 2)
		{
			FILE *fp = fopen("assistant.txt", "r");
			if (fp == 0)
			{
				cout << "无法查看课程助教信息！" << endl;
				return false;
			}
			char id[31] = { '\0' };
			
			fscanf(fp, "%s", id);
			while (!feof(fp))
			{
				char ta[5000] = { '\0' };
				if (!strcmp(id, first->id))
				{
					fscanf(fp, "%s", ta);
					if (!strcmp(ta, "Null"))
						cout << "该课程目前没有助教！" << endl;
					else cout << "目前该课程的助教有：" << ta << endl;;
					break;
				}
				fscanf(fp, "%s", ta);
				fscanf(fp, "%s", id);
			}
			fclose(fp);
			return true;
		}
		else
		{ 
			error();
			cout << "操作无效！返回上级界面！";
		}
	}
	return false;
	}
bool initial_ta_info()
{
	FILE *fp = fopen("assistant.txt", "w+");
		if (fp == 0)
		{
			cout << "助教名单初始化失败！";
			return false;
		}
	for (auto i = info.begin(); i < info.end(); i++)
		{
			fprintf(fp, "%s", i->id);
			fprintf(fp, "%s"," ");
			fprintf(fp, "%s\n", "Null");
		}
		fclose(fp);
		return true;
}
bool view_ta_stu()
{
	char lesson_id[4] = { '\0' };
	FILE *fp = fopen("student.txt", "r");
	FILE *fk = fopen("assistant.txt", "r");
	if (fp == 0 || fk == 0)
	{
		error();
		cout << "查看助教对应学生名单失败！请先导入课程信息！" << endl;
		return false;
	}
	else
	{
		cout<<"请输入欲查看的课程ID：";
		cin >> lesson_id;
		char already[4] = { '\0' }, temp[5000] = { '\0' }; int flag1 = 1;
		fscanf(fk, "%s",already);
		while (!feof(fk))
		{
			fscanf(fk, "%s", temp);
			if (!strcmp(already, lesson_id))
			{
				flag1 = 0;
				break;
			}
			fscanf(fk, "%s", already);
		}
		if (flag1)
		{
			error();
			cout << "所输入的课程不存在！" << endl;
			return false;
		}
		if (!strcmp(temp, "Null"))
		{
			cout << "该课程目前没有助教！" << endl;
			return false;
		}
		cout << "课程助教的学生名单如下：" << endl;
			int i = 0;
			for (int j = 0; temp[j]; j++)
				if (temp[j] == ',')
					i++;
			fseek(fk, -(int)strlen(temp), SEEK_CUR);
			for (int j = 0; j < i; j++)
			{
				int flag2 = 1;
				fscanf(fk, "%[^,]", temp);
				fseek(fk, 1, SEEK_CUR);
				cout << "助教" << temp << "的学生有：";
				fseek(fp, 0, SEEK_SET);
				char id[31] = { '\0' };
				fscanf(fp, "%[^,]", id);
				while (!feof(fp))
				{
					char ide[25] = { '\0' };
					strcpy(ide, id);
					strcat(ide, ".txt");
					FILE *fpp = fopen(ide, "r");
					if (fpp == 0)
					{
						cout << "查看助教信息失败！";
						return false;
					}
					char tmp[4] = { '\0' };
					char ta[31] = { '\0' };
					fscanf(fpp, "%s", tmp);
					while (!feof(fpp))
					{
						fscanf(fpp, "%s", ta);
						if (!strcmp(tmp, lesson_id))
						{
							if (!strcmp(ta, temp))
							{
								flag2 = 0;
								cout << id << " ";
								break;
							}
							else break;
						}
						fscanf(fpp, "%s", tmp);
					}
					fclose(fpp);
					fseek(fp, 1, SEEK_CUR);
					fscanf(fp, "%s", id);
					fseek(fp, 2, SEEK_CUR);
					fscanf(fp, "%[^,]", id);
				}
				if (flag2)
					cout << "无"<< endl;
				else cout << endl;
			}
			fscanf(fk, "%s", temp);
			cout << "助教" << temp << "的学生有：";
		char id[31] = { '\0' };
		fseek(fp, 0, SEEK_SET);
		fscanf(fp, "%[^,]", id);
		int flag2 = 1;
		while (!feof(fp))
		{
			char ide[25] = { '\0' };
			strcpy(ide, id);
			strcat(ide, ".txt");
			FILE *fpp = fopen(ide, "r");
			if (fpp == 0)
			{
				cout << "查看助教信息失败！";
				return false;
			}
			char tmp[4] = { '\0' };
			char ta[31] = { '\0' };
			fscanf(fpp, "%s", tmp);	
			while (!feof(fpp))
			{
				fscanf(fpp, "%s", ta);
				if (!strcmp(tmp, lesson_id))
				{
					if(!strcmp(ta,temp))
					{
						flag2 = 0;
						cout << id << " ";
						break;
					}
				}
			fscanf(fpp, "%s", tmp);
			}
			fclose(fpp);
			fseek(fp, 1, SEEK_CUR);
			fscanf(fp, "%s", id);
			fseek(fp, 2, SEEK_CUR);
			fscanf(fp, "%[^,]", id);
		}
		if (flag2)
			cout << "无" << endl;
		else cout << endl;
	}
	fclose(fp);
	return true;
}

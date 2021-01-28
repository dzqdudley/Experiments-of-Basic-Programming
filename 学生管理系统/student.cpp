#define _CRT_SECURE_NO_WARNINGS
#include "student.h"
string splitliner(110, '=');
void stufunc()
{
	while (1)
	{
		cout << endl << "请选择您的操作：\n" << "\033[34m"<<splitliner << endl;
		cout << "1.注销登录\t2.查看课程信息\t3.选课\t4.查看个人课表\t5.退课\t6.举手报名助教\t7.选择个人助教\t8.修改账户密码";
		cout<< endl << splitliner<<"\033[30m" << endl;
		char i[31] = { '\0' };
		cout << "请输入您的操作：";
		cin >> i; 
		cout << endl;
		if (!strcmp(i, "1"))
			signout();
		else if (!strcmp(i, "2"))
			view_alllesson_info();
		else if (!strcmp(i, "3"))
			choose_lesson();
		else if (!strcmp(i, "4"))
			view_schedule(0);
		else if (!strcmp(i, "5"))
			quit_lesson();
		else if (!strcmp(i, "6"))
			sign_for_ta();
		else if (!strcmp(i, "7"))
			choose_ta();
		else if (!strcmp(i, "8"))
			change_pass();
		else
		{
			error();
			cout << "指令无效！" << endl;
		}
		notice();
		cout << "现返回上级界面！" << endl;
	}
}
bool choose_lesson()
{
	if ((stu.major + stu.minor) >= 10)
	{
		error();
		cout << "您的课程数已达最大值，无法再选课！" << endl;
		return false;
	}
	FILE *fp = fopen("current_course.txt", "r");
		if (fp == 0)
		{
			cout << "选课失败！";
			return false;
		}
		view_alllesson_info();
		cout << "请输入欲选课程ID（用空格分隔）：";
		char  input_id[4] = { '\0' }, stu_file[25] = { '\0' };
		int flag6 = 1;
		while(1)
		{
			cin >> input_id;
			if ((stu.major + stu.minor) >= 10)
			{
				if (flag6)
				{
					error();
					cout << "您的课程数已达最大值，无法再选课！" << endl;
					flag6 = 0;
				}
				if (getchar() == '\n')
					break;
				else continue;
			}
		strcpy(stu_file, stu.stu_id);
		strcat(stu_file, ".txt");
		FILE *fpp = fopen(stu_file, "r");
		if (fpp == 0)
		{
			cout << "选课失败！"<<endl;
			fclose(fp);
			return false;
		}
		vector<lesson>::iterator first = info.begin(), last = info.end();
		int flag1 = 1,flag2=0,flag3=0;
		while (first != last)
		{
			 if((!strcmp(first->id, input_id)) && (first->max == first->current))
			{
				 flag2 = 1;
				 error();
				cout << "ID为"<<input_id<<"的课程人数已满，选课失败！"<<endl;
				break;
			}
			else if (!strcmp(first->id, input_id))
			{
				flag1 = 0;
				break;
			}
			first++;
		}
		char already[4] = { '\0' };
		fscanf(fpp, "%s", already);
		while (!feof(fpp))
		{
			char temp[31] = { '\0' };
			if (!strcmp(already, input_id))
			{
				flag3 = 1;
				error();
				cout << "你已选择过ID为"<<input_id<<"的课程，不可重复选择！" << endl;
				break;
			}
			fscanf(fpp, "%s", temp);
			fscanf(fpp, "%s", already);
		}
		if (flag1)
		{
			error();
			cout << "不存在ID为"<<input_id<<"的课程，选课失败！" << endl;
			if (getchar() == '\n')
				break;
			else continue;
		}
		else if (flag2 || flag3)
		{
			if (getchar() == '\n')
				break;
			else continue;
		}
		if (!strcmp(first->type, "专业课"))
				stu.major++;
			else	stu.minor++;
			int i = 0;
			fseek(fpp, 0, SEEK_SET);
			char tmp[5000] = { '\0' };
			fscanf(fpp, "%s", already);
			while (!feof(fpp))
			{
				if (strcmp(input_id, already) < 0)
					break;
				char temp[31] = { '\0' };
				i++;
				fscanf(fpp, "%s", temp);
				fscanf(fpp, "%s", already);
			}
			fseek(fpp, 0, SEEK_SET);
			int j = 0;
			while (!feof(fpp) && j < i)
			{
				fscanf(fpp, "%s", already);
				strcat(tmp, already);
				strcat(tmp, " ");
				char temp[31] = { '\0' };
				j++;
				fscanf(fpp, "%s", temp);
				strcat(tmp, temp);
				strcat(tmp, "\n");
			}
				char key[5000] = { '\0' };
				if(i!=0)
				fseek(fpp, 2, SEEK_CUR);
				fscanf(fpp, "%[^,]", key);
				fclose(fpp);
				FILE *fk = fopen(stu_file, "w+");
				fprintf(fk, tmp);
				fprintf(fk, "%s%s%s", input_id, " ", "Null");
				fprintf(fk, "%s", "\n");
				fprintf(fk, key);
				fclose(fk);
				success();
			cout << star<<"ID为"<<input_id<<"的课程选择成功！"<<star<<endl;
			first->current++;
			first->chosen_by.push_back(stu.stu_id);
			fclose(fp);
			update_cur_cour();
			if (getchar() == '\n')
				break;
		}
			return true;
}
bool view_schedule(int boot)
{
	char stu_file[25] = { '\0' };
	strcpy(stu_file, stu.stu_id);
	strcat(stu_file, ".txt");
	FILE *fpp = fopen(stu_file, "r");
	if (fpp == 0)
	{
		cout << "查看个人课表失败！"<<endl;
		return false;
	}
	if (stu.major == 0 && stu.minor == 0)
	{
		notice();
		cout << "目前您的课表为空！无法查看！" << endl;
		notice();
		cout << "根据学院要求，目前所选课程数不达标！还需选择4门专业课，以及2门非专业课，请尽快选择！"<<endl;
		return false;
	}
	cout << "目前所选课程有：" << endl << starline << endl; 
	cout << setw(10) << std::left << "课程编号" << setw(30) << std::left << "课程名称" << setw(20) << std::left
		<< "授课教师" << setw(15) << std::left << "课程类型" << setw(15) << std::left<<"个人助教"<< endl;
	vector<lesson>::iterator loc = info.begin(), last = info.end();
	while (loc != last)
	{
			char already[31] = { '\0' };
			fseek(fpp, 0, SEEK_SET);
			fscanf(fpp, "%s", already);
			while (!feof(fpp))
			{
				char temp[31] = { '\0' };
				fscanf(fpp, "%s", temp);
				if (!strcmp(already,loc->id))
				{
					cout << setw(10) << std::left << loc->id << setw(30) << std::left << loc->name
						<< setw(20) << std::left << loc->tutor << setw(15) << std::left << loc->type << setw(15) << std::left <<temp<< endl;
					break;
				}
				else 
					fscanf(fpp, "%s", already);
			}
			loc++;
	}
	cout << starline << endl;
	fclose(fpp);
	if (boot == 1)
		return true;
	notice();
	if (stu.major < 4 && stu.minor < 2)
		cout << "根据学院要求，目前所选课程数不达标！还需选择" << 4 - stu.major << "门专业课，以及" << 2 - stu.minor << "门非专业课，请尽快选择！";
	else if (stu.major < 4 && stu.minor >= 2)
		cout << "根据学院要求，目前所选课程数不达标！还需选择" << 4 - stu.major << "门专业课，请尽快选择！";
	else if (stu.major >= 4 && stu.minor < 2)
		cout << "根据学院要求，目前所选课程数不达标！还需选择" << 2 - stu.minor << "门非专业课，请尽快选择！";
	else
		cout << "您所选择的课程数已达到学院要求！";
	cout << endl;
	return true;
}
bool quit_lesson()
{
	view_schedule(1);
	char input_id[4] = { '\0' };
	cout << "请输入欲删除的课程ID（用空格间隔）:";
	
	char stu_file[25] = { '\0' };
	strcpy(stu_file, stu.stu_id);
	strcat(stu_file, ".txt");
	while (1)
	{
		FILE *fpp = fopen(stu_file, "r+");
		if (fpp == 0)
		{
			cout << "退课失败！" << endl;
			return false;
		}
		cin >> input_id;
		vector<lesson>::iterator first = info.begin(), last = info.end();
		int flag1 = 1;
		while (first != last)
		{
			if (!strcmp(first->id, input_id))
			{
				flag1 = 0;
				break;
			}
			first++;
		}
		fseek(fpp, 0, SEEK_SET);
		char already[31] = { '\0' };
		fscanf(fpp, "%s", already);
		int flag = 1, i = 0;
		while (!feof(fpp))
		{
			char temp[31] = { '\0' };
			fscanf(fpp, "%s", temp);
			if (!strcmp(already, input_id))
			{
				flag = 0;
				break;
			}
			else
				fscanf(fpp, "%s", already);
			i++;
		}
		if (flag1)
		{
			error();
			cout << "不存在ID为"<<input_id<<"的课程，退课失败！" << endl;
			goto end;
		}
		else if (flag)
		{
			error();
			cout << "ID为"<<input_id<<"的课程不在您的课表中，退课失败！" << endl;
			goto end;
		}
		else
		{
			first->current--;
			for (auto i = first->chosen_by.begin(); i < first->chosen_by.end(); i++)
				if (*i == stu.stu_id)
				{
					first->chosen_by.erase(i);
					break;
				}
			if (!strcmp(first->type, "专业课"))
				stu.major--;
			else	stu.minor--;
			fseek(fpp, 0, SEEK_SET);
			int j = 0;
			char tmp1[5000] = { '\0' };
			while (!feof(fpp) && j < i)
			{
				fscanf(fpp, "%s", already);
				strcat(tmp1, already);
				strcat(tmp1, " ");
				char temp[31] = { '\0' };
				j++;
				fscanf(fpp, "%s", temp);
				strcat(tmp1, temp);
				strcat(tmp1, "\n");
			}
			if (strlen(tmp1) > 1)
				strncpy(tmp1, tmp1, strlen(tmp1) - 1);
			fscanf(fpp, "%s", already);
			char temp[31] = { '\0' };
			fscanf(fpp, "%s", temp);
			fseek(fpp, 2, SEEK_CUR);
			char tmp[5000] = { '\0' };
			fscanf(fpp, "%[^,]", tmp);
			fclose(fpp);
			FILE *fk = fopen(stu_file, "w+");
			fprintf(fk, tmp1);
			fprintf(fk, tmp);
			success();
			cout << star << "ID为" << input_id << "的课程退课成功！" << star << endl;
			fclose(fk);
			update_cur_cour();
			clear_assistant(input_id);
		}
		end:	if (getchar() == '\n')
			break;
	}
	cout << "退课后，";
		view_schedule(0);
		return true;
}
bool sign_for_ta()
{
	if (stu.signed_ta == 2)
	{
		error();
		cout << "您已经报名两门课程的助教，无法再报！"<<endl;
		return false;
	}
	else if (stu.major == 0 && stu.minor == 0)
	{
		error();
		cout << "您还未选择任何课程，无法报名助教！" << endl;
		return false;
	}
	view_schedule(1);
		cout << "请输入欲报名助教的课程ID或课程名：";
		char input_id[31] = { '\0' };
		cin >> input_id;
		int flag1 = 1, flag2 = 1;
		vector<lesson>::iterator loc = info.begin(), end = info.end();
			while (loc != end)
			{
				if (!(strcmp(loc->name, input_id)&&strcmp(loc->id,input_id)))
				{
					flag1 = 0;
					strcpy(input_id, loc->id);
					break;
				}
				loc++;
			}
		char stu_file[25] = { '\0' };
		strcpy(stu_file, stu.stu_id);
		strcat(stu_file, ".txt");
		FILE *fk = fopen(stu_file, "r+");
		if (fk == 0)
		{
			cout << "报名助教失败！";
			return false;
		}
		char already[4] = { '\0' };
		fscanf(fk, "%s", already);
		while (!feof(fk))
		{
			char temp[31] = { '\0' };
			fscanf(fk, "%s", temp);
			if (!strcmp(already, input_id))
			{
				flag2 = 0;
				break;
			}
			else
				fscanf(fk, "%s", already);
		}
		fclose(fk);
		if (flag1)
		{
			error();
			cout << "该课程不存在！"<<endl;
			return false;
		}
		else if (flag2)
		{
			error();
			cout << "您未选择该课程，无法报名助教！"<<endl;
			return false;
		}
			FILE *fpp = fopen("assistant.txt", "r+");
			char id[4] = { '\0' };
			char temp[5000] = { '\0' },tmp[5000]={'\0'};
			fscanf(fpp, "%s", id);
			while (!feof(fpp))
			{
				strcat(tmp, id);
				strcat(tmp, " ");
				fscanf(fpp, "%s", temp);
				if (fabs(strcmp(id, input_id)))
				{
					strcat(tmp, temp);
					strcat(tmp, "\n");
				}
				if (!strcmp(id, input_id) && !strcmp(temp, "Null"))
				{
					char buffer[5000]={'\0'};
					fscanf(fpp, "%[^;]", buffer);
					fclose(fpp);
					FILE *fk = fopen("assistant.txt", "w+");
					fprintf(fk, tmp);
					fprintf(fk, "%s", stu.stu_id);
					fprintf(fk, buffer);
					fclose(fk);
					success();
					cout <<star<< "报名助教成功！"<<star<<endl;
					stu.signed_ta++;
					break;
				}
				else if (!strcmp(id, input_id))
				{
					int flag3 = 0;
					int i = 0;
					for (int j = 0; temp[j]; j++)
						if (temp[j] == ',')
							i++;
					fseek(fpp, -(int)strlen(temp), SEEK_CUR);
					for (int j = 0; j < i; j++)
					{
						fscanf(fpp, "%[^,]", temp);
						fseek(fpp, 1, SEEK_CUR);
						if (!strcmp(temp, stu.stu_id))
						{
							flag3 = 1;
							break;
						}
					}
					fscanf(fpp, "%s", temp);
					if (!strcmp(temp, stu.stu_id))
						flag3 = 1;
					if (flag3)
					{
						fclose(fpp);
						error();
						cout << "您已经是该课程助教，无法重复报名！"<<endl;
						return false;
					}
					success();
						stu.signed_ta++;
						cout <<star<< "报名助教成功！"<<star<<endl;
						int pos = ftell(fpp);
						char key[5000] = { '\0' };
						fscanf(fpp, "%[^;]", key);
						fseek(fpp, pos, 0);
						fprintf(fpp, "%c", ',');
						fprintf(fpp,"%s", stu.stu_id);
						fprintf(fpp, key);
						fclose(fpp);
					break; 
				}
				fscanf(fpp, "%s", id);
			}
			return true;
}
bool choose_ta()
{
	if (stu.major == 0 && stu.minor == 0)
	{
		error();
		cout << "您目前未选择任何课程，无法选择助教！" << endl;
		return false;
	}
	view_schedule(1);
	cout << "请输入欲选择助教的课程ID或课程名称：";
	char input_id[31] = { '\0' };
	cin >> input_id;
	int flag1 = 1, flag2 = 1, flag3 = 0;
	vector<lesson>::iterator loc = info.begin(), end = info.end();
	while (loc != end)
	{
		if (!(strcmp(loc->name, input_id) && strcmp(loc->id, input_id)))
		{
			flag1 = 0;
			strcpy(input_id, loc->id);
			break;
		}
		loc++;
	}
	char stu_file[25] = { '\0' };
	strcpy(stu_file, stu.stu_id);
	strcat(stu_file, ".txt");
	FILE *fpp = fopen(stu_file, "r+");
	if (fpp == 0)
	{
		cout << "选择助教失败！"<<endl;
		exit(-1);
	}
	char already[4] = { '\0' };
	fscanf(fpp, "%s", already);
	while (!feof(fpp))
	{
		char temp[31] = { '\0' };
		fscanf(fpp, "%s", temp);
		if (!strcmp(already, input_id))
		{
			flag2 = 0;
			if (fabs(strcmp(temp, "Null")))
				flag3 = 1;
			break;
		}
		else
			fscanf(fpp, "%s", already);
	}
	if (flag1 || flag2 || flag3)
	{
		error();
		if (flag1)
			cout << "该课程不存在！无法选择助教！" << endl;
		else if (flag2)
			cout << "您未选择该课程，无法选择该课程的助教！" << endl;
		else if (flag3)
			cout << "该课程您已选择过助教，无法再次选择！" << endl;
		return false;
	}
	
		FILE *fp = fopen("assistant.txt", "r+");
		char id[4] = { '\0' };
		char temp[5000] = { '\0' };
		fseek(fpp, 0, SEEK_SET);
		fscanf(fp, "%s", id);
		while (!feof(fp))
		{
			fscanf(fp, "%s", temp);
			if (!strcmp(id, input_id))
			{
				if (!strcmp(temp, stu.stu_id))
				{
					notice();
					cout << "该门课程助教只有您自己，无法选择助教！" << endl;
					return false;
				}
				else if (!strcmp(temp, "Null"))
				{
					notice();
					cout << "该门课程目前没有助教！无法选择！" << endl;
					return false;
				}
				choose_ta_again:
				cout << "目前这门课的助教有：" << temp << endl;
				cout << "请选择您的个人助教：";
				char name[31] = { "\0" };
				cin >> name;
				if (!strcmp(name, stu.stu_id))
				{
					error();
					cout << "您不能选择自己作为助教，请重新选择！"<<endl;
					goto choose_ta_again;
				}
					int i = 0,flag4=1;
					for (int j = 0; temp[j]; j++)
						if (temp[j] == ',')
							i++;
					fseek(fp, -(int)strlen(temp), SEEK_CUR);
					for (int j = 0; j < i; j++)
					{
						fscanf(fp, "%[^,]", temp);
						fseek(fp, 1, SEEK_CUR);
						if (!strcmp(temp, name))
						{
							flag4 = 0;
							break;
						}
					}
					fscanf(fp, "%s", temp);
					if (!strcmp(temp, name))
						flag4 = 0;
					if (flag4)
					{
						fclose(fp);
						error();
						cout << "所输入学生非该课程助教，选择助教失败！" << endl;
						return false;
					}
				else
				{
						success();
					cout <<star<< "助教选择成功！"<<star<<endl;
					fclose(fp);
					fseek(fpp, 0, SEEK_SET);
					char already[4] = { '\0' }, tem[31] = { '\0' }, change[5000] = { '\0' };
					fscanf(fpp, "%s", already);
					while (!feof(fpp))
					{
						strcat(change, already);
						strcat(change, " ");
						if (!strcmp(already, input_id))
							break;
						else
							fscanf(fpp, "%s", tem);
						strcat(change, tem);
						strcat(change, "\n");
						fscanf(fpp, "%s", already);
					}
					char temp1[5000] = { '\0' };
					fscanf(fpp, "%s", tem);
					fscanf(fpp, "%[^;]", temp1);
					fclose(fpp);
					FILE *fk = fopen(stu_file, "w+");
					fprintf(fk, change);
					fprintf(fk, "%s", name);
					fprintf(fk, temp1);
					fclose(fk);
					break;
				}
			}
			fscanf(fp, "%s", id);
		}
		return true;
}
void clear_assistant(char input_id[ ])
{
	FILE *fpp = fopen("assistant.txt", "r+");
	char id[4] = { '\0' };
	char temp[5000] = { '\0' };
	char tmp[5000] = { '\0' };
	fscanf(fpp, "%s", id);
	int flag = 0;
	while (!feof(fpp))
	{
		strcat(tmp, id);
		strcat(tmp, " ");
		fscanf(fpp, "%s", temp);
		if (!strcmp(id, input_id))
		{
			int i = 0,j=0;
			for (int j = 0; temp[j]; j++)
				if (temp[j] == ',')
					i++;
			fseek(fpp, -(int)strlen(temp), SEEK_CUR);
			for (j = 0; j <= i; j++)
			{
				if (j < i)
				{
					fscanf(fpp, "%[^,]", temp);
					fseek(fpp, 1, SEEK_CUR);
					if (fabs(strcmp(temp, stu.stu_id)))
					{
						strcat(tmp, temp);
							strcat(tmp, ",");
					}
				}
				else if(j==i)
				{
					fscanf(fpp, "%s", temp);
					if (fabs(strcmp(temp, stu.stu_id)))
						{
							strcat(tmp, temp);
								strcat(tmp, "\n");
						}
				}
				if (!strcmp(temp, stu.stu_id))
				{
					flag = 1;
					break;
				}
			}
			if (flag)
			{
				stu.signed_ta--;
				char key[5000] = { '\0' };
				fscanf(fpp, "%[^;]", key);
				fclose(fpp);
				FILE *fk = fopen("assistant.txt", "w+");
				fprintf(fk, tmp);
				if (i == j&&i!=0)
					fseek(fk, -1, SEEK_CUR);
				if (i == 0)
					fprintf(fk, "%s", "Null");
				fprintf(fk, key);
				fclose(fk);
				break;
			}
		}
		strcat(tmp, temp);
		strcat(tmp, "\n");
		fscanf(fpp, "%s", id);
	}
	if (flag == 0)
		fclose(fpp);
}
void update_cur_cour()
{
	fstream fpp("current_course.txt", ios::out);
	if (fpp.fail())
	{
		cout << "文件录入失败！";
		exit(-1);
	}
	vector<lesson>::iterator p = info.begin(), last = info.end();
	while (!fpp.eof() && p != last)
	{
		fpp << p->id << " " << p->name << " " << p->tutor << " " << p->max << " " << p->current << " " << p->type << endl;
		p++;
	}
	fpp.close();
}
bool change_pass()
{
	FILE *fp = fopen("student.txt", "r+");
	if (fp == 0)
	{
		cout << "无法修改密码！" << endl;
		return false;
	}
		char name[30] = { '\0' }, temp[30] = { '\0' }, tmp[5000] = { '\0' },pass[30]={'\0'},buffer[5000]={'\0'};
		cout << "请输入当前密码：";
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
				pass[i++] = ch;
				cout << "*";
			}
		}
		cout << endl;
		fscanf(fp, "%[^,]", name);
		while (!feof(fp))
		{
			strcat(buffer, name);
			strcat(buffer, ",");
			if (!strcmp(name, stu.stu_id))
			{
				fseek(fp, 1, SEEK_CUR);
				fscanf(fp, "%s", temp);
				if (!strcmp(pass, temp))
				{
					fscanf(fp, "%[^;]", tmp);
					again: cout << "请输入新密码：";
					char newpass[30] = { '\0' };
					cin >> newpass;
					if (!strcmp(newpass, temp))
					{
						error();
						cout << "新密码不能与原密码相同！请重新输入！" << endl;
						goto again;
					}
					for (int i = 0; newpass[i]; i++)
						if (!(('0' <= newpass[i] && newpass[i] <= '9') || ('a' <= newpass[i] && newpass[i] <= 'z')))
						{
							error();
							cout << "密码中只能含有小写字母或数字，请重新输入密码！" << endl;
							goto again;
						}
					success();
					cout << star << "密码修改成功！" << star << endl;
					fclose(fp);
					FILE *fk = fopen("student.txt", "w+");
					fprintf(fk, buffer);
					fprintf(fk, newpass);
					fprintf(fk, tmp);
					fclose(fk);
					return true;
				}
				else
				{
					error();
					cout << "现密码错误！无法修改密码！" << endl;
					return false;
				}
			}
			else
				fseek(fp, 1, SEEK_CUR);
				fscanf(fp, "%s", temp);
				strcat(buffer, temp);
				strcat(buffer, "\n");
				fseek(fp, 2, SEEK_CUR);
			fscanf(fp, "%[^,]", name);
		}
		fclose(fp);
		return true;
	}



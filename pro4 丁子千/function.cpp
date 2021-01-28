#include "head.h"
string star(12, '*');
string namesplit(70, '=');
struct func
{
	string func_name;//存储多项式名称
	int lenth;//存储多项式长度
	double nums[50];//存储多项式各项系数
};
bool operator > (const func& c1, const func& c2)//重载大于符号
{
	
	if (c1.lenth > c2.lenth)
	{
		int tmp = c1.lenth - c2.lenth;
		for (int i = 0; i < tmp; i++)
			if(fabs(c1.nums[i])>1e-5)
				return true;
		for (int i = tmp; i < c1.lenth; i++)
			if(fabs(c1.nums[i])>1e-5&&fabs(c2.nums[i-tmp])<1e-5||fabs(c1.nums[i])>fabs(c2.nums[i-tmp]))
				return true;
	}
	else
	{
		int tmp = c2.lenth - c1.lenth;
		for (int i = 0; i < tmp; i++)
			if(fabs(c2.nums[i])>1e-5)
				return false;
		for (int i = tmp; i < c2.lenth; i++)
			if(fabs(c1.nums[i])>1e-5&&fabs(c2.nums[i-tmp])<1e-5||fabs(c1.nums[i])>fabs(c2.nums[i-tmp]))
				return true;
	}
	return false;
}
func operator + (const func& c1, const func& c2)//重载加号
{
	func result;
	initialize_func(result);
	result.func_name ="t" ;
	if (c1.lenth > c2.lenth)
	{
		int tmp = c1.lenth - c2.lenth;
		result.lenth = c1.lenth;
		for (int i = 0; i < tmp; i++)
			result.nums[i] = c1.nums[i];
		for (int i = tmp; i < c1.lenth; i++)
			result.nums[i] = c1.nums[i] + c2.nums[i-tmp];
	}
	else
	{
		int tmp = c2.lenth - c1.lenth;
		result.lenth = c2.lenth;
		for (int i = 0; i < tmp; i++)
			result.nums[i] = c2.nums[i];
		for (int i = tmp; i < c2.lenth; i++)
			result.nums[i] = c1.nums[i-tmp] + c2.nums[i];
	}
	return result;
}
func operator - (const func& c1, const func& c2)
{
	func result;
	initialize_func(result);
	result.func_name = "t";
	if (c1.lenth > c2.lenth)
	{
		int tmp = c1.lenth - c2.lenth;
		result.lenth = c1.lenth;
		for (int i = 0; i < tmp; i++)
			result.nums[i] = c1.nums[i];
		for (int i = tmp; i < c1.lenth; i++)
			result.nums[i] = c1.nums[i] - c2.nums[i - tmp];
	}
	else
	{
		int tmp = c2.lenth - c1.lenth;
		result.lenth = c2.lenth;
		for (int i = 0; i < tmp; i++)
			result.nums[i] = -c2.nums[i];
		for (int i = tmp; i < c2.lenth; i++)
			result.nums[i] = c1.nums[i - tmp] - c2.nums[i];
	}
	return result;
}
func operator * (const func& c1, const func& c2)//重载乘号
{
	func result;
	initialize_func(result);
	result.func_name= "t";
	result.lenth = c1.lenth+c2.lenth-1;
	for(int i=0;i<c1.lenth;i++)
		for (int j = 0; j < c2.lenth; j++)
			result.nums[i + j] += c1.nums[i] * c2.nums[j];
		return result;
}
vector <func> functions;
void func_loop()//the template pad
{
	while (1)
	{
		struct func result;
		initialize_func(result);
		char i[31] = { '\0' };
		cout << star << "欢迎使用多项式计算器！" << star << endl;//modify
		cout << "请选择您的操作:" << endl;
		cout <<  namesplit << endl;//modify
		cout << "1.输入\t2.混合运算\t3.查看\t4.求根\t5.求逆\t6.除法\t7.关闭计算器\n";
		cout << namesplit<< endl << endl;//modify
		cout << "请输入您的操作:";
		cin >> i;
		if (!strcmp(i, "1"))
		{
			again1:
			if(get_func())
				cout << "输入成功，是否继续输入(y/n):";
			else cout<<"输入失败，是否继续输入(y/n):";
			string choice;
			cin>>choice;
			if(choice=="y")
			goto again1;
		}
		else if (!strcmp(i, "2"))
		{
			again:
			char name[500] = { '\0' }, ch;
			int i = 0;
			cout << "请输入运算式：";
			cin >> ch;
			while (ch != '\n')
			{
				name[i] = ch;
				i++;
				ch=cin.get();
			}
			if (is_legal(name))
				{
					cout<<name<<"=";
					mass_calculate(name);
					cout << "运算成功，是否继续运算(y/n):";
				}
			else cout << "运算失败，是否需要重新输入(y/n):";
			string choice;
			cin>>choice;
			if(choice=="y")
			goto again;
		}
		else if (!strcmp(i, "3"))
		{
			char name[100];
			cout << "请输入多项式名称：";
			cin >> name;
			cout<<name<<"=";
			print_func(name);
		}
		else if (!strcmp(i, "4"))
			get_root();
		else if (!strcmp(i, "5"))
		{
			struct func result;
			string name1;
			cout << "请输入多项式名称：";
			cin >> name1;
			if (get_reverse(name1, result))
			{
				cout <<"逆元为："<<result.func_name<<"=";
				functions.push_back(result);
				print_func(result.func_name);
				functions.pop_back();
			}
		}
		else if(!strcmp(i,"6"))
			divide();
		else if(!strcmp(i,"7"))
			break;
		else 
			cout << "指令无效，请重新输入！" << endl;
	}
}

bool print_func(string name)
{
	vector<func>::iterator start = functions.begin(), last = functions.end();
	int find = 0;
	while (start != last)
	{
		if (start->func_name== name)
		{
			find = 1;
			break;
		}
		else
			start++;
	}
	if (!find)
	{
		cout << "所输入多项式不存在！" << endl;
		return false;
	}
	int lenth = start->lenth, header = 0;
	for (int i = 0; i < lenth; i++)
	{
		if (fabs(start->nums[i] - 0) < 1e-5)
			continue;
		if (fabs(start->nums[i] - 1) < 1e-5)
		{
			if (header == 0)
					header = 1;
			else
				cout << "+" ;
		}
		else if (fabs(start->nums[i] + 1) < 1e-5)
		{
			if (header == 0)
				header = 1;
				cout << "-";
		}
		else if (start->nums[i] > 0)
		{
			if (header == 0)
			{
				header = 1;
				cout << start->nums[i];
			}
			else
				cout << "+" << start->nums[i];
		}
		else if (start->nums[i] < 0)
		{
			if(header==0)
			header=1;
			cout << start->nums[i];
		}
		if (lenth - 1 - i == 0)
		{
			if (fabs(start->nums[i] + 1) < 1e-5|| fabs(start->nums[i] - 1) < 1e-5)
				cout << 1;
		}
		else if (lenth - 1 - i == 1)
				cout << "x ";
		else if (lenth-1-i>1)
				cout << "x^"<<lenth-1-i<<" ";
	}
	if(header==0)
	cout<<0;
	cout << endl;
	return true;
}
bool get_func()
{
	int lenth,error=0;
	struct func new_func;
	initialize_func(new_func);
	cout << "多项式长度为：";
	cin >> lenth;
	cout << "请输入各项系数：";
	for (int i = 0; i < lenth; i++)//modify
		cin>>new_func.nums[i];
	char ch = cin.get();
	char name_[100];
	if (ch != '\n')
	{
		error = 1;
		cin.getline(name_, 100);
	}
	if (error)
	{
		cout << "输入的系数个数超出所输入多项式的长度" << endl;
		return false;
	}
	string name;
	again:cout << "请输入多项式的名称：";
	cin >> name;
	vector<func>::iterator start = functions.begin(), last = functions.end();
	while (start != last)
	{
		if (start->func_name== name)
		{
			cout << "所输入名称已被占用！" << endl;
			return false;
		}
		start++;
	}
	for (int i = 0; name[i]; i++)
		if (!(('A' <= name[i] && name[i] <= 'Z') || ('a' <= name[i] && name[i] <= 'z')))
		{
			cout << "多项式名称只能含有字母，请重新输入！" << endl;
			goto again;
		}
	new_func.lenth = lenth;
	new_func.func_name= name;
	functions.push_back(new_func);
	return true;
}
bool is_legal(char name[])//判断输入合法性
{
	if (!(('A' <= name[0] && name[0] <= 'Z') || ('a' <= name[0] && name[0] <= 'z') || name[0] == '(' || name[0] == '$'))//开头不合法
		return false;
	int len = strlen(name)-1;
	if (!(('A' <= name[len] && name[len] <= 'Z') || ('a' <= name[len] && name[len] <= 'z') || name[len] == ')' || name[len] == '!'))//结尾不合法
		return false;
	for (int i = 0; name[i]; i++)
	{
		if (('A' <= name[i] && name[i] <= 'Z') || ('a' <= name[i] && name[i] <= 'z'))
		{
			string funct;
			funct = name[i];
			int j;
			for (j = i+1; name[j]; j++)
			{
				if (('A' <= name[j] && name[j] <= 'Z') || ('a' <= name[j] && name[j] <= 'z'))
					funct += name[j];
				else
				{
					i = j-1;
					break;
				}
			}
			if(!name[j])
			i=j-1;
			vector<func>::iterator start = functions.begin(), last = functions.end();
			int no_exi = 1;
			while (start != last)
			{
				if (start->func_name==funct)
				{
					no_exi = 0;
					break;
				}
				start++;
			}
			if (no_exi)
				return false;
			if (name[i+1]!='\0'&&!(name[i+1]=='-'||name[i + 1] == '+' || name[i + 1] == '*' || name[i + 1] == ')' || name[i + 1] == '!'))
				return false;
		}
		else if ((name[i] == '+'||name[i]=='-' ||name[i]=='*')&& name[i + 1] != '\0' && !(('A' <= name[i + 1] && name[i + 1] <= 'Z') || ('a' <= name[i + 1] && name[i + 1] <= 'z') || name[i + 1] == '(' || name[i + 1] == '$'))//check + *
			return false;
		else if (name[i] == '$')//if $
		{
			if (name[i + 1] != '[')
				return false;
			int period = 1, j;
			if (!(name[i + 2] == '-' || name[i + 2] >= '0' && name[i + 2] <= '9'||name[i+2]=='.'))
				return false;
			if (name[i + 2] == '.')
				period = 0;
			for (j = i + 3; name[j]; j++)//first part
			{
				if (name[j] >= '0' && name[j] <= '9')
					continue;
				else if (name[j] == '.' && period)
					period = 0;
				else if (name[j] == '.' && !period)
					return false;
				else break;
			}
			if (name[j] != ',')//comma
				return false;
			period = 1;
			j = j + 1;
			if (!(name[j] == '-' || name[j] >= '0' && name[j] <= '9'||name[j]=='.'))
				return false;
			if (name[j] == '.')
				period = 0;
			for (j = j+1; name[j]; j++)//second part
			{
				if (name[j] >= '0' && name[j] <= '9')
					continue;
				else if (name[j] == '.' && period)
					period = 0;
				else if (name[j] == '.' && !period)
					return false;
				else break;
			}
			if (name[j] != ']')//end
				return false;
			j = j + 1;
			if(name[j]=='(')
				i = j;
			else if (('A' <= name[j] && name[j] <= 'Z') || ('a' <= name[j] && name[j] <= 'z'))//function name input
				{
					i = j - 1;
					continue;
				}
			else return false;
		}
		else if (name[i] == '!' &&name[i+1]!='\0'&& !(name[i + 1] == ')' || name[i+1]=='-'||name[i + 1] == '+' || name[i + 1] == '*'))//check !
			return false;
		else if (name[i] == '(' &&name[i+1]!='\0'&& !(('A' <= name[i + 1] && name[i + 1] <= 'Z') || ('a' <= name[i + 1] && name[i + 1] <= 'z') || name[i + 1] == '(' || name[i + 1] == '$'))//check (
			return false;
		else if (name[i] == ')' &&name[i+1]!='\0'&& !(name[i + 1] == ')' || name[i+1]=='-'||name[i + 1] == '+' || name[i + 1] == '*'||name[i+1]=='!'))//check )
			return false;
	}
	return true;
}
void initialize_func(struct func& result)
{
	for (int i = 0; i < 50; i++)
		result.nums[i] = 0;
	result.lenth = 0;
}
void get_postfix(stack <string> &optstk,vector <string>& calcu,char name[])//转换成后缀表达式
{
	for (int i = 0; name[i]; i++)
	{
		if (('A' <= name[i] && name[i] <= 'Z') || ('a' <= name[i] && name[i] <= 'z'))
		{
			string funcname;
			funcname += name[i];
			int j;
			for (j = i + 1; name[j]; j++)
			{
				if (('A' <= name[j] && name[j] <= 'Z') || ('a' <= name[j] && name[j] <= 'z'))
					funcname += name[j];
				else
					break;
			}
			calcu.push_back(funcname);
			i=j-1;
		}
		else if (name[i] == '+')
		{
			while (1)
			{
				if (!optstk.empty() && (optstk.top() == "!" || optstk.top()[0] == '$' || optstk.top() == "*"|| optstk.top() == "+" || optstk.top() == "-"))
				{
					calcu.push_back(optstk.top());
					optstk.pop();
				}
				else break;
			}
			optstk.push("+");
		}
		else if (name[i] == '-')
		{
			while (1)
			{
				if (!optstk.empty() && (optstk.top() == "!" || optstk.top()[0] == '$' || optstk.top() == "*" || optstk.top() == "+" || optstk.top() == "-"))
				{
					calcu.push_back(optstk.top());
					optstk.pop();
				}
				else break;
			}
			optstk.push("-");
		}
		else if (name[i] == '*')
		{
			while (1)
			{
				if (!optstk.empty() && (optstk.top() == "!" || optstk.top()[0] == '$' || optstk.top()[0] == '*'))
				{
					calcu.push_back(optstk.top());
					optstk.pop();
				}
				else break;
			}
			optstk.push("*");
		}
		else if (name[i] == '$')//if $
		{
			string funcname;
			int j;
			for (j = i; name[j] != ']'; j++)
				funcname += name[j];
			funcname += "]";
			while (1)
			{
				if (!optstk.empty() && (optstk.top() == "!" || optstk.top()[0] == '$'))
				{
					calcu.push_back(optstk.top());
					optstk.pop();
				}
				else break;
			}
			optstk.push(funcname);
			i=j;
		}
		else if (name[i] == '!')//if !
		{
			while (1)
			{
				if (!optstk.empty() &&optstk.top() == "!")
				{
					calcu.push_back(optstk.top());
					optstk.pop();
				}
				else break;
			}
			optstk.push("!");
		}
		else if (name[i] == '(')//if (
			optstk.push("(");
		else if (name[i] == ')')//if )
		{
			while (1)
			{
				if (!optstk.empty() && optstk.top() == "(")
				{
					optstk.pop();
					break;
				}
				else if (!optstk.empty())
				{
					calcu.push_back(optstk.top());
					optstk.pop();
				}
			}
		}
	}
	while (!optstk.empty())
	{
		calcu.push_back(optstk.top());
		optstk.pop();
	}
}
void mass_calculate(char name[])//进行混合运算
{
	stack <string> optstk;
	vector<string>calcu;//存储后缀表达式
	stack<struct func> calcu_list;//存储运算式
	struct func temp;
	temp.func_name = "temp";
	get_postfix(optstk, calcu, name);
	vector<string>::iterator i = calcu.begin();
	for (; i != calcu.end(); i++)
	{
		if (('A' <= (*i)[0] && (*i)[0] <= 'Z') || ('a' <= (*i)[0] && (*i)[0] <= 'z'))
		{
			vector<func>::iterator start = functions.begin(), last = functions.end(),tmp;
			while (start != last)
			{
				if ((string)start->func_name == *i)
					break;
				start++;
			}
			calcu_list.push(*start);
		}
		else if (*i == "+")
		{
			struct func tmp1 = calcu_list.top();
			calcu_list.pop();
			struct func tmp2 = calcu_list.top();
			calcu_list.pop();
			temp = tmp1 + tmp2;
			calcu_list.push(temp);
		}
		else if (*i == "-")
		{
			struct func tmp1 = calcu_list.top();
			calcu_list.pop();
			struct func tmp2 = calcu_list.top();
			calcu_list.pop();
			temp = tmp2-tmp1;
			calcu_list.push(temp);
		}
		else if (*i == "*")
		{
			struct func tmp1 = calcu_list.top();
			calcu_list.pop();
			struct func tmp2 = calcu_list.top();
			calcu_list.pop();
			temp = tmp1 * tmp2;
			calcu_list.push(temp);
		}
		else if (*i == "!")
		{
			struct func tmp1 = calcu_list.top();
			calcu_list.pop();
			temp = derive(tmp1);
			calcu_list.push(temp);
		}
		else if ((*i)[0] == '$')
		{
			struct func tmp1 = calcu_list.top();
			calcu_list.pop();
			temp = integrate(*i,tmp1);
			calcu_list.push(temp);
		}
	}
	struct func result = calcu_list.top();
	result.func_name = "1";
	functions.push_back(result);
	print_func(result.func_name);
	functions.pop_back();
}
double assign_value(struct func name, double m)//计算函数值
{
	double sum = 0;
	for (int i = 0; i < name.lenth; i++)
	{
		double multi = 1;
		for (int j = 0; j < name.lenth - i - 1; j++)
			multi *= m;
		sum += name.nums[i] * multi;
	}
	return sum;
}
struct func integrate(string name,struct func poly)//计算积分
{
	struct func temp;
	temp.func_name = "temp";
	temp.lenth = poly.lenth+1;
	for (int i = 0; i < poly.lenth; i++)
	{
		temp.nums[i] = poly.nums[i] / (1.0*poly.lenth - 1.0*i);
	}
	temp.nums[temp.lenth - 1] = 0;
	name = name.substr(2);
	double a = atof(name.c_str());
	int i;
	for (i = 0; name[i] != ','; i++);
	name = name.substr(i + 1);
	double b = atof(name.c_str());
	double value = assign_value(temp, b) - assign_value(temp, a);
	temp.lenth = 1;
	temp.nums[0] = value;
	return temp;
}
struct func derive(struct func poly)//计算导数
{
	struct func temp;
	temp.func_name = "temp";
	temp.lenth = poly.lenth - 1;
	for (int i = 0; i < temp.lenth; i++)
		temp.nums[i] = poly.nums[i] * (1.0*temp.lenth - 1.0*i);
	return temp;
}
bool get_root()//求根
{
	string name1;
	cout << "请输入多项式名称：";
	cin >> name1;
	vector<func>::iterator start = functions.begin(), last = functions.end(), xk;
	int no_exi = 1;
	while (start != last)
	{
		if (start->func_name == name1)
		{
			no_exi = 0;
			xk = start;
			break;
		}
		start++;
	}
	if (no_exi)
	{
		cout << "所输入多项式不存在！" << endl;
		return false;
	}
	struct func temp_f = *xk;
	if(fabs(assign_value(temp_f,0))<1e-5)
	{
		cout << name1<<"=0的根为：0"<< endl;
		return true;
	}
	struct func temp_fd = derive(temp_f);
	double x0 = 1,x1;
	while (fabs(assign_value(temp_f, x1)) > 1e-5)
	{
		while(fabs(assign_value(temp_f,x0))<1e-5)
		{
			if(x0>0)
				x0=-x0;
			else
				x0=-x0+1;
		}
		x1 = x0 - (assign_value(temp_f, x0) / assign_value(temp_fd, x0));
		while (fabs(assign_value(temp_f, x1)) > 1e-5)
		{
			x0 = x1;
			if(fabs(assign_value(temp_f,x0))<1e-5)
			break;
			x1 = x0 - (assign_value(temp_f, x0) / assign_value(temp_fd, x0));
		}
		if(x0>0)
			x0=-x0;
		else
			x0=-x0+1;
		
	}
	cout << name1<<"=0的根为：" << x1 << endl;
	return true;
}
bool get_reverse(string name1,struct func &result)//求逆，分治FFT
{
	vector<func>::iterator start = functions.begin(), last = functions.end(), xk;
	int no_exi = 1;
	while (start != last)
	{
		if (start->func_name == name1)
		{
			no_exi = 0;
			xk = start;
			break;
		}
		start++;
	}
	if (no_exi)
	{
		cout << "多项式不存在！" << endl;
		return false;
	}
	struct func temp = *xk;
	if (fabs(temp.nums[temp.lenth - 1]) < 1e-5)
	{
		cout << "多项式不存在逆元！" << endl;
		return false;
	}
	initialize_func(result);
	result.func_name = name1;
	result.func_name += "^(-1)";
	result.lenth = temp.lenth;
	double temp_num[100] = { 0 }, result_num[100] = { 0 };
	for (int i = 0; i < temp.lenth; i++)
		temp_num[i] = temp.nums[temp.lenth - i - 1];
	result_num[0] = 1 / temp_num[0];
	for (int i = 1; i < temp.lenth; i++)//求逆
	{
		for (int j = 0; j < i; j++)
			result_num[i] += result_num[j] * (-temp_num[i - j] / temp_num[0]);
	}
	for (int i = 0; i < temp.lenth; i++)
		result.nums[i] = result_num[temp.lenth - i - 1];
	return true;
}
bool divide()//多项式除法
{
	string name1,name2;
	cout << "请依次输入被除数和除数（用空格隔开）：";
	cin >> name1 >> name2;
	vector<func>::iterator start = functions.begin(), last = functions.end(),x1,x2;
	int f1 = 1, f2 = 1;
	while (start != last)
	{
		if (start->func_name == name1)
		{
			x1 = start;
			f1 = 0;
		}
		if (start->func_name == name2)
		{
			x2 = start;
			f2 = 0;
		}
		if (f1==0&&f2==0)
			break;
		start++;
	}
	if (f1 || f2)
	{
		cout << "所输入多项式不存在！";
		return false;
	}
	struct func f = *x1, g = *x2, new_q, d,gr,q;
	initialize_func(d);
	initialize_func(new_q);
	initialize_func(gr);
	initialize_func(q);
	struct func inif = f, inig = g;//存储f g初始值
	if (g > f)
		{
			cout << f.func_name << "/" << g.func_name << "的商为:0" << endl << "余数为：";
			functions.push_back(f);
			print_func(f.func_name);
			functions.pop_back();
			return true;
		}
	int n = f.lenth - 1, m = g.lenth - 1;
	if(g.lenth<n-m+1)
	{
		for(int i=g.lenth;i<n-m+1;i++)
			g.nums[i]=0;
	}
	g.lenth = n - m + 1;//g mode
	gr.lenth = g.lenth;
	gr.nums[0] = 1 / g.nums[0];
	for (int k = 1; k < n - m + 1; ++k)
 {
		double para = 0;
		for (int i = 0; i < k; ++i)
			para += gr.nums[i] * (-g.nums[k - i] / g.nums[0]);
		gr.nums[k] = para;
	}
	q = f * gr;
	q.lenth = n - m + 1;
	struct func q_result,q_inig,r_result;
	initialize_func(q_result);
	initialize_func(q_inig);
	initialize_func(r_result);
	q_result = q;
	q_inig = q * inig;
	vector<double> R;
	for (int i = 0; i <q_inig.lenth; i++) 
		q_inig.nums[i] *= -1;
	for (int i = 0; i < f.lenth - q_inig.lenth; i++) //F-Q*G
		R.push_back(f.nums[i]);
	for (int i = f.lenth - q_inig.lenth; i < f.lenth; i++)
		R.push_back(f.nums[i] + q_inig.nums[i - f.lenth + q_inig.lenth]);
	r_result.lenth = R.size();
	r_result.func_name = "-1";
	q.func_name = "-2";
	vector<double>::iterator start1 = R.begin(), last1 = R.end();
	int i = 0;
	while (start1 != last1)
	{
		r_result.nums[i] = *start1;
		i++;
		start1++;
	}
	cout << inif.func_name << "/" << inig.func_name << "的商为：";
	functions.push_back(q);
	print_func(q.func_name);
	functions.pop_back();
	cout << "余数为：";
	functions.push_back(r_result);
	print_func(r_result.func_name);
	functions.pop_back();
	return true;
}


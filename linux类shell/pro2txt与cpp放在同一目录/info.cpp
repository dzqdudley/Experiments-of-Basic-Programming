#include "head.h"
int line_num_all=0,w_count_all=0;
long int lenth_all=0;
bool count_all()
{
line_num_all=0,w_count_all=0,lenth_all=0;
char* file1;
char filesum[2560]={'\0'};
char filepre[2560]={'\0'};
cin.getline(filesum,2560);
strcpy(filepre,filesum);//preserve the initial value of filesum
file1=strtok(filesum," ");
int f1=0,f2=0,f3=0;
while(file1!=NULL)
{
if(!strncmp(file1,"-",1))
	{
	if(!strcmp(file1,"-l"))
	f1=1;
	else if(!strcmp(file1,"-w"))
	f2=1;
	else if(!strcmp(file1,"-c"))
	f3=1;
	else
		{
		cout<<"wc: unrecognized option '"<<file1<<"'"<<endl;
		return false;
		}
	}
file1=strtok(NULL," ");
}
print_wc(filepre,f1,f2,f3);
}

bool print_wc(char filepre[],int f1,int f2,int f3)
{
struct stat statbuf;
char *file;
int flag=0,count=0;
file=strtok(filepre," ");
while(file!=NULL)
{
if(!strcmp(file,"-l")||!strcmp(file,"-w")||!strcmp(file,"-c"))
{
file=strtok(NULL," ");
continue;
}
flag=1;
count++;
char file1[256]={'\0'};
strcpy(file1,file);
to_absolute(file1);
lstat(file1,&statbuf);
if(S_ISDIR(statbuf.st_mode))
{
cout<<"wc: "<<file<<": Is a directory"<<endl;
if(!f1&&!f2&&!f3||f1&&f2&&f3)
cout<<0<<"\t"<<0<<"\t"<<0<<"\t"<<file<<endl;
else if(f1&&f2||f1&&f3||f2&&f3)
cout<<0<<"\t"<<0<<"\t"<<file<<endl;
else if(f1||f2||f3)
cout<<0<<"\t"<<file<<endl;
flag=0;
}
else if(access(file1,00)<0)//if file does not exist
{
cout<<"wc: "<<file<<": No such file or directory"<<endl;
flag=0;
}
else if(access(file1,04)<0)// if user has no access to file
{
cout<<"wc: "<<file<<": Permission denied"<<endl;
flag=0;
}
else 
{
FILE *fp=fopen(file1,"r");
if(fp==0)
{
cout<<"wc: "<<file<<": No such file or directory"<<endl;
flag=0;
}
else fclose(fp);
}
if(!f1&&!f2&&!f3&flag)
{
count_line(file1);
count_word(file1);
count_byte(file1);
cout<<file<<endl;
file=strtok(NULL," ");
continue;
}
if(f1&&flag)
count_line(file1);
if(f2&&flag)
count_word(file1);
if(f3&&flag)
count_byte(file1);
if(flag)
cout<<file<<endl;
file=strtok(NULL," ");
}
if(count<=1)
return true;
if(!f1&&!f2&&!f3||f1&&f2&&f3)
cout<<line_num_all<<"\t"<<w_count_all<<"\t"<<lenth_all<<"\t";
else if(f1&&f2)
cout<<line_num_all<<"\t"<<w_count_all<<"\t";
else if(f1&&f3)
cout<<line_num_all<<"\t"<<lenth_all<<"\t";
else if(f2&&f3)
cout<<w_count_all<<"\t"<<lenth_all<<"\t";
else if(f1)
cout<<line_num_all<<"\t";
else if(f2)
cout<<w_count_all<<"\t";
else if(f3)
cout<<lenth_all<<"\t";
cout<<"total"<<endl;
}
bool count_byte(char file1[ ])
{
FILE *fp=fopen(file1,"r");
if(fp==0)
{
cout<<"wc: "<<file1<<": No such file or directory"<<endl;
return false;
}
char ch=fgetc(fp);
if(ch==EOF)//if file is empty
{
cout<<0<<" ";
return true;
}
fseek(fp,0,SEEK_END);
long int lenth=ftell(fp);
lenth_all+=lenth;
cout<<lenth<<"\t";
fclose(fp);
return true;
}

bool count_word(char file1[ ])
{
FILE *fp=fopen(file1,"r");
if(fp==0)
{
cout<<"wc: "<<file1<<": No such file or directory"<<endl;
return false;
}
char ch=fgetc(fp);
if(ch==EOF)//if file is empty
{
cout<<0<<" ";
return true;
}
fseek(fp,0,SEEK_SET);
int w_count=0;
char *tmp=new char [100];
fscanf(fp,"%s",tmp);
while(!feof(fp))
{
int i;
for(i=0;tmp[i];i++);
if(i<99)//check if tmp is full
w_count++;
fscanf(fp,"%s",tmp);
}
w_count_all+=w_count;
cout<<w_count<<"\t";
fclose(fp);
delete[] tmp;
return true;
}

bool count_line(char file1[ ])
{
FILE *fp=fopen(file1,"r");
if(fp==0)
{
cout<<"wc: "<<file1<<": No such file or directory"<<endl;
return false;
}
char ch=fgetc(fp);
if(ch==EOF)//if file is empty
{
cout<<0<<" ";
return true;
}
fseek(fp,0,SEEK_END);
long int lenth=ftell(fp);
int line_num=0;
fseek(fp,0,SEEK_SET);
char *tmp=new char [lenth+1];
fgets(tmp,lenth+1,fp);
while(!feof(fp))
{
fgets(tmp,lenth+1,fp);
line_num++;
}
line_num_all+=line_num;
cout<<line_num<<"\t";
fclose(fp);
delete[] tmp;
return true;
}

bool view_file(char file1[])
{
to_absolute(file1);
struct stat statbuf;
lstat(file1,&statbuf);
if(S_ISDIR(statbuf.st_mode))
{
cout<<"cat: "<<file1<<": Is a directory"<<endl;
return false;
}
if(access(file1,00)<0)//if file1 does not exist
{
cout<<"cat: "<<file1<<"': No such file or directory"<<endl;
return false;
}
if(access(file1,04)<0)// if user has no access to file1
{
cout<<"cat: '"<<file1<<"': Permission denied"<<endl;
return false;
}
FILE *fp=fopen(file1,"r");
if(fp==0)
{
cout<<"cat: "<<file1<<": No such file or directory"<<endl;
return false;
}
char ch=fgetc(fp);
if(ch==EOF)//if file is empty
return true;
fseek(fp,0,SEEK_END);
long int lenth=ftell(fp);
fseek(fp,0,SEEK_SET);
char *tmp=new char [lenth+1];
fgets(tmp,lenth+1,fp);
while(!feof(fp))
{
cout<<tmp;
fgets(tmp,lenth+1,fp);
}
fclose(fp);
delete[] tmp;
return true;
}

bool compare(char file1[],char file2[])
{
to_absolute(file1);
to_absolute(file2);//shift to absolute path
if(!strcmp(file1,file2))//if two files are identical
return true;
struct stat statbuf;
lstat(file1,&statbuf);
if(S_ISDIR(statbuf.st_mode))//if file1 is a folder
{
cout<<"cmp: "<<file1<<": Is a directory"<<endl;
return false;
}
lstat(file2,&statbuf);
if(S_ISDIR(statbuf.st_mode))//if file2 is a folder
{
cout<<"cmp: "<<file2<<": Is a directory"<<endl;
return false;
}
if(access(file1,00)<0)//if file1 does not exist
{
cout<<"cmp: "<<file1<<"': No such file or directory"<<endl;
return false;
}
if(access(file1,04)<0)// if user has no access to file1
{
cout<<"cmp: '"<<file1<<"': Permission denied"<<endl;
return false;
}
if(access(file2,00)<0)//if file1 does not exist
{
cout<<"cmp: "<<file2<<"': No such file or directory"<<endl;
return false;
}
if(access(file2,04)<0)// if user has no access to file2
{
cout<<"cmp: '"<<file2<<"': Permission denied"<<endl;
return false;
}
FILE *fp=fopen(file1,"r");
if(fp==0)
{
cout<<"cmp: "<<file1<<": No such file or directory"<<endl;
return false;
}
FILE *fpp=fopen(file2,"r");
if(fpp==0)
{
cout<<"cmp: "<<file2<<": No such file or directory"<<endl;
return false;
}
fseek(fp,0,SEEK_END);
long int lenth1=ftell(fp);
fseek(fp,0,SEEK_SET);
fseek(fpp,0,SEEK_END);
long int lenth2=ftell(fpp);
fseek(fpp,0,SEEK_SET);
int temp1,temp2;
temp1=fgetc(fp);
temp2=fgetc(fpp);
int i=1,j=1;
if(feof(fp))//if file1 is empty
{
cout<<"cmp: "<<"EOF on "<<file2<<endl;
return true;
}
if(feof(fpp))//if file2 is empty
{
cout<<"cmp: "<<"EOF on "<<file1<<endl;
return true;
}
while(!feof(fp)&&!feof(fpp))
{
if(temp1!=temp2)
{
cout<<file1<<" "<<file2<<" "<<"differ: "<<"byte "<<i<<", "<<"line "<<j<<endl;
return true;
}
if(temp1=='\n')
j++;
if(feof(fp)||feof(fpp))
break;
temp1=fgetc(fp);
temp2=fgetc(fpp);
i++;
}
fclose(fp);
fclose(fpp);
if(lenth1==lenth2)
return true;
if(lenth1<lenth2&&temp1==-1)//if file1 is onto file2
cout<<"cmp: "<<"EOF on "<<file2<<endl;
else if(lenth2<lenth1&&temp2==-1)//if file2 is onto file1
cout<<"cmp: "<<"EOF on "<<file1<<endl;
else 
cout<<file1<<" "<<file2<<" "<<"differ: "<<"byte "<<i+1<<", "<<"line "<<j<<endl;
return true;
}

bool view_cmd_info(char cmd[])
{
char cmdd[256]={'\0'};
strcpy(cmdd,cmd);//preserve the initial value of cmd
strcat(cmd,".txt");
FILE *fp=fopen(cmd,"r");
if(fp==NULL)
{
cout<<"No manual entry for "<<cmdd<<endl;
return false;
}
cout<<"\033c";// clear the screen
view_file(cmd);
char p;
while(cin>>p)
if(p=='q')
break;//press q to quit
cout<<"\033c";// clear the screen
return true;
}

bool run_sh(char file[])
{
struct stat statbuf;
char file1[256]={'\0'};
strcpy(file1,file);//preserve the initial value of file
to_absolute(file);
lstat(file,&statbuf);
if(access(file,00)<0)//if file does not exist
{
cout<<"sh: 0:"<<" Can't open "<<file1<<endl;
return false;
}
else if(S_ISDIR(statbuf.st_mode))
{
cout<<"sh: 0: "<<file1<<" :Is a directory"<<endl;
return false;
}
else if(access(file,04)<0)// if user has no access to file
{
cout<<"sh: 0:"<<" Can't open "<<file1<<endl;
return false;
}
FILE *fp=fopen(file,"r");
if(fp==0)
{
cout<<"sh: 0: Can't open "<<file1<<endl;
return false;
}
else fclose(fp);
fstream fpp;
fpp.open(file,ios::in);
streambuf *backup=cin.rdbuf();//back up cin's streambuf
streambuf *x=fpp.rdbuf();//get file's streambuf
cin.rdbuf(x);//assign streambuf to cin
func_loop(0);
fpp.close();
cin.rdbuf(backup);//restore cin's original streambuf
return true;
}
void to_absolute(char file[])//change file path to absolute path
{
char filename[256]={'\0'},curdir[256]={'\0'};
getcwd(filename,256);
strcpy(curdir,filename);//preserve the current working directory
struct passwd* pwd;
pwd=getpwuid(getuid());
chdir("..");
char sysdir[256]={'\0'};
getcwd(sysdir,256);//get the directory of '../'
chdir(filename);
if(!strncmp(file,"./",2))
{
strcpy(file,strchr(file,'/'));
strcat(filename,file);
strcpy(file,filename);
return;
}
else if(!strncmp(file,"../",3))
{
strcpy(file,strchr(file,'/'));
strcat(sysdir,file);
strcpy(file,sysdir);
return;
}
else if(!strncmp(file,"~/",2))
{
strcpy(sysdir,pwd->pw_dir);//get the home directory
strcpy(file,strtok(file,"~"));
strcat(sysdir,file);
strcpy(file,sysdir);
return;
}
else if(strncmp(file,"/",1)!=0)
{
strcat(curdir,"/");
strcat(curdir,file);
strcpy(file,curdir);
}
return;
}
void get_head(char head[])//get the system heading
{
char host[50]={'\0'},cwd[100]={'\0'};
struct passwd* pwd;
pwd=getpwuid(getuid());
gethostname(host,50);
getcwd(cwd,100);
if(!strcmp(strtok(cwd,"/"),"home")&&!strcmp(strtok(NULL,"/"),pwd->pw_name))
{
char temp[100]="~/";
strcat(temp,strtok(NULL," "));
strcpy(cwd,temp);
}
sprintf(head,"\033[1;32m%s@%s\033[0m:\033[1;34m%s\033[0m$ ",pwd->pw_name,host,cwd);
}

void func_loop(int flag)
{
char head[256]={'\0'};
get_head(head);//get the system header
char cwd[256]={'\0'};
getcwd(cwd,256);
while(1)
{
char file1[256]={'\0'},file2[256]={'\0'},i[256]={'\0'};
if(flag)
cout<<head;
cin>>i;//get the first command
if(!strcmp(i,"wc"))
count_all();
else if(!strcmp(i,"cat"))
{
if(cin.get()=='\n')
cout<<"cat: missing file operand"<<endl;
else {cin>>file1;
view_file(file1);
}
}
else if(!strcmp(i,"man"))
{
if(cin.get()=='\n')//no command operand after man
cout<<"What manual page do you want?"<<endl;
else{
cin>>file1;
view_cmd_info(file1);
}
}
else if(!strcmp(i,"cp"))
{
if(cin.get()=='\n')
cout<<"cp: missing file operand"<<endl;
else{copy();
chdir(cwd);//shift back to the current working directory
}
}
else if(!strcmp(i,"cmp"))
{
if(cin.get()=='\n')
cout<<"cmp: missing file operand after 'cmp'"<<endl;
else
{
cin>>file1>>file2;
compare(file1,file2);
}
}
else if(!strcmp(i,"sh"))
{
if(cin.get()=='\n')
cout<<"sh: missing file operand"<<endl;
else {cin>>file1;
run_sh(file1);
}
}
else if(!strcmp(i,"rm"))
{
cin>>file1;
char ch=cin.get();
if(ch=='\n')
remove_file(file1);
else{
cin>>file2;
if(!strcmp(file1,"-rf"))
remove_dir(file2);
else if(!strcmp(file2,"-rf"))
remove_dir(file1);
else cout<<"rm: invalid option"<<endl; 
}
}
else if(!strcmp(i,"mkdir"))
create_dir();
else if(!strcmp(i,"pwd"))
current_wd();
else if(!strcmp(i,"cd"))
change_dir();
else if(!strcmp(i,"exit")||!strcmp(i,"\000"))
break;
else //invalid command input
{
char ch=cin.get();
while(ch!='\n')
{
cin>>file1;
ch=cin.get();
}
cout<<i<<": command not found"<<endl;
}
}
}
void current_wd()
{
char dir[256]={'\0'};
getcwd(dir,256);
cout<<dir<<endl;
}

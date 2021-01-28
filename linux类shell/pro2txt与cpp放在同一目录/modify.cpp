#include "head.h"
int flag;
char olddir[256]={'\0'};
void copy()
{
flag=0;
int flag=0,extra=0,error=0;
char file1[256]={'\0'};
char file2[256]={'\0'};
char file[256]={'\0'};
while(cin>>file)//enable correct function with random input of '-r'
{
if(!strncmp(file,"-r",2))
{
int i;
for(i=1;file[i];i++)
if(file[i]!='r')
{cout<<"cp: invalid option -- '"<<file[i]<<"'"<<endl;
error=1;
}//enable correct function with input '-rrrrrrr'
else flag=1;// check copy directories or single files
}
else if(!strcmp(file1,"\0"))
strcpy(file1,file);
else if(!strcmp(file2,"\0"))
strcpy(file2,file);
else extra=1;
if(cin.get()=='\n')
break;
}
if(error)
return;
else if(!strcmp(file1,"\0")&&!strcmp(file2,"\0"))
cout<<"missing file operand"<<endl;
else if(strcmp(file1,"\0")!=0&&!strcmp(file2,"\0"))
cout<<"cp: missing destination file operand after '"<<file1<<"'"<<endl;
else if(extra)//if more than 2 files have been input
cout<<"cp: cannot stat '"<<file<<"': No such file or directory"<<endl;
else if(!strcmp(file2,"\0"))
cout<<"cp: missing destination file operand after '"<<file1<<"'"<<endl;
else if(flag==0)
cpy_single(file1,file2);
else if(flag)
cpy_dir(file1,file2);
}
bool cpy_single(char file1[],char file2[])
{
char file1x[256]={'\0'},file2x[256]={'\0'};
strcpy(file1x,file1);
strcpy(file2x,file2);//preserve the innitial value of file1, file2
to_absolute(file1);
to_absolute(file2);//shift to absolute path
if(access(file1,00)<0)//if file1 does not exist
{
cout<<"cp: cannot stat '"<<file1x<<"': No such file or directory"<<endl;
return false;
}
if(access(file1,04)<0)// if user has no access to file1
{
cout<<"cp: cannot open '"<<file1x<<"' for reading: Permission denied"<<endl;
return false;
}
struct stat statbuf1,statbuf2;
lstat(file1,&statbuf1);// initialize info of file1
if(S_ISDIR(statbuf1.st_mode))//if file1 is a directory
{
cout<<"cp: omitting directory '"<<file1x<<"'"<<endl;
return false;
}
if(!strcmp(file1,file2))
{
cout<<"cp: '"<<file1x<<"' and '"<<file2x<<"' are the same file"<<endl;
return false;
}
lstat(file2,&statbuf2);// initialize info of file2
if(S_ISDIR(statbuf2.st_mode)&&(access(file2,00)>-1))//if file2 is an existing directory
{
int flax=0;
if(access(file2,02)<0||access(file2,01))//cannot write into file2
flax=1;
strcat(file2,"/");
int j=0;
for(int i=0;file1[i];i++)
if(file1[i]=='/')
j=i;
char temp[1000]={'\0'};
for(int i=j+1;file1[i];i++)
temp[i-j-1]=file1[i];
strcat(file2,temp);//move the destination path into the file2 directory
if(flax)
{
cout<<"cp: cannot create regular file '"<<file2<<"': Permission denied"<<endl;
return false;
}
}
FILE *fp=fopen(file1,"r");
umask(0000);
int fpp=open(file2,O_RDWR|O_CREAT,0777);
if(fpp<0)
{
cout<<"cp: cannot create regular file '"<<file2<<"': Permission denied"<<endl;
return false;
}
int temp1;
temp1=fgetc(fp);
while(!feof(fp))//read the file byte by byte
{
write(fpp,&temp1,1);
temp1=fgetc(fp);
}
fclose(fp);
close(fpp);
chmod(file2,statbuf1.st_mode&ALLPERMS);
return true;
}
void cpy_dir(char dir1[],char dir2[])
{
flag++;
char dir1x[256]={'\0'},dir2x[256]={'\0'};
strcpy(dir1x,dir1);
strcpy(dir2x,dir2);//preserve the innitial value of dir1, dir2
to_absolute(dir1);
to_absolute(dir2);//shift to absolute path
if(!strcmp(dir1,dir2))//cannot copy one directory into itself
{
cout<<"cp: cannot copy a directory, '"<<dir1x<<"', into itself, '"<<dir1x<<"'"<<endl;
return;
}
struct stat statbuf,statbuf1,statbuf2;
lstat(dir1,&statbuf1);// initialize info of dir1
if(!S_ISDIR(statbuf1.st_mode)&&S_ISREG(statbuf1.st_mode))//if dir1 is not a dir but a file
{
cpy_single(dir1,dir2);
return;
}
if(access(dir1,00)<0)// if dir1 does not exist
{
cout<<"cp: cannot stat '"<<dir1x<<"': No such file or directory"<<endl;
return;
}
int non_dir1=0;
if(access(dir1,04)<0||access(dir1,01)<0)//if user has no access to dir1
{
cout<<"cp: cannot access '"<<dir1x<<"': Permission denied"<<endl;
non_dir1=1;//sign to create empty folder in dir2
}
if(lstat(dir2,&statbuf2)>-1&&S_ISREG(statbuf2.st_mode))//if dir2 is not a dir but a file
{
cout<<"cp: cannot overwrite non-directory '"<<dir2x<<"' with directory '"<<dir1x<<"'"<<endl;
return;
}
struct dirent *entry;
if((access(dir2,00)>-1)&&flag==1)//check if the destination directory already exists
{
int flax=0;
if(access(dir2,02)<0||access(dir2,01)<0)//if cannot write into dir2
flax=1;
strcat(dir2,"/");
int j=0;
for(int i=0;dir1[i];i++)
if(dir1[i]=='/')
j=i;
char temp[256]={'\0'};
for(int i=j+1;dir1[i];i++)
temp[i-j-1]=dir1[i];
strcat(dir2,temp);//move the destination path into the exisiting directory
if(flax)
{
cout<<"cp: cannot create directory '"<<dir2<<"': Permission denied"<<endl;
return;
}
cpy_dir(dir1,dir2);//copy the target directory into the existing directory
return;
}
else if(access(dir2,00)<0)
mkdir(dir2,0777);//create the destination directory 
lstat(dir2,&statbuf2);// initialize info of dir2
if(!non_dir1)
{
DIR *dp=opendir(dir1);//open dir1
chdir(dir1);
while((entry=readdir(dp))!=NULL)//go through every file & directory in dir1
{
lstat(entry->d_name,&statbuf);// initialize info of entry->d_name
if(S_ISDIR(statbuf.st_mode))//check if dir1 is a directory
{
if(strcmp(entry->d_name,".")==0||
strcmp(entry->d_name,"..")==0)//check if dir1 is the current or previous directory
continue;
chdir(dir2);
mkdir(entry->d_name,0777);//create new directory in dir2
char buffer1[256]={'\0'};
char buffer2[256]={'\0'};
strcat(buffer1,dir1);
strcat(buffer2,dir2);
strcat(buffer2,"/");
strcat(buffer2,entry->d_name);
chdir(dir1);
strcat(buffer1,"/");
strcat(buffer1,entry->d_name);
if((access(buffer1,04))<0||access(buffer1,01)<0)//if user has no access to buffer1
{
lstat(buffer1,&statbuf);
chmod(buffer2,statbuf.st_mode&ALLPERMS);
cout<<"cp: cannot access '"<<buffer1<<"': Permission denied"<<endl;
continue;
}
cpy_dir(buffer1,buffer2);//copy files & directories from dir1 to dir2
lstat(buffer1,&statbuf);
chmod(buffer2,statbuf.st_mode&ALLPERMS);
}
else
{
char name1[1000]={'\0'},name2[1000]={'\0'};
strcat(name1,dir1);
strcat(name1,"/");
strcat(name1,entry->d_name);
strcat(name2,dir2);
strcat(name2,"/");
strcat(name2,entry->d_name);//get the absolute paths of two files
cpy_single(name1,name2);//copy the single file
}
}
chmod(dir2,statbuf1.st_mode&ALLPERMS);
closedir(dp);
}
else
chmod(dir2,statbuf1.st_mode&ALLPERMS);
chdir("..");//reset the working directory
}

void remove_dir(char file[])
{
struct stat statbuf;
char file1[256]={'\0'};
strcpy(file1,file);//preserve the initial value of file
to_absolute(file);
lstat(file,&statbuf);
if(access(file,00)<0)//if file does not exist
{
cout<<"rm: cannot remove '"<<file1<<"': No such file or directory"<<endl;
return;
}
if(S_ISREG(statbuf.st_mode))
{
remove(file);
return;
}
if(S_ISDIR(statbuf.st_mode))
{
if(access(file,01)<0||access(file,02)<0||access(file,04)<0)// if user has no access to file
{
cout<<"rm: cannot remove '"<<file1<<"': Permission denied"<<endl;
return;
}
DIR *dir=opendir(file);
struct dirent *dp;
while((dp=readdir(dir))!=NULL)
{
if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..")==0)
continue;
char newdir[256]={'\0'};
strcpy(newdir,file);
strcat(newdir,"/");
strcat(newdir,dp->d_name);
remove_dir(newdir);
}
closedir(dir);
rmdir(file);
}
}
void remove_file(char file[])
{
struct stat statbuf;
char file1[256]={'\0'};
strcpy(file1,file);//preserve the initial value of file
to_absolute(file);
lstat(file,&statbuf);
if(access(file,00)<0)//if file does not exist
{
cout<<"rm: cannot remove '"<<file1<<"': No such file or directory"<<endl;
return;
}
if(S_ISDIR(statbuf.st_mode))
{
cout<<"rm: cannot remove '"<<file1<<"': Is a directory"<<endl;
return;
}
if(S_ISREG(statbuf.st_mode))
{
if(access(file,02)<0)// if user has no access to file
{
cout<<"rm: remove write-protected regular file '"<<file1<<"'?";
char in[30]={'\0'};
cin>>in;
if(!strcmp(in,"y")||!strcmp(in,"Y")||!strcmp(in,"yes"))
remove(file);
return;
}
else remove(file);
return;
}
}
void create_dir()
{
int mode;
char* file1,perm[256]={'\0'};
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
	if(!strcmp(file1,"-m"))
	f1=1;
	else if(!strcmp(file1,"-p"))
	f2=1;
	else
		{
		cout<<"wc: unrecognized option '"<<file1<<"'"<<endl;
		return;
		}
	}

if(file1[0]>='0'&&file1[0]<='9'&&!strcmp(perm,"\0"))//try to find the right mode
{
int pp=1;
for(int i=0;file1[i];i++)
if(file1[i]>'7')
pp=0;
if(pp){
strcat(perm,file1);
mode=(file1[0]-'0')*8*8+(file1[1]-'0')*8+(file1[2]-'0');//shift mode to octal int
}
}
file1=strtok(NULL," ");
}
struct stat statbuf;
char *file;
int flag=0;
file=strtok(filepre," ");
while(file!=NULL)
{
if(!strcmp(file,"-m")||!strcmp(file,"-p"))
{
file=strtok(NULL," ");
continue;
}
char file2[256]={'\0'};
strcpy(file2,file);
to_absolute(file2);
if(access(file2,00)>-1)
{
cout<<"mkdir: cannot create directory ‘"<<file<<"’: File exists"<<endl;
return;
}
if(f1&&!strcmp(perm,"\0"))
{
cout<<"mkdir: missing operand"<<endl;
return;
}
if(!f1&&!f2){
if(mkdir(file2,0775)<0)
cout<<"mkdir: cannot create directory ‘"<<file2<<"’"<<endl;
}
else if(f1&&!f2){
if(mkdir(file2,mode)<0)
cout<<"mkdir: cannot create directory ‘"<<file2<<"’"<<endl;
}
else if(f2&&!f1)
recur_mkdir(file2,0775);
else if(f1&&f2)
recur_mkdir(file2,mode);
file=strtok(NULL," ");
}
}
void recur_mkdir(char file[],int perm)
{
for(int i=0;file[i];i++)
{
if(file[i]=='/')
{
file[i]='\0';
if(access(file,00)<0)
{
if(mkdir(file,0775)<0)
cout<<"mkdir: cannot create directory ‘"<<file<<"’"<<endl;
}
file[i]='/';
}
}
if(strlen(file)>0&&access(file,0)<0){
if(mkdir(file,perm)<0)
cout<<"mkdir: cannot create directory ‘"<<file<<"’"<<endl;
}
}

void change_dir()
{
struct passwd* pwd;
pwd=getpwuid(getuid());
char sysdir[256]={'\0'},cur_dir[256]={'\0'};
getcwd(cur_dir,256);//preserve the current working directory
strcpy(sysdir,pwd->pw_dir);
char ch=cin.get();
if(ch=='\n')// if cd
{
strcpy(olddir,sysdir);
chdir(sysdir);
return;
}
char cmd[256]={'\0'};
cin>>cmd;
if(!strcmp(cmd,"."))
return;
else if(!strcmp(cmd,"~"))
{
strcpy(olddir,sysdir);
chdir(sysdir);
return;
}
else if(!strcmp(cmd,"-"))
{
	if(!strcmp(olddir,"\0"))//there is no previous directory
	{
	cout<<"bash: cd: OLDPWD not set"<<endl;
	return;
	}
	if(chdir(olddir)<0)
	{
	cout<<"bash: cd: fail to change the working directory"<<endl;
	return;
	}
	else return;
}
else if(!strcmp(cmd,"/"))// goto the root directory
{
	if(chdir("/")<0)
	{
	cout<<"bash: cd: fail to change the working directory"<<endl;
	return;
	}
	else return;
}
else if(!strcmp(cmd,".."))// goto the previous directory
{
int num1=strlen(cur_dir);
int num2=strlen(strrchr(cur_dir,'/'));
char newdir[256]={'\0'};
strncpy(newdir,cur_dir,num1-num2);
if(chdir(newdir)<0)
{
cout<<"bash: cd: fail to change the working directory"<<endl;
return;
}
else return;
}
else if(!strcmp(cmd,"../.."))// goto the previous previous directory
{
char newdir[256]={'\0'},newdir1[256]={'\0'};
int num1=strlen(cur_dir);
int num2=strlen(strrchr(cur_dir,'/'));
strncpy(newdir,cur_dir,num1-num2);
num1=strlen(newdir);
if(num1==0)
{
cout<<"bash: cd: fail to change the working directory"<<endl;
return;
}
num2=strlen(strrchr(newdir,'/'));
strncpy(newdir1,newdir,num1-num2);
if(chdir(newdir1)<0)
{
cout<<"bash: cd: fail to change the working directory"<<endl;
return;
}
else return;
}
else
{
to_absolute(cmd);
if(chdir(cmd)<0)
{
cout<<"bash: cd: fail to change the working directory"<<endl;
return;
}
else return;
}
}

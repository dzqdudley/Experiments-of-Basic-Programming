#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<vector>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include<cmath>
#include<pwd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
using namespace std;
extern bool compare(char file1[], char file2[]);
extern bool count_byte(char file1[]);
extern bool count_word(char file1[]);
extern bool count_line(char file1[]);
extern bool view_file(char file1[]);
extern bool view_cmd_info(char file1[]);
extern bool cpy_single(char file1[],char file2[]);
extern void cpy_dir(char file1[],char file2[]);
extern bool count_all();
extern void copy();
extern void get_head(char[]);
extern bool run_sh(char file[]);
extern void to_absolute(char file[]);
extern bool print_wc(char filepre[],int f1,int f2,int f3);
extern void func_loop(int flag);
extern void remove_dir(char file[]);
extern void remove_file(char file[]);
extern void create_dir();
extern void recur_mkdir(char file[],int perm);
extern void current_wd();
extern void change_dir();

#include "MyPlayer.h"
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
//�������ʵ�ֿ�ʼ
using namespace QuoridorUtils;
struct node
{
    int x, y;//����
    node* next;//���ڻ��ݵ�һ��λ��
};
node queue[1000];//�ṹ����queue
int targetY = 0,head=0, end=0, num = 0,my_remain=10, ene_remain=10;
std::vector<QuoridorUtils::BlockBar> blocks;
Location seek_path(const QuoridorUtils::Location myLoc, int& min_step,int target,Location exclude);
bool set_block(QuoridorUtils::BlockBar new_blo);//�ж���new_blo���õ����Ƿ�Ϸ�
void evaluate(const ChessboardChange& newChange, Location& next_loc,
    QuoridorUtils::BlockBar& next_bar, double &mark_bar, double &mark_step);//��������
void  possible_way(const QuoridorUtils::Location& myLoc, bool directions[]);//Ѱ�Ҹ�λ�ô��ܹ����ߵķ���
bool is_dirty(const ChessboardChange& newChange, BlockBar& anti_dirty_bar);//����ǲ�������·
int rule_break = 0, stepcount = 0, win_status = 0, roundcount = 1,anti_dirty=1;
namespace QuoridorUtils
{
    Step MyPlayer::nextStep(const ChessboardChange& newChange)
    {
        if (targetY == 0) {                                // �жϲ���¼�Լ���Ŀ��
           
            if (newChange.myLoc == PLAYER0_LOC) {                // ������ʼ����Ϊ (5,1) ��Ŀ��Ϊ (x,9)
                targetY = PLAYER1_LOC.y;
            }
            else {                                             // ������ʼ����Ϊ (5,9) ��Ŀ��Ϊ (x,1)
                targetY = PLAYER0_LOC.y;
            }
        }
        std::cout << newChange;                     // ������յ������ݵ�����̨��ʾ  
        if (!newChange.newEnemyBlockBar.isNan()) {               // �Է������˵��� 
            BlockBar tmp = newChange.newEnemyBlockBar;
            tmp.normalization();                                 // �淶Ϊ start ����С�� stop �������ʽ 
            blocks.push_back(tmp);  // �洢���� 
            ene_remain--;
        }
        stepcount++;
        Location next_loc;
        BlockBar next_bar,anti_dirty_bar;
        double mark_bar=0, mark_step=0;
        evaluate(newChange, next_loc, next_bar, mark_bar, mark_step);
        Step step;  
        if (anti_dirty&&stepcount>=5&&stepcount<=7&&is_dirty(newChange,anti_dirty_bar)&&anti_dirty_bar.start.x>=0)//���������·
        {
            anti_dirty = 0;
            next_bar.normalization();
            step.myNewBlockBar = anti_dirty_bar;
            blocks.push_back(next_bar);  // �洢���� 
            my_remain--;
        }
       else if ((mark_bar > mark_step)&&(next_bar.start.x > -1)&&my_remain>0)//����Ӧ�Ű����ܹ��Ű�
        {
           next_bar.normalization();                                 // �淶Ϊ start ����С�� stop �������ʽ 
            blocks.push_back(next_bar);  // �洢���� 
            my_remain--;
            step.myNewBlockBar = next_bar;
        }
        else//�Ų��˰���
        {
            if (next_loc == newChange.enemyLoc)//�Ų��˵��壬����һ��λ���ǵз�λ��
            {
                int i = newChange.enemyLoc.x - newChange.myLoc.x;
                int j = newChange.enemyLoc.y - newChange.myLoc.y;
                bool directions[4];
                possible_way(next_loc, directions);
                if (!directions[0] && j > 0 || !directions[1] && j < 0 || !directions[2] && i < 0 || !directions[3] && i > 0)//�޷�ֱ�������з�λ��
                {
                    if (my_remain>0 && next_bar.start.x > -1)//���ǷŰ���
                    {
                        next_bar.normalization();
                        blocks.push_back(next_bar);  // �洢���� 
                        step.myNewBlockBar = next_bar;
                        my_remain--;
                    }
                    else//�Ų��˰���
                    {
                        int vaccant = -1;
                        next_loc = seek_path(newChange.myLoc, vaccant, targetY, newChange.enemyLoc);
                        if(next_loc.x>0&&vaccant>0)//����·�����ǻ�ʤ��Ψһ·��������·��
                        step.myNewLoc = next_loc;
                        else if (rule_break < 1)//����·�ǻ�ʤ��Ψһ·����Υ�������Χ�ڣ��Ͳ�����
                        {
                            step.myNewLoc.x = newChange.myLoc.x;
                            step.myNewLoc.y = newChange.myLoc.y;
                            rule_break++;
                        }
                        else//�ǻ�ʤ��Ψһ·�����Ҳ�����Υ���ˣ�ֻ��ս������
                        {
                            next_loc = newChange.myLoc;
                            possible_way(next_loc, directions);
                             if (directions[2]&&!(i<0))
                                next_loc.x -= 1;
                            else if (directions[3]&&!(i>0))
                                next_loc.x += 1; 
                            else if (directions[0]&&!(j>0))
                                next_loc.y += 1;
                            else if (directions[1]&&!(j<0))
                                next_loc.y -= 1;
                            step.myNewLoc = next_loc;
                        }
                    }
                }
                else//����ֱ�������з�
                {
                    next_loc.x += i;
                    next_loc.y += j;
                    step.myNewLoc = next_loc;
                }
            }
            else//ֱ������ͺ���
                step.myNewLoc = next_loc;
        }
        std::cout << " -> " << step << std::endl;                          // ����ҵľ��ߵ�����̨��ʾ 
        return step;
    }
  void MyPlayer::restart() {
        blocks.clear();
        targetY = 0;
        head = 0;
        end = 0;
        num = 0;
        my_remain = 10;
        ene_remain = 10;
        rule_break = 0;
        stepcount = 0;
        anti_dirty = 1;
       if (roundcount > 3)
        { 
           win_status = 0;
            roundcount = 1;
        }
    }
}
void  possible_way(const QuoridorUtils::Location& myLoc, bool directions[])//Ѱ�Ҹ�λ�ô��ܹ����ߵķ���
{
    directions[0] = myLoc.y < QuoridorUtils::SIZE;           // �Ƿ���������� 
    directions[1] = myLoc.y > 1;                             // �Ƿ���������� 
    directions[2] = myLoc.x > 1;                             // �Ƿ���������� 
    directions[3] = myLoc.x < QuoridorUtils::SIZE;           // �Ƿ���������� 
    for (auto block : blocks)// ���������б�, �ҵ����赲�ķ���
    {
        if (block.isH()) // ˮƽ���򵲰�, start �� stop �� y ���
        {
            if (block.start.x == myLoc.x - 1 || block.start.x == myLoc.x - 2) // ���ܵ�·�ĵ��� 
            {
                if (block.start.y == myLoc.y)//�Ϸ���·
                    directions[0] = false;
                else if (block.start.y == myLoc.y - 1)//�·���·
                    directions[1] = false;
            }
            else continue;
        }
        else if (block.isV())// ��ֱ���򵲰�, start �� stop �� x ���
        {
            if (block.start.y == myLoc.y - 1 || block.start.y == myLoc.y - 2) // ���ܵ�·�ĵ��� 
            {
                if (block.start.x == myLoc.x - 1)//�󷽵�·
                    directions[2] = false;
                else if (block.start.x == myLoc.x)//�ҷ���·
                    directions[3] = false;
            }
            else continue;
        }
    }
}
node* pop()//�������г�Ա
{
    node* p= &queue[head];
    head++;
    return p;
}
void push(int x, int y, node* next)//�������ĵ������У�nextΪָ����һ���ڵ��ָ��
{
    queue[end].x = x;
    queue[end].y = y;
    queue[end].next = next; //ͷ�����룬�Զ����򣬱��ڲ��ҵ�һ��λ��
    end++;
}
Location seek_path(const QuoridorUtils::Location myLoc, int& min_step,int target,Location exclude)//������������̻�ʤ·��
{
    head = 0,end = 0,num=0;
    QuoridorUtils::Location nextLoc;
    push(myLoc.x, myLoc.y, NULL);//��ʼ��queue����
    int board[9][9] = { 0 };
    if(exclude.x>0&&exclude.y>0&&exclude.x<10&&exclude.y<10)
        board[exclude.x - 1][9 - exclude.y] = 1;
    int i = 1;
    while (1)
    {
        if (head >= end)
        {
            min_step = -1;//û�л�ʤ·��
            return nextLoc;//�������С�û�С�Ԫ��ʱ�˳�
        }
        node* temp = pop();//tempΪ�����С���һ����Ԫ��
        if (temp->y == target)//�ߵ��յ�ʱ����way���鱣�淵�ص�����·��
        {
            while (temp->next->next != NULL)//����Ѱ�����·���ġ���һ����λ��
            {
                num++;
                temp = temp->next;
            }
            nextLoc.x = temp->x;
            nextLoc.y = temp->y;
            min_step = num;//���·������
            return nextLoc;
        }
        bool directions[4];
        nextLoc.x = temp->x;//temp->x-1
        nextLoc.y = temp->y;//temp->y-1
        possible_way(nextLoc, directions);
        board[temp->x - 1][9-temp->y] = 1;
        i = -i;
        if (targetY == 1&&i==1)
        { 
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//����������
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//����������
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
            if (directions[3] && !board[temp->x][9 - temp->y])//����������
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            }
            if (directions[0] && !board[temp->x - 1][8 - temp->y])//����������
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
        }
        else  if (targetY == 1 && i == -1)
        {  
            if (directions[3] && !board[temp->x][9 - temp->y])//����������
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            } 
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//����������
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
           if (directions[0] && !board[temp->x - 1][8 - temp->y])//����������
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//����������
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
        }
        else  if (targetY == 9 && i == -1)
        { 
            if (directions[0] && !board[temp->x - 1][8 - temp->y])//����������
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//����������
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
            if (directions[3] && !board[temp->x][9 - temp->y])//����������
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            }
          
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//����������
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
        }
        else  if (targetY == 9 && i == 1)
        {
            if (directions[3] && !board[temp->x][9 - temp->y])//����������
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            }
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//����������
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
            
            if (directions[0] && !board[temp->x - 1][8 - temp->y])//����������
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//����������
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
        }
    }
    node* tmp = &queue[end];//���������һ��Ԫ��
    return myLoc;
}
void evaluate(const ChessboardChange& newChange, Location& next_loc,
    QuoridorUtils::BlockBar& next_bar, double& mark_bar, double& mark_step)
{
    int my_dist, ene_dist, bar_my_dist=-1, bar_ene_dist=-1, max = 0, newgap = 0, gap = 0;
    BlockBar best_bar;
    Location null;
    null.x = -1;
    null.y = -1;
    next_loc = seek_path(newChange.myLoc, my_dist, targetY,null);
    seek_path(newChange.enemyLoc, ene_dist, 10 - targetY,null);
    gap = -ene_dist + my_dist;
    if (my_remain)
    {
        for (int i = 0; i < 10; i++)//�������Է��õ����λ�ò����д��
        {
            for (int j = 0; j < 10; j++)
            {
                next_bar.start.x = i;
                next_bar.start.y = j;
                next_bar.stop.x = i;
                next_bar.stop.y = j+2;
                if (i != 0 && i != 9 && set_block(next_bar))
                {
                   blocks.push_back(next_bar);  // �洢���� 
                    seek_path(newChange.myLoc, bar_my_dist, targetY, null);//�������õ���󼺷���ʤ���·��
                    seek_path(newChange.enemyLoc, bar_ene_dist, 10 - targetY, null);//�������õ����з���ʤ���·��
                    newgap = -bar_ene_dist + bar_my_dist; 
                    if (bar_my_dist > 0 && bar_ene_dist > 0 && max < gap - newgap)
                    { 
                        int vaccant = -1;
                       seek_path(newChange.enemyLoc, vaccant, 10 - targetY, newChange.myLoc);
                       if (vaccant > 0)
                       {
                           max = gap - newgap;
                           best_bar = next_bar;
                       }
                    }
                    blocks.pop_back();  
                }
                next_bar.stop.x = i+2;
                next_bar.stop.y = j;
                if (j != 0 && j != 9 && set_block(next_bar))
                {
                    blocks.push_back(next_bar);  // �洢���� 
                    seek_path(newChange.myLoc, bar_my_dist, targetY, null);//�������õ���󼺷���ʤ���·��
                    seek_path(newChange.enemyLoc, bar_ene_dist, 10 - targetY, null);//�������õ����з���ʤ���·��
                    newgap = -bar_ene_dist + bar_my_dist;
                    if (bar_my_dist > 0 && bar_ene_dist > 0 && max < gap - newgap)
                    {
                        int vaccant = -1;
                        seek_path(newChange.enemyLoc, vaccant, 10 - targetY, newChange.myLoc);
                        if (vaccant > 0)
                        {
                            max = gap - newgap;
                            best_bar = next_bar;
                        }
                    }
                    blocks.pop_back();
                }
            }
        }
    }
    next_bar = best_bar;
    int random = 0;
    if (roundcount == 3 && win_status == 0)
        random = 2;
    if (targetY == 9)
    {
        int add=0;
        if (ene_dist<=5&&my_dist>ene_dist)
            add = 2;
        mark_bar = 10 * (-1.0 * ene_dist + 1.0 * my_dist + 1.35 * max+random+add)
            - 0.75 * my_dist - 0.25 * ene_dist + 0.75 * (10.0-my_remain) - 0.75* ene_remain;//modify
        mark_step = 10 * (-1.0 * ene_dist + 1.0 * my_dist+0.2)
            + 0.75 * my_dist  - 0.75 *(10.0- my_remain) + 0.75 * ene_remain;//modify
    }
    else 
    {
        int add = 0;
        if (ene_dist <= 8 && my_dist > ene_dist)
            add = 3;
        mark_bar = 10 * (-1.0 * ene_dist + 1.0 * my_dist + 1.0* max-random+add)
            - 0.75 * my_dist - 0.25 * ene_dist  - 0.75 * ene_remain+ 0.75 * (10.0-my_remain);//modify
        mark_step = 10 * (-1.0 * ene_dist + 1.0 * my_dist+0.2) 
            + 0.75 * my_dist+0.25*ene_dist - 0.75 *(10.0-my_remain) + 1.0 * ene_remain;//modify
     }
}
bool set_block(QuoridorUtils::BlockBar new_blo)//�ж���new_blo���õ����Ƿ�Ϸ�
{
    if (new_blo.isNan())
        return false;
    if (new_blo.isV())
    {
        new_blo.normalization();  // �淶Ϊ start ����С�� stop �������ʽ 
        for (auto block : blocks)
        {
            if (block.isH())
                continue;
            if (!(block.start.x == new_blo.start.x))
                continue;
            if (new_blo.start.y == block.start.y || new_blo.start.y == block.start.y + 1|| 
new_blo.start.y == block.start.y - 1)
                return false;
        }
        return true;
    }
    else if (new_blo.isH())
    {
        new_blo.normalization();  // �淶Ϊ start ����С�� stop �������ʽ 
        for (auto block : blocks)
        {
            if (block.isV())
                continue;
            if (!(block.start.y == new_blo.start.y))
                continue;
            if (new_blo.start.x == block.start.x || new_blo.start.x == block.start.x + 1||new_blo.start.x == block.start.x - 1)
                return false;
        }
        return true;
    }
    else return false;
}
bool is_dirty(const ChessboardChange& newChange,BlockBar& anti_dirty_bar)
{
    int my_dist, bar_my_dist = -1,bar_ene_dist=-1;
    BlockBar bar;
    Location null;
    null.x = -1;
    null.y = -1;
    seek_path(newChange.myLoc, my_dist, targetY, null);
    if (targetY == 1)
        {
            for (int i = 0; i < 10; i++)
            {
                bar.start.x = i;
                bar.start.y = 1;
                bar.stop.x = i + 2;
                bar.stop.y = 1;
                if (set_block(bar))
                {
                    blocks.push_back(bar);  // �洢���� 
                    seek_path(newChange.myLoc, bar_my_dist, targetY, null);
                    blocks.pop_back();
                    if (bar_my_dist - my_dist >= 8)
                    {
                        anti_dirty_bar.start.x = 7;
                        anti_dirty_bar.start.y = 2;
                        anti_dirty_bar.stop.x = 9;
                        anti_dirty_bar.stop.y = 2;
                        if (set_block(anti_dirty_bar)) 
                        {
                            blocks.push_back(anti_dirty_bar);
                            seek_path(newChange.myLoc, bar_my_dist, targetY, null);
                            seek_path(newChange.enemyLoc, bar_ene_dist, 10 - targetY, null);
                            blocks.pop_back();
                            if(bar_my_dist>0&&bar_ene_dist>0)
                            return true;
                        }
                    }
                }
            }
        }
        else if (targetY == 9)
        {
            for (int i = 0; i < 10; i++)
            {
                bar.start.x = i;
                bar.start.y = 8;
                bar.stop.x = i + 2;
                bar.stop.y = 8;
                if (set_block(bar))
                {
                    blocks.push_back(bar);  // �洢���� 
                    seek_path(newChange.myLoc, bar_my_dist, targetY, null);
                    blocks.pop_back();
                    if (bar_my_dist - my_dist >= 8)
                    {
                        anti_dirty_bar.start.x = 7;
                        anti_dirty_bar.start.y = 7;
                        anti_dirty_bar.stop.x = 9;
                        anti_dirty_bar.stop.y = 7;
                        if (set_block(anti_dirty_bar))
                        {
                            blocks.push_back(anti_dirty_bar);
                            seek_path(newChange.myLoc, bar_my_dist, targetY, null);
                            seek_path(newChange.enemyLoc, bar_ene_dist, 10 - targetY, null);
                            blocks.pop_back();
                            if (bar_my_dist > 0 && bar_ene_dist > 0)
                                return true;
                        }
                    }
                }
            }
        }
        return false;
}

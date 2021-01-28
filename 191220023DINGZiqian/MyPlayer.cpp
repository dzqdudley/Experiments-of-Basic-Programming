#include "MyPlayer.h"
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
//面向过程实现开始
using namespace QuoridorUtils;
struct node
{
    int x, y;//坐标
    node* next;//便于回溯第一个位置
};
node queue[1000];//结构数组queue
int targetY = 0,head=0, end=0, num = 0,my_remain=10, ene_remain=10;
std::vector<QuoridorUtils::BlockBar> blocks;
Location seek_path(const QuoridorUtils::Location myLoc, int& min_step,int target,Location exclude);
bool set_block(QuoridorUtils::BlockBar new_blo);//判断在new_blo放置挡板是否合法
void evaluate(const ChessboardChange& newChange, Location& next_loc,
    QuoridorUtils::BlockBar& next_bar, double &mark_bar, double &mark_step);//评估函数
void  possible_way(const QuoridorUtils::Location& myLoc, bool directions[]);//寻找该位置处能够行走的方向
bool is_dirty(const ChessboardChange& newChange, BlockBar& anti_dirty_bar);//检查是不是脏套路
int rule_break = 0, stepcount = 0, win_status = 0, roundcount = 1,anti_dirty=1;
namespace QuoridorUtils
{
    Step MyPlayer::nextStep(const ChessboardChange& newChange)
    {
        if (targetY == 0) {                                // 判断并记录自己的目标
           
            if (newChange.myLoc == PLAYER0_LOC) {                // 己方初始坐标为 (5,1) 则目标为 (x,9)
                targetY = PLAYER1_LOC.y;
            }
            else {                                             // 己方初始坐标为 (5,9) 则目标为 (x,1)
                targetY = PLAYER0_LOC.y;
            }
        }
        std::cout << newChange;                     // 输出接收到的数据到控制台显示  
        if (!newChange.newEnemyBlockBar.isNan()) {               // 对方放置了挡板 
            BlockBar tmp = newChange.newEnemyBlockBar;
            tmp.normalization();                                 // 规范为 start 坐标小于 stop 坐标的形式 
            blocks.push_back(tmp);  // 存储挡板 
            ene_remain--;
        }
        stepcount++;
        Location next_loc;
        BlockBar next_bar,anti_dirty_bar;
        double mark_bar=0, mark_step=0;
        evaluate(newChange, next_loc, next_bar, mark_bar, mark_step);
        Step step;  
        if (anti_dirty&&stepcount>=5&&stepcount<=7&&is_dirty(newChange,anti_dirty_bar)&&anti_dirty_bar.start.x>=0)//如果是脏套路
        {
            anti_dirty = 0;
            next_bar.normalization();
            step.myNewBlockBar = anti_dirty_bar;
            blocks.push_back(next_bar);  // 存储挡板 
            my_remain--;
        }
       else if ((mark_bar > mark_step)&&(next_bar.start.x > -1)&&my_remain>0)//评估应放板且能够放板
        {
           next_bar.normalization();                                 // 规范为 start 坐标小于 stop 坐标的形式 
            blocks.push_back(next_bar);  // 存储挡板 
            my_remain--;
            step.myNewBlockBar = next_bar;
        }
        else//放不了板子
        {
            if (next_loc == newChange.enemyLoc)//放不了挡板，且下一步位置是敌方位置
            {
                int i = newChange.enemyLoc.x - newChange.myLoc.x;
                int j = newChange.enemyLoc.y - newChange.myLoc.y;
                bool directions[4];
                possible_way(next_loc, directions);
                if (!directions[0] && j > 0 || !directions[1] && j < 0 || !directions[2] && i < 0 || !directions[3] && i > 0)//无法直接跳过敌方位置
                {
                    if (my_remain>0 && next_bar.start.x > -1)//考虑放板子
                    {
                        next_bar.normalization();
                        blocks.push_back(next_bar);  // 存储挡板 
                        step.myNewBlockBar = next_bar;
                        my_remain--;
                    }
                    else//放不了板子
                    {
                        int vaccant = -1;
                        next_loc = seek_path(newChange.myLoc, vaccant, targetY, newChange.enemyLoc);
                        if(next_loc.x>0&&vaccant>0)//这条路径不是获胜的唯一路径，换条路走
                        step.myNewLoc = next_loc;
                        else if (rule_break < 1)//这条路是获胜的唯一路径，违规次数范围内，就不走了
                        {
                            step.myNewLoc.x = newChange.myLoc.x;
                            step.myNewLoc.y = newChange.myLoc.y;
                            rule_break++;
                        }
                        else//是获胜的唯一路径，且不能再违规了，只能战术后退
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
                else//可以直接跳过敌方
                {
                    next_loc.x += i;
                    next_loc.y += j;
                    step.myNewLoc = next_loc;
                }
            }
            else//直接走棋就好啦
                step.myNewLoc = next_loc;
        }
        std::cout << " -> " << step << std::endl;                          // 输出我的决策到控制台显示 
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
void  possible_way(const QuoridorUtils::Location& myLoc, bool directions[])//寻找该位置处能够行走的方向
{
    directions[0] = myLoc.y < QuoridorUtils::SIZE;           // 是否可以向上走 
    directions[1] = myLoc.y > 1;                             // 是否可以向下走 
    directions[2] = myLoc.x > 1;                             // 是否可以向左走 
    directions[3] = myLoc.x < QuoridorUtils::SIZE;           // 是否可以向右走 
    for (auto block : blocks)// 遍历挡板列表, 找到被阻挡的方向
    {
        if (block.isH()) // 水平方向挡板, start 与 stop 的 y 相等
        {
            if (block.start.x == myLoc.x - 1 || block.start.x == myLoc.x - 2) // 可能挡路的挡板 
            {
                if (block.start.y == myLoc.y)//上方挡路
                    directions[0] = false;
                else if (block.start.y == myLoc.y - 1)//下方挡路
                    directions[1] = false;
            }
            else continue;
        }
        else if (block.isV())// 竖直方向挡板, start 与 stop 的 x 相等
        {
            if (block.start.y == myLoc.y - 1 || block.start.y == myLoc.y - 2) // 可能挡路的挡板 
            {
                if (block.start.x == myLoc.x - 1)//左方挡路
                    directions[2] = false;
                else if (block.start.x == myLoc.x)//右方挡路
                    directions[3] = false;
            }
            else continue;
        }
    }
}
node* pop()//弹出队列成员
{
    node* p= &queue[head];
    head++;
    return p;
}
void push(int x, int y, node* next)//将经过的点加入队列，next为指向上一个节点的指针
{
    queue[end].x = x;
    queue[end].y = y;
    queue[end].next = next; //头部插入，自动反向，便于查找第一个位置
    end++;
}
Location seek_path(const QuoridorUtils::Location myLoc, int& min_step,int target,Location exclude)//遍历棋盘找最短获胜路径
{
    head = 0,end = 0,num=0;
    QuoridorUtils::Location nextLoc;
    push(myLoc.x, myLoc.y, NULL);//初始化queue队列
    int board[9][9] = { 0 };
    if(exclude.x>0&&exclude.y>0&&exclude.x<10&&exclude.y<10)
        board[exclude.x - 1][9 - exclude.y] = 1;
    int i = 1;
    while (1)
    {
        if (head >= end)
        {
            min_step = -1;//没有获胜路径
            return nextLoc;//当队列中“没有”元素时退出
        }
        node* temp = pop();//temp为队列中“第一个”元素
        if (temp->y == target)//走到终点时，用way数组保存返回到起点的路径
        {
            while (temp->next->next != NULL)//回溯寻找最短路径的“下一步”位置
            {
                num++;
                temp = temp->next;
            }
            nextLoc.x = temp->x;
            nextLoc.y = temp->y;
            min_step = num;//最短路径长度
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
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//可以向下走
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//可以向左走
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
            if (directions[3] && !board[temp->x][9 - temp->y])//可以向右走
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            }
            if (directions[0] && !board[temp->x - 1][8 - temp->y])//可以向上走
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
        }
        else  if (targetY == 1 && i == -1)
        {  
            if (directions[3] && !board[temp->x][9 - temp->y])//可以向右走
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            } 
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//可以向左走
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
           if (directions[0] && !board[temp->x - 1][8 - temp->y])//可以向上走
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//可以向下走
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
        }
        else  if (targetY == 9 && i == -1)
        { 
            if (directions[0] && !board[temp->x - 1][8 - temp->y])//可以向上走
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//可以向左走
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
            if (directions[3] && !board[temp->x][9 - temp->y])//可以向右走
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            }
          
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//可以向下走
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
        }
        else  if (targetY == 9 && i == 1)
        {
            if (directions[3] && !board[temp->x][9 - temp->y])//可以向右走
            {
                push(temp->x + 1, temp->y, temp);
                board[temp->x][8 - temp->y + 1] = 1;
            }
            if (directions[2] && !board[temp->x - 2][9 - temp->y])//可以向左走
            {
                push(temp->x - 1, temp->y, temp);
                board[temp->x - 2][8 - temp->y + 1] = 1;
            }
            
            if (directions[0] && !board[temp->x - 1][8 - temp->y])//可以向上走
            {
                push(temp->x, temp->y + 1, temp);
                board[temp->x - 1][8 - temp->y] = 1;
            }
            if (directions[1] && !board[temp->x - 1][10 - temp->y])//可以向下走
            {
                push(temp->x, temp->y - 1, temp);
                board[temp->x - 1][8 - temp->y + 2] = 1;
            }
        }
    }
    node* tmp = &queue[end];//返回链表第一个元素
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
        for (int i = 0; i < 10; i++)//遍历可以放置挡板的位置并进行打分
        {
            for (int j = 0; j < 10; j++)
            {
                next_bar.start.x = i;
                next_bar.start.y = j;
                next_bar.stop.x = i;
                next_bar.stop.y = j+2;
                if (i != 0 && i != 9 && set_block(next_bar))
                {
                   blocks.push_back(next_bar);  // 存储挡板 
                    seek_path(newChange.myLoc, bar_my_dist, targetY, null);//计算设置挡板后己方获胜最短路径
                    seek_path(newChange.enemyLoc, bar_ene_dist, 10 - targetY, null);//计算设置挡板后敌方获胜最短路径
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
                    blocks.push_back(next_bar);  // 存储挡板 
                    seek_path(newChange.myLoc, bar_my_dist, targetY, null);//计算设置挡板后己方获胜最短路径
                    seek_path(newChange.enemyLoc, bar_ene_dist, 10 - targetY, null);//计算设置挡板后敌方获胜最短路径
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
bool set_block(QuoridorUtils::BlockBar new_blo)//判断在new_blo放置挡板是否合法
{
    if (new_blo.isNan())
        return false;
    if (new_blo.isV())
    {
        new_blo.normalization();  // 规范为 start 坐标小于 stop 坐标的形式 
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
        new_blo.normalization();  // 规范为 start 坐标小于 stop 坐标的形式 
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
                    blocks.push_back(bar);  // 存储挡板 
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
                    blocks.push_back(bar);  // 存储挡板 
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

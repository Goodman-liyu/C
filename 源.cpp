#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#define N 5//停车区最多有5辆车
#define M 3//候车区最多有2辆车
#define Length 9
#define price1 3
#define price2 5

typedef struct
{
	char Carno[N][Length];//车牌号
	int CarTime_min[N];
	int CarTime_hour[N];//进入停车场的时刻
	int top;//栈指针
}sqstack;//顺序栈类型声明
void initstack(sqstack *&s)//初始化栈
{
	s = (sqstack*)malloc(sizeof(sqstack));
	s->top = -1;
}
bool stackempty(sqstack *s)//判断停车场是否为空
{
	return(s->top == -1);
}
bool stackfull(sqstack *s)//判断停车场是否为满
{
	return(s->top == N - 1);
}
bool push(sqstack *&s, char carno[Length], int hour,int min)//将车辆停入停车区
{
	if (s->top == N - 1)//如果栈满，则进栈失败
		return false;
	s->top++;
	for(int i=0;i<Length;i++)
	s->Carno[s->top][i] = carno[i];
	s->CarTime_hour[s->top] = hour;
	s->CarTime_min[s->top] = min;
	return true;
}
bool pop(sqstack *&s, char carno[Length], int &hour,int &min)//将车辆驶出停车区
{
	if (s->top == -1)//如果栈空，出栈失败
		return false;
	for (int i = 0; i < Length; i++)
	carno[i] = s->Carno[s->top][i];
	hour= s->CarTime_hour[s->top];
	min = s->CarTime_min[s->top];
	s->top--;
	return true;
}
void dispstack(sqstack *s)//显示停车区中的车辆信息
{
	int j;
	for (int i = s->top; i > -1; i--)
	{
		for (j = 0; j < Length; j++)
		{
			printf("%c", s->Carno[i][j]);
		}
		printf("   入库时间%d时%d分", s->CarTime_hour[i], s->CarTime_min[i]);
		printf("\n");
	}

}
//建立环形队列
typedef struct
{
    char carno[M][Length];
	int front, rear;
}SQqueue;
//环形队列的初始化
void initqueue(SQqueue *&l)
{
	l = (SQqueue*)malloc(sizeof(SQqueue));
	l->front = 0;
	l->rear = 0;
}
bool QueueEmpty(SQqueue *l)//判断环形队列是否为空
{
	return(l->front ==l->rear);
}
bool QueueFull(SQqueue *l)//判断队列是否为满
{
	return(l->front == (l->rear + 1) % M);
}
bool enQueue(SQqueue *&l, char carno[Length])
{
	if (QueueFull(l)) return false;//若队已满，入队失败
	l->rear = (l->rear + 1) % M;
	for (int i = 0; i < Length; i++)
	l->carno[l->rear][i] = carno[i];
	return true;
}
bool deQueue(SQqueue *&l, char carno[Length])
{
	if (QueueEmpty(l)) return false;//若队为空，入队失败
	l->front = (l->front + 1)&M;
	for (int i = 0; i < Length; i++)
	carno[i] = l->carno[l->front][i];
	return true;
}
//展示候车区中的车辆情况
void dispqueue(SQqueue *l)
{
	int j;
	int i = (l->front + 1) % M;
	for (j = 0; j < Length; j++)
	printf("%c", l->carno[i][j]);
	while(abs(l->rear-i)%M!=0)//当i不与队尾指针重合时循环输出
	{ 
		i = (i + 1) % M;
		for ( j = 0; j < Length; j++)
			printf("%c", l->carno[i][j]);
		printf("\n");
	}
	printf("\n");
}
//将停车数据保留下来
void file_write(char carno[], int time[])
{
	FILE *in;
	static int count = 1;//通过静态变量判断是否时第一次写入数据
	//第一次写入数据时，采用只写的方式打开，并写入相关的表头
	if (count == 1)
	{
		if ((in = fopen("car_data.txt", "w")) == NULL)
		{
			printf("数据写入失败!\n");
			exit(1);
		}
		fprintf(in, "%s", "	         驶入时间        离开时间\n");
		fprintf(in, "%s", " 车牌号         时      分       时      分\n");
		count++;
	}
	//以追加的形式打开文件，并向文件中写入数据
	else
	{
		if ((in = fopen("car_data.txt", "a+")) == NULL)
		{
			printf("数据写入失败!\n");
			exit(1);
		}
	}
	int v = 0;
	while (v < Length)
	{
		fprintf(in, "%c", carno[v]);
		v++;
	}
	v = 0;
	while (v < 4)
	{
		fprintf(in, "%8d", time[v]);
		v++;
	}
	fprintf(in, "%c", '\n');
	printf("%s 车辆停车信息已保存.\n", carno);
	fclose(in);
}
bool read_code(char temp_user[],char temp_code[])
{
	char user[10], code[10];
	FILE *out;//从文件code.txt指定位置中读取用户名和密码
	if ((out = fopen("code.txt", "r")) == NULL)
	{
		printf("error!\n");
		exit(1);
	}
	fseek(out, 9l, 0);
	int i;
	for (i = 0; ; i++)
	{
		fscanf(out, "%c", &user[i]);
		if (user[i] == '\n')
		{
			user[i] = '\0';  break;
		}
	}
	int length1 = i + 1;
	fseek(out, 5l, 1);
	i = 0;
	while(!feof(out))
	{
		fscanf(out, "%c", &code[i]);
		i++;
	}
	code[i-1] = '\0';
	int length2 = i;
	fclose(out);
	//判断用户名和密码是否匹配
	for (i = 0; temp_user[i] != '\0'; i++);
	if (i+1 != length1)return false;
	for (i = 0; temp_code[i] != '\0'; i++);
	if (i+1 != length2)return false;
	for (i = 0; user[i] != '\0'; i++)
	{
		if (user[i] != temp_user[i])
			return false;
	}
	for (i = 0; code[i] != '\0'; i++)
	{
		if (code[i] != temp_code[i])
			return false;
	}
		return true;
}
void pre_display()
{
	printf("%30s************************************\n", "");
	printf("%30s*%7s单路停车站管理系统%9s*\n", "","","");
	printf("%30s*%4s停车区容量：5  候车区容量：2%2s*\n", "", "", "");
	printf("%30s************************************\n", "");
	printf("%30s*%7s请按照所需选择功能%9s*\n", "", "", "");
	printf("%30s*%8s1.车辆到达%16s*\n", "", "", "");
	printf("%30s*%8s2.车辆离开%16s*\n", "", "", "");
	printf("%30s*%8s3.显示当前停车区状况 %5s*\n", "", "", "");
	printf("%30s*%8s4.显示当前候车区状况 %5s*\n", "", "", "");
	printf("%30s*%8s0.退出%20s*\n", "", "", "");
	printf("%30s************************************\n", "");
}
int main()
{
	printf("%30s*%5s欢迎使用单路停车站管理系统%3s*\n", "", "", "");
	bool is_match = true;
	int t = 3;//一共有3次登录机会，若3次密码均错误，程序结束,正确密码保存在code.txt文件中
	while (is_match)
	{
		printf("请输入用户名:");
		char temp_user[10];
		rewind(stdin);
		gets_s(temp_user);
		printf("请输入密码:");
		char temp_code[10];
		rewind(stdin);
		gets_s(temp_code);
		//从文件中读取密码
		if (read_code(temp_user, temp_code))
		{
			is_match = false;
			printf("登录成功！\n");
		}
		else
		{
			printf("用户名或密码错误,还有%d次机会\n", --t);
			if (t == 0)
			{
				printf("登录次数过多，程序结束");
				return 0;
			}

		}
	}
	pre_display();
	sqstack  *p, *tmp;//p为停车区对应的栈，tmp为临时存车栈
	SQqueue  *q;//q为候车区对应的环形队列
	//对栈和队列进行初始化
	initstack(p);
	initstack(tmp);
	initqueue(q);
	//定义所需的变量
	int n,arrive_time_hour,arrive_time_min,leave_time_hour,leave_time_min;
	int i;
	char a;
	bool islegal,isfind = false, temp;
	char carno[Length];
	static int h = 0;
	
	do
	{
		printf(">功能号：");
		scanf("%d", &n);
		switch (n)
		{
		case 1:
			islegal = true;
			while (islegal)
			{
				rewind(stdin);
				islegal = false;
				printf("请连续输入车牌号(如:鄂A88888，共7位):");
				for (i = 0; i < Length-1; i++)
				{
					scanf("%c", &carno[i]);
					if (carno[i] == '\n'|| carno[i] == ' ')
					{
						islegal = true;
						break;
					}
				}		
				if (islegal)
				{
					printf("车牌号输入不规范（不连续），请重新输入\n,");
					continue;
				}
				carno[Length - 1] = '\0';
			    
				a = getchar();//从键盘缓冲区读取数据，判断输入是否规范
				if (a != '\n') 
				{
					printf("车牌号输入不规范（过长），请重新输入\n");
					islegal = true;
				}
			}
			islegal = true;
			while (islegal)
			{
				printf("请输入抵达时间(如8 30 即代表8：30抵达):");
				rewind(stdin);
				scanf("%d%d", &arrive_time_hour, &arrive_time_min);
				islegal = false;
				if (arrive_time_hour < 0 || arrive_time_hour>24 || arrive_time_min < 0 || arrive_time_min>60)
				{
					islegal = true;
					printf("时间输入不规范，请重新输入\n");
				}
				if (!stackempty(p))
				{
					if (arrive_time_hour < p->CarTime_hour[p->top] || (arrive_time_hour == p->CarTime_hour[p->top] && arrive_time_min < p->CarTime_min[p->top]))
					{
						islegal = true;
						printf("时间输入不规范，请重新输入\n");
					}
				}
			}
			if (!stackfull(p))//若停车区有空位，即栈p不满时
			{
				push(p, carno, arrive_time_hour, arrive_time_min);
				printf("车辆已停入停车区，位置:%d\n", p->top+1);
			}
			else//停车区已满，考虑候车区
			{
				
				if (!QueueFull(q))
				{
					enQueue(q, carno);
					printf("车辆在候车区等待，位置:%d\n", ++h);
			    }
				else
				{
					printf("候车区已满，无法停车\n");
				}
			
			}
			break;
		case 2:
			if (stackempty(p))
			{
				printf("此时停车场中无车！\n");
				break;
			}
			islegal = true;
			while (islegal)
			{
				rewind(stdin);
				islegal = false;
				printf("请连续输入车牌号(如:鄂A88888，共7位):");
				for (i = 0; i < Length - 1; i++)
				{
					scanf("%c", &carno[i]);
					if (carno[i] == '\n'|| carno[i] == ' ')
					{
						islegal = true;
						break;
					}
				}
				if (islegal)
				{
					printf("车牌号输入不规范（不连续），请重新输入\n,");
					continue;
				}
				carno[Length - 1] = '\0';

				a = getchar();//从键盘缓冲区读取数据，判断输入是否规范
				if (a != '\n')
				{
					printf("车牌号输入不规范（过长），请重新输入\n");
					islegal = true;
				}
			}
			
			int m ,j;
			int temp_hour, temp_min;
			for ( m = 0; m <= p->top; m++)
			{
				temp = true;
				for ( j = 0; j < 9; j++)
				{
					if (p->Carno[m][j] != carno[j])
					{
						temp = false;
						break;
					}
			   }
				if (temp)
				{
					isfind = true;
					break;
				}
			}
			if (!isfind)printf("未找到该编号汽车\n");
			else {
				printf("请输入离开时间（小时+分钟）:");
				islegal = true;
				while (islegal)
				{
					rewind(stdin);
					islegal = false;
					scanf("%d%d", &leave_time_hour, &leave_time_min);

					if (leave_time_hour < 0 || leave_time_min < 0|| leave_time_hour >24 || leave_time_min >60  )
					{
						printf("时间为负，请重新输入\n");
						islegal = true;
					}
					if (islegal)continue;
					if (leave_time_hour < p->CarTime_hour[m] ||( (leave_time_hour == p->CarTime_hour[m]) && leave_time_min < p->CarTime_min[m]))
					{
						printf("离开时间早于抵达时间，请重新输入\n");
						islegal = true;
					}
				}
				for (j = p->top; j>=m; j--)//将出口路径上的车辆先倒入临时栈中(包括要离开的车辆）
				{
					pop(p, carno, temp_hour, temp_min);
					push(tmp, carno, temp_hour, temp_min);
				}
				tmp->top--;
				for (j = 0; j < Length; j++)
					printf("%c", carno[j]);
				printf("车辆的驶入时间为%d点%d分，离开时间为%d点%d分\n",temp_hour,temp_min,leave_time_hour,leave_time_min );
				int time_write[4] = { temp_hour,temp_min,leave_time_hour,leave_time_min };
				//每当有车辆离开时，调用文件读写函数，将该车辆的信息记录下来
				file_write(carno,time_write);
				
				int total = (leave_time_hour - temp_hour) * 60 + leave_time_min-temp_min ;
				int time_30 = total / 30;
				if (total % 30 != 0)time_30++;
				if(time_30<=12)//收费为分段收费，前6个小时，每半个小时收费3元，6个小时后，每半个小时收费5元
				printf("停留%d分钟，收费%d元（不满半小时也按半小时收费）\n", total, time_30*price1);
				else 	
				printf("停留%d分钟，收费%d元（不满半小时也按半小时收费）\n", total, 12*price1+(time_30-12)*price2);
				
				while (!stackempty(tmp))
				{
					pop(tmp, carno, temp_hour, temp_min);
					push(p, carno, temp_hour, temp_min);
				}
				if (!QueueEmpty(q))
				{
					deQueue(q, carno);
					push(p, carno, leave_time_hour, leave_time_min);
					h--;
				}
			}
			break;
		case 3:
			if (!stackempty(p))
			{
				printf("停车区中的车辆(按时间由近及远排序）:\n");
				dispstack(p);
			}
			else printf("停车区中无车\n");
			break;
		case 4:
			if (!QueueEmpty(q))
			{
				printf("候车区中的车辆:\n");
				dispqueue(q);
			}
			else printf("候车区中无车\n");			
			break;
		case 0:printf("欢迎下次使用\n");
			break;
		default:
			printf("输入的命令有误，请重新输入\n");
			rewind(stdin);
			break;
		}
		printf("-----------------------------------------------------------------------------------------------------------\n");
	} while (n!=0);
	return 0;

}
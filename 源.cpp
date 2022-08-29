#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#define N 5//ͣ���������5����
#define M 3//���������2����
#define Length 9
#define price1 3
#define price2 5

typedef struct
{
	char Carno[N][Length];//���ƺ�
	int CarTime_min[N];
	int CarTime_hour[N];//����ͣ������ʱ��
	int top;//ջָ��
}sqstack;//˳��ջ��������
void initstack(sqstack *&s)//��ʼ��ջ
{
	s = (sqstack*)malloc(sizeof(sqstack));
	s->top = -1;
}
bool stackempty(sqstack *s)//�ж�ͣ�����Ƿ�Ϊ��
{
	return(s->top == -1);
}
bool stackfull(sqstack *s)//�ж�ͣ�����Ƿ�Ϊ��
{
	return(s->top == N - 1);
}
bool push(sqstack *&s, char carno[Length], int hour,int min)//������ͣ��ͣ����
{
	if (s->top == N - 1)//���ջ�������ջʧ��
		return false;
	s->top++;
	for(int i=0;i<Length;i++)
	s->Carno[s->top][i] = carno[i];
	s->CarTime_hour[s->top] = hour;
	s->CarTime_min[s->top] = min;
	return true;
}
bool pop(sqstack *&s, char carno[Length], int &hour,int &min)//������ʻ��ͣ����
{
	if (s->top == -1)//���ջ�գ���ջʧ��
		return false;
	for (int i = 0; i < Length; i++)
	carno[i] = s->Carno[s->top][i];
	hour= s->CarTime_hour[s->top];
	min = s->CarTime_min[s->top];
	s->top--;
	return true;
}
void dispstack(sqstack *s)//��ʾͣ�����еĳ�����Ϣ
{
	int j;
	for (int i = s->top; i > -1; i--)
	{
		for (j = 0; j < Length; j++)
		{
			printf("%c", s->Carno[i][j]);
		}
		printf("   ���ʱ��%dʱ%d��", s->CarTime_hour[i], s->CarTime_min[i]);
		printf("\n");
	}

}
//�������ζ���
typedef struct
{
    char carno[M][Length];
	int front, rear;
}SQqueue;
//���ζ��еĳ�ʼ��
void initqueue(SQqueue *&l)
{
	l = (SQqueue*)malloc(sizeof(SQqueue));
	l->front = 0;
	l->rear = 0;
}
bool QueueEmpty(SQqueue *l)//�жϻ��ζ����Ƿ�Ϊ��
{
	return(l->front ==l->rear);
}
bool QueueFull(SQqueue *l)//�ж϶����Ƿ�Ϊ��
{
	return(l->front == (l->rear + 1) % M);
}
bool enQueue(SQqueue *&l, char carno[Length])
{
	if (QueueFull(l)) return false;//�������������ʧ��
	l->rear = (l->rear + 1) % M;
	for (int i = 0; i < Length; i++)
	l->carno[l->rear][i] = carno[i];
	return true;
}
bool deQueue(SQqueue *&l, char carno[Length])
{
	if (QueueEmpty(l)) return false;//����Ϊ�գ����ʧ��
	l->front = (l->front + 1)&M;
	for (int i = 0; i < Length; i++)
	carno[i] = l->carno[l->front][i];
	return true;
}
//չʾ�����еĳ������
void dispqueue(SQqueue *l)
{
	int j;
	int i = (l->front + 1) % M;
	for (j = 0; j < Length; j++)
	printf("%c", l->carno[i][j]);
	while(abs(l->rear-i)%M!=0)//��i�����βָ���غ�ʱѭ�����
	{ 
		i = (i + 1) % M;
		for ( j = 0; j < Length; j++)
			printf("%c", l->carno[i][j]);
		printf("\n");
	}
	printf("\n");
}
//��ͣ�����ݱ�������
void file_write(char carno[], int time[])
{
	FILE *in;
	static int count = 1;//ͨ����̬�����ж��Ƿ�ʱ��һ��д������
	//��һ��д������ʱ������ֻд�ķ�ʽ�򿪣���д����صı�ͷ
	if (count == 1)
	{
		if ((in = fopen("car_data.txt", "w")) == NULL)
		{
			printf("����д��ʧ��!\n");
			exit(1);
		}
		fprintf(in, "%s", "	         ʻ��ʱ��        �뿪ʱ��\n");
		fprintf(in, "%s", " ���ƺ�         ʱ      ��       ʱ      ��\n");
		count++;
	}
	//��׷�ӵ���ʽ���ļ��������ļ���д������
	else
	{
		if ((in = fopen("car_data.txt", "a+")) == NULL)
		{
			printf("����д��ʧ��!\n");
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
	printf("%s ����ͣ����Ϣ�ѱ���.\n", carno);
	fclose(in);
}
bool read_code(char temp_user[],char temp_code[])
{
	char user[10], code[10];
	FILE *out;//���ļ�code.txtָ��λ���ж�ȡ�û���������
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
	//�ж��û����������Ƿ�ƥ��
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
	printf("%30s*%7s��·ͣ��վ����ϵͳ%9s*\n", "","","");
	printf("%30s*%4sͣ����������5  ����������2%2s*\n", "", "", "");
	printf("%30s************************************\n", "");
	printf("%30s*%7s�밴������ѡ����%9s*\n", "", "", "");
	printf("%30s*%8s1.��������%16s*\n", "", "", "");
	printf("%30s*%8s2.�����뿪%16s*\n", "", "", "");
	printf("%30s*%8s3.��ʾ��ǰͣ����״�� %5s*\n", "", "", "");
	printf("%30s*%8s4.��ʾ��ǰ����״�� %5s*\n", "", "", "");
	printf("%30s*%8s0.�˳�%20s*\n", "", "", "");
	printf("%30s************************************\n", "");
}
int main()
{
	printf("%30s*%5s��ӭʹ�õ�·ͣ��վ����ϵͳ%3s*\n", "", "", "");
	bool is_match = true;
	int t = 3;//һ����3�ε�¼���ᣬ��3����������󣬳������,��ȷ���뱣����code.txt�ļ���
	while (is_match)
	{
		printf("�������û���:");
		char temp_user[10];
		rewind(stdin);
		gets_s(temp_user);
		printf("����������:");
		char temp_code[10];
		rewind(stdin);
		gets_s(temp_code);
		//���ļ��ж�ȡ����
		if (read_code(temp_user, temp_code))
		{
			is_match = false;
			printf("��¼�ɹ���\n");
		}
		else
		{
			printf("�û������������,����%d�λ���\n", --t);
			if (t == 0)
			{
				printf("��¼�������࣬�������");
				return 0;
			}

		}
	}
	pre_display();
	sqstack  *p, *tmp;//pΪͣ������Ӧ��ջ��tmpΪ��ʱ�泵ջ
	SQqueue  *q;//qΪ������Ӧ�Ļ��ζ���
	//��ջ�Ͷ��н��г�ʼ��
	initstack(p);
	initstack(tmp);
	initqueue(q);
	//��������ı���
	int n,arrive_time_hour,arrive_time_min,leave_time_hour,leave_time_min;
	int i;
	char a;
	bool islegal,isfind = false, temp;
	char carno[Length];
	static int h = 0;
	
	do
	{
		printf(">���ܺţ�");
		scanf("%d", &n);
		switch (n)
		{
		case 1:
			islegal = true;
			while (islegal)
			{
				rewind(stdin);
				islegal = false;
				printf("���������복�ƺ�(��:��A88888����7λ):");
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
					printf("���ƺ����벻�淶����������������������\n,");
					continue;
				}
				carno[Length - 1] = '\0';
			    
				a = getchar();//�Ӽ��̻�������ȡ���ݣ��ж������Ƿ�淶
				if (a != '\n') 
				{
					printf("���ƺ����벻�淶��������������������\n");
					islegal = true;
				}
			}
			islegal = true;
			while (islegal)
			{
				printf("������ִ�ʱ��(��8 30 ������8��30�ִ�):");
				rewind(stdin);
				scanf("%d%d", &arrive_time_hour, &arrive_time_min);
				islegal = false;
				if (arrive_time_hour < 0 || arrive_time_hour>24 || arrive_time_min < 0 || arrive_time_min>60)
				{
					islegal = true;
					printf("ʱ�����벻�淶������������\n");
				}
				if (!stackempty(p))
				{
					if (arrive_time_hour < p->CarTime_hour[p->top] || (arrive_time_hour == p->CarTime_hour[p->top] && arrive_time_min < p->CarTime_min[p->top]))
					{
						islegal = true;
						printf("ʱ�����벻�淶������������\n");
					}
				}
			}
			if (!stackfull(p))//��ͣ�����п�λ����ջp����ʱ
			{
				push(p, carno, arrive_time_hour, arrive_time_min);
				printf("������ͣ��ͣ������λ��:%d\n", p->top+1);
			}
			else//ͣ�������������Ǻ���
			{
				
				if (!QueueFull(q))
				{
					enQueue(q, carno);
					printf("�����ں����ȴ���λ��:%d\n", ++h);
			    }
				else
				{
					printf("�����������޷�ͣ��\n");
				}
			
			}
			break;
		case 2:
			if (stackempty(p))
			{
				printf("��ʱͣ�������޳���\n");
				break;
			}
			islegal = true;
			while (islegal)
			{
				rewind(stdin);
				islegal = false;
				printf("���������복�ƺ�(��:��A88888����7λ):");
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
					printf("���ƺ����벻�淶����������������������\n,");
					continue;
				}
				carno[Length - 1] = '\0';

				a = getchar();//�Ӽ��̻�������ȡ���ݣ��ж������Ƿ�淶
				if (a != '\n')
				{
					printf("���ƺ����벻�淶��������������������\n");
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
			if (!isfind)printf("δ�ҵ��ñ������\n");
			else {
				printf("�������뿪ʱ�䣨Сʱ+���ӣ�:");
				islegal = true;
				while (islegal)
				{
					rewind(stdin);
					islegal = false;
					scanf("%d%d", &leave_time_hour, &leave_time_min);

					if (leave_time_hour < 0 || leave_time_min < 0|| leave_time_hour >24 || leave_time_min >60  )
					{
						printf("ʱ��Ϊ��������������\n");
						islegal = true;
					}
					if (islegal)continue;
					if (leave_time_hour < p->CarTime_hour[m] ||( (leave_time_hour == p->CarTime_hour[m]) && leave_time_min < p->CarTime_min[m]))
					{
						printf("�뿪ʱ�����ڵִ�ʱ�䣬����������\n");
						islegal = true;
					}
				}
				for (j = p->top; j>=m; j--)//������·���ϵĳ����ȵ�����ʱջ��(����Ҫ�뿪�ĳ�����
				{
					pop(p, carno, temp_hour, temp_min);
					push(tmp, carno, temp_hour, temp_min);
				}
				tmp->top--;
				for (j = 0; j < Length; j++)
					printf("%c", carno[j]);
				printf("������ʻ��ʱ��Ϊ%d��%d�֣��뿪ʱ��Ϊ%d��%d��\n",temp_hour,temp_min,leave_time_hour,leave_time_min );
				int time_write[4] = { temp_hour,temp_min,leave_time_hour,leave_time_min };
				//ÿ���г����뿪ʱ�������ļ���д���������ó�������Ϣ��¼����
				file_write(carno,time_write);
				
				int total = (leave_time_hour - temp_hour) * 60 + leave_time_min-temp_min ;
				int time_30 = total / 30;
				if (total % 30 != 0)time_30++;
				if(time_30<=12)//�շ�Ϊ�ֶ��շѣ�ǰ6��Сʱ��ÿ���Сʱ�շ�3Ԫ��6��Сʱ��ÿ���Сʱ�շ�5Ԫ
				printf("ͣ��%d���ӣ��շ�%dԪ��������СʱҲ����Сʱ�շѣ�\n", total, time_30*price1);
				else 	
				printf("ͣ��%d���ӣ��շ�%dԪ��������СʱҲ����Сʱ�շѣ�\n", total, 12*price1+(time_30-12)*price2);
				
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
				printf("ͣ�����еĳ���(��ʱ���ɽ���Զ����:\n");
				dispstack(p);
			}
			else printf("ͣ�������޳�\n");
			break;
		case 4:
			if (!QueueEmpty(q))
			{
				printf("�����еĳ���:\n");
				dispqueue(q);
			}
			else printf("�������޳�\n");			
			break;
		case 0:printf("��ӭ�´�ʹ��\n");
			break;
		default:
			printf("�����������������������\n");
			rewind(stdin);
			break;
		}
		printf("-----------------------------------------------------------------------------------------------------------\n");
	} while (n!=0);
	return 0;

}
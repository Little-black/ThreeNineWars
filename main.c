/***********************************************
还有两个bug：
1.ai的行动有时候还没结束就会清屏
2.ai自己打自己
***********************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#define CLEAR system("cls");start();showme()//清屏并输出必要信息

/*基本单位结构member***********************************/
typedef struct member{
    int place;//地点1-9
    int character;//性格1.2.3:鹰鸽复
    int people;//人数
    int attack;//攻击力(动态的)
    char team;//阵营abcdefghi(改成了大写)
    struct member *next;
}member;
member *p;//全局变量：操作指针
member *entry;//全局变量：入口指针
char me;//全局变量：玩家阵营
int round=1;//全局变量：回合数
int TIME=800;//全局变量：游戏速度

/*函数声明***************************************/
/*1*/int create(int l);//创建链表并随机初始化
/*2*/void start();//显示情况
/*3*/char choose();//选择玩家阵营
/*4*/void mine();//玩家的回合.
/*5*/int *showme();//显示玩家阵营位置.
/*6*/void fight(int att,int def);//战斗函数
/*7*/void aiturn();//AI的回合.
/*8*/void aithink(int character,int place);//AI的行为模式
/*8-2*/void aithink2(int i,double j,char *team,int *people,int place);//aithink函数的内定义函数，用于优化程序结构
/*9*/int ifdeath();//判断玩家是否死亡或胜利
/*10*/void supply();//每回合结束自动补兵
/*11*/void save();//存档函数（保存链表，回合数，玩家阵营）
/*12*/void load();//读档函数

/*主函数*****************************************/
int main(void)
{
    int i=0;//万能的临时变量
	system("color 0E");//改变窗口颜色

    //创建地图
    do{
        if(create(i)==-1){
            start();
            goto LOAD;
        }
        //输出显示
        start();
        printf("\n是否确认用此地图？\n");
        printf("1.确认 2.重新\n");
        scanf("%d",&i);
        if(i!=1)system("cls");//清屏
    }while(i!=1);
    //选择位置
    me=choose();
    LOAD://读档位置
    /******正式开始游戏*****/
    i=0;
    do{
        printf("\n现在是第%d回合\n",round++);
        //玩家的回合
        if(i==0)//i=0时玩家没有死
        mine();
        //AI的回合
        aiturn();
        //判断玩家是否死亡或胜利
        i=ifdeath();
        supply();//补充士兵
    }while(i!=2);
    return 0;
}

/*函数1：创建链表并随机初始化********************/
int create(int l)
{

    int place=1;//地点
    //attack=people/10+1; 攻击力（动态的）
    int team='A';//初始阵营
    /*读取进度*/
    int i;
	int size=sizeof(member);
	srand((unsigned)time(NULL));//随机数种子

	while(l==0){
        printf("是否读取进度？\n1.yes 2.no\n");
        scanf("%d",&i);
        if(i==1){
            load();return -1;
        }
        else system("cls");
        break;
    }
    /*链表创建并初始化，九个阵营随机性格*/

    entry=(member*)malloc(size);
    p=entry;
    for(place=1;place<=9;place++){
        p->place=place;
        p->character=rand()%3+1;
        p->people=rand()%41+10;
        p->attack=1;
        p->team=team;
        team++;
        if(place==9){
            p->next=NULL;
            break;
        }
        p->next=(member*)malloc(size);
        p=p->next;
    }
    return 0;
}

/*函数2：显示情况********************/
void start()
{
    member *p2,*flag;
	int i;
    p=p2=flag=entry;
    printf("************************************************************************\n");
    /*输出123 456*/
    for(i=1;i<=2;i++){
        flag=flag->next->next->next;
        for(p=p2;p->place!=flag->place;p=p->next){
            printf("%d.",p->place);
            printf("%c\t\t\t",p->team);
        }
        printf("\n");
        for(p=p2;p->place!=flag->place;p=p->next)
            printf("people:%d,attack:%d\t",p->people,p->people/10+1);
        printf("\n");
        for(p=p2;p->place!=flag->place;p=p->next){
            if(p->character==1) printf("鹰派  \t\t\t");
            else if(p->character==2) printf("鸽派  \t\t\t");
            else if(p->character==3) printf("复仇者\t\t\t");
        }
        printf("\n");printf("\n");
        p2=flag;
    }
    /*输出789*/
    for(p=p2;p!=NULL;p=p->next){
        printf("%d.",p->place);
        printf("%c\t\t\t",p->team);
    }
    printf("\n");
    for(p=p2;p!=NULL;p=p->next)
        printf("people:%d,attack:%d\t",p->people,p->people/10+1);
    printf("\n");
    for(p=p2;p!=NULL;p=p->next){
        if(p->character==1) printf("鹰派  \t\t\t");
        else if(p->character==2) printf("鸽派  \t\t\t");
        else if(p->character==3) printf("复仇者\t\t\t");
    }
    printf("\n");
    printf("************************************************************************\n");
}

/*函数3：选择玩家阵营********************/
char choose()
{
    char me=1;
    int i=0;
    CHOOSE:
    do{
        printf("请选择你的位置：");
        scanf("%d",&me);
    }while(me<1||me>9);
    me+='A'-1;//转换成阵营
    printf("以下是你的信息，请确认：\n");
    /************************************************/
    printf("***************\n");
    for(p=entry;p->team!=me;p=p->next);
    printf("%d.",p->place);
    printf("team:%c\n",p->team);
    printf("people:%d\n",p->people);
    if(p->character==1) printf("鹰派\n");
    else if(p->character==2) printf("鸽派\n");
    else if(p->character==3) printf("复仇\n");
    printf("***************\n");
    /************************************************/
    while(i!='1'){
        printf("1.确认 2.重新选\n");
        getchar();
        i=getchar();
        if('2'==i)goto CHOOSE;
        if('1'==i){
            system("cls");
            start();
        }
    }
    return me;
}

/*函数4：玩家的回合*********************/
void mine()
{
    int *a;//存储你的阵营位置
    int count=0;//控制a数组

    int enemy;//你要打的敌人的位置

    int b[5];//你可以控制去打的你自己的位置
    int countb=0;//控制b数组

    int myarmy;//你最终选择派遣的军队
    int flag;//是否合法标志
	int i;
    a=showme();//显示你的阵营位置
    //输入你要攻打的位置并判断可以用哪个来打
    save();
   while(1){
    printf("输入你要攻打的位置，0为放弃攻击，你只能攻打你的势力临近的位置，并且不能斜着打：");
    ENEMY:
    scanf("%d",&enemy);
    if(enemy!=0)
        printf("你可以使用以下位置的军队去攻打你刚刚选中的目标%d：\n",enemy);
    switch(enemy){
        case 0:CLEAR;
               printf("现在进入AI的回合，按任意键继续...\n");
               getch();
               return;
        case 1:for(count=0;a[count]!=-1;count++)
                 if(a[count]==2||a[count]==4)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 2:for(count=0;a[count]!=-1;count++)
                 if(a[count]==1||a[count]==3||a[count]==5)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 3:for(count=0;a[count]!=-1;count++)
                 if(a[count]==2||a[count]==6)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 4:for(count=0;a[count]!=-1;count++)
                 if(a[count]==1||a[count]==5||a[count]==7)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 5:for(count=0;a[count]!=-1;count++)
                 if(a[count]==2||a[count]==4||a[count]==6||a[count]==8)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 6:for(count=0;a[count]!=-1;count++)
                 if(a[count]==3||a[count]==5||a[count]==9)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 7:for(count=0;a[count]!=-1;count++)
                 if(a[count]==4||a[count]==8)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 8:for(count=0;a[count]!=-1;count++)
                 if(a[count]==5||a[count]==7||a[count]==9)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        case 9:for(count=0;a[count]!=-1;count++)
                 if(a[count]==6||a[count]==8)
                    b[countb++]=a[count],printf("%d ",a[count]);
                break;
        default:break;
    }
    b[countb]=-1;
    if(b[0]==-1){
        //这句用于覆盖enemy!=0时的输出
        for(i=1;i<=50;i++)printf("\b");
        printf("你攻打不了这里，请重选:                        \n");
        goto ENEMY;
    }
    flag=0;//是否合法标志
    do{
        printf("\n选择你要派遣的军队（输入位置）：");
        scanf("%d",&myarmy);
        for(countb=0;b[countb]!=-1;countb++){
            if(myarmy==b[countb]){
                flag=1;break;
            }
        }
        if(flag==0)printf("该位置不合法！\n");
    }while(flag!=1);
    CLEAR;
    printf("派遣成功！现在%d攻打%d\n",myarmy,enemy);
    fight(myarmy,enemy);//开打开打
    //更新阵营位置
    count=0;
    for(p=entry;p!=NULL;p=p->next){
        if(p->team==me){
            a[count++]=p->place;
        }
    }
    a[count]=-1;
    countb=0;
   }
}

/*函数5：显示玩家阵营位置*********************/
int *showme()
{
    int *a=(int *)malloc(10*sizeof(int));
    int count=0;
    printf("你的阵营：%c ",me);
    printf("你的阵营位置：");
    //找出所有位置输出并存储
    for(p=entry;p!=NULL;p=p->next){
        if(p->team==me){
            printf("%d ",p->place);
            a[count++]=p->place;
        }
    }
    a[count]=-1;
    printf("\n");
    return a;
}

/*函数6：战斗函数*********************/
void fight(int att,int def)
{
    int app;//攻方人数
    int fpp;//防守人数
    int acha;//a的性格 123鹰鸽复
    int bcha;//b的性格
    char ateam;//a的阵营
    char bteam;//b的阵营
    int judge;//用来判断是玩家进攻还是电脑进攻
    judge=0;
    //给攻方赋值
    for(p=entry;p->place!=att;p=p->next);
    app=p->people;
    if(app==0)return;
    acha=p->character;
    ateam=p->team;
    //给防守方赋值
    for(p=entry;p->place!=def;p=p->next);
    fpp=p->people;
    bcha=p->character;
    bteam=p->team;
    //判断是谁发起的进攻，电脑还是玩家
    if(ateam==me)judge=-1;
    //开始战斗
    while(app>0&&fpp>0){
        //攻方攻击
        fpp-=app/10+1;
        if(acha==1){fpp-=app/10+1;fpp-=app/10+1;}//鹰派三倍
        if(acha==3){fpp-=app/10+1;}//复仇者两倍
        if(fpp<=0)break;//判断是否胜利
        //防守方反击
        app-=fpp/10+1;
        if(bcha==2){app-=fpp/10+1;app-=fpp/10+1;}//鸽派三倍
        if(bcha==3){app-=fpp/10+1;}//复仇者两倍
    }
    //定位失败者并修改数据
    printf("%d(%c)攻打了%d(%c),",att,ateam,def,bteam);
    if(app<=0){
        p->people=fpp/2;
        for(p=entry;p->place!=att;p=p->next);
        p->people=fpp/2;
        p->character=bcha;
        p->team=bteam;
        printf("%d(%c)胜利！\n",def,bteam);
    }

    else if(fpp<=0){
        p->people=app/2;
        p->character=acha;
        p->team=ateam;
        for(p=entry;p->place!=att;p=p->next);
        p->people=app/2;
        printf("%d(%c)胜利！\n",att,ateam);
    }
    Sleep(TIME);
    if(judge==-1)
    {
        Sleep(TIME);//玩家攻打后延长时间加倍
        CLEAR;
    }
}

/*函数7：AI的回合*********************/
void aiturn()
{
    member *p2;//操作指针，避免与p重复
    printf("**********************\n");
    printf("现在是AI的行动时间...\n");
    printf("**********************\n");
    for(p2=entry;p2!=NULL;p2=p2->next){
        if(p2->team==me)continue;//跳过玩家阵营
        //判断可攻打位置与是否攻打
        aithink(p2->character,p2->place);
        printf("第%d个ai结束了\n",p2->place);
        printf("**********************\n");
        Sleep(TIME);
    }
    printf("当前形势：\n");
    start();

}

/*函数8：AI的行为模式*********************/
void aithink(int character,int place)
{
    int people[9];//存储各位置人数
    char team[9];//存储各位置阵营
    int i=0;//控制循环变量
    double j;//控制三种性格的行为
    /*赋值*/
    for(p=entry;p!=NULL;p=p->next,i++){
        people[i]=p->people;
        team[i]=p->team;
    }
    /*j越大，入侵倾向越大*/
    if(character==1)j=2;
    if(character==2)j=0.5;
    if(character==3)j=1;
    /*行为方式*/
        if(place==1){//2.4
            i=2;
                aithink2(i,j,team,people,place);
            i=4;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==3){//2.6
            i=2;
                aithink2(i,j,team,people,place);
            i=6;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==7){//4.8
            i=4;
                aithink2(i,j,team,people,place);
            i=8;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==9){//6.8
            i=6;
                aithink2(i,j,team,people,place);
            i=8;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==2){//1.3.5
            i=1;
                aithink2(i,j,team,people,place);
            i=3;
                aithink2(i,j,team,people,place);
            i=5;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==4){//1.5.7
            i=1;
                aithink2(i,j,team,people,place);
            i=5;
                aithink2(i,j,team,people,place);
            i=7;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==6){//3.5.9
            i=3;
                aithink2(i,j,team,people,place);
            i=5;
                aithink2(i,j,team,people,place);
            i=9;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==8){//5.7.9
            i=5;
                aithink2(i,j,team,people,place);
            i=7;
                aithink2(i,j,team,people,place);
            i=9;
                aithink2(i,j,team,people,place);
            return;
        }
        if(place==5){//2.4.6.8
            i=2;
                aithink2(i,j,team,people,place);
            i=4;
                aithink2(i,j,team,people,place);
            i=6;
                aithink2(i,j,team,people,place);
            i=8;
                aithink2(i,j,team,people,place);
            return;
        }
}

/*函数8-2：用于优化函数aithink的代码结构*/
void aithink2(int i,double j,char *team,int *people,int place){
    if(people[place-1]*j>=people[i-1])//人数
        if(team[place-1]!=team[i-1])//阵营
                fight(place,i);
}

/*函数9：判断玩家是否死亡或胜利***************/
int ifdeath()
{
    int i=0;
    /*赢了*/
    for(p=entry;p!=NULL;p=p->next){
        if(p->team!=me){
            i=1;break;
        }
    }
    if(i==0){
        printf("恭喜你获得游戏胜利！\n");
        printf("恭喜你获得游戏胜利！\n");
        printf("恭喜你获得游戏胜利！\n");
        system("pause");
        exit(0);
    }
    /*没死也没赢*/
    for(p=entry;p!=NULL;p=p->next){
        if(p->team==me)return 0;
    }
    /*死了*/
    printf("你的阵营灭亡了...\n1.继续观战 2.退出游戏\n");
    do{
        scanf("%d",&i);
    }while(i!=1&&i!=2);
    if(i==1){TIME=200;return 1;}//加快游戏速度进行观战
    if(i==2)exit(1);//直接退出游戏
    return 0;
}

/*函数10：每回合结束自动补兵*************/
void supply()
{
    printf("现在进入下一回合，按任意键继续...\n");
    getch();
    system("cls");
    for(p=entry;p!=NULL;p=p->next){
        p->people+=p->people/5+1;
        /*if(p->team==me)//作弊
            p->people+=100;*/
    }
    start();
    printf("每个位置补充20%%士兵\n");

}

/*函数11：存档函数（保存链表，回合数，玩家阵营）*************/
void save()
{
    int i;
    FILE *fp;
	printf("自动存档中");
    for(i=1;i<=3;i++){
        printf(".");
        Sleep(TIME/3);
    }
    printf("\n");

    fp=fopen("data.txt","w");
    if(fp==NULL)printf("File can't open!\n"),system("pause"),exit(0);//检查文件是否打开
    fprintf(fp,"%d %c\n",round,me);
    for(p=entry;p!=NULL;p=p->next){
        fprintf(fp,"%d %d %d %d %c ",p->place,p->character,p->people,p->attack,p->team);
    }
    /*关闭文件*/
    if(fclose(fp)){
        printf("关闭失败！\n");
        system("pause");
        exit(1);
    }else printf("save successfully!\n");
}

/*函数12：读档函数*************/
void load()
{
    FILE *fp2;
    int size=sizeof(member);
    int i;
    fp2=fopen("data.txt","r");
    if(fp2==NULL){printf("读档失败！\n");system("pause");exit(0);}
    fscanf(fp2,"%d %c\n",&round,&me);
    entry=(member*)malloc(size);
    p=entry;
    for(i=1;i<=9;i++){
        fscanf(fp2,"%d %d %d %d %c ",&p->place,&p->character,&p->people,&p->attack,&p->team);
        if(i!=9)
            p->next=(member*)malloc(size);
        else p->next=NULL;
        p=p->next;
    }
    /*关闭文件*/
    if(fclose(fp2)){
        printf("File can't be closed！\n");
    }
    else printf("File read successfully!\n");
    round--;
}

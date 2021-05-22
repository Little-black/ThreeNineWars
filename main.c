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
int rounds=1;//全局变量：回合数
int TIME=800;//全局变量：游戏速度

/*函数声明***************************************/
/*1*/int create(int l);//创建链表并随机初始化
/*2*/void start();//显示情况
/*3*/char choose();//选择玩家阵营
/*4*/void mine();//玩家的回合.
/*5*/int *showme();//显示玩家阵营位置.
/*6*/void fight(int att,int def);//战斗或增援函数
/*7*/void aiturn();//AI的回合.
/*8*/int aithink(int character,int place);//AI的行为模式
/*8-2*/int aithink2(int i,double j,char *team,int *people,int place);//aithink函数的内定义函数，用于优化程序结构
/*9*/int ifdeath();//判断玩家是否死亡或胜利
/*10*/void supply();//每回合结束自动补兵
/*11*/void save();//存档函数（保存链表，回合数，玩家阵营）
/*12*/void load();//读档函数

/*主函数*****************************************/
int main(void)
{
    int i=0;//万能的临时变量
	system("color 0E");//改变窗口颜色
	system("chcp 65001");//改变控制台编码
    while(1){//重新开始
        rounds = 1;//重置回合数
        TIME=800;//重置游戏速度
        system("cls");//清屏
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
            i = getch();
            if(i!='1')system("cls");//清屏
        }while(i!='1');
        //选择位置
        me=choose();
        LOAD://读档位置
        /******正式开始游戏循环*****/
        i=0;
        do{
            printf("\n现在是第%d回合\n",rounds++);
            //玩家的回合
            if(i==0)//i=0时玩家没有死
            mine();
            //AI的回合
            aiturn();
            //判断玩家是否死亡或胜利
            i=ifdeath();
            if(i!=2){
                supply();//补充士兵
            }
        }while(i!=2);
    }

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
        printf("是否开始新游戏（输入数字进行选择）？\n1.是，新的征程\n2.否，旧的回忆\n");
        i = getch();
        if(i=='2'){
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
            printf("士兵数:%d,战斗力:%d\t",p->people,p->people/10+1);
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
        printf("士兵数:%d,战斗力:%d\t",p->people,p->people/10+1);
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
        me = getch();
        printf("%c\n",me);
    }while(me<'1'||me>'9');
    me+='A'-'1';//转换成阵营
    printf("以下是你的信息，请确认：\n");
    /************************************************/
    printf("***************\n");
    for(p=entry;p->team!=me;p=p->next);
    printf("%d.",p->place);
    printf("阵营:%c\n",p->team);
    printf("士兵数:%d\n",p->people);
    printf("战斗力：%d\n",p->people/10+1);
    if(p->character==1) printf("鹰派\n");
    else if(p->character==2) printf("鸽派\n");
    else if(p->character==3) printf("复仇\n");
    printf("***************\n");
    /************************************************/
    while(i!='1'){
        printf("1.确认 2.重新选\n");
        i=getch();
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
    int ourarmy;//你将要行动的我军位置
    int b[5];//你可以去打或增援的位置，最多上下左右4个，末尾加个-1
    int countb=0;//控制b数组
    int enemy;//你最终选择攻打的敌人
    int flag;//目的位置是否合法标志
	int i;

    a=showme();//显示你的阵营位置
    save();

    //玩家输入控制军队从哪里到哪里（from to）
    while(1){
        //玩家选择from
        printf("\n请选择你要行动的军队（输入位置），输入0进入下一回合：");
        while(1){
            ourarmy = getch();
            printf("%c\n",ourarmy);
            //判断是否为0
            if(ourarmy=='0'){break;}
            //判断选择是否确实是自己的军队
            for(count=0;a[count]!=-1;count++){
                if(a[count]==ourarmy-'0'){break;}
            }
            //所选不是自己的军队
            if(a[count]==-1){
                printf("你选择的不是自己的军队，请重选：");
                continue;
            }else{
                break;
            }
        }
        if(ourarmy!='0'){
            printf("你可以攻打或增援以下位置（你选择位置临近的上下左右）：");
        }
        //显示并存储合法的选项
        switch(ourarmy){
            //放弃进攻
            case '0':CLEAR;
                    printf("现在进入AI的回合，按任意键继续...\n");
                    getch();
                    return;
            case '1':printf("2 4\n");
                    b[countb++]=2;b[countb++]=4;
                    break;
            case '2':printf("1 3 5\n");
                    b[countb++]=1;b[countb++]=3;b[countb++]=5;
                    break;
            case '3':printf("2 6\n");
                    b[countb++]=2;b[countb++]=6;
                    break;
            case '4':printf("1 5 7\n");
                    b[countb++]=1;b[countb++]=5;b[countb++]=7;
                    break;
            case '5':printf("2 4 6 8\n");
                    b[countb++]=2;b[countb++]=4;b[countb++]=6;b[countb++]=8;
                    break;
            case '6':printf("3 5 9\n");
                    b[countb++]=3;b[countb++]=5;b[countb++]=9;
                    break;
            case '7':printf("4 8\n");
                    b[countb++]=4;b[countb++]=8;
                    break;
            case '8':printf("5 7 9\n");
                    b[countb++]=5;b[countb++]=7;b[countb++]=9;
                    break;
            case '9':printf("6 8\n");
                    b[countb++]=6;b[countb++]=8;
                    break;
            default:break;
        }
        b[countb]=-1;
        flag=0;//目的位置是否合法
        do{
            printf("请选择你要攻打或增援的位置：");
            enemy = getch();
            printf("%c\n",enemy);
            for(countb=0;b[countb]!=-1;countb++){
                if(enemy==(b[countb]+'0')){
                    flag=1;break;
                }
            }
            if(flag==0){printf("该位置不合法，你只能选择攻打或增援当前军队临近上下左右的位置！\n");}
        }while(flag!=1);
        CLEAR;
        printf("%c前往%c的军队派遣成功！\n",ourarmy,enemy);
        fight(ourarmy,enemy);//开打开打
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
    printf("你阵营的军队位置：");
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

/*函数6：战斗或增援函数*********************/
void fight(int att,int def)
{
    int app;//攻方人数
    int fpp;//防守人数
    int acha;//a的性格 123鹰鸽复
    int bcha;//b的性格
    char ateam;//a的阵营
    char bteam;//b的阵营
    int judge;//用来判断是玩家进攻还是电脑进攻
    int temp;//用于确定人数的临时变量
    judge=0;
    //参数类型转换(历史遗留问题，有时att、def是字符，有时是数字)
    if(att>9&&def>9){
        att = att-'0';
        def = def-'0';
    }
    //给攻方赋值
    for(p=entry;p->place!=att;p=p->next);//定位攻方
    app=p->people;
    if(app==0)return;//判断是否攻击失败
    acha=p->character;
    ateam=p->team;
    //给防守方赋值
    for(p=entry;p->place!=def;p=p->next);
    fpp=p->people;
    bcha=p->character;
    bteam=p->team;
    //判断是谁发起的进攻，电脑还是玩家
    if(ateam==me)judge=-1;
    //判断是否是增援
    if(ateam==bteam){
        printf("%d(%c)派军增援了%d(%c)\n",att,ateam,def,bteam);
        temp = app/2;
        app = app-temp;
        fpp = fpp+temp;
        p->people = fpp;
        for(p=entry;p->place!=att;p=p->next);
        p->people = app;
        printf("增援完成，增援人数为%d人！\n",temp);
    }
    //开始战斗
    else{
        while(app>0&&fpp>0){
            //攻方攻击
            fpp-=app/10+1;
            if(acha==1){fpp-=app/10+1;fpp-=app/10+1;}//鹰派三倍
            if(acha==3){fpp-=app/10+1;}//复仇者两倍
            if(fpp<=0)break;//判断是否攻击胜利
            //防守方反击
            app-=fpp/10+1;
            if(bcha==2){app-=fpp/10+1;app-=fpp/10+1;app-=fpp/10+1;app-=fpp/10+1;}//鸽派五倍
            if(bcha==3){app-=fpp/10+1;}//复仇者两倍
        }
        //定位失败者并修改数据
        printf("%d(%c)攻打了%d(%c),",att,ateam,def,bteam);
        //防守方胜利
        if(app<=0){
            //修改防守方数据
            temp=fpp/2;
            p->people=fpp-temp;
            //修改攻击方数据，转化为防守方阵营
            for(p=entry;p->place!=att;p=p->next);
            p->people=temp;
            p->character=bcha;
            p->team=bteam;
            printf("%d(%c)胜利！\n",def,bteam);
        }
        //攻击方胜利
        else if(fpp<=0){
            //修改防守方数据，转化为攻击方阵营
            temp=app/2;
            p->people=app-temp;
            p->character=acha;
            p->team=ateam;
            //修改攻击方数据
            for(p=entry;p->place!=att;p=p->next);
            p->people=temp;
            printf("%d(%c)胜利！\n",att,ateam);
        }
    }
    //结束
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
    int team = 'A'; //当前行动阵营
    int expand = 1; //当前阵营是否还要行动，1是，0否
    int temp = 0; //万能的临时变量
    printf("**********************\n");
    printf("现在是AI的行动时间...\n");
    printf("**********************\n");

    //遍历所有阵营
    for(team='A';team!='I'+1;team++){
        //跳过玩家阵营
        if(team==me){continue;}
        //非玩家阵营开始行动
        printf("阵营%c开始行动！\n",team);
        //扩张欲望：true
        expand = 1;
        while(expand){
            //循环结束条件：该阵营没有位置需要行动了
            expand = 0;
            //遍历该阵营所有的位置
            for(p2=entry;p2!=NULL;p2=p2->next){
                //跳过不属于当前阵营的位置
                if(p2->team!=team){continue;}
                //属于当前阵营的位置，AI行动
                temp = aithink(p2->character,p2->place);
                //当前AI某些位置有所行动
                if(temp==1){
                    expand = 1;
                }
            }
        }
        printf("阵营%c的回合结束了\n",team);
        printf("**************************\n");
        Sleep(TIME);
    }

    printf("当前形势：\n");
    start();

}

/*函数8：AI的行为模式*********************/
int aithink(int character,int place)
{
    int people[9];//存储各位置人数
    char team[9];//存储各位置阵营
    int i=0;//控制循环变量
    double j;//控制三种性格的行为
    int ifaction=0;//当前位置是否存在有效行动
    int temp=0;//万能的临时变量

    /*赋值记录所有位置的人数和阵营以供AI进行判断*/
    for(p=entry;p!=NULL;p=p->next,i++){
        people[i]=p->people;
        team[i]=p->team;
    }
    /*性格系数，123鸽鹰复，j越小，入侵倾向越大*/
    if(character==1)j=1.5; //鸽派以多打少
    if(character==2)j=0.75; //鹰派以少胜多
    if(character==3)j=1;   //复仇者势均力敌

    /*行为方式：根据自身位置逐个判断临近的位置，有效行动后返回1，否则返回0*/
        if(place==1){//2.4
            i=2;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=4;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==3){//2.6
            i=2;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=6;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==7){//4.8
            i=4;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=8;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==9){//6.8
            i=6;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=8;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==2){//1.3.5
            i=1;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=3;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=5;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==4){//1.5.7
            i=1;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=5;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=7;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==6){//3.5.9
            i=3;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=5;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=9;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==8){//5.7.9
            i=5;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=7;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=9;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
        if(place==5){//2.4.6.8
            i=2;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=4;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=6;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            i=8;
                temp=aithink2(i,j,team,people,place);
                if(temp==1){ifaction=1;}
            return ifaction;
        }
}

/*函数8-2：用于优化函数aithink的代码结构*/
int aithink2(int i,double j,char *team,int *people,int place){
    int ifaction=0;//是否有效行动
    //place：AI当前操纵，i：AI选择的目标。放在数组中时要减1
    if(people[place-1]*j>=people[i-1]){//己方人数乘以性格系数大于目标
        if(team[place-1]!=team[i-1]){//阵营不同
            fight(place,i);//开打
            ifaction=1;
        }
    }else{//己方人数小于目标
        if(team[place-1]==team[i-1]){//阵营相同
            if(people[place-1]>27){ //人数大于27
                fight(place,i);//增援
                ifaction=1;
            }
        }
    }
    //有效行动后返回1，否则返回0
    return ifaction;
}

/*函数9：判断玩家是否死亡或胜利***************/
int ifdeath()
{
    int i=0;
    /*检测到非玩家阵营，赋值i=1*/
    for(p=entry;p!=NULL;p=p->next){
        if(p->team!=me){
            i=1;break;
        }
    }
    /*未检测到非玩家阵营，玩家胜利*/
    if(i==0){
        printf("恭喜你获得游戏胜利！\n");
        printf("恭喜你获得游戏胜利！\n");
        printf("恭喜你获得游戏胜利！\n");
        printf("秦王扫六合,虎视何雄哉!\n1.重新开始 2.退出游戏\n");
        do{
            i=getch();
        }while(i!='1'&&i!='2');
        if(i=='1'){return 2;}//重新开始游戏
        if(i=='2'){exit(0);}
    }
    /*检测到玩家阵营，说明玩家没有输，退出函数*/
    for(p=entry;p!=NULL;p=p->next){
        if(p->team==me)return 0;
    }
    /*死了*/
    printf("你的阵营灭亡了...\n胜败乃兵家常事，大侠请重新来过！\n1.继续观战 2.重新开始\n");
    do{
        i=getch();
    }while(i!='1'&&i!='2');
    if(i=='1'){TIME=200;return 1;}//加快游戏速度进行观战
    if(i=='2'){return 2;};//重新开始游戏
    return 0;
}

/*函数10：每回合结束自动补兵*************/
void supply()
{
    printf("现在进入下一回合，按任意键继续...\n");
    getch();
    system("cls");
    for(p=entry;p!=NULL;p=p->next){
        if(p->people > 100){
            p->people += 0;
        }else{
            p->people += p->people * (100-p->people)/100 + 1;
        }
        /*if(p->team==me)//作弊
            p->people+=100;*/
    }
    start();
    printf("每个位置补充一定量的士兵，位置原士兵越多则新补充士兵相对越少\n");

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
    if(fp==NULL)printf("无法保存存档文件，请检查程序目录权限!\n"),system("pause"),exit(0);//检查文件是否打开
    fprintf(fp,"%d %c\n",rounds,me);
    for(p=entry;p!=NULL;p=p->next){
        fprintf(fp,"%d %d %d %d %c ",p->place,p->character,p->people,p->attack,p->team);
    }
    /*关闭文件*/
    if(fclose(fp)){
        printf("保存进度失败，请检查存档文件是否被占用！\n");
        system("pause");
        exit(1);
    }else printf("进度存档成功!\n");
}

/*函数12：读档函数*************/
void load()
{
    FILE *fp2;
    int size=sizeof(member);
    int i;
    fp2=fopen("data.txt","r");
    if(fp2==NULL){printf("读档失败，没有存档文件！\n");system("pause");exit(0);}
    fscanf(fp2,"%d %c\n",&rounds,&me);
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
        printf("读取进度失败，请检查存档文件是否被占用！\n");
    }
    else printf("读取进度成功!\n");
    rounds--;
}

/***********************************************
��������bug��
1.ai���ж���ʱ��û�����ͻ�����
2.ai�Լ����Լ�
***********************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#define CLEAR system("cls");start();showme()//�����������Ҫ��Ϣ

/*������λ�ṹmember***********************************/
typedef struct member{
    int place;//�ص�1-9
    int character;//�Ը�1.2.3:ӥ�븴
    int people;//����
    int attack;//������(��̬��)
    char team;//��Ӫabcdefghi(�ĳ��˴�д)
    struct member *next;
}member;
member *p;//ȫ�ֱ���������ָ��
member *entry;//ȫ�ֱ��������ָ��
char me;//ȫ�ֱ����������Ӫ
int round=1;//ȫ�ֱ������غ���
int TIME=800;//ȫ�ֱ�������Ϸ�ٶ�

/*��������***************************************/
/*1*/int create(int l);//�������������ʼ��
/*2*/void start();//��ʾ���
/*3*/char choose();//ѡ�������Ӫ
/*4*/void mine();//��ҵĻغ�.
/*5*/int *showme();//��ʾ�����Ӫλ��.
/*6*/void fight(int att,int def);//ս������
/*7*/void aiturn();//AI�Ļغ�.
/*8*/void aithink(int character,int place);//AI����Ϊģʽ
/*8-2*/void aithink2(int i,double j,char *team,int *people,int place);//aithink�������ڶ��庯���������Ż�����ṹ
/*9*/int ifdeath();//�ж�����Ƿ�������ʤ��
/*10*/void supply();//ÿ�غϽ����Զ�����
/*11*/void save();//�浵���������������غ����������Ӫ��
/*12*/void load();//��������

/*������*****************************************/
int main(void)
{
    int i=0;//���ܵ���ʱ����
	system("color 0E");//�ı䴰����ɫ

    //������ͼ
    do{
        if(create(i)==-1){
            start();
            goto LOAD;
        }
        //�����ʾ
        start();
        printf("\n�Ƿ�ȷ���ô˵�ͼ��\n");
        printf("1.ȷ�� 2.����\n");
        scanf("%d",&i);
        if(i!=1)system("cls");//����
    }while(i!=1);
    //ѡ��λ��
    me=choose();
    LOAD://����λ��
    /******��ʽ��ʼ��Ϸ*****/
    i=0;
    do{
        printf("\n�����ǵ�%d�غ�\n",round++);
        //��ҵĻغ�
        if(i==0)//i=0ʱ���û����
        mine();
        //AI�Ļغ�
        aiturn();
        //�ж�����Ƿ�������ʤ��
        i=ifdeath();
        supply();//����ʿ��
    }while(i!=2);
    return 0;
}

/*����1���������������ʼ��********************/
int create(int l)
{

    int place=1;//�ص�
    //attack=people/10+1; ����������̬�ģ�
    int team='A';//��ʼ��Ӫ
    /*��ȡ����*/
    int i;
	int size=sizeof(member);
	srand((unsigned)time(NULL));//���������

	while(l==0){
        printf("�Ƿ��ȡ���ȣ�\n1.yes 2.no\n");
        scanf("%d",&i);
        if(i==1){
            load();return -1;
        }
        else system("cls");
        break;
    }
    /*����������ʼ�����Ÿ���Ӫ����Ը�*/

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

/*����2����ʾ���********************/
void start()
{
    member *p2,*flag;
	int i;
    p=p2=flag=entry;
    printf("************************************************************************\n");
    /*���123 456*/
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
            if(p->character==1) printf("ӥ��  \t\t\t");
            else if(p->character==2) printf("����  \t\t\t");
            else if(p->character==3) printf("������\t\t\t");
        }
        printf("\n");printf("\n");
        p2=flag;
    }
    /*���789*/
    for(p=p2;p!=NULL;p=p->next){
        printf("%d.",p->place);
        printf("%c\t\t\t",p->team);
    }
    printf("\n");
    for(p=p2;p!=NULL;p=p->next)
        printf("people:%d,attack:%d\t",p->people,p->people/10+1);
    printf("\n");
    for(p=p2;p!=NULL;p=p->next){
        if(p->character==1) printf("ӥ��  \t\t\t");
        else if(p->character==2) printf("����  \t\t\t");
        else if(p->character==3) printf("������\t\t\t");
    }
    printf("\n");
    printf("************************************************************************\n");
}

/*����3��ѡ�������Ӫ********************/
char choose()
{
    char me=1;
    int i=0;
    CHOOSE:
    do{
        printf("��ѡ�����λ�ã�");
        scanf("%d",&me);
    }while(me<1||me>9);
    me+='A'-1;//ת������Ӫ
    printf("�����������Ϣ����ȷ�ϣ�\n");
    /************************************************/
    printf("***************\n");
    for(p=entry;p->team!=me;p=p->next);
    printf("%d.",p->place);
    printf("team:%c\n",p->team);
    printf("people:%d\n",p->people);
    if(p->character==1) printf("ӥ��\n");
    else if(p->character==2) printf("����\n");
    else if(p->character==3) printf("����\n");
    printf("***************\n");
    /************************************************/
    while(i!='1'){
        printf("1.ȷ�� 2.����ѡ\n");
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

/*����4����ҵĻغ�*********************/
void mine()
{
    int *a;//�洢�����Ӫλ��
    int count=0;//����a����

    int enemy;//��Ҫ��ĵ��˵�λ��

    int b[5];//����Կ���ȥ������Լ���λ��
    int countb=0;//����b����

    int myarmy;//������ѡ����ǲ�ľ���
    int flag;//�Ƿ�Ϸ���־
	int i;
    a=showme();//��ʾ�����Ӫλ��
    //������Ҫ�����λ�ò��жϿ������ĸ�����
    save();
   while(1){
    printf("������Ҫ�����λ�ã�0Ϊ������������ֻ�ܹ�����������ٽ���λ�ã����Ҳ���б�Ŵ�");
    ENEMY:
    scanf("%d",&enemy);
    if(enemy!=0)
        printf("�����ʹ������λ�õľ���ȥ������ո�ѡ�е�Ŀ��%d��\n",enemy);
    switch(enemy){
        case 0:CLEAR;
               printf("���ڽ���AI�Ļغϣ������������...\n");
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
        //������ڸ���enemy!=0ʱ�����
        for(i=1;i<=50;i++)printf("\b");
        printf("�㹥�����������ѡ:                        \n");
        goto ENEMY;
    }
    flag=0;//�Ƿ�Ϸ���־
    do{
        printf("\nѡ����Ҫ��ǲ�ľ��ӣ�����λ�ã���");
        scanf("%d",&myarmy);
        for(countb=0;b[countb]!=-1;countb++){
            if(myarmy==b[countb]){
                flag=1;break;
            }
        }
        if(flag==0)printf("��λ�ò��Ϸ���\n");
    }while(flag!=1);
    CLEAR;
    printf("��ǲ�ɹ�������%d����%d\n",myarmy,enemy);
    fight(myarmy,enemy);//���򿪴�
    //������Ӫλ��
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

/*����5����ʾ�����Ӫλ��*********************/
int *showme()
{
    int *a=(int *)malloc(10*sizeof(int));
    int count=0;
    printf("�����Ӫ��%c ",me);
    printf("�����Ӫλ�ã�");
    //�ҳ�����λ��������洢
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

/*����6��ս������*********************/
void fight(int att,int def)
{
    int app;//��������
    int fpp;//��������
    int acha;//a���Ը� 123ӥ�븴
    int bcha;//b���Ը�
    char ateam;//a����Ӫ
    char bteam;//b����Ӫ
    int judge;//�����ж�����ҽ������ǵ��Խ���
    judge=0;
    //��������ֵ
    for(p=entry;p->place!=att;p=p->next);
    app=p->people;
    if(app==0)return;
    acha=p->character;
    ateam=p->team;
    //�����ط���ֵ
    for(p=entry;p->place!=def;p=p->next);
    fpp=p->people;
    bcha=p->character;
    bteam=p->team;
    //�ж���˭����Ľ��������Ի������
    if(ateam==me)judge=-1;
    //��ʼս��
    while(app>0&&fpp>0){
        //��������
        fpp-=app/10+1;
        if(acha==1){fpp-=app/10+1;fpp-=app/10+1;}//ӥ������
        if(acha==3){fpp-=app/10+1;}//����������
        if(fpp<=0)break;//�ж��Ƿ�ʤ��
        //���ط�����
        app-=fpp/10+1;
        if(bcha==2){app-=fpp/10+1;app-=fpp/10+1;}//��������
        if(bcha==3){app-=fpp/10+1;}//����������
    }
    //��λʧ���߲��޸�����
    printf("%d(%c)������%d(%c),",att,ateam,def,bteam);
    if(app<=0){
        p->people=fpp/2;
        for(p=entry;p->place!=att;p=p->next);
        p->people=fpp/2;
        p->character=bcha;
        p->team=bteam;
        printf("%d(%c)ʤ����\n",def,bteam);
    }

    else if(fpp<=0){
        p->people=app/2;
        p->character=acha;
        p->team=ateam;
        for(p=entry;p->place!=att;p=p->next);
        p->people=app/2;
        printf("%d(%c)ʤ����\n",att,ateam);
    }
    Sleep(TIME);
    if(judge==-1)
    {
        Sleep(TIME);//��ҹ�����ӳ�ʱ��ӱ�
        CLEAR;
    }
}

/*����7��AI�Ļغ�*********************/
void aiturn()
{
    member *p2;//����ָ�룬������p�ظ�
    printf("**********************\n");
    printf("������AI���ж�ʱ��...\n");
    printf("**********************\n");
    for(p2=entry;p2!=NULL;p2=p2->next){
        if(p2->team==me)continue;//���������Ӫ
        //�жϿɹ���λ�����Ƿ񹥴�
        aithink(p2->character,p2->place);
        printf("��%d��ai������\n",p2->place);
        printf("**********************\n");
        Sleep(TIME);
    }
    printf("��ǰ���ƣ�\n");
    start();

}

/*����8��AI����Ϊģʽ*********************/
void aithink(int character,int place)
{
    int people[9];//�洢��λ������
    char team[9];//�洢��λ����Ӫ
    int i=0;//����ѭ������
    double j;//���������Ը����Ϊ
    /*��ֵ*/
    for(p=entry;p!=NULL;p=p->next,i++){
        people[i]=p->people;
        team[i]=p->team;
    }
    /*jԽ����������Խ��*/
    if(character==1)j=2;
    if(character==2)j=0.5;
    if(character==3)j=1;
    /*��Ϊ��ʽ*/
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

/*����8-2�������Ż�����aithink�Ĵ���ṹ*/
void aithink2(int i,double j,char *team,int *people,int place){
    if(people[place-1]*j>=people[i-1])//����
        if(team[place-1]!=team[i-1])//��Ӫ
                fight(place,i);
}

/*����9���ж�����Ƿ�������ʤ��***************/
int ifdeath()
{
    int i=0;
    /*Ӯ��*/
    for(p=entry;p!=NULL;p=p->next){
        if(p->team!=me){
            i=1;break;
        }
    }
    if(i==0){
        printf("��ϲ������Ϸʤ����\n");
        printf("��ϲ������Ϸʤ����\n");
        printf("��ϲ������Ϸʤ����\n");
        system("pause");
        exit(0);
    }
    /*û��ҲûӮ*/
    for(p=entry;p!=NULL;p=p->next){
        if(p->team==me)return 0;
    }
    /*����*/
    printf("�����Ӫ������...\n1.������ս 2.�˳���Ϸ\n");
    do{
        scanf("%d",&i);
    }while(i!=1&&i!=2);
    if(i==1){TIME=200;return 1;}//�ӿ���Ϸ�ٶȽ��й�ս
    if(i==2)exit(1);//ֱ���˳���Ϸ
    return 0;
}

/*����10��ÿ�غϽ����Զ�����*************/
void supply()
{
    printf("���ڽ�����һ�غϣ������������...\n");
    getch();
    system("cls");
    for(p=entry;p!=NULL;p=p->next){
        p->people+=p->people/5+1;
        /*if(p->team==me)//����
            p->people+=100;*/
    }
    start();
    printf("ÿ��λ�ò���20%%ʿ��\n");

}

/*����11���浵���������������غ����������Ӫ��*************/
void save()
{
    int i;
    FILE *fp;
	printf("�Զ��浵��");
    for(i=1;i<=3;i++){
        printf(".");
        Sleep(TIME/3);
    }
    printf("\n");

    fp=fopen("data.txt","w");
    if(fp==NULL)printf("File can't open!\n"),system("pause"),exit(0);//����ļ��Ƿ��
    fprintf(fp,"%d %c\n",round,me);
    for(p=entry;p!=NULL;p=p->next){
        fprintf(fp,"%d %d %d %d %c ",p->place,p->character,p->people,p->attack,p->team);
    }
    /*�ر��ļ�*/
    if(fclose(fp)){
        printf("�ر�ʧ�ܣ�\n");
        system("pause");
        exit(1);
    }else printf("save successfully!\n");
}

/*����12����������*************/
void load()
{
    FILE *fp2;
    int size=sizeof(member);
    int i;
    fp2=fopen("data.txt","r");
    if(fp2==NULL){printf("����ʧ�ܣ�\n");system("pause");exit(0);}
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
    /*�ر��ļ�*/
    if(fclose(fp2)){
        printf("File can't be closed��\n");
    }
    else printf("File read successfully!\n");
    round--;
}

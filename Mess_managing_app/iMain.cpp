# include "iGraphics.h"
#define HEIGHT 720
#define WIDTH 1280
#define button_color 54, 176, 217
#define button_color_2 214, 49, 99
#define gif_font GLUT_BITMAP_HELVETICA_18
/*
	function iDraw() is called again and again by the system.

	*/
char str[1000]={0},str2[1000]={0};
char ext[5]=".txt";
int len,bck_scrn=-1,bck_wrt=-1,new_mess=0,new_mess_2=0,new_login=0,new_meal=0,nn,breakfast=0,lunch=0,dinner=0,done=0,day,payment_clear=0,meal_allocate=0,egg=0;
int xg=0,yg=0,wrt=0;
int back_wrt[100];
FILE *fp[1000];
FILE *fp_list;
typedef struct{
    int x;
    int y;
}bac;
bac back_scrn[100];

void center(char* line,int y)
{
    iSetColor(0,0,0);
    int x = WIDTH/2 - strlen(line)*6;
    iText(x,y, line, GLUT_BITMAP_TIMES_ROMAN_24);
}

void left(char* line, int y)
{
    iSetColor(0,0,0);
    int x=0;
    iText(x,y,line,GLUT_BITMAP_TIMES_ROMAN_24);
}

void back_button()
{
    iSetColor(237, 73, 62);
    iFilledRectangle(10,690,40,20);
    iSetColor(255,255,255);
    iText(15,694,"BACK");
}

void show_gif()
{
    static int gif=0;
    iSetColor(0,0,0);
    if(gif>=0 && gif<=500){
        iText(550,550,"Mess",gif_font);
        iText(650,650,"App",gif_font);
    }
    else if(gif>500 && gif<=1000){
        iText(550,600,"Mess",gif_font);
        iText(650,600,"App",gif_font);
    }
    else if(gif>1000 && gif<1500){
        iText(550,650,"Mess",gif_font);
        iText(650,550,"App",gif_font);
    }
    else{
        gif=0;
    }
    gif++;
}

void show_date()
{
    struct tm *date;
	time_t ltime;
	time(&ltime);
	date=localtime(&ltime);
	char *st=asctime(date);
	st[strlen(st)-1]=0;
	iSetColor(0,0,0);
	iText(1050,700,st);
}

void set_bg1()
{
    iSetColor(3, 252, 144);
    iFilledRectangle(0,0,320,720);
    iFilledRectangle(640,0,320,720);
    iSetColor(210, 247, 252);
    iFilledRectangle(320,0,320,720);
    iFilledRectangle(960,0,320,720);
    show_date();
}

void set_bg2()
{
    iSetColor(242, 240, 131);
    iFilledRectangle(0,0,320,720);
    iFilledRectangle(640,0,320,720);
    iSetColor(247, 207, 131);
    iFilledRectangle(320,0,320,720);
    iFilledRectangle(960,0,320,720);
    show_date();
}

void set_bg3()
{
    iSetColor(242, 179, 215);
    iFilledRectangle(0,0,320,720);
    iFilledRectangle(640,0,320,720);
    iSetColor(234, 181, 247);
    iFilledRectangle(320,0,320,720);
    iFilledRectangle(960,0,320,720);
    show_date();
}

void fix_file(FILE *fp)
{
    char str_in[100];
    fseek(fp,0,SEEK_SET);
    for(int i=0;i<8;i++){
        fscanf(fp,"%s",str_in);
    }
    char *p=strtok(str_in,",");
    int n=atoi(p);
    fseek(fp,0,SEEK_SET);

    FILE *temp=fopen("temp.txt","w");
    for(int i=0;i<8+n;i++){
        fscanf(fp,"%s",str_in);
        fprintf(temp,"%s\n",str_in);
    }
    fclose(temp);
    fclose(fp);

    temp=fopen("temp.txt","r");
    char name[100];
    sprintf(name,"%d",nn);
    strcat(name,ext);
    fp=fopen(name,"w");
    char c = fgetc(temp);
    while (c != EOF)
    {
        fputc(c, fp);
        c = fgetc(temp);
    }
    fclose(temp);
    fclose(fp);
    fp=fopen(name,"r+");
}

void overwrite(FILE *fp, long int n, char *s)
{
    FILE *temp;
    temp=fopen("temp.txt","w");
    rewind(fp);
    while(!feof(fp)){
        if(ftell(fp) == n){
            break;
        }
        fputc(fgetc(fp),temp);
    }
    fprintf(temp,s);
    while(!feof(fp)){
        char c=fgetc(fp);
        if(c == '\n' || c == ','){
            break;
        }
    }
    fseek(fp,ftell(fp)-1,SEEK_SET);
    char c = fgetc(fp);
    while (c != EOF)
    {
        fputc(c, temp);
        c = fgetc(fp);
    }

    fclose(temp);
    temp=fopen("temp.txt","r");
    fseek(fp,0,SEEK_SET);

    c = fgetc(temp);
    while (c != EOF)
    {
        fputc(c, fp);
        c = fgetc(temp);
    }

    fclose(temp);
}

void scrn_write(char c, int* p)
{
    int i;
    if(c == 8){
        if(len >= 0){
            len--;
            str[len]=0;
        }
    }
    else{
        if(c == '\r')
        {
            *p = *p + 1;
            strcpy(str2, str);
            printf("%s\n",str2);
            for(i = 0; i < len; i++)
                str[i] = 0;
            len = 0;
        }
        else
        {
            str[len] = c;
            len++;
        }
    }
}

void intro()
{
    set_bg1();
    show_gif();
    show_date();

	iSetColor(button_color);
	iFilledRectangle(340, 300, 250, 100 );
    char *box1="Login to mess";
    iSetColor(0,0,0);
    iText(380, 344, box1, GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(button_color);
	iFilledRectangle(650, 300, 250, 100 );
    char *box2="Sign-up new mess";
    iSetColor(0,0,0);
    iText(680, 344, box2, GLUT_BITMAP_TIMES_ROMAN_24);
}

void login()
{
    set_bg2();
    back_button();
    char *line="Enter mess code:";
    iSetColor(0,0,0);
    if(wrt==0){
        iSetColor(0,0,0);
        center(line,500);
        iRectangle(630,450,100,20);
        iText(635,455,str);
        return;
    }
    iSetColor(button_color);
    iFilledRectangle(450,300,180,50);
    iFilledRectangle(650,300,190,50);

    iSetColor(0,0,0);
    iText(460,320,"Login as boarder",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(660,320,"Login as manager",GLUT_BITMAP_TIMES_ROMAN_24);

    static int dtct=0;

    if(new_login==1){
        dtct=0;
        new_login=0;
    }

    if(dtct==0){
        nn=atoi(str2);
        int ls;
        char str_in[20];
        fseek(fp_list,0,SEEK_SET);
        fscanf(fp_list,"%d",&ls);
        if(nn>ls){
            iSetColor(255,255,255);
            iFilledRectangle(0,0,1280,720);
            iSetColor(0,0,0);
            iText(600,600,"Mess not registered",GLUT_BITMAP_TIMES_ROMAN_24);
            back_button();
            return;
        }
        sprintf(str_in,"%d",nn);
        strcat(str_in,ext);
        fp[nn]=fopen(str_in,"r+");
        if(fp[nn]==NULL){
            printf("error in login");
            exit(1);
        }
        dtct=1;
    }
    return;
}



void signup()
{
    set_bg3();
    back_button();
    iSetColor(0,0,0);
    center("Enter number of boarders:",600);
    center("Enter names:",400);

    iSetColor(0,0,0);
    int new_mess_id;
    static int dtct=0;

    if(new_mess==1){
        dtct=0;
    }

    if(wrt==1 && dtct==0){
        iSetColor(0,0,0);
        iRectangle(600,500,100,20);
        iText(615,505,str);
        return;
    }

    static int n=atoi(str2);
    static char str_in[20], str_out[20];
    if(new_mess==1){
        n=atoi(str2);
        new_mess=0;

        fseek(fp_list,0,SEEK_SET);
        if(fscanf(fp_list,"%d",&nn)==-1){
            rewind(fp_list);
            fprintf(fp_list,"1\n");
            nn=0;
            fflush(fp_list);
        }
        nn++;
        rewind(fp_list);
        fprintf(fp_list,"%d\n",nn);
        fflush(fp_list);
        new_mess_id=nn;
        sprintf(str_in,"%d",nn);
        strcat(str_in,ext);
        fp[nn]=fopen(str_in,"w+");
        fprintf(fp[nn],"\n\n\n\n\n\n\n");
    }
    dtct=1;
    if(wrt==2 && n>0){
        wrt--;
        n--;
        fprintf(fp[nn],"%s,0,0,0,0\n",str2);
    }
    if(wrt==2 && n==0){
        fprintf(fp[nn],"%s,0,0,0,0\n",str2);
        n--;
    }
    if(wrt ==1){
        iSetColor(0,0,0);
        iRectangle(600,330,100,20);
        iText(610,335,str);
        return;
    }

    xg--;
    return;
}

void signup_2()
{
    set_bg2();
    back_button();
    iSetColor(0,0,0);
    iText(200,650,"Enter item for breakfast, lunch and dinner for 7 days:",GLUT_BITMAP_TIMES_ROMAN_24);

    int static day_2=1,n=0;
    if(new_mess_2==1){
        day_2=1;
        n=0;
        new_mess_2=0;
    }
    char comma[2]=",",buff[1000];
    if(day_2==1){
        iText(300,550,"Day 1 -");

        if(wrt==2 && n==0){
            iSetColor(0,0,0);
            iText(400,550,"Breakfast:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==0){
            strcat(str2,comma);
            overwrite(fp[nn],0,str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==1){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==1){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==2){
            iSetColor(0,0,0);
            iText(400,550,"Lunch:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==2){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==3){
            iSetColor(0,0,0);
            iText(400,550,"Price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==3){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==4){
            iSetColor(0,0,0);
            iText(400,550,"Dinner:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==4){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==5){
            iSetColor(0,0,0);
            iText(400,550,"Price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==5){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }
        day_2++;
    }
    if(day_2==2){
        iText(300,550,"Day 2 -");

        if(wrt==2 && n==6){
            iSetColor(0,0,0);
            iText(400,550,"Breakfast:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==6){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            fgetc(fp[nn]);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==7){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==7){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==8){
            iSetColor(0,0,0);
            iText(400,550,"Lunch:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==8){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==9){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==9){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==10){
            iSetColor(0,0,0);
            iText(400,550,"Dinner:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==10){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==11){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==11){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            fscanf(fp[nn],"%s",buff);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }
        day_2++;
    }
    if(day_2==3){
        iText(300,550,"Day 3 -");

        if(wrt==2 && n==12){
            iSetColor(0,0,0);
            iText(400,550,"Breakfast:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==12){
            rewind(fp[nn]);
            fscanf(fp[nn],"%s",buff);
            fscanf(fp[nn],"%s",buff);
            fgetc(fp[nn]);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==13){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==13){
            rewind(fp[nn]);
            for(int i=0;i<3;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==14){
            iSetColor(0,0,0);
            iText(400,550,"Lunch:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==14){
            rewind(fp[nn]);
            for(int i=0;i<3;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==15){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==15){
            rewind(fp[nn]);
            for(int i=0;i<3;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==16){
            iSetColor(0,0,0);
            iText(400,550,"Dinner:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==16){
            rewind(fp[nn]);
            for(int i=0;i<3;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==17){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==17){
            rewind(fp[nn]);
            for(int i=0;i<3;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }
        day_2++;
    }
    if(day_2==4){
        iText(300,550,"Day 4 -");

        if(wrt==2 && n==18){
            iSetColor(0,0,0);
            iText(400,550,"Breakfast:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==18){
            rewind(fp[nn]);
            for(int i=0;i<3;i++){fscanf(fp[nn],"%s",buff);}
            fgetc(fp[nn]);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==19){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==19){
            rewind(fp[nn]);
            for(int i=0;i<4;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==20){
            iSetColor(0,0,0);
            iText(400,550,"Lunch:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==20){
            rewind(fp[nn]);
            for(int i=0;i<4;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==21){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==21){
            rewind(fp[nn]);
            for(int i=0;i<4;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==22){
            iSetColor(0,0,0);
            iText(400,550,"Dinner:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==22){
            rewind(fp[nn]);
            for(int i=0;i<4;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==23){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==23){
            rewind(fp[nn]);
            for(int i=0;i<4;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }
        day_2++;
    }
    if(day_2==5){
        iText(300,550,"Day 5 -");

        if(wrt==2 && n==24){
            iSetColor(0,0,0);
            iText(400,550,"Breakfast:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==24){
            rewind(fp[nn]);
            for(int i=0;i<4;i++){fscanf(fp[nn],"%s",buff);}
            fgetc(fp[nn]);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==25){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==25){
            rewind(fp[nn]);
            for(int i=0;i<5;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==26){
            iSetColor(0,0,0);
            iText(400,550,"Lunch:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==26){
            rewind(fp[nn]);
            for(int i=0;i<5;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==27){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==27){
            rewind(fp[nn]);
            for(int i=0;i<5;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==28){
            iSetColor(0,0,0);
            iText(400,550,"Dinner:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==28){
            rewind(fp[nn]);
            for(int i=0;i<5;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==29){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==29){
            rewind(fp[nn]);
            for(int i=0;i<5;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }
        day_2++;
    }
    if(day_2==6){
        iText(300,550,"Day 6 -");

        if(wrt==2 && n==30){
            iSetColor(0,0,0);
            iText(400,550,"Breakfast:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==30){
            rewind(fp[nn]);
            for(int i=0;i<5;i++){fscanf(fp[nn],"%s",buff);}
            fgetc(fp[nn]);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==31){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==31){
            rewind(fp[nn]);
            for(int i=0;i<6;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==32){
            iSetColor(0,0,0);
            iText(400,550,"Lunch:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==32){
            rewind(fp[nn]);
            for(int i=0;i<6;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==33){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==33){
            rewind(fp[nn]);
            for(int i=0;i<6;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==34){
            iSetColor(0,0,0);
            iText(400,550,"Dinner:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==34){
            rewind(fp[nn]);
            for(int i=0;i<6;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==35){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==35){
            rewind(fp[nn]);
            for(int i=0;i<6;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }
        day_2++;
    }
    if(day_2==7){
        iText(300,550,"Day 7 -");

        if(wrt==2 && n==36){
            iSetColor(0,0,0);
            iText(400,550,"Breakfast:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==36){
            rewind(fp[nn]);
            for(int i=0;i<6;i++){fscanf(fp[nn],"%s",buff);}
            fgetc(fp[nn]);
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==37){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==37){
            rewind(fp[nn]);
            for(int i=0;i<7;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==38){
            iSetColor(0,0,0);
            iText(400,550,"Lunch:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==38){
            rewind(fp[nn]);
            for(int i=0;i<7;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==39){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==39){
            rewind(fp[nn]);
            for(int i=0;i<7;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==40){
            iSetColor(0,0,0);
            iText(400,550,"Dinner:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==40){
            rewind(fp[nn]);
            for(int i=0;i<7;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }

        if(wrt==2 && n==41){
            iSetColor(0,0,0);
            iText(400,550,"price:");
            iRectangle(500,550,100,20);
            iText(520,555,str);
            return;
        }
        if(n==41){
            rewind(fp[nn]);
            for(int i=0;i<7;i++){fscanf(fp[nn],"%s",buff);}
            strcat(str2,comma);
            overwrite(fp[nn],ftell(fp[nn]),str2);
            wrt--;
            n++;
        }
    }
    fseek(fp[nn],0,SEEK_SET);

    iSetColor(255, 255, 255);
	iFilledRectangle(0, 0, WIDTH, HEIGHT);
	iSetColor(0,0,0);
	iText(550,600,"Your mess id is:",GLUT_BITMAP_TIMES_ROMAN_24);
	sprintf(buff,"%d",nn);
	iText(600,550,buff,GLUT_BITMAP_TIMES_ROMAN_24);
	back_button();
    wrt++;
}

void boarder()
{
    set_bg3();
    back_button();
    if(wrt==3){
        iSetColor(0,0,0);
        center("Enter boarder code:",600);
        iSetColor(0,0,0);
        iRectangle(600,500,80,40);
        iText(620,520,str);
        return;
    }
    static int n;
    if(wrt==4){
        char str_in[100];
        n=atoi(str2);
        fseek(fp[nn],0,SEEK_SET);
        for(int i=0;i<7;i++){
            fscanf(fp[nn],"%s",str_in);
        }
        fgetc(fp[nn]);
        int num;
        fscanf(fp[nn],"%d",&num);
        if(n>num){
            center("Boarder not registered",500);
            back_button();
            return;
        }
        wrt++;
    }
    if(wrt==5){
        wrt++;
        return;
    }
    int bill=0,prev_bill;

    center("Select meal:",600);
    iSetColor(button_color);
    iFilledRectangle(200,500,100,50);
    iFilledRectangle(200,400,100,50);
    iFilledRectangle(200,300,100,50);
    iFilledRectangle(200,200,50,50);

    iSetColor(0,0,0);
    iText(210,520,"Breakfast");
    iText(210,420,"Lunch");
    iText(210,320,"Dinner");
    iText(210,220,"Done");

    if(breakfast==1){
        iSetColor(button_color_2);
        iFilledRectangle(200,500,100,50);
        iSetColor(0,0,0);
        iText(210,520,"Breakfast");
    }
    if(lunch==1){
        iSetColor(button_color_2);
        iFilledRectangle(200,400,100,50);
        iSetColor(0,0,0);
        iText(210,420,"Lunch");
    }
    if(dinner==1){
        iSetColor(button_color_2);
        iFilledRectangle(200,300,100,50);
        iSetColor(0,0,0);
        iText(210,320,"Dinner");
    }
    static int dtct=0;
    if(new_meal==1){
        dtct=0;
        new_meal=0;
    }

    char str_in[100];
    static char total[100];
    long int pos;
    if(done==1){
        fseek(fp[nn],0,SEEK_SET);
        for(int i=0;i<7+n;i++){
            fscanf(fp[nn],"%s",str_in);
        }
        fgetc(fp[nn]);
        pos=ftell(fp[nn]);
        if(breakfast==1){
            int countn=0;
            fseek(fp[nn],pos,SEEK_SET);
            while(!feof(fp[nn])){
                if(fgetc(fp[nn])==','){
                    if(countn==0){
                        break;
                    }
                    else{
                        countn--;
                    }
                }
            }
            overwrite(fp[nn],ftell(fp[nn]),"1");

            fseek(fp[nn],0,SEEK_SET);
            for(int i=0;i<day;i++){
                fscanf(fp[nn],"%s",str_in);
            }
            char *p=strtok(str_in,",");
            int cnt=1;
            while(cnt--){
                p=strtok(NULL,",");
            }
            bill=bill+atoi(p);

            breakfast=0;
        }
        if(lunch==1){
            int countn=1;
            fseek(fp[nn],pos,SEEK_SET);
            while(!feof(fp[nn])){
                if(fgetc(fp[nn])==','){
                    if(countn==0){
                        break;
                    }
                    else{
                        countn--;
                    }
                }
            }
            overwrite(fp[nn],ftell(fp[nn]),"1");

            fseek(fp[nn],0,SEEK_SET);
            for(int i=0;i<day;i++){
                fscanf(fp[nn],"%s",str_in);
            }
            char *p=strtok(str_in,",");
            int cnt=3;
            while(cnt--){
                p=strtok(NULL,",");
            }
            bill=bill+atoi(p);

            lunch=0;
        }
        if(dinner==1){
            int countn=2;
            fseek(fp[nn],pos,SEEK_SET);
            while(!feof(fp[nn])){
                if(fgetc(fp[nn])==','){
                    if(countn==0){
                        break;
                    }
                    else{
                        countn--;
                    }
                }
            }
            overwrite(fp[nn],ftell(fp[nn]),"1");

            fseek(fp[nn],0,SEEK_SET);
            for(int i=0;i<day;i++){
                fscanf(fp[nn],"%s",str_in);
            }
            char *p=strtok(str_in,",");
            int cnt=5;
            while(cnt--){
                p=strtok(NULL,",");
            }
            bill=bill+atoi(p);

            dinner=0;
        }
        int countn=3;
        fseek(fp[nn],pos,SEEK_SET);
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                if(countn==0){
                    break;
                }
                else{
                    countn--;
                }
            }
        }
        fscanf(fp[nn],"%d",&prev_bill);
        bill+=prev_bill;
        countn=3;
        fseek(fp[nn],pos,SEEK_SET);
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                if(countn==0){
                    break;
                }
                else{
                    countn--;
                }
            }
        }
        sprintf(str_in,"%d",bill);
        overwrite(fp[nn],ftell(fp[nn]),str_in);
        fseek(fp[nn],0,SEEK_SET);
        strcpy(total,str_in);

        done=0;
        dtct=1;
        return;
    }

    if(dtct==1){
        iSetColor(255,255,255);
        iFilledRectangle(0,0,1280,720);
        center("Total due bill:",500);
        iText(620,400,total);

        back_button();
        new_login=1;
    }
    return;
}

void manager()
{
    set_bg3();
    back_button();
    iSetColor(button_color);
    iFilledRectangle(200,500,120,50);
    iFilledRectangle(200,400,120,50);
    iFilledRectangle(200,300,120,50);
    iFilledRectangle(200,200,120,50);

    iSetColor(0,0,0);
    iText(210,520,"Menu");
    iText(210,420,"Boarder list");
    iText(210,320,"Meals");
    iText(210,220,"Payments");
}

void display_menu(int y,char *day)
{
    char *p;
    char str_in[100];

    fscanf(fp[nn],"%s",str_in);
    iText(110,y,day,GLUT_BITMAP_TIMES_ROMAN_24);
    p=strtok(str_in,",");
    iText(260,y,p,GLUT_BITMAP_TIMES_ROMAN_24);
    p=strtok(NULL,",");
    iText(410,y,p,GLUT_BITMAP_TIMES_ROMAN_24);
    p=strtok(NULL,",");
    iText(560,y,p,GLUT_BITMAP_TIMES_ROMAN_24);
    p=strtok(NULL,",");
    iText(710,y,p,GLUT_BITMAP_TIMES_ROMAN_24);
    p=strtok(NULL,",");
    iText(860,y,p,GLUT_BITMAP_TIMES_ROMAN_24);
    p=strtok(NULL,",");
    iText(1010,y,p,GLUT_BITMAP_TIMES_ROMAN_24);
}

void menu()
{
    set_bg1();
    back_button();
    iSetColor(0,0,0);
    iRectangle(100,650,100,45);
    iRectangle(250,650,110,45);
    iRectangle(400,650,100,45);
    iRectangle(550,650,100,45);
    iRectangle(700,650,100,45);
    iRectangle(850,650,100,45);
    iRectangle(1000,650,100,45);

    iText(110,660,"Day",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(260,660,"Breakfast",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(410,660,"Price",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(560,660,"Lunch",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(710,660,"Price",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(860,660,"Dinner",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(1010,660,"Price",GLUT_BITMAP_TIMES_ROMAN_24);

    char str_in[100];
    char *p;
    fseek(fp[nn],0,SEEK_SET);

    display_menu(610,"1");
    display_menu(560,"2");
    display_menu(510,"3");
    display_menu(460,"4");
    display_menu(410,"5");
    display_menu(360,"6");
    display_menu(310,"7");

    fseek(fp[nn],0,SEEK_SET);

    iSetColor(button_color);
    iFilledRectangle(600,200,100,50);
    iSetColor(0,0,0);
    iText(630,225,"Edit");
}

void menu_edit()
{
    set_bg2();
    back_button();
    char str_in[100];
    if(wrt==11){
        iSetColor(0,0,0);
        center("Enter day to change:",500);
        iRectangle(620,400,50,50);
        iText(640,420,str);
        return;
    }
    if(wrt==12){
        int n=atoi(str2);
        fseek(fp[nn],0,SEEK_SET);
        for(int i=0;i<n-1;i++){
            fscanf(fp[nn],"%s",str_in);
        }
        if(n!=1){
            fgetc(fp[nn]);
        }
        wrt++;
    }
    if(wrt==13){
        iSetColor(0,0,0);
        center("Breakfast:",500);
        iRectangle(600,400,100,50);
        iText(610,420,str);
        return;
    }
    if(wrt==14){
        long int pos = ftell(fp[nn]);
        overwrite(fp[nn],ftell(fp[nn]),str2);
        fix_file(fp[nn]);
        fseek(fp[nn],pos,SEEK_SET);
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                break;
            }
        }
        wrt++;
    }
    if(wrt==15){
        iSetColor(0,0,0);
        center("Price",500);
        iRectangle(600,400,100,50);
        iText(610,420,str);
        return;
    }
    if(wrt==16){
        long int pos = ftell(fp[nn]);
        overwrite(fp[nn],ftell(fp[nn]),str2);
        fix_file(fp[nn]);
        fseek(fp[nn],pos,SEEK_SET);
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                break;
            }
        }
        wrt++;
    }
    if(wrt==17){
        iSetColor(0,0,0);
        center("Lunch:",500);
        iRectangle(600,400,100,50);
        iText(610,420,str);
        return;
    }
    if(wrt==18){
        long int pos = ftell(fp[nn]);
        overwrite(fp[nn],ftell(fp[nn]),str2);
        fix_file(fp[nn]);
        fseek(fp[nn],pos,SEEK_SET);
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                break;
            }
        }
        wrt++;
    }
    if(wrt==19){
        iSetColor(0,0,0);
        center("Price",500);
        iRectangle(600,400,100,50);
        iText(610,420,str);
        return;
    }
    if(wrt==20){
        long int pos = ftell(fp[nn]);
        overwrite(fp[nn],ftell(fp[nn]),str2);
        fix_file(fp[nn]);
        fseek(fp[nn],pos,SEEK_SET);
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                break;
            }
        }
        wrt++;
    }
    if(wrt==21){
        iSetColor(0,0,0);
        center("Dinner:",500);
        iRectangle(600,400,100,50);
        iText(610,420,str);
        return;
    }
    if(wrt==22){
        long int pos = ftell(fp[nn]);
        overwrite(fp[nn],ftell(fp[nn]),str2);
        fix_file(fp[nn]);
        fseek(fp[nn],pos,SEEK_SET);
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                break;
            }
        }
        wrt++;
    }
    if(wrt==23){
        iSetColor(0,0,0);
        center("Price",500);
        iRectangle(600,400,100,50);
        iText(610,420,str);
        return;
    }
    if(wrt==24){
        long int pos = ftell(fp[nn]);
        overwrite(fp[nn],ftell(fp[nn]),str2);
        fix_file(fp[nn]);
        fseek(fp[nn],pos,SEEK_SET);
        wrt++;
    }
    iSetColor(255,255,255);
    center("Menu updated successfully",500);
    back_button();
}

void boarder_list()
{
    set_bg1();
    back_button();
    iSetColor(0,0,0);
    iRectangle(400,600,150,50);
    iRectangle(600,600,150,50);

    iText(450,615,"ID",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(650,615,"Name",GLUT_BITMAP_TIMES_ROMAN_24);

    char str_in[100];
    char *p;
    fseek(fp[nn],0,SEEK_SET);
    for(int i=0;i<8;i++){
        fscanf(fp[nn],"%s",str_in);
    }

    p=strtok(str_in,",");
    int cnt=atoi(p);
    for(int i=1;i<=cnt;i++){
        fscanf(fp[nn],"%s",str_in);
        char line[10];
        sprintf(line,"%d",i);
        iText(450,615-i*40,line,GLUT_BITMAP_TIMES_ROMAN_24);
        p=strtok(str_in,",");
        iText(650,615-i*40,p,GLUT_BITMAP_TIMES_ROMAN_24);
    }
    fseek(fp[nn],0,SEEK_SET);

    iSetColor(button_color);
    iFilledRectangle(900,200,100,50);
    iSetColor(0,0,0);
    iText(920,220,"Edit");
}

void boarder_list_edit()
{
    set_bg3();
    back_button();
    char str_in[100];
    char *p;
    if(wrt==8){
        center("Enter number of new boarders:",500);
        iSetColor(0,0,0);
        iRectangle(600,400,50,50);
        iText(620,410,str);
        return;
    }
    static int new_boarders;
    if(wrt==9){
        new_boarders=atoi(str2);
        fseek(fp[nn],0,SEEK_SET);
        for(int i=0;i<8;i++){
            fscanf(fp[nn],"%s",str_in);
        }
        p=strtok(str_in,",");
        int prev_boarders=atoi(p);
        int boarders=new_boarders+prev_boarders;
        fseek(fp[nn],0,SEEK_SET);
        for(int i=0;i<7;i++){
            fscanf(fp[nn],"%s",str_in);
        }
        fgetc(fp[nn]);
        sprintf(str_in,"%d",boarders);
        overwrite(fp[nn],ftell(fp[nn]),str_in);
        fseek(fp[nn],0,SEEK_END);
        wrt++;
    }
    if(wrt==10){
        iSetColor(0,0,0);
        center("Enter names:",500);
        iRectangle(550,400,200,50);
        center(str,410);
        return;
    }
    if(new_boarders>0){
        fprintf(fp[nn],"%s,0,0,0,0\n",str2);
        wrt--;
        new_boarders--;
        if(new_boarders == 0){
            wrt++;
        }
        return;
    }
    fseek(fp[nn],0,SEEK_SET);
    iSetColor(255,255,255);
    iFilledRectangle(0,0,1280,720);
    back_button();
    center("Added successfully",500);
}

void meals()
{
    set_bg2();
    back_button();
    if(wrt==7){
        wrt++;
        return;
    }
    char str_in[100];
    char *p;
    fseek(fp[nn],0,SEEK_SET);
    for(int i=0;i<day;i++){
        fscanf(fp[nn],"%s",str_in);
    }
    int breakfast_bill,lunch_bill,dinner_bill;
    p=strtok(str_in,",");
    p=strtok(NULL,",");
    breakfast_bill=atoi(p);
    p=strtok(NULL,",");
    p=strtok(NULL,",");
    lunch_bill=atoi(p);
    p=strtok(NULL,",");
    p=strtok(NULL,",");
    dinner_bill=atoi(p);

    fseek(fp[nn],0,SEEK_SET);
    for(int i=0;i<8;i++){
        fscanf(fp[nn],"%s",str_in);
    }
    int breakfast_total=0,lunch_total=0,dinner_total=0;
    while(fscanf(fp[nn],"%s",str_in) != -1){
        p=strtok(str_in,",");
        p=strtok(NULL,",");
        breakfast_total=breakfast_total+atoi(p);
        p=strtok(NULL,",");
        lunch_total=lunch_total+atoi(p);
        p=strtok(NULL,",");
        dinner_total=dinner_total+atoi(p);
    }
    fseek(fp[nn],0,SEEK_SET);
    int bill_total= breakfast_total*breakfast_bill + lunch_total*lunch_bill + dinner_total*dinner_bill;

    center("Total breakfast:",650);
    center("Total lunch:",550);
    center("Total dinner:",450);
    center("Total bill:",350);

    sprintf(str_in,"%d",breakfast_total);
    iText(620,600,str_in);
    sprintf(str_in,"%d",lunch_total);
    iText(620,500,str_in);
    sprintf(str_in,"%d",dinner_total);
    iText(620,400,str_in);
    sprintf(str_in,"%d",bill_total);
    iText(620,300,str_in);

    iSetColor(button_color);
    iFilledRectangle(550,200,200,50);
    iSetColor(0,0,0);
    iText(570,210,"Allocate meal");

    if(meal_allocate==1){
        fseek(fp[nn],0,SEEK_SET);
        for(int i=0;i<8;i++){
            fscanf(fp[nn],"%s",str_in);
        }
        fgetc(fp[nn]);
        char *p=strtok(str_in,",");
        int cnt=atoi(p);
        while(cnt--){
            for(int i=0;i<3;i++){
                while(!feof(fp[nn])){
                    if(fgetc(fp[nn])==','){
                        break;
                    }
                }
                long int pos=ftell(fp[nn]);
                overwrite(fp[nn],pos,"0");
                fseek(fp[nn],pos,SEEK_SET);
            }
            fscanf(fp[nn],"%s",str_in);
            fgetc(fp[nn]);
        }
        fseek(fp[nn],0,SEEK_SET);
    }

    back_button();
    return;
}

void payments()
{
    set_bg3();
    back_button();
    if(wrt==6){
        iSetColor(0,0,0);
        center("Enter boarder id:",500);
        iRectangle(600,400,50,50);
        iText(620,410,str);
        return;
    }
    int n=atoi(str2);
    char str_in[100];
    fseek(fp[nn],0,SEEK_SET);
    for(int i=0;i<8+n;i++){
        fscanf(fp[nn],"%s",str_in);
    }
    char *p=strtok(str_in,",");
    center("Name:",550);
    iText(620,500,p,GLUT_BITMAP_HELVETICA_18);
    int cnt=4;
    while(cnt--){
        p=strtok(NULL,",");
    }
    center("Due payment:",430);
    iText(620,400,p);

    iSetColor(button_color);
    iFilledRectangle(550,300,200,50);
    iSetColor(0,0,0);
    iText(570,310,"Payment clear");

    if(payment_clear==1){
        fseek(fp[nn],0,SEEK_SET);
        for(int i=0;i<7+n;i++){
            fscanf(fp[nn],"%s",str_in);
        }
        fgetc(fp[nn]);
        int countn=3;
        while(!feof(fp[nn])){
            if(fgetc(fp[nn])==','){
                if(countn==0){
                    break;
                }
                else{
                    countn--;
                }
            }
        }
        overwrite(fp[nn],ftell(fp[nn]),"0");
        fseek(fp[nn],0,SEEK_SET);

        iSetColor(255,255,255);
        iFilledRectangle(0,0,1280,720);
        center("Payment cleared",400);
        back_button();
    }
}

void iDraw() {
	//place your drawing codes here
	iClear();
	iSetColor(210, 247, 252);
	iFilledRectangle(0, 0, WIDTH, HEIGHT);
	if(xg==0 && yg==0){
        intro();
        return;
	}
	back_button();

	if(xg==1 && yg==0){
        login();
        return;
	}
	if(xg==-1 && yg==0){
        signup();
        return;
	}
	if(xg==-2 && yg==0){
        signup_2();
        return;
	}
	if(xg==1 && yg==1){
        boarder();
        return;
	}
	if(xg==2 && yg==0){
        manager();
        return;
	}
	if(xg==2 && yg==1){
        menu();
        return;
	}
	if(xg==3 && yg==1){
        boarder_list();
        return;
	}
	if(xg==3 && yg==0){
        meals();
        return;
	}
	if(xg==2 && yg==-1){
        payments();
        return;
	}
	if(xg==4 && yg==2){
        boarder_list_edit();
        return;
	}
	if(xg==2 && yg==2){
        menu_edit();
        return;
	}
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	//printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(mx > 10 && mx < 50 && my > 690 && my < 710){
            bck_scrn--;
            xg=back_scrn[bck_scrn].x;
            yg=back_scrn[bck_scrn].y;

            bck_wrt--;
            wrt=back_wrt[bck_wrt];
            egg=0;
            return;
        }
        if(xg==0 && yg==0){
            if((mx>WIDTH/2 - 300 && mx<WIDTH/2 - 50) && (my<400 && my>300)){
                xg++;
                wrt=0;
                new_login=1;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;

                bck_wrt++;
                back_wrt[bck_wrt]=0;
                return;
            }
            else if((mx>WIDTH/2 + 10 && mx<WIDTH/2 + 260) && (my<400 && my>300)){
                xg--;
                wrt=1;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;

                bck_wrt++;
                back_wrt[bck_wrt]=1;

                new_mess=1;
                new_mess_2=1;

                return;
            }
            //place your codes here

        }
        if(xg==1 && yg==0){
            if(mx>450 && mx<450+180 && my>300 && my<350){
                yg++;
                wrt=3;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;

                bck_wrt++;
                back_wrt[bck_wrt]=3;

                new_meal=1;
                return;
            }
            if(mx>650 && mx<650+180 && my>300 && my<350){
                xg++;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;
                return;
            }
        }
        if(xg==1 && yg==1){
            if(mx>200 && mx<300 && my>500 && my<550){
                breakfast=1;
            }
            if(mx>200 && mx<300 && my>400 && my<450){
                lunch=1;
            }
            if(mx>200 && mx<300 && my>300 && my<350){
                dinner=1;
            }
            if(mx>200 && mx<250 && my>200 && my<250){
                done=1;
            }
        }
        if(xg==2 && yg==0){
            if(mx>200 && mx<320 && my>500 && my<550){
                yg=1;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;
                return;
            }
            if(mx>200 && mx<320 && my>400 && my<450){
                xg=3;
                yg=1;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;
                return;
            }
            if(mx>200 && mx<320 && my>300 && my<350){
                xg=3;
                wrt=7;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;
                return;
            }
            if(mx>200 && mx<320 && my>200 && my<250){
                yg=-1;
                wrt=6;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;

                bck_wrt++;
                back_wrt[bck_wrt]=6;

                payment_clear=0;
                return;
            }
        }
        if(xg==3 && yg==0){
            if(mx>550 && mx<750 && my>200 && my<250){
                meal_allocate=1;
            }
        }
        if(xg==2 && yg==-1){
            if(mx>550 && mx<750 && my>300 && my<350){
                payment_clear=1;
            }
        }
        if(xg==3 && yg==1){
            if(mx>900 && mx<1000 && my>200 && my<250){
                xg=4;
                yg=2;
                wrt=8;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;

                bck_wrt++;
                back_wrt[bck_wrt]=8;
                return;
            }
        }
        if(xg==2 && yg==1){
            if(mx>600 && mx<700 && my>200 && my<250){
                xg=2;
                yg=2;
                wrt=11;
                bck_scrn++;
                back_scrn[bck_scrn].x=xg;
                back_scrn[bck_scrn].y=yg;

                bck_wrt++;
                back_wrt[bck_wrt]=11;
                return;
            }
        }
    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
    scrn_write(key,&wrt);
    return;

	//place your codes for other keys here
}


/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		exit(0);
	}
	//place your codes for other keys here
}

int main() {
	//place your own initialization codes here.

	struct tm *date;
	time_t ltime;
	time(&ltime);
	date=localtime(&ltime);
	char *st=asctime(date);
	char *p=strtok(st," ");
	if(strcmp(p,"Sat")==0){
        day=2;
	}
	else if(strcmp(p,"Sun")==0){
        day=3;
	}
	else if(strcmp(p,"Mon")==0){
        day=4;
	}
	else if(strcmp(p,"Tue")==0){
        day=5;
	}
	else if(strcmp(p,"Wed")==0){
        day=6;
	}
	else if(strcmp(p,"Thu")==0){
        day=7;
	}
	else if(strcmp(p,"Fri")==0){
        day=1;
	}
	fp_list=fopen("List.txt","r+");
    if(fp_list == NULL){
        printf("error\n");
        exit(1);
    }
	iInitialize(WIDTH, HEIGHT, "Mess App");

	fclose(fp_list);
	return 0;
}

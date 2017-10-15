#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[100];
    char pid[10];
    char service[100];
}progress;

int excmd(char* cmd,  progress* now, int* now_length){
    //#cmd is the command that you want to excute in windows cmd(here is "tasklist /fo csv /svc /nh"), now is running progress;
    //#excute cmd command and return print to now;
    FILE* pp=popen(cmd, "r");
    char buffer[256]={0};
    int m=0;

    if(!pp) return 0;
loop:for(int i=0; !feof(pp); i++){
        if(fgets(buffer, 256, pp)){
            if(buffer[0]!='#'){
                char temp[10][100]={0};
                    int l=0;
                //printf("%s\n",buffer);
                for(int j=0; j<strlen(buffer); j++){
                    int k;
            //printf("lalala\n");
                    if(buffer[j]==34){
                        for(k=j+1; ; k++){
                            if(buffer[k]==34) {
                                //printf("%c\n",buffer[k-1]);
                                break;
                            }
                            temp[l][k-(j+1)]=buffer[k];
                            //printf("%c", temp[0][k-j-1]);
                        }
                        l++;
                        j=k;
                    }
                }
                //printf("%s ",temp[0]);
                //printf("%s ",temp[1]);
                //printf("%s\n",temp[2]);
                strcpy(now[m].name,temp[0]);
                strcpy(now[m].pid,temp[1]);
                strcpy(now[m].service,temp[2]);
                m++;
                memset(buffer, 0, sizeof(buffer));
                //if(m==135){
                    //break;
                //}
            }
        }
        //else return 0;
    }
    fclose(pp);
    *now_length=m;
    return 1;
}

int readFile(const char* filename, char* template){
    //#read virus template from filename
    FILE * fp=fopen(filename,"r");
    char buffer[256];

    template[0]=0;
    if(!fp) return 0;
    while(!feof(fp)){
        if(fgets(buffer, 256, fp)){
    //        printf("%s\n",buffer);
            strcat(template,buffer);
        }
    }
    pclose(fp);
    //printf("%s\n", "ladsfaksdfkasd");
    return 1;
}

int sort(progress* now, int now_length){
    //#progress is a struct, now is a array of progress, now_length is its length;
    //#this function is to sort now to small to big by now[i].name and now[i].service;
    for(int i=0;i<now_length;i++){
        for(int k=0,j=i+1;j<now_length; j++){
            if(strcmp(now[i].name, now[j].name)>0){
            //how to reuse the following 3 lines code?
                progress temp=now[j];
                now[j]=now[i];
                now[i]=temp;
            }
            else if(strcmp(now[i].name, now[j].name)==0){
                if(strcmp(now[i].service, now[j].service)>0){
                    progress temp=now[j];
                    now[j]=now[i];
                    now[i]=temp;
                }
            }
//loop:if(now[i].name[k]>now[j].name[k]){
                //else if(now[i].name[k]==now[j].name[k]){
                //    if(now[i].name[k]==0 ||now[j].name[k]==0) break;
                //    k++;
                //    goto loop;
                //}
        }
    }
    return 1;
}
int sort_1(progress* now, int now_length){
    //#progress is a struct, now is a array of progress, now_length is its length;
    //#this function is to sort now to small to big by now[i].name and now[i].service;
    for(int i=0;i<now_length;i++){
        for(int k=0,j=i+1;j<now_length; j++){
//loop:if(now[i].name[k]>now[j].name[k]){
            if(strcmp(now[i].service, now[j].service)>0){
            //how to reuse the following 3 lines code?
                progress temp=now[j];
                now[j]=now[i];
                now[i]=temp;
            }
            else if(strcmp(now[i].service, now[j].service)==0){
                if(strcmp(now[i].name, now[j].name)>0){
                    progress temp=now[j];
                    now[j]=now[i];
                    now[i]=temp;
                }
            }
                //else if(now[i].name[k]==now[j].name[k]){
                //    if(now[i].name[k]==0 ||now[j].name[k]==0) break;
                //    k++;
                //    goto loop;
                //}
        }
    }
    return 1;
}

int compare(progress* now, int now_length, char* template){
    //#compare runing progress now with template;
    
    int j;
    sort(now, now_length);
    for(int i=0; i<now_length; i++){
        for(j=i+1; ; j++){
            if(strcmp(now[i].name,now[j].name)!=0 || strcmp(now[i].service, now[j].service)!=0) {
                break;
            }
        }
        if(i!=j-1){
            i=j-1;
            continue;
        }
        if(!strstr(template, now[i].name) || !strstr(template, now[i].service)){
            printf("%30s, %10s,\t%s\n", now[i].name, now[i].pid, now[i].service);
        }
    }
    return 1;
}



int main(int argc, const char *argv[])
{
    int now_length=0;
    char template[10000]={0};
    progress now[1000];

    readFile(argv[1], template);//int readFile(const char* filename, char* template)
    excmd("tasklist /fo csv /svc /nh", now, &now_length);
    //printf("%d\n", now_length);
    //for(int i=0; i<now_length; i++){
      //  printf("%s, %s, %s\n", now[i].name, now[i].pid, now[i].service);
    //}
    //printf("%s\n",template);
    compare(now, now_length, template);

    return 0;
}

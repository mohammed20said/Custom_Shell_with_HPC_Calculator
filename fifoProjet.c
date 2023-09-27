#include<stdio.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
void main(int argc, char * argv[])
{
int p1,p2;
int R;
if (argc!=2){printf("introduire un nom de fichier\n");exit(1);}

if((p1=open("tubenom",O_WRONLY))==-1)
{perror("ouverture impossible du tube");exit(3);}
dup2(p1,1);

if((p2=open(argv[1],O_RDONLY))==-1)
{perror("ouverture impossible du fichier");exit(3);}
dup2(p2,0);
if(fork()==0){
execlp("./majuscule","./majuscule",NULL);
perror("Pb execlp");
exit(4);
}

}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//c'est pour utiliser les fonction de chaine de caractéres
#include <sys/wait.h>//c'est pour utuliser la fonction wait
#include <signal.h>// x'est pour utiliser les signaux
#include<sys/msg.h>//c'est pour la technique de files de messages
#include<sys/ipc.h>//c'est utiliser les fonctions IPC

typedef int Semaphore[2];
typedef struct{
int mtype;
char mtext[256];
}msg;
msg msg1;



Semaphore S;

void Inetsem(Semaphore S , int N){

	char c='a';
	pipe(S);
	for(int i=1;i<=N;i++){
	
		write(S[1],&c,1);
	
	}


}

void P(Semaphore S){

	char c;

	read(S[0],&c,1);

}

void V(Semaphore S){
	char c='a';
	
	write(S[1],&c,1);
}




void op1(int i,int j){
	if(fork()==0){
	        int res=i*j;
	        key_t cle = ftok("/tmp", 'C');
	        int msgid ;	
	        msgid=msgget(cle, IPC_CREAT | 0666);
	        
	        msg1.mtype=i;
	        sprintf(msg1.mtext, "%d", res);
	        if(msgsnd(msgid,&msg1,sizeof(msg1),0)==-1)
		{
		perror("case 4 erreur msgsnd");
		}
		printf("le resultat de processus 1 ou 2 est :%s\n",msg1.mtext);
		
		V(S);
		exit(0);
	}

}

void op2(){
	
	if(fork()==0){
	       P(S);
	       P(S);
	        key_t cle = ftok("/tmp", 'C');
	        int msgid;
	        msgid =msgget(cle,IPC_CREAT|0666);
	        
	        msgrcv(msgid,&msg1,1000,0,IPC_NOWAIT);
	        int x=atoi(msg1.mtext);
	        
		
		
	        msgrcv(msgid,&msg1,1000,0,IPC_NOWAIT);
		int y=atoi(msg1.mtext);
		
		printf("le resultat de processus 3 est :%d\n",x+y);
		msgctl(msgid,IPC_RMID,NULL);
		exit(0);
	}

}

void main(int argc,char * argv[])
{


if(argc!=5){perror("error des argumants \n");exit(0);}

Inetsem(S,0);
op1(atoi(argv[1]),atoi(argv[2]));
op1(atoi(argv[3]),atoi(argv[4]));
op2();
for(int i=0;i<3;i++)
wait(0);
}


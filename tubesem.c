#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "tubesem.h"


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

void attente(int duree){
	sleep(duree);
}

void msg(int N,char* s){

int nb=(N-1)*20;

for(int i=0; i<nb;i++) putchar(' ');
printf("%s\n",s);
fflush(stdout);


}

void voiture(int voit,int duree){

	if(fork()==0){
	
		attente(duree);
		msg(voit,"arrivee");
		P(S);
		attente(duree);
		msg(voit,"treversee");
		attente(duree);
		msg(voit,"depart");
		V(S);
		exit(0);
	}

}	


void main(){
	printf("%s%20s%20s%20s\n","VOITURE 1","VOITURE 2","VOITURE 3","VOITURE 4");
	Inetsem(S,1);
	voiture(1,1);
	voiture(2,2);
	voiture(3,3);
	voiture(4,4);
	
	
	for (int i =0;i<4;i++)
		wait(0);
	

}



























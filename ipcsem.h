#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/wait.h>


void Creersem(key_t key,int N);
void Initsem(int N,int V);
void attente(int duree);
void message(int i , char* c);
void detruiresem();
void P(int N);
void V(int N);

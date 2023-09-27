#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include"ipcsem.h"

#define SHM_SIZE 1024
int Num;

void attente(int duree){
sleep(duree);
}

void msg(int N,char* s){

int nb=(N-1)*20;

for(int i=0; i<nb;i++) putchar(' ');
printf("%s\n",s);
fflush(stdout);


}

void Creersem(key_t key,int N){
	Num=semget(key,N,0666|IPC_CREAT);

}

void P(int N){
	struct sembuf tabop1;
	tabop1.sem_num=N;
	tabop1.sem_op=-1;
	tabop1.sem_flg=0;

	semop(Num,&tabop1,1);

}

void V(int N){
	struct sembuf tabop1;
	tabop1.sem_num=N;
	tabop1.sem_op=1;
	tabop1.sem_flg=0;

	semop(Num,&tabop1,1);

}

void detruiresem(){
	semctl(Num,0,IPC_RMID,0);
}

void Initsem(int N,int V){
	semctl(Num,N,SETVAL,V);
}

typedef struct {
    int tache1;
    int tache2;
    int tache3;

} Calcul;

void main(int argc, char* argv[]) {
    if(argc!=5){
	
	printf("usage : calculer <int a> <int b> <int c> <int d>  \n");
	exit(0);
    }
    int a, b, c, d, resultat;
    clock_t debut, fin;
    double temps;

    // saisie des valeurs initiales
    a=atoi(argv[1]);
    b=atoi(argv[2]);
    c=atoi(argv[3]);
    d=atoi(argv[4]);

    
    
    
    #define occupe 1
    #define libre 0
    #define moy 2
    
    int num1 ;
    Creersem(1000,3);
    Initsem(libre,2);
    Initsem(moy,0);
    Initsem(occupe,0);
    key_t key = ftok("calculer.c", 'R');
    key_t key1 = ftok("calculer.c", 'Z');
    int shmid = shmget(key, SHM_SIZE, 0666|IPC_CREAT);
    Calcul *calcul = (Calcul*) shmat(shmid, (void*)0, 0);
    
    calcul->tache1=a;
    calcul->tache2=b;

    // création des processus fils
    pid_t pid1, pid2, pid3, pid4; 
    
    debut = clock();

    pid1 = fork();
    if (pid1 == 0) {
    	P(libre);
    	// tâche 1 : addition de a et b
        calcul->tache1 =a + b; 
        printf("Résultat de la tache 1 : %d\n", calcul->tache1);
        shmdt(calcul);
        V(moy);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        P(libre);
    	// tâche 2 : multiplication de c et d 
        calcul->tache2 =c*d; 
        printf("Résultat de la tache 2 : %d\n", calcul->tache2);
        shmdt(calcul);
        V(moy);
        exit(0);
    }

    pid3 = fork();
    if (pid3 == 0) {
    	P(moy);P(moy);
        // tâche 3 : multiplication des résultats des tâches 1 et 2
        calcul->tache3=calcul->tache1*calcul->tache2;
        printf("Résultat de la tache 3 : %d\n", calcul->tache3);
        shmdt(calcul);
        V(occupe);
        exit(0);
    }
    pid4=fork();
    if (pid4 == 0) {
	// tâche 4 : addition du résultat de la tâche 3 et de a
	P(occupe);
	resultat =  calcul->tache3 + a;
	printf("Le résultat final de la tache 4 est : %d\n", resultat);
	shmdt(calcul);
    	exit(0);
    }
        int resultat1;
    	waitpid(pid4, &resultat1, 0);
    	fin = clock();
	temps = (double)(fin - debut) / CLOCKS_PER_SEC;

	// affichage du résultat et du temps d'exécution en mode multiprocessus
	
	printf("Temps d'exécution (mode multiprocessus) : %f ms	\n", temps*1000);
	// calcul de l'opération en mode séquentiel
	debut = clock();

	int resultat_seq = (a + b) ;
	resultat_seq*= (c * d) ;
	resultat_seq+= a;

	fin = clock();
	temps = (double)(fin - debut) / CLOCKS_PER_SEC;

	// affichage du résultat et du temps d'exécution en mode séquentiel
	printf("Le résultat (mode séquentiel) est : %d\n", resultat_seq);
	printf("Temps d'exécution (mode séquentiel) : %f ms\n", temps*1000);
    
    
        

	

}


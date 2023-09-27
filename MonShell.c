#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//c'est pour utiliser les fonction de chaine de caractéres
#include <sys/wait.h>//c'est pour utuliser la fonction wait
#include <signal.h>// x'est pour utiliser les signaux
#include<sys/msg.h>//c'est pour la technique de files de messages
#include<sys/ipc.h>//c'est utiliser les fonctions IPC
#include<sys/shm.h>//c'est pour utiliser la mémoire partagée
#include"ipcsem.h"//c'est une ninleothéque pour utiliser les fonctions P , V ...

#define MAX_LINE 80
#define LGMES sizeof(message1.mtext)
key_t cle;
int Num;
typedef struct
{
long mtype;
char mtext[256];
}MESSAGE;
MESSAGE message1;

char *commandes[] = {   "creer",
			"lister",
			"nettoyer",
			"afficher_chemin_absolu",
			"afficher_processus",
			"chercher",
			"copier",
			"deplacer",
			"supprimer",
			"changer_DA",
			"lire",
			"creer_fich",
			"creer_dos",
			"connecter",
			"lister1",
			"addip",
			"changer_repertoire",
			"majuscule",
			"calculer",
			"avant",
			"calculer2"
		    };
   

	
	
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


//_____________________________________c'est le groupe de commande qu'on a changer le nom de la commande

void lister1(char** args) {
    if (strcmp(args[0], "lister1") == 0) {
    		args[0] = "ls";
    }
}


void chercher(char** args) {
    if (strcmp(args[0], "chercher") == 0) {
        args[0] = "grep";
    }
}


void copier(char** args) {
    if (strcmp(args[0], "copier") == 0) {
        args[0] = "cp";
    }
}

void deplacer(char** args) {
    if (strcmp(args[0], "deplacer") == 0) {
        args[0] = "mv";
    }
}

void supprimer(char** args) {
    if (strcmp(args[0], "supprimer") == 0) {
        args[0] = "rm";
    }
}

void nettoyer(char** args) {
    if (strcmp(args[0], "nettoyer") == 0) {
        args[0] = "clear";
    }
}

void afficher_chemin_absolu(char** args) {
    if (strcmp(args[0], "afficher_chemin_absolu") == 0) {
        args[0] = "pwd";
    }
}

void changer_DA(char** args) {
    if (strcmp(args[0], "changer_DA") == 0) {
        args[0] = "chmod";
    }
}

void creer_fich(char** args) {
    if (strcmp(args[0], "creer_fich") == 0) {
        args[0] = "touch";
    }
}

void creer_dos(char** args) {
    if (strcmp(args[0], "creer_dos") == 0) {
        args[0] = "mkdir";
    }
}

void afficher_processus(char** args) {
    if (strcmp(args[0], "afficher_processus") == 0) {
        args[0] = "ps";
    }
}

//_____________________________________fin de cette partie

//_____________________________________la partie qu'on a utilisé exec(commande) et fork()

void addip(char** args) {
    if (strcmp(args[0], "addip") == 0) {
        if(fork()==0){
		args[0] = "hostname";
		execlp(args[0],args[0],"-I",NULL);
		perror("pb exec");
		exit(1);
        }
        else wait(0);
    }
}

void connecter(char** args) {
    if (strcmp(args[0], "connecter") == 0) {
    pid_t pid = fork();
	if (pid < 0)
	{
	    printf("Error: fork failed\n");
	    exit(1);
	}
	else if (pid == 0)
	{
	    args[0] = "ping";
	    execvp(args[0],args);
	    perror("error connecter");
	    exit(0);	
	    
	}else
	{ 
	    wait(NULL);
	}
    }
}

void creer(char** args) {

        
    if (strcmp(args[0], "creer") == 0) {
		args[0] = "echo";
		execvp(args[0],args);
		perror("error creer");
	        exit(0);
        
    }
}

//________________________________________fin de cette partie

//________________________________________la prtie qu'on a utiliser chdir()
void changer_repertoire(char** args) {
    if (strcmp(args[0], "changer_repertoire") == 0) {
    
        int a = chdir(args[1]);
        printf(" ");
        printf(" ");
        if(a==0){
        	printf("vous avez changer le repertoire vers : %s\n",args[1]);
        	exit(1);
        }
        else {
       		printf("error de changer le repertoire !! \n");
       		exit(1);
       	}
    }
}

//_____________________________________fin de cette partie


//_____________________________________la partie qu'on a utlisé les différents techniques IPC

//ici on a utilisré les pipes et dup2
void lister(char** args) {
    if (strcmp(args[0], "lister") == 0) {
        	execlp("./lspipe","./lspipe",args[1],NULL);
		perror("error lire");
		exit(0);
    }
}

//ici on a utilisré les pipes et dup2
void lire(char** args) {
    if (strcmp(args[0], "lire") == 0) {
		execlp("./catpipe","./catpipe",args[1],NULL);
		perror("error lire");
		exit(0);
    }
}


//ici on a utilisré les signaux
void fct(int sig){
	pid_t pid = fork();
        if (pid < 0)
        {
            printf("Error: fork failed\n");
            exit(1);
        }
        else if (pid == 0)
        {
            if(sig==SIGINT){
                    printf("\n");
		    execlp("ls","ls",NULL);
		    perror("pb exec");
		    exit(1);
		    printf("\n");
            }
            if(sig==SIGTSTP){
            	    printf("\n");
		    execlp("clear","clear",NULL);
		    perror("pb exec");
		    exit(1);
		    printf("\n");
            }
            if(sig==SIGQUIT){
		    printf("\n");
		    execlp("./calculer","./calculer","15","17","19","5",NULL);
		    perror("pb exec");
		    exit(1);
		    printf("\n");
            }
            
        }else
        {
            
		wait(NULL);
		printf("\033[1;31m");
		printf("boukhryss shell> ");
		printf("\033[0m");
		fflush(stdout);
        }
}


//ici on a utilisré les fifos et dup2
void majuscule(char** args,int i){

	if (i!=2){
		printf("spécifier le nom du fichier !! \n");
		exit(1);
	}else{
		if (fork()==0){
			execlp("./fifoProjet","./fifoProjet",args[1],NULL);
			perror("pb exec fifo !!");
			exit(1);
		}
		
		if (fork()==0){	
			execlp("./fifo2Projet","./fifo2Projet",NULL);
			perror("pb exec fifo2 !!");
			exit(1);
		
		}
	}
}


//ici on a utilisé les semaphores , les pipes et les mémoires partagées
void calculer(char** args){

	if(strcmp(args[0], "calculer") == 0){
		args[0] = "./calculer";
		execvp(args[0],args);
		//perror("error calculer");
		//exit(0);
	}

}


//ici on a utilisé les semaphores , les pipes et les files de messages
void calculer2(char** args){

	if(strcmp(args[0], "calculer2") == 0){
		args[0] = "./calculer2";
		execvp(args[0],args);
		//perror("error calculer");
		//exit(0);
	}

}

void mes_commandes(char** args,int i){
	    int y=i;
	    lister1(args);
	    lister(args);
            chercher(args);
            nettoyer(args);
            copier(args);
            deplacer(args);
            supprimer(args);
            afficher_chemin_absolu(args);
            changer_DA(args);//pour changer les droits d'accés
            lire(args);
            creer_fich(args);
            creer_dos(args);
            afficher_processus(args);
            calculer(args);
            creer(args);
            calculer2(args);
            if (strcmp(args[0], "majuscule") == 0){
            	majuscule(args,y);
            	exit(1);
            }
         
            	
            	
            if (strcmp(args[0], "addip") == 0){
            	addip(args);
            	exit(1);
            }
            
            if (strcmp(args[0], "connecter") == 0){          	 
            	connecter(args); 
            	exit(1);
            }
           
            if (strcmp(args[0], "changer_repertoire") == 0){
            	exit(1);
            } 

}


//on a changé ici | par ? et on a utilisé les pipes avec dup2
void fct_pipe(char** args,int i , int j){

	int test[2];
	pipe(test);
	int saved_stdin = dup(0);
	char *arg[10];
	int y=0;
	pid_t pid = fork();
        if (pid < 0)
        {
            perror("Error: fork failed\n");
            exit(1);
        }
        else if (pid == 0)
        {
        	
        	close(test[0]);
        	dup2(test[1],1);
        	close(test[1]);
        	
		for (y=0;y<j;y++){
			arg[y]=args[y];	
		}
		arg[j]=NULL;
        	mes_commandes(arg,j);
        	execvp(arg[0],arg);
                perror("pb exec premier argumant !! ");
                exit(0);
        }else{
            int e=0;
            wait(NULL);
            close(test[1]);
            dup2(test[0], 0);
            close(test[0]);
            char *sec_args[10] ;
            for(e=j+1;e<i;e++){
            	sec_args[e-j-1]=args[e];
            }
            sec_args[e-j-1]=NULL;
            if(fork()==0){
		    mes_commandes(sec_args,e-j-1);
		    execvp(sec_args[0], sec_args);
		    printf("Erreur lors de l'exécution de la commande %s\n",sec_args[0]);
		    exit(0);
            }   
            else{
         	wait(0);
         	}
         }
        close(0);
	dup2(saved_stdin, 0);
	close(saved_stdin);
	close(test[0]); 
	close(test[1]);
         
	
}

//on travaille ici avec les files de messages pour creer dans la file
void avant1(key_t cle, char ** avantTable, int i){	

	int numfms;
	numfms=msgget(cle,IPC_CREAT | 0666);
	if(numfms==-1)
	{
	perror("case 1 erreur msgget");
	exit(1);
	}
	strcpy(message1.mtext,"");
	message1.mtype=1;
	
	int j=0;
	for(j=0;j<i;j++){
		strcat(message1.mtext, avantTable[j]);
		strcat(message1.mtext, " ");
	}
	
	if(msgsnd(numfms,&message1,LGMES,IPC_NOWAIT)==-1)
	{
	perror("case 4 erreur msgsnd");
	}
}



//on travaille ici avec les files de messages pour lire le message écrit la file
void avant2(key_t cle){
	int numfms;
	numfms=msgget(cle,IPC_CREAT | 0666);
	if(numfms==-1)
	{
	perror("case 1 erreur msgget");
	exit(1);
	}
	
	int n=msgrcv(numfms,&message1,LGMES,1,IPC_NOWAIT);
	if(n==-1)
	{
	perror("case 5 erreur msgrcv");
	exit(4);
	}
	else printf("\n--------------------la derniére commande c'est :  %s ----------------------\n",message1.mtext);
	if (msgctl(numfms,IPC_RMID,NULL)==-1)
	{
	perror("case 2 msgctl");
	exit(2);
	}


}

//c'east la foction de la commande historique , on a utilisé les mémoires partagées et les semaphores et les pipes
void fct_historique(char  historique[][200], int hist){
	#define occupe 1
	#define libre 0
	int num1 , i ; char * Entier;
	Creersem(1000,2);
	Initsem(libre,1);
	Initsem(occupe,0);
	num1=shmget(IPC_PRIVATE,20,0666|IPC_CREAT);
	Entier=(char*) shmat(num1,NULL,0);

	if(fork()==0){
		for(i=0;i<hist;i++){
		P(occupe);
		printf("--%d-- : %s \n",(i+1),Entier);
		fflush(stdout);
		V(libre);
		}
		exit(0);
	}
	else {
		for(i=0;i<hist;i++){
		P(libre);
		strcpy(Entier, historique[i]);
		fflush(stdout);
		V(occupe);
	}
		wait(0);detruiresem();
	}



}


int main(void)
{

	key_t cle = ftok("/tmp", 'A');
	
	int p=0;
	int i=0;
	int hist=0;
	int should_run = 1; 
	
	
	char *avantTable[MAX_LINE/2 + 1];
	char *args[MAX_LINE/2 + 1]; 
	char com[256];
	char historique[200][200];
	char *command; 
	
	if(fork()==0){
		execlp("clear","clear",NULL);
		exit(0);
	}
	else{
		wait(0);
	}
	while (should_run)
	{
		printf("\033[1;31m"); // séquence d'échappement pour la couleur rouge
		printf("boukhryss shell> ");
		fflush(stdout);
		printf("\033[0m");// séquence d'échappement pour la couleur original

		//c'est la prtie pour l'utilisation de signaux
		signal(SIGINT,fct);//kill -2
		signal(SIGTSTP,fct);//kill -20
		signal(SIGQUIT,fct);//kill -3


		ssize_t bufsize = 0;
		getline(&command, &bufsize, stdin);

		int i = 0;
		int j=0;
		
		args[i] = strtok(command, " \n");

		while (args[i] != NULL)
		{
			i++;
			args[i] = strtok(NULL, " \n");
		}
		args[i] = NULL;

		//cette partie est responsable de stocker tout les commandes dans un tableau historique
		strcpy(com,"");
		for(j=0;j<i;j++){
			strcat(com,args[j]);
			strcat(com, " ");
		}
		
		strcpy(historique[hist],com);
		hist=hist+1;
		if(strcmp(args[0],"historique")==0){
			if(i>1){
				printf("utilisation : historique\n");
				continue;
			       }
			else{
				fct_historique(historique,hist);
				continue;
			}
		}

		//cette partie est responsable d'executer la commande avant
		if(strcmp(args[0],"avant")!=0){
			for(p=0;p<i;p++){
				avantTable[p]=args[p];
			}
			avantTable[i]=NULL;
		}


		int o=1;
		if (strcmp(args[0], "avant") == 0)
		{
			if(i>1){
				printf("utilisation : avant\n");
				continue;
			       }
			if(strcmp(avantTable[0],"entrer une autre commande aprés entrer 'avant'")==0){
				printf("%s\n",avantTable[0]);
				o=0;
				}
			else{
				avant1(cle,avantTable,p);
				avant2(cle);
				for(i=0;i<p;i++){
					args[i]=avantTable[i];
				}
			    }
			args[i]=NULL;
		}

		if(o==0){
			continue;
		}
		
		

		//on peut tapper sortir pour quitter notre shell
		if (strcmp(args[0], "sortir") == 0)
		{
			if(i>1){
				printf("utilisation : sortir\n");
				continue;
			}
			should_run = 0;
			execlp("clear","clear",NULL);
			break;
		}


		//c'est la prtie pour changerv | par ?
		int x=1;
		for(j=0;j<i;j++){
			if (strcmp(args[j], "?") == 0)
			{
				fct_pipe(args,i,j);
				x=0;
				break;
			}
		}
		if(x==0) continue;
		
		
		//c'est la prtie de la commande changer_repertoire
		if (strcmp(args[0], "changer_repertoire") == 0){
			chdir(args[1]);
		} 


		pid_t pid = fork();
		if (pid < 0)
		{
			printf("Error: fork failed\n");
			exit(1);
		}
		else if (pid == 0)
		{
			int z=0;
			for (z = 0; z < sizeof(commandes)/sizeof(commandes[0]); z++) {
				if (strcmp(args[0], commandes[z]) == 0) {
					z=1;
					break;
				}
			}
			mes_commandes(args,i);


			if(z==1){
				execvp(args[0], args);
				printf("commande non trouvée\n");
				exit(1);
			}
			else{
				printf("commande non trouvée 111\n");
				exit(1);
			}
		}
		else wait(NULL);
	}

	return 0;
}


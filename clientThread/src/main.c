#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <sys/time.h>
#include <pthread.h>

/* Programme client */


int sendTCP(int sock,void * msg, int sizeMsg){
   if (sizeMsg < 0) {
        perror("Invalid message size");
        close(sock);
        return -1;
    }
   int resSend = 0;
   int octet_send = 0;
   while (octet_send < sizeMsg)
   {
      resSend = write(sock,msg+octet_send,sizeMsg-octet_send);
      if (resSend == -1 )
      {
         perror("Send Error when I try to send the message");
         close(sock);
         return -1;
      }
      octet_send = octet_send + resSend;
   }
   return 0;
}

int recvTCP(int sock,void * msg, int sizeMsg){
   if (sizeMsg < 0) {
        perror("Invalid message size");
        close(sock);
        return -1;
    }
   int resRecv = 0;
   int octet_recv = 0;
   while (octet_recv<sizeMsg) 
   {
      //printf("ok\n");
      //printf("%d\n",sizeMsg-octet_recv);
      resRecv = read(sock,msg+octet_recv,sizeMsg-octet_recv);
      octet_recv = octet_recv + resRecv;
      //printf("ok2\n");
      //printf("%d\n",resRecv);
      //printf("%d\n",sizeMsg);
      //printf("%d\n",octet_recv);
      if (resRecv == -1 )
      {
         perror("Recv Error when I try to receive the message");
         close(sock);
         return -1;
      }else
      {
         if (resRecv == 0)
         {
            perror("hote distant fermé");
            close(sock);
            return -1;
         }
         
      }
      
      
   }
   //printf("end\n");
   return 0;
}

typedef struct structEnvoi
{
    char uuidPlayer[64];
    int x;
    int y;
    int action;
}structEnvoi;



typedef struct param_thread
{
    struct sockaddr_in infoSocket;
}param_thread;


void * senderThread(void * param){
    param_thread * parameter = (param_thread *) param;
    struct sockaddr_in serverAddr  = parameter->infoSocket;
    size_t addrServeurLen = (size_t) sizeof(serverAddr);
    while (1)
    {
        int ds = socket(AF_INET, SOCK_STREAM, 0);
        if (ds == -1){
            perror("Client : pb creation socket :");
            exit(1); // je choisis ici d'arrêter le programme car le reste
                // dépendent de la réussite de la création de la socket.
        }
        if (connect(ds,(struct sockaddr *)&serverAddr,addrServeurLen)==-1){
            perror("connect error"); 
            close(ds);
            exit(1);
        }
        while (1)
        {
            structEnvoi structTCP;
            char placeholderuuid[64] = "btr-265-verv-ver";
            structTCP.action = 1;
            structTCP.x = 0;
            structTCP.y = 1;
            strcpy(structTCP.uuidPlayer,placeholderuuid);
            
            if (sendTCP(ds,&structTCP,sizeof(structEnvoi)))
            {
                close(ds);
                exit(EXIT_FAILURE);
                free(parameter);
            }
            if(recvTCP(ds,&structTCP,sizeof(structEnvoi)))
            {
                close(ds);
                exit(EXIT_FAILURE);
                free(parameter);
            }   
        }
    }
    
}







int main(int argc, char *argv[]) {

  /* je passe en paramètre l'adresse de la socket du serveur (IP et
     numéro de port) et un numéro de port à donner à la socket créée plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     paramètres sont à adapter en fonction des besoins. Sans ces
     paramètres, l'exécution doit être arrétée, autrement, elle
     aboutira à des erreurs.*/
  if (argc != 4){
    printf("utilisation : %s ip_serveur port_serveur nbProc\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int f = 1;
  int i = 0;
  int nb = atoi(argv[3]);
  FILE *file = fopen("ping_times.txt", "a");

  struct sockaddr_in addrServeur;
  socklen_t addrServeurLen = (socklen_t) sizeof(addrServeur);
  addrServeur.sin_addr.s_addr = inet_addr(argv[1]);//inet_pton dans le cas on ça ne fonctionne pas
  addrServeur.sin_family=AF_INET;
  addrServeur.sin_port = htons((short)atoi(argv[2]));

  for (i; i < nb && f > 0; i++)
  {
    pthread_t thread;
    param_thread * parameter = (param_thread *) malloc(sizeof(param_thread));
    parameter->infoSocket = addrServeur;
    if (pthread_create(&thread,NULL,senderThread,parameter) !=0 )
    {
        perror("erreur creation thread");
        exit(1);
    }
  }
  int ds = socket(AF_INET, SOCK_STREAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Client : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépendent de la réussite de la création de la socket.
  }
  
  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  //printf("Client : creation de la socket réussie \n");
  
  if (connect(ds,(struct sockaddr *)&addrServeur,addrServeurLen)==-1){
   perror("connect error"); 
   close(ds);
   exit(1);
  }
  struct timeval start, end;
  long seconds, useconds;
  double total_time;

  struct timeval begin;
  long secondsF, usecondsF;
  double total_timeF;
   gettimeofday(&begin, NULL);
  while (1)
  {
        structEnvoi structTCP;
        char placeholderuuid[64] = "btr-265-verv-ver";
        structTCP.action = 1;
        structTCP.x = 0;
        structTCP.y = 1;
        strcpy(structTCP.uuidPlayer,placeholderuuid);    
        gettimeofday(&start, NULL); 
        if (sendTCP(ds,&structTCP,sizeof(structEnvoi)))
        {
            close(ds);
            exit(EXIT_FAILURE);
        }
        if(recvTCP(ds,&structTCP,sizeof(structEnvoi)))
        {
            close(ds);
            exit(EXIT_FAILURE);
        }   
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    secondsF = end.tv_sec  - begin.tv_sec;
    usecondsF = end.tv_usec - begin.tv_usec;


    total_time = seconds + useconds / 1e6;
    total_timeF = secondsF +usecondsF /1e6;
    if (f > 0)
    {
        fprintf(file,"%d;%f;%f\n",nb, total_time,total_timeF);
    }
    
  }
  
    //printf("connect reussit, entrez une message\n");
    
    

   


  close(ds);
  printf("Client : je termine\n");
  return 0;
}


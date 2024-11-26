#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <pthread.h>

/* Programme serveur */

typedef struct structEnvoi
{
    char uuidPlayer[64];
    int x;
    int y;
    int action;
}structEnvoi;


int sendTCP(int sock,void * msg, int sizeMsg){
   if (sizeMsg < 0) {
        printf("Invalid message size");
        //close(sock);
        return -1;
    }
   int resSend = 0;
   int octet_send = 0;
   while (octet_send < sizeMsg)
   {
      resSend = write(sock,msg+octet_send,sizeMsg-octet_send);
      if (resSend == -1 || resSend == 0)
      {
         return resSend;
      }
      octet_send = octet_send + resSend;
   }
   return octet_send;
}

int recvTCP(int sock,void * msg, int sizeMsg){
   if (sizeMsg < 0) {
        printf("Invalid message size");

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
     // printf("%d\n",sizeMsg);
     // printf("%d\n",octet_recv);
      if (resRecv == -1 )
      {
         return -1;
      }else
      {
         if (resRecv == 0)
         {
            return -1;
         }
         
      }
      
      
   }
   //printf("end\n");
   return 0;
}

struct param_thread_serv{
   int socketClient;
};


void * thread_serv(void * param)
{
  struct param_thread_serv * param_serv = (struct param_thread_serv *) param;
  
  printf("Client accepte\n");
  int taille;
  printf("debut de recvTCP\n"); 

   structEnvoi buffer;
   while (1)
   {
    /* code */
        taille = recvTCP(param_serv->socketClient,&buffer,sizeof(structEnvoi));
        if (taille == -1)
        {
            perror("revc error");
            close(param_serv->socketClient);
            free(param_serv);
            pthread_exit(NULL);
        }
        //printf("recvTCP 2 fini\n");
        printf("uuid :%s\n", buffer.uuidPlayer);
        //ça devrait simuler une temps de calcul
        for (int i = 0; i < 513; i++)
        {
         //toujour de la similation
        }
        if(sendTCP(param_serv->socketClient,&buffer,sizeof(structEnvoi))==-1){
            perror("send error");
            close(param_serv->socketClient);
            free(param_serv);
            pthread_exit(NULL);
        }
    }
   close(param_serv->socketClient);
   free(param_serv);
   pthread_exit(NULL);
  }




int main(int argc, char *argv[]) {

  /* Je passe en paramètre le numéro de port qui sera donné à la socket créée plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     paramètres sont à adapter en fonction des besoins. Sans ces
     paramètres, l'exécution doit être arrétée, autrement, elle
     aboutira à des erreurs.*/
  if (argc != 1){
    printf("utilisation : %s\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Serveur : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépendent de la réussite de la création de la socket.
  }
  
  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("Serveur : creation de la socket réussie \n");
  
  struct sockaddr_in myaddr;socklen_t len_myaddr =(socklen_t) sizeof(myaddr);
  myaddr.sin_family=PF_INET;
  myaddr.sin_port=htons((short)0);
  myaddr.sin_addr.s_addr=INADDR_ANY;

  if(bind(ds,(struct sockaddr *)&myaddr,len_myaddr)){
   perror("bind error");
   close(ds);
   exit(1);
  }

   getsockname(ds,(struct sockaddr *)&myaddr, &len_myaddr);
   printf("%d\n",(ntohs(myaddr.sin_port)));

  if(listen(ds,100)==-1){
   perror("Listen error");
   close(ds);
   exit(1);
  }
  printf("On bloque avant accept\n");
  int socketClient;
  scanf("\n");
  while (1)
  {
   /* code */
  printf("On attent une connection\n");
  socketClient=accept(ds,(struct sockaddr *)&myaddr,(socklen_t*)&len_myaddr);
  if (socketClient==-1){ 
   perror("accept error");
   close(ds);
   exit(1);
  }
  

  struct param_thread_serv * parameter = (struct param_thread_serv *) malloc(sizeof(struct param_thread_serv));
  parameter->socketClient = socketClient;
  pthread_t thread;
  printf("On va lancer un nouveau thread\n");
  if (pthread_create(&thread,NULL,thread_serv,parameter) !=0 )
  {
      perror("erreur creation thread");
      exit(1);
  }
   }
  
  close(ds);

  printf("Serveur : je termine\n");
  return 0;
}

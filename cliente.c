#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#define PORT 9999
#define MAXDATASIZE 100000

int main(int argc, char *argv[]){
	
	struct sockaddr_in remoto;
	struct in_addr;
	int conector = socket(AF_INET,SOCK_STREAM,0);
	int conector2;	
	int numbytes=20;
	char buf[MAXDATASIZE], buf2[MAXDATASIZE];
	
	char comando[50];
	
	if(argc>2){
		if(conector==-1){
			printf("Error de conexión\n");
			exit(-1);
		}
		
		remoto.sin_family = AF_INET;
		remoto.sin_port = htons(PORT);
		remoto.sin_addr.s_addr = inet_addr(argv[1]); /*Tenemos una estructura "dest" del tipo sockaddr_in*/
		bzero(&(remoto.sin_zero),8);
		
		if(connect(conector, (struct sockaddr *)&remoto, sizeof(struct sockaddr))==-1){
			printf("Error de connect\n");
			exit(-1);
		}
		
		//Ver qué hace
		if ((numbytes=recv(conector,buf,MAXDATASIZE,0)) == -1){  
		  /* llamada a recv() */
		  printf("Error en recv() \n");
		  exit(-1);
	   }
		
			
		buf[numbytes]='\0';
		printf("Mensaje del Servidor: %s\n", buf);
		
		
		//Enviar yo datos
		
		//Podemos enviar datos
		send(conector, argv[2], 100,0);	//sizeof(argv[1])
		
		if ((numbytes=recv(conector,buf2,sizeof(buf2),0)) == -1){  
			/* llamada a recv() */
			printf("Error en recv() \n");
			exit(-1);
		}

		
		buf2[numbytes]='\0';
		printf("%s\n", buf2);
		
		close(conector);	
	}	

}

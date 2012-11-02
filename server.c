//      server.c
//      
//      Copyright 2012 Adrian Garcia <adrian@adrian-laptop>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#define PORT 9999
#define BACKLOG 200
#define MAXLEN 850

int main(){
	
	struct sockaddr_in server;
	struct sockaddr_in client; 
	int conexion,conexion2;
	int conexion3;
	int sin_size;
	char comandoRemoto[MAXLEN];
	int fd_lectura;
	
	server.sin_family = AF_INET; 
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server.sin_zero),8);
	
	conexion = socket(AF_INET,SOCK_STREAM,0);
	
	if (bind(conexion,(struct sockaddr*)&server, sizeof (struct sockaddr)) == -1) 
	{ 
		printf ("Error en el bind\n"); 
	}
	if(listen(conexion,BACKLOG) == -1) {  /* llamada a listen() */
      printf("error en listen()\n");
      exit(-1);
   }
   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      /* A continuación la llamada a accept() */
      if ((conexion2 = accept(conexion,(struct sockaddr *)&client,&sin_size))==-1){
         printf("error en accept()\n");
         exit(-1);
      }
	   printf("Se obtuvo una conexión desde %s\n",inet_ntoa(client.sin_addr) ); 
	   send(conexion2,"Bienvenido a mi servidor.\n",40,0); 
	   /* que enviará el mensaje de bienvenida al cliente */
		  
		 
			fd_lectura=recv(conexion2,comandoRemoto,sizeof(comandoRemoto),0);
		  comandoRemoto[fd_lectura]='\0';
		  printf("Bytes recibidos: %dB\n",fd_lectura);
		  printf("Comando recibido: %s\n",comandoRemoto);
		pid_t hijo;
		int tuberia[2];
		pipe(tuberia);
		
		hijo=fork();
		if(hijo==0){ //hijo
			close(tuberia[0]);
			dup2(tuberia[1],1);
			dup2(tuberia[1],2);
			close(tuberia[1]);
			execlp(comandoRemoto,comandoRemoto,NULL,NULL);
		} else { //padre
			char buffer[5000]={0};
			close(tuberia[1]);
			while(read(tuberia[0],buffer,sizeof(buffer))!=0);
			send(conexion2,buffer,sizeof(buffer),0);
			
		}
		
	}
}

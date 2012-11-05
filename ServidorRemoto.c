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
#include <strings.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 9999
#define BACKLOG 200
#define MAXLEN 850
#define TEXT_ERROR "Error en el servidor"

int main(){
	
	struct sockaddr_in server;
	struct sockaddr_in client; 
	int serverfd;
	int clientfd;
	int fd_lectura;
	int tuberia[2];
	char comandoRemoto[MAXLEN];
	char hostname_client[MAXLEN];
	pid_t hijo;

	server.sin_family = AF_INET; 
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server.sin_zero),8);
	
	serverfd = socket(AF_INET,SOCK_STREAM,0);
	
	if (bind(serverfd,(struct sockaddr*)&server, sizeof (struct sockaddr)) == -1) perror(TEXT_ERROR);
	
	if(listen(serverfd,BACKLOG) == -1) perror(TEXT_ERROR);
	
	
   while(1) {
		int sin_size=sizeof(struct sockaddr_in);
		  
		if ((clientfd = accept(serverfd,(struct sockaddr *)&client,&sin_size))==-1) perror(TEXT_ERROR);
		
		getnameinfo((struct sockaddr *) &client, sizeof(client), hostname_client, sizeof(hostname_client), NULL, 0, NI_NAMEREQD);
		printf(">> Conexion entrante: IP %s (%s)\n",(char*)inet_ntoa(client.sin_addr),hostname_client); 
		send(clientfd,"Connected",sizeof("Connected"),0);
		pipe(tuberia);
		fd_lectura=recv(clientfd,comandoRemoto,sizeof(comandoRemoto),0);
		comandoRemoto[fd_lectura]='\0';
		printf(">> Comando recibido: %s\n",comandoRemoto);
		printf(">> Ejecutando %s...\n",comandoRemoto);

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
			send(clientfd,buffer,sizeof(buffer),0);
		}
	}
}

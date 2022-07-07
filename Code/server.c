/**
 * @file server.c
 * @author Alary Dorian
 * @brief 
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"


/* Structure */
struct client_s
{
   SOCKET sock;
   int id;
};


/**
 * @brief Main function
 * 
 * @param argc number of arguments
 * @param argv list of arguments
 * @return int exit value
 */
int main()
{
   init();
   appS();
   end();

   return EXIT_SUCCESS;
}


/**
 * @brief Initialisation of dll in windows to use socket
 * 
 */
static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      fprintf(stderr, "Error : WSAStartup()\n");
      exit(EXIT_FAILLURE_INIT);
   }
#endif
}


/**
 * @brief Kill dll in windows
 * 
 */
static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}


/**
 * @brief Init connection, creation of a socket
 * 
 * @return SOCKET, FD of socket connection
 */
static SOCKET initConnection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin;

   if(sock == INVALID_SOCKET)
   {
      fprintf(stderr, "Error : socket()\n");
      exit(EXIT_FAILURE_SOCKET);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      fprintf(stderr, "Error : bind()\n");
      exit(EXIT_FAILURE_BIND);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      fprintf(stderr, "Error : listen()\n");
      exit(EXIT_FAILURE_LISTEN);
   }

   printf("Server open...\n");

   return sock;
}


/**
 * @brief Close connection socket
 * 
 * @param sock socket
 */
static void endConnection(SOCKET sock)
{
   closesocket(sock);
}


/**
 * @brief Read data of client and store data in buffer
 * 
 * @param sock socket of client
 * @param buffer variable that stores the message
 * @param len len of the buffer, less than or equal to the size of the buffer
 * @return int number of characters read
 */
static int readClient(SOCKET sock, char *buffer, int len)
{
   int n = 0;

   if((n = recv(sock, buffer, len-1, 0)) < 0)
   {
      fprintf(stderr, "Error : recv()\n");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;
   return n;
}


/**
 * @brief Write data to the client
 * 
 * @param sock socket of client
 * @param buffer variable that stores the message
 */
static void writeClient(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      fprintf(stderr, "Error : send()\n");
      exit(EXIT_FAILURE_SEND);
   }
}


/**
 * @brief Server application
 * 
 */
static void appS(void)
{
   SOCKET sock = initConnection();
   fd_set rdfs; //variable which is used to use select
   Connected *client_list = list_create();
   ListIterator *iterator_client_list = listIterator_create(client_list, FORWARD_ITERATOR);
   Client *c;

   int connection = 1; //keep the application alive
   char buffer[BUF_SIZE];
   int max = sock; //max socket number which is used to use select
   int id = 0; //id of client
   int i; //follow element in iterator list

   while(connection)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket to see new connection */
      FD_SET(sock, &rdfs);

      /* add the clients socket to see new message of clients*/
      for(iterator_client_list=listIterator_begin(iterator_client_list) ; !listIterator_end(iterator_client_list) ; iterator_client_list=listIterator_next(iterator_client_list))
      { 
         FD_SET(((Client*)listIterator_value(iterator_client_list))->sock, &rdfs);
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         fprintf(stderr, "Error : select()\n");
         exit(EXIT_FAILURE_SELECT);
      }

      if(FD_ISSET(STDIN_FILENO, &rdfs)) /* something from standard input : i.e keyboard -> leave */
      {
         /* stop process when type on keyboard */
         connection = 0;
         break;
      }
      else if(FD_ISSET(sock, &rdfs)) /* new client */
      {
         
         SOCKADDR_IN client_sin;
         socklen_t client_sin_size = sizeof(client_sin);
         int client_sock;

         if((client_sock = accept(sock, (SOCKADDR *)&client_sin, &client_sin_size)) == SOCKET_ERROR)
         {
            fprintf(stderr, "Error : accept()\n");
            continue;
         }

         /* what is the new maximum fd ? */
         max = (client_sock > max) ? client_sock : max;

         c = malloc(sizeof(Client));
         c->sock = client_sock;
         c->id = id;
         id++;
         list_push_front(client_list, c);
         printf("Client connexion.. Id client=%d\n", c->id);
      }
      else
      {
         i = 0;
         for(iterator_client_list=listIterator_begin(iterator_client_list) ; !listIterator_end(iterator_client_list) ; iterator_client_list=listIterator_next(iterator_client_list))
         { 
            c = ((Client *)listIterator_value(iterator_client_list));
            if(FD_ISSET(c->sock, &rdfs))
            {
               if(readClient(c->sock, buffer, BUF_SIZE) == 0)
               {
                  closesocket(c->sock);
                  list_remove_at(client_list, i);
                  printf("Client deconnexion.. Id client=%d\n", c->id);
                  break; //break everytime when a value is remove or pop in the list, listIterator_begin puts the iterator at the right value, after we can start new loop
               }
               else
               {
                  printf("[%d] : %s", c->id, buffer);
               }
            }
            i++;
         }
      }
   }
   
   for(iterator_client_list=listIterator_begin(iterator_client_list) ; !listIterator_end(iterator_client_list) ; iterator_client_list=listIterator_next(iterator_client_list))
   { 
      closesocket(((Client *)listIterator_value(iterator_client_list))->sock);
   }
   list_delete(client_list);
   listIterator_delete(iterator_client_list);
   endConnection(sock);
}

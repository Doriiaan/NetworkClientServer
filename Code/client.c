/**
 * @file client.c
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
#include <string.h>
#include <pthread.h>
#include "client.h"

/* Structures */
struct client_s
{
   SOCKET sock;
   int *connection;
};


/**
 * @brief Main function
 * 
 * @param argc number of arguments
 * @param argv list of arguments
 * @return int exit value
 */
int main(int argc, char **argv)
{
   if(argc != 2)
   {
      printf("Usage : %s [address]\n", argv[0]);
      return EXIT_FAILURE;
   }

   init();
   appC(argv[1]);
   end();

   return EXIT_SUCCESS;
}


/**
 * @brief Initialisation of dll in windows to use socket
 * 
 */
static void init(void){
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      fprintf(stdrr, "Error : WSAStartup()\n");
      exit(EXIT_FAILURE_INIT);
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
 * @brief Initialisation of connection to the address, with socket
 * 
 * @param address of server
 * @return SOCKET, FD of socket
 */
static SOCKET initConnection(const char *address)
{
    SOCKET sock;
    SOCKADDR_IN sin;

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        fprintf(stderr, "Error : socket()\n");
        exit(EXIT_FAILURE_SOCKET);
    }

    sin.sin_addr.s_addr = inet_addr(address);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Error : connect()\n");
        exit(EXIT_FAILURE_CONNECTION);
    }

    return sock;
}


/**
 * @brief Close socket connection
 * 
 * @param sock the socket to close
 */
static void endConnection(SOCKET sock)
{
    closesocket(sock);
}


/**
 * @brief Blocking function to read message sent from the server
 * 
 * @param sock socket
 * @param buffer variable that stores the message
 * @param len len of the buffer, less than or equal to the size of the buffer
 * @return int number of characters read
 */
static int readServer(SOCKET sock, char *buffer, int len)
{
   int n = 0;

   if((n = recv(sock, buffer, len-1, 0)) < 0)
   {
      fprintf(stderr, "Error : recv()\n");
      exit(EXIT_FAILURE_RECV);
   }

   buffer[n] = '\0';

   return n;

}


/**
 * @brief Sends message to the server
 * 
 * @param sock socket
 * @param buffer variable that stores the message
 */
static void writeServer(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < (ssize_t)strlen(buffer))
   {
      fprintf(stderr, "Error : send()\n");
      exit(EXIT_FAILURE_SEND);
   }
}


/**
 * @brief Application of client
 * 
 * @param address Adress of server 
 */
static void appC(const char *address)
{

    SOCKET sock = initConnection(address);

    int connection = 1; //keep the app alive
    char buffer[BUF_SIZE]; //to store message

    Client c; //the client connection
    c.connection = &connection;
    c.sock = sock;

    pthread_t wait_disconnection_message;
    pthread_create(&wait_disconnection_message, NULL, wait_server_disconnection, &c);

    while(connection)
    {
        printf("\n-----------------------------------------------------\n");
        printf("Menu :\n");
        printf("\t[0] Send a message to the server\n");
        printf("\t[1] Deconnexion\n");
        printf("\nGive your choice : ");
        if(fgets(buffer, BUF_SIZE-1, stdin) == NULL)
        {
            printf("\nAn error occurs..\n");
            continue;
        }
        if(connection) //logout message sent from the server may happen during fgets
        { 
            if(strcmp(buffer, "0\n") == 0)
            {
                printf("\nYou're message :\n\n\t");
                if(fgets(buffer, BUF_SIZE-1, stdin) == NULL)
                {
                    printf("\nAn error occurs..\n");
                    continue;
                }
                if(connection) //logout message sent from the server may happen during fgets
                    writeServer(sock, buffer);
            }
            else if(strcmp(buffer, "1\n") == 0)
            {
                fprintf(stdin, "Disconnected..\n");
                pthread_cancel(wait_disconnection_message);
                connection = 0;
            }
            else
            {
                printf("It's not an option !\n");
            }
        }
    }

    endConnection(sock);
}


/**
 * @brief Wait server logout
 * 
 * @param arg Client connection {socket, connection}
 * @return void* 
 */
void *wait_server_disconnection(void *arg)
{

    Client *c = (Client *) arg;
    char buffer[2];

    int n = readServer(c->sock, buffer, 2); //wait to '\0' message
    /* server down */
    if(n == 0)
    {
        printf("\n\nServer disconnected !\n");
        *(c->connection) = 0;
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

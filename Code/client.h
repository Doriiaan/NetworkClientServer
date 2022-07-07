/**
 * @file client.h
 * @author Alary Dorian
 * @brief 
 * @version 0.1
 * @date 2022-07-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __CLIENT_H__
#define __CLIENT_H__

#ifdef WIN32 /* windows environment */

#include <winsock2.h>

#elif defined (linux) /* linux environment */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else
#error not defined for this platform
#endif


/* Exit defines */
#define EXIT_FAILLURE_INIT 1
#define EXIT_FAILURE_SOCKET 2
#define EXIT_FAILURE_CONNECTION 3
#define EXIT_FAILURE_RECV 4
#define EXIT_FAILURE_SEND 5
#define EXIT_FAILURE_SELECT 6


/* Value defines */
#define PORT 27000
#define BUF_SIZE 1024


/* Structures */
typedef struct client_s Client;

/* Functions */
static void init(void);
static void end(void);
static void appC(const char *address);
static SOCKET initConnection(const char *address);
static void endConnection(SOCKET sock);
static int readServer(SOCKET sock, char *buffer, int len);
static void writeServer(SOCKET sock, const char *buffer);
void *wait_server_disconnection(void *arg);

#endif
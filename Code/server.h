#ifndef __SERVER_H__
#define __SERVER_H__

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

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


/* Includes */
#include "List/list.h"
#include "Queue/queue.h"

/* Exit values */
#define EXIT_FAILLURE_INIT 1
#define EXIT_FAILURE_SOCKET 2
#define EXIT_FAILURE_CONNECTION 3
#define EXIT_FAILURE_RECV 4
#define EXIT_FAILURE_SEND 5
#define EXIT_FAILURE_SELECT 6
#define EXIT_FAILURE_BIND 7
#define EXIT_FAILURE_LISTEN 8


/* Values */
#define PORT 27000
#define MAX_CLIENTS 10
#define BUF_SIZE 1024


/* Structures */
typedef struct client_s Client;
typedef List Connected;
typedef Queue Waiting;


/* Functions */
/**
 * @brief Initialisation of dll in windows to use socket
 * 
 */
static void init(void);


/**
 * @brief Kill dll in windows
 * 
 */
static void end(void);


/**
 * @brief Init connection, creation of a socket
 * 
 * @return SOCKET, FD of socket connection
 */
static SOCKET initConnection(void);


/**
 * @brief Close connection socket
 * 
 * @param sock socket
 */
static void endConnection(SOCKET sock);


/**
 * @brief Read data of client and store data in buffer
 * 
 * @param sock socket of client
 * @param buffer variable that stores the message
 * @param len len of the buffer, less than or equal to the size of the buffer
 * @return int number of characters read
 */
static int readClient(SOCKET sock, char *buffer, int len);


/**
 * @brief Write data to the client
 * 
 * @param sock socket of client
 * @param buffer variable that stores the message
 */
static void writeClient(SOCKET sock, const char *buffer);


/**
 * @brief Server application
 * 
 */
static void appS(void);

#endif
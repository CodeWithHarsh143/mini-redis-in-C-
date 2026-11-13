/* server*/
#ifndef SERVER
#define SERVER
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
/* defines */
#define HOST "127.0.0.1"
#define PORT "12049"
#define CONNECT_NO 20
/* typedef's*/
typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char int8;
typedef struct sock_client Client;
typedef int32 (*Callback)(Client *, int8, int8);
struct s_cmdHandler {
  int8 *cmd;
  Callback handler;
};
typedef s_cmdHandler CmdHandler;
/* code init */
int32 handle_hello(Client *, int8 *, int8 *);
Callback getcmd(int8 *);
void mainloop(int);
int initServer(int16);
int main(int, char **);
#endif

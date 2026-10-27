/* cache.c */
#include "cache.h"
#include <errno.h>
bool server_continuation;

void mainloop(int serverFD) {
  struct sockaddr_in client;
  int ser_cli_FD;
  int32 len;
  int16 port;
  char *ip;

  ser_cli_FD =
      accept(serverFD, (struct sockaddr *)&client, (unsigned int *)&len);

  if (ser_cli_FD == -1) {
    perror("Accept");
    return;
  }

  port = (int16)htons((int)client.sin_port);
  ip = inet_ntoa(client.sin_addr);

  printf("Connection from %s:%d\n", ip, port);
}
int initServer(int16 port) {
  struct sockaddr_in sock;
  int serverFD;
  errno = 0;

  sock.sin_port = htons(port);
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = inet_addr(HOST);

  serverFD = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFD == -1) {
    perror("socket");
    return -1;
  }

  if (bind(serverFD, (struct sockaddr *)&sock, sizeof(sock))) {
    // non-zero values are error
    perror("Binding");
    return -1;
  }

  if (listen(serverFD, CONNECT_NO)) {
    // non-zero values are error
    perror("listen");
    return -1;
  }

  printf("Server listing on %s:%d\n", HOST, port);
  return serverFD;
}

int main(int argc, char *argv[]) {
  char *server_port;
  int16 port;
  int serverFD;
  if (argc < 2)
    server_port = PORT;
  else
    server_port = argv[1];
  port = (int16)atoi(server_port);
  server_continuation = true;
  serverFD = initServer(port);
  while (server_continuation)
    mainloop(serverFD);
  printf("Server is successfully closed...\n");
  while (server_continuation)
    mainloop(serverFD);
  close(serverFD);
  return 0;
}

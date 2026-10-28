/* server.c */
#include "server.h"
#include <stdlib.h>
bool server_continuation;
bool child_contination;
void childloop(Client *client)
{
  // tmep
  sleep(1);
  return;
}
void mainloop(int serverFD)
{
  struct sockaddr_in cli;
  int ser_cli_FD;
  socklen_t len;
  int16 port;
  char *ip;
  len = sizeof(cli);
  pid_t pid;
  ser_cli_FD = accept(serverFD, (struct sockaddr *)&cli, &len);

  if (ser_cli_FD == -1)
  {
    close(ser_cli_FD);
    return;
  }

  port = ntohs(cli.sin_port);
  ip = inet_ntoa(cli.sin_addr);

  printf("Connection from %s:%d\n", ip, port);

  Client *client = malloc(sizeof(struct sock_client));
  if (client == NULL)
  {
    perror("malloc");
    close(ser_cli_FD);
    return;
  }
  *client = (Client){0};
  client->port = port;
  client->FD = ser_cli_FD;
  strcpy(client->ip, ip);
  pid = fork();
  if (pid)
  {
    // not in child process
    free(client);
    return;
  }
  else
  {
    //  in child process
    dprintf(ser_cli_FD, "Client connected to the cache server");
    child_contination = true;
    while (child_contination)
      childloop(client);
    close(ser_cli_FD);
    free(client);
  }

  return;
}
int initServer(int16 port)
{
  struct sockaddr_in sock = {0};
  int serverFD;

  sock.sin_port = htons(port);
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = inet_addr(HOST); // ip address

  serverFD = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFD == -1)
  {
    perror("socket");
    return -1;
  }

  if (bind(serverFD, (struct sockaddr *)&sock, sizeof(sock)))
  {
    // non-zero values are error
    perror("Binding");
    close(serverFD);
    return -1;
  }

  if (listen(serverFD, CONNECT_NO))
  {
    // non-zero values are error
    perror("listen");
    close(serverFD);
    return -1;
  }

  printf("Server listing on %s:%d\n", HOST, port);
  return serverFD;
}

int main(int argc, char *argv[])
{
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
  if (serverFD == -1)
    return EXIT_FAILURE;
  while (server_continuation)
    mainloop(serverFD);
  printf("Server is successfully closed...\n");
  close(serverFD);
  return 0;
}

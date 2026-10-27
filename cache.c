/* cache.c */
#include "cache.h"
#include <errno.h>
bool server_continuation;

void mainloop(int16 port) {
  struct sockaddr_in sock;
  int serverFD;
  errno = 0;

  sock.sin_port = htons(port);
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = inet_addr(HOST);

  serverFD = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFD == -1) {
    perror("socket");
    return;
  }

  if (bind(serverFD, (struct sockaddr *)&sock, sizeof(sock))) {
    // non-zero values are error
    perror("Binding");
    return;
  }

  if (listen(serverFD, CONNECT_NO)) {
    // non-zero values are error
    perror("listen");
    return;
  }
}

int main(int argc, char *argv[]) {
  char *server_port;
  int16 port;
  if (argc < 2)
    server_port = PORT;
  else
    server_port = argv[1];
  port = (int16)atoi(server_port);
  server_continuation = true;
  mainloop(port);
}

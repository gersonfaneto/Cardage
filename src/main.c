// See: https://gist.github.com/jdah/08eb80c74b13176027c08cbf48b239c9/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <netdb.h>

#include <caffe/core/assert.h>

#include <raylib.h>

void server(void);
void client(int port);

#ifndef __GAME_UI__

int main(int argc, char* argv[])
{
  if (argc > 1 && !strcmp(argv[1], "client"))
  {
    if (argc != 3)
    {
      fprintf(stderr, "not enough args!");
      return -1;
    }

    int port;
    sscanf(argv[2], "%d", &port);

    client(port);
  }
  else
  {
    server();
  }

  return 0;
}

#else

#define WINDOW_WIDHT 800
#define WINDOW_HEIGHT 600

int main(void)
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  InitWindow(WINDOW_WIDHT, WINDOW_HEIGHT, "Cardage");

  SetExitKey(KEY_Q);

  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    int wh, ww;

    wh = GetScreenHeight();
    ww = GetScreenWidth();

    BeginDrawing();

    {
      DrawFPS(10, 10);

      ClearBackground(GetColor(0xFBF1C7FF));

      if (wh < WINDOW_HEIGHT || ww < WINDOW_WIDHT)
      {
        ClearBackground(RED);
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

#endif

void server(void)
{
  // create socket
  const int fd = socket(PF_INET, SOCK_STREAM, 0);

  // bind to open port
  struct sockaddr_in addr;
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(fd, ( struct sockaddr* )&addr, sizeof(addr)))
  {
    perror("bind error:");
    return;
  }

  // read port
  socklen_t addr_len = sizeof(addr);
  getsockname(fd, ( struct sockaddr* )&addr, &addr_len);
  printf("server is on port %d\n", ( int )ntohs(addr.sin_port));

  if (listen(fd, 1))
  {
    perror("listen error:");
    return;
  }

  // accept incoming connection
  struct sockaddr_storage caddr;
  socklen_t               caddr_len = sizeof(caddr);
  const int               cfd       = accept(fd, ( struct sockaddr* )&caddr, &caddr_len);

  // read from client with recv!
  char buf[1024];
  recv(cfd, buf, sizeof(buf), 0);

  // print without looking
  printf("client says:\n    %s\n", buf);

  close(cfd);
  close(fd);
}

void client(int port)
{
  const int fd = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr = {0};
  addr.sin_family         = AF_INET;
  addr.sin_port           = htons(( short )port);

  // connect to local machine at specified port
  char addrstr[NI_MAXHOST + NI_MAXSERV + 1];
  snprintf(addrstr, sizeof(addrstr), "127.0.0.1:%d", port);

  // parse into address
  inet_pton(AF_INET, addrstr, &addr.sin_addr);

  // connect to server
  if (connect(fd, ( struct sockaddr* )&addr, sizeof(addr)))
  {
    perror("connect error:");
    return;
  }

  // say hey with send!
  const char* msg = "the client says hello!";
  send(fd, msg, strlen(msg) + 1, 0);

  close(fd);
}

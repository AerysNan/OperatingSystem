#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }
  char buffer[4];
  int p2c[2], c2p[2];
  if (pipe(p2c) < 0 || pipe(c2p) < 0)
  {
    fprintf(2, "pingpong: failed to create pipe\n");
    exit(1);
  }
  int pid = fork();
  if (pid < 0)
  {
    fprintf(2, "pingpong: failed to create subprocess\n");
    exit(1);
  }
  else if (pid > 0)
  {
    close(p2c[0]);
    close(c2p[1]);
    write(p2c[1], "ping", 4);
    read(c2p[0], buffer, 4);
    printf("%d: received %s\n", getpid(), buffer);
  }
  else
  {
    close(p2c[1]);
    close(c2p[0]);
    read(p2c[0], buffer, 4);
    printf("%d: received %s\n", getpid(), buffer);
    write(c2p[1], "pong", 4);
  }
  exit(0);
}
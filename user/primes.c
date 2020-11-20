#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int in)
{
  int p;
  if (read(in, &p, sizeof(int)) <= 0)
  {
    close(in);
    return;
  }
  printf("prime %d\n", p);
  int fd[2];
  if (pipe(fd) < 0)
  {
    close(in);
    fprintf(2, "primes: failed to create pipe\n");
    exit(1);
  }
  if (fork() > 0)
  {
    close(fd[1]);
    sieve(fd[0]);
  }
  else
  {
    close(fd[0]);
    int n;
    while (read(in, &n, sizeof(int)))
      if (n % p != 0)
        write(fd[1], &n, sizeof(int));
    close(in);
    close(fd[1]);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf(2, "Usage: primes\n");
    exit(1);
  }
  int fd[2];
  if (pipe(fd) < 0)
  {
    fprintf(2, "primes: failed to create pipe\n");
    exit(1);
  }
  if (fork() > 0)
  {
    close(fd[1]);
    sieve(fd[0]);
  }
  else
  {
    close(fd[0]);
    for (int i = 2; i <= 35; i++)
      write(fd[1], &i, sizeof(int));
    close(fd[1]);
  }
  exit(0);
}

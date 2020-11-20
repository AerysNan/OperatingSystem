#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char buffer[512], *args[32];
  char m[32][32];
  for (int i = 0; i < 32; i++)
    args[i] = m[i];
  for (int i = 1; i < argc; i++)
    strcpy(m[i - 1], argv[i]);
  if (read(0, buffer, sizeof(buffer)) <= 0)
  {
    fprintf(2, "xargs: failed to read command\n");
    exit(1);
  }
  else
  {
    int index = argc - 1, st;
    char *c = m[index];
    for (char *p = buffer; *p > 0; p++)
    {
      if (*p == ' ' || *p == '\n')
      {
        *c = '\0';
        c = m[++index];
      }
      else
        *c++ = *p;
    }
    *c = '\0';
    args[++index] = 0;
    if (fork())
      wait(&st);
    else
      exec(args[0], args);
  }
  exit(0);
}
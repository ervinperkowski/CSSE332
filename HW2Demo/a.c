#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  printf("Hello from program %d\n", getpid());
  while(1);
  return 0;
}

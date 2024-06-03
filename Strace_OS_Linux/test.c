#include <stdio.h>
#define INT_MAX 2147483647
int main(volatile int argc, char **argv) {
      int i = 3;
      char str[] = "Hello\n";
      FILE *fd = fopen("test_out.txt", "w");
      for (int j = 0; j < 100; j++){
        fwrite(str, sizeof(str), 1, fd);
      }
      fclose(fd);
      return 0;
}
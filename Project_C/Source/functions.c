#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>

int listc(void) {
  DIR *d;
  struct dirent *dir;
  d = opendir("./Serverfiles");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n",dir->d_name);
    }
    closedir(d);
    return 0;
  } else {
      return -1;
  }
}

  void get(char* filename) {
  unsigned char buffer[4096];
  char* src_path = strcat("./Serverfiles/", filename);
  char* dst_path = strcat("./Clientfiles/", filename);
  int src_fd = open(src_path, O_RDONLY);
  int dst_fd = open(dst_path, O_CREAT | O_WRONLY);

  while (1) {
      int err = read(src_fd, buffer, 4096);
      if (err == -1) {
          printf("Error reading file.\n");
      }
      int n = err;

      if (n == 0) break;

      err = write(dst_fd, buffer, n);
      if (err == -1) {
          printf("Error writing to file.\n");
      }
  }

    close(src_fd);
    close(dst_fd);
}

int hexit(void) {
  int pid = getpid();
  printf("Exiting...");
  kill(pid, SIGHUP);
}
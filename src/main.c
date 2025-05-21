#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "shell.h"
#include "disk.h"

void lsh_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv) {
  // Initialize file system
  if(argc!=3) {
    printf("use: %s <diskfile> <nblocks>\n",argv[0]);
    return 1;
  }

  if(!disk_init(argv[1],atoi(argv[2]))) {
    printf("couldn't initialize %s: %s\n",argv[1],strerror(errno));
    return 1;
  }

  printf("opened emulated disk image %s with %d blocks\n",argv[1],disk_size());
  // Load config files, if any

  // Run command loop
  lsh_loop();

  // Perform any shutdown/cleanup
  printf("closing emulated disk.\n");
  disk_close();

  return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

// *** Add SYS_LOG stuff ***

int main (int argc, char* argv[]) {
  // First we check for the correct number of arguments,
  // exiting if there aren't exactly 2.
  if (argc != 3) {
    fprintf (stderr, "Usage: %s <directory> <string>\n", argv[0]);
    openlog(NULL, 0, LOG_USER);
    syslog(LOG_ERR, "Attempted to run %s with incorrect number of arguments (%d - takes 2).\n", argv[0], (argc-1));
    exit(1);
  }

  // Split out the directory name.
  char *file = basename(argv[1]);
  char *dir = dirname(argv[1]);

  // Create command line for mkdir.
  // Might be cheating a bit, but we don't need to create directory anyway.
  char *tempString = malloc(strlen(dir) + strlen(file) + strlen(".txt") + 12);
  sprintf(tempString, "mkdir -p %s", dir);

  // Check if the directory exists, create it if not.
  if (system(tempString) != 0) {
    FILE *nowhere;
    nowhere = fopen("/dev/null", "w");
    fprintf (nowhere, "mkdir: %s: %s\n", dir, strerror (errno));
  }

  // Rebuild the path.
  sprintf(tempString, "%s/%s", dir, file);

  // Create the file and write the contents.
  // IAW Ch2 of LSP book.
  int fd;
  fd = open(tempString, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    fprintf (stderr, "open: %s: %s\n", tempString, strerror (errno));
    openlog(NULL, 0, LOG_USER);
    syslog(LOG_ERR, "Could not create file when running %s: %s.\n", argv[0], strerror (errno));
  }

  // Write to syslog.
  openlog(NULL, 0, LOG_USER);
  syslog(LOG_DEBUG, "Writing %s to %s\n", argv[2], tempString);
  
  // Write the string to the file.  
  int retVal = write(fd, argv[2], strlen(argv[2]));
  (void) retVal;

  free(tempString);

  return 0;
}

#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("WKP");
    exit(0);
  }
}

int main() {
  int from_client;
  int parent = getpid();
  while (1){
    from_client = server_setup();
    if (getpid() != parent) {
      subserver(from_client);
    }
  }
  return 0;
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  while ( read(from_client, buffer, sizeof(buffer)) ) {
    printf("Received from client: %s\n", buffer);
    process(buffer);
    write(to_client, buffer, sizeof(buffer));
  }
  exit(0);
}

void process(char * s) { //rot13
  int i;
  for ( i=0 ; i < strlen(s) ; i++ ) {
    
    if(s[i] - 'A' > -1 && s[i] - 'A' < 26)
      s[i] = ((s[i] - 'A') + 13) % 26 + 'A';
    else if(s[i] - 'a' > -1 && s[i] - 'a' < 26)
      s[i] = ((s[i] - 'a') +13) % 26 + 'a';
  }
}

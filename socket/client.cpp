#include "common.h"

int doWrite(const int fd) {
  static char buf[] = "Hello, world!";
  int n = write(fd, buf, strlen(buf));
  printf("file: %s, line: %d\n\tret: %d, errno: %d, error: %s\n", __FILE__,
         __LINE__, n, errno, strerror(errno));
  positiveOrExit(n);
  printf("write %d bytes\n", n);
  return n;
}

int doRead(const int fd) {
  char buf[128];
  memset(buf, 0, sizeof(buf));
  int n = read(fd, buf, sizeof(buf));
  printf("file: %s, line: %d\n\tret: %d, errno: %d, error: %s\n", __FILE__,
         __LINE__, n, errno, strerror(errno));
  // positiveOrExit(n);
  return n;
}

int main() {
  int fd = socket(PF_INET, SOCK_STREAM, 0);
  positiveOrExit(fd);

  int buf_size = 0;
  socklen_t size = sizeof(buf_size);
  zeroOrExit(getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buf_size, &size));
  printf("SO_SNDBUF: %d\n", buf_size);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3333);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  zeroOrExit(connect(fd, (struct sockaddr*)&addr, sizeof(addr)));
  printf("connected\n");
  setNonBlock(fd);
  setTimeout(fd, 6, 2);

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "netstat -tn | grep ':%hu ' | sed 's/  */|/g'",
           ntohs(addr.sin_port));
  system(cmd);

  int n = 1;
  n = doWrite(fd);

  while (true) {
    n = doRead(fd);
    sleep(1);
  }

  close(fd);

  return 0;
}

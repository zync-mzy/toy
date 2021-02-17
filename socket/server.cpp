#include "common.h"

int main() {
  int fd = socket(PF_INET, SOCK_STREAM, 0);
  positiveOrExit(fd);

  int enable = 1;
  zeroOrExit(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)));
  struct linger lo = {1, 0};
  // setsockopt(fd, SOL_SOCKET, SO_LINGER, &lo, sizeof(lo));
  int buf_size = 128;
  zeroOrExit(
      setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)));
  socklen_t size = sizeof(buf_size);
  zeroOrExit(getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &buf_size, &size));
  printf("SO_RCVBUF: %d\n", buf_size);
  // setNonBlock(fd);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3333);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  zeroOrExit(bind(fd, (struct sockaddr*)&addr, sizeof(addr)));
  zeroOrExit(listen(fd, 0));

  while (true) {
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    printf("listening...\n");
    int client = accept(fd, NULL, NULL);
    positiveOrExit(client);
    printf("accepted: %d\n", client);
    setTimeout(client, 6, 1);

    int n = read(client, buf, sizeof(buf));
    positiveOrExit(n);
    printf("recv: %s\n", buf);
    sprintf(buf + n, "\nYou too!");

    n = write(client, buf, strlen(buf));
    positiveOrExit(n);
    printf("write %d bytes\n", n);

    close(client);
  }

  close(fd);

  return 0;
}

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define printAndExit(ret)                                                                                    \
  {                                                                                                          \
    fprintf(stderr, "file: %s, line: %d\n\tret: %d, errno: %d, error: %s\n", __FILE__, __LINE__, ret, errno, \
            strerror(errno));                                                                                \
    exit(ret);                                                                                               \
  }
#define positiveOrExit(ret)          \
  {                                  \
    if (ret <= 0) printAndExit(ret); \
  }
#define zeroOrExit(ret)              \
  {                                  \
    if (ret != 0) printAndExit(ret); \
  }
inline void setNonBlock(const int fd) {
  int flag = fcntl(fd, F_GETFL);
  positiveOrExit(flag);
  int ret = fcntl(fd, F_SETFL, flag | O_NONBLOCK);
  zeroOrExit(ret);
}
inline void setTimeout(const int fd, const int rw, const int sec, const int usec = 0) {
  struct timeval timeout;
  timeout.tv_sec = sec;
  timeout.tv_usec = usec;

  if ((rw & 2) != 0) zeroOrExit(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)));
  if ((rw & 4) != 0) zeroOrExit(setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)));
}

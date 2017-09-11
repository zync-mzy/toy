#include <cstdio>

int main() {
    long me = 281474976710656, you = 367850545737;
    long love = me + you;
    printf("%s\n", (const char*)&love);
    return 0;
}
// We don't know the meaning of our lives until we meet each other

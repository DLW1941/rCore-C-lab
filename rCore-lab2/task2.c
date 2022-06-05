#include "user.h"

int main() {
    char *s = "Task[2] print0 \n";
    isize w;
    for (int i = 0; i < 3; i++) {
        s[13] = (i % 10) + '0';
        write(FD_STDOUT, s);
        w = get_time() + 30;
        while (get_time() < w); // yield();
    }
    return 0;
}

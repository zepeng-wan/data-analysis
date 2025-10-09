// touch_grab.c (fixed)
// compile: aarch64-linux-gcc -O2 -o touch_grab touch_grab.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>        // <-- required for va_start/va_end
#include <sys/ioctl.h>
#include <linux/input.h>
#include <time.h>

static int fd = -1;
static const char *DEV = "/dev/input/event1";
static const char *LOGF = "/userdata/touch_grab.log";
static volatile sig_atomic_t keep_running = 1;

static void handle_sig(int sig) {
    (void)sig;
    keep_running = 0;
}

static void logmsg(const char *fmt, ...) {
    va_list ap;
    FILE *f = fopen(LOGF, "a");
    if (!f) return;
    time_t t = time(NULL);
    struct tm lt;
    localtime_r(&t, &lt);
    char ts[64];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &lt);

    fprintf(f, "[%s] ", ts);
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fprintf(f, "\n");
    fclose(f);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    fd = open(DEV, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "open(%s) failed: %s\n", DEV, strerror(errno));
        return 1;
    }

    signal(SIGINT, handle_sig);
    signal(SIGTERM, handle_sig);
    signal(SIGQUIT, handle_sig);

    if (ioctl(fd, EVIOCGRAB, (void*)1) == -1) {
        fprintf(stderr, "EVIOCGRAB failed: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    printf("Grabbed %s; press Ctrl+C to exit.\n", DEV);
    logmsg("Grabbed %s", DEV);

    struct input_event ev;
    int cur_slot = 0;

    while (keep_running) {
        ssize_t r = read(fd, &ev, sizeof(ev));
        if (r < (ssize_t)sizeof(ev)) {
            if (r == -1 && errno == EINTR) continue;
            logmsg("read error: %s", strerror(errno));
            break;
        }

        if (ev.type == EV_SYN && ev.code == SYN_REPORT) {
            continue;
        }

        if (ev.type == EV_ABS) {
            if (ev.code == ABS_X) {
                printf("ABS_X = %d\n", ev.value);
                logmsg("ABS_X = %d", ev.value);
            } else if (ev.code == ABS_Y) {
                printf("ABS_Y = %d\n", ev.value);
                logmsg("ABS_Y = %d", ev.value);
            } else if (ev.code == ABS_MT_SLOT) {
                cur_slot = ev.value;
            } else if (ev.code == ABS_MT_POSITION_X) {
                printf("MT[%d] X = %d\n", cur_slot, ev.value);
                logmsg("MT[%d] X = %d", cur_slot, ev.value);
            } else if (ev.code == ABS_MT_POSITION_Y) {
                printf("MT[%d] Y = %d\n", cur_slot, ev.value);
                logmsg("MT[%d] Y = %d", cur_slot, ev.value);
            }
        } else if (ev.type == EV_KEY) {
            if (ev.code == BTN_TOUCH) {
                printf("BTN_TOUCH = %d\n", ev.value);
                logmsg("BTN_TOUCH = %d", ev.value);
            }
        }
        fflush(stdout);
    }

    ioctl(fd, EVIOCGRAB, (void*)0);
    logmsg("Released %s", DEV);
    close(fd);
    printf("Released and exiting\n");
    return 0;
}

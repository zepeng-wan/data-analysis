// touch_latency.c
// build: aarch64-linux-gcc -O2 -o touch_latency touch_latency.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/time.h>
#include <stdint.h>
#include <inttypes.h>

static volatile sig_atomic_t done = 0;
static const char *device = "/dev/input/event1";

void handle_sigint(int s){ (void)s; done = 1; }

static inline int64_t ev_time_us(const struct input_event *ev) {
    return (int64_t)ev->time.tv_sec * 1000000LL + (int64_t)ev->time.tv_usec;
}
static inline int64_t now_us(void) {
    struct timeval tv; gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000LL + (int64_t)tv.tv_usec;
}

int cmp_ll(const void *a, const void *b) {
    int64_t x = *(const int64_t*)a;
    int64_t y = *(const int64_t*)b;
    return (x > y) - (x < y);
}

int main(int argc, char **argv){
    int target = 1000;
    if (argc > 1 && strcmp(argv[1],"-n")==0 && argc>2) target = atoi(argv[2]);
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    int fd = open(device, O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    if (ioctl(fd, EVIOCGRAB, (void*)1) == -1) {
        fprintf(stderr, "EVIOCGRAB failed: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    printf("Grabbed %s, collecting up to %d samples. Ctrl+C to stop and show stats.\n", device, target);

    int64_t *samples = calloc(target+100, sizeof(int64_t));
    if (!samples) { perror("calloc"); ioctl(fd, EVIOCGRAB, (void*)0); close(fd); return 1; }
    int idx = 0;

    struct input_event ev;
    while (!done && idx < target) {
        ssize_t r = read(fd, &ev, sizeof(ev));
        if (r < (ssize_t)sizeof(ev)) {
            if (r == -1 && errno == EINTR) continue;
            fprintf(stderr, "read error: %s\n", strerror(errno));
            break;
        }
        // only count touch-related events (ABS/MT/BTN_TOUCH)
        if (ev.type == EV_ABS || ev.type == EV_KEY) {
            int64_t ev_us = ev_time_us(&ev);
            int64_t now = now_us();
            int64_t diff = now - ev_us; // in microseconds
            samples[idx++] = diff;
            if ((idx % 100) == 0) printf("sample %d: %" PRId64 " us\n", idx, diff);
        }
    }

    ioctl(fd, EVIOCGRAB, (void*)0);
    close(fd);

    if (idx == 0) {
        fprintf(stderr, "No samples collected.\n");
        free(samples);
        return 1;
    }

    qsort(samples, idx, sizeof(int64_t), cmp_ll);
    int64_t min = samples[0];
    int64_t max = samples[idx-1];
    double sum = 0;
    for (int i=0;i<idx;i++) sum += (double)samples[i];
    double avg = sum / idx;
    int mid = idx/2;
    double median = (idx%2) ? samples[mid] : ((samples[mid-1]+samples[mid]) / 2.0);
    int p95 = (int)(idx * 0.95);
    int p99 = (int)(idx * 0.99);
    if (p95 >= idx) p95 = idx-1;
    if (p99 >= idx) p99 = idx-1;

    printf("\nCollected %d samples\n", idx);
    printf("min     = %" PRId64 " us\n", min);
    printf("median  = %.2f us\n", median);
    printf("avg     = %.2f us\n", avg);
    printf("95pct   = %" PRId64 " us\n", samples[p95]);
    printf("99pct   = %" PRId64 " us\n", samples[p99]);
    printf("max     = %" PRId64 " us\n", max);

    free(samples);
    return 0;
}


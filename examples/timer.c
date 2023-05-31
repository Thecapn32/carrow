#include "carrow.h"

#include <clog.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/timerfd.h>


typedef struct timer {
    unsigned int interval;
    unsigned long value;
    const char *title;
} timer;


#undef CARROW_ENTITY
#define CARROW_ENTITY timer
#include "carrow_generic.h"
#include "carrow_generic.c"


#define WORKING 99999999
volatile int status = WORKING;
static struct sigaction old_action;


void sighandler(int s) {
    PRINTE(CR);
    status = EXIT_SUCCESS;
}


void catch_signal() {
    struct sigaction new_action = {sighandler, 0, 0, 0, 0};
    if (sigaction(SIGINT, &new_action, &old_action) != 0) {
        FATAL("sigaction");
    }
}


int
maketimer(unsigned int interval) {
    int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    if (fd == -1) {
        return -1;
    }
    
    struct timespec sec1 = {interval, 0};
    struct itimerspec spec = {sec1, sec1};
    if (timerfd_settime(fd, 0, &spec, NULL) == -1) {
        return -1;
    }
    return fd;
}


void
timerA(struct timer_coro *self, struct timer *state) {
    CORO_START;
    unsigned long tmp;
    ssize_t bytes;

    self->fd = maketimer(state->interval);
    if (self->fd == -1) {
        CORO_REJECT("maketimer");
    }
    
    while (true) {
        CORO_WAIT(self->fd, CIN);
        bytes = read(self->fd, &tmp, sizeof(tmp));
        state->value += tmp;
        INFO("%s, fd: %d, value: %lu", state->title, self->fd, state->value);
    }

    CORO_FINALLY;
    if (self->fd != -1) {
        timer_evloop_unregister(self->fd);
        close(self->fd);
    }
    CORO_END;
}


int
main() {
    clog_verbosity = CLOG_DEBUG;
    catch_signal();
    struct timer state1 = {
        .title = "Foo", 
        .interval = 1,
        .value = 0,
    };
    struct timer state2 = {
        .title = "Bar", 
        .interval = 3,
        .value = 0,
    };

    carrow_init(0);
    timer_coro_create_and_run(timerA, &state1);
    timer_coro_create_and_run(timerA, &state2);
    carrow_evloop(&status);
    carrow_deinit();
}

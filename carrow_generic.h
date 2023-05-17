#ifndef CARROW_H
#error "carrow.h must be imported before importing the carrow_generic.h"
#error "And also #undef and #define CARROW_ENTITY before importing the " \
    "carrow_generic.h"
#else

#include <stdbool.h>


typedef struct CARROW_NAME(coro) CARROW_NAME(coro);


typedef CARROW_NAME(coro) (*CARROW_NAME(coro_resolver)) 
    (CARROW_NAME(coro) *self, CARROW_ENTITY *state);


typedef CARROW_NAME(coro) (*CARROW_NAME(coro_rejector)) 
    (CARROW_NAME(coro) *self, CARROW_ENTITY *state, int no);


void
CARROW_NAME(resolve) (CARROW_NAME(coro) *self, CARROW_ENTITY *s);


CARROW_NAME(coro)
CARROW_NAME(coro_create) (CARROW_NAME(coro_resolver) f, 
        CARROW_NAME(coro_rejector) r);


CARROW_NAME(coro)
CARROW_NAME(coro_create_from) (CARROW_NAME(coro) *base, 
        CARROW_NAME(coro_resolver) f);


CARROW_NAME(coro)
CARROW_NAME(coro_stop) ();


CARROW_NAME(coro)
CARROW_NAME(coro_reject) (CARROW_NAME(coro) *self, CARROW_ENTITY *s, int no, 
        const char *filename, int lineno, const char *function, 
        const char *format, ... );


void
CARROW_NAME(coro_run) (CARROW_NAME(coro) *self, CARROW_ENTITY *s);


void
CARROW_NAME(coro_create_and_run) (CARROW_NAME(coro_resolver) f, 
        CARROW_NAME(coro_rejector) r, CARROW_ENTITY *state);


int
CARROW_NAME(evloop_register) (CARROW_NAME(coro) *c, CARROW_ENTITY *s, 
        struct carrow_event *e, int fd, int op);


int
CARROW_NAME(evloop_modify) (CARROW_NAME(coro) *c, CARROW_ENTITY *s, 
        struct carrow_event *e, int fd, int op);


int
CARROW_NAME(evloop_modify_or_register) (CARROW_NAME(coro) *c, 
        CARROW_ENTITY *s, struct carrow_event *e, int fd, int op);


int
CARROW_NAME(evloop_unregister) (int fd);


#endif

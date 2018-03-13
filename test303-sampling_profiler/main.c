// Sampling-based profiler
//
// Use setitimer(2) and SIGPROF to periodically collect RIP values during a
// heavy computation.

#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


static const struct timeval sampling_period = {
    .tv_sec = 0,
    .tv_usec = 100000
};

static const int task_size = 10000;


static int start_profiler(void);
static void run_task(int size);
static void handle_profiler_signal(int sig, siginfo_t *info, void *uap);
static uintptr_t get_program_counter(const mcontext_t *ctx);


int main(void)
{
    if (start_profiler() == -1) {
        return EXIT_FAILURE;
    }
    run_task(task_size);

    return EXIT_SUCCESS;
}

int start_profiler(void)
{
    struct sigaction action = {
        .sa_sigaction = handle_profiler_signal,
        .sa_flags = SA_RESTART | SA_SIGINFO
    };
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGPROF, &action, NULL) == -1) {
        perror("failed to set a signal handler");
        return -1;
    }

    struct itimerval timer = {
        .it_interval = sampling_period,
        .it_value = sampling_period
    };

    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("failed to set a profiling timer");
        return -1;
    }

    return 0;
}

void run_task(int size)
{
    double sum = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            sum += sin(i) * cos(j);
        }
    }

    printf("%g\n", sum);
}

void handle_profiler_signal(int sig, siginfo_t* info, void *uap)
{
    (void) sig;
    (void) info;

    ucontext_t *ucontext = uap;
    printf("pc: %lx\n", get_program_counter(&ucontext->uc_mcontext));

    // NOTE: printf(3) is not guaranteed to be async-signal-safe [1], so the
    // above code is bad. We could use safe syscalls such as write(2) and
    // send(2). Or we could implement a user space queue using sig_atomic_t
    // index and process queued values in another thread.
    //
    // [1]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_04_03
}

uintptr_t get_program_counter(const mcontext_t *ctx)
{
#if defined(__FreeBSD__)
    return ctx->mc_rip;
#elif defined(__APPLE__)
    return (*ctx)->__ss.__rip;
#elif
    return 0; // Dunno.
#endif
}

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

void ping_pong(int runcount);
double iavg(int[], int);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <runcount>\n", argv[0]);
    exit(1);
  }

  int runcount = atoi(argv[1]);
  if (runcount <= 0) {
    fprintf(stderr, "runcount must be > 0\n");
    exit(1);
  }

  // Children inherit CPU affinity of parent
  cpu_set_t cpus;
  CPU_ZERO(&cpus);
  CPU_SET(0, &cpus);
  sched_setaffinity(0, sizeof(cpu_set_t), &cpus);

  ping_pong(runcount);
  return 0;
}

void ping_pong(int runcount) {
  int runs[runcount];
  int pipefd1[2];
  int pipefd2[2];
  struct timeval tv;
  char buf[1];
  int run;

  if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0) {
    fprintf(stderr, "Unable to create pipes\n");
    exit(1);
  }

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Unable to fork child 1\n");
    exit(1);
  } else if (pid == 0) {
    // First child
    // Writes on pipe 1, reads on pipe 2
    close(pipefd1[0]);
    close(pipefd2[1]);

    for (run = 0; run < runcount + 1; run++) {
      buf[0] = 'X';
      write(pipefd1[1], buf, 1);
      read(pipefd2[0], buf, 1);
    }

    exit(0);
  }

  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Unable to fork child 2\n");
    exit(1);
  } else if (pid == 0) {
    // Second child
    // Reads on pipe 1, writes on pipe 2
    close(pipefd1[1]);
    close(pipefd2[0]);

    for (run = 0; run < runcount + 1; run++) {
      read(pipefd1[0], buf, 1);

      long prev_us = tv.tv_usec;
      gettimeofday(&tv, NULL);
      if (run > 0) {
        runs[run] = tv.tv_usec - prev_us;
      }

      buf[0] = 'Y';
      write(pipefd2[1], buf, 1);
    }

    // Print out average
    // Alternatively, we could return this value from the parent, but
    // that would involve additional "plumbing" in the form of pipes.
    // Also, note that `runs` records durations of 2 back-to-back
    // context switches. This is why we divide the average by 2.
    double avg = iavg(runs, runcount) / 2.0;
    printf("Average context switch duration: %f\n", avg);
    exit(0);
  }

  wait(NULL);
  wait(NULL);
}

double iavg(int ns[], int count) {
  double avg = 0.0;

  for (int i = 0; i < count; i++) {
    avg += (double) ns[i];
  }

  avg /= (double) count;

  return avg;
}

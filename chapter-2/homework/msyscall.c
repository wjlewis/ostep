#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define RUN_COUNT  1000000

void make_syscall(void);
double iavg(int[], int);

int main(void) {
  int runs[RUN_COUNT];

  struct timeval tv_before;
  struct timeval tv_after;

  for (int r = 0; r < RUN_COUNT; r++) {
    if (gettimeofday(&tv_before, NULL) < 0) {
      fprintf(stderr, "Failed to get time before syscall\n");
      exit(1);
    }

    make_syscall();

    if (gettimeofday(&tv_after, NULL) < 0) {
      fprintf(stderr, "Failed to get time after syscall\n");
      exit(1);
    }

    runs[r] = tv_after.tv_usec - tv_before.tv_usec;
  }

  // Clean up negatives (are these anomalies?)
  for (int i = 0; i < RUN_COUNT; i++) {
    if (runs[i] < 0) {
      // Hope and pray that runs[0] isn't negative...
      runs[i] = runs[i-1];
    }
  }

  double avg = iavg(runs, RUN_COUNT);
  printf("Avg syscall duration (us): %f\n", avg);

  return 0;
}

void make_syscall(void) {
  read(STDIN_FILENO, NULL, 0);
}

double iavg(int ns[], int count) {
  double avg = 0.0;

  for (int i = 0; i < count; i++) {
    avg += (double) ns[i];
  }

  avg /= (double) count;
  return avg;
}

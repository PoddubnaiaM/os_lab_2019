#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/time.h>
#include <unistd.h>
#include "utils.h"
#include <pthread.h>


int sum_array(int* array, int begin, int end) {
  int sum = 0;
  //int i = 0;
  // while (begin + i <= end)
  // {
    //printf("arr = %d begin = %d end = %d\n", array[i], begin, end);
    sum += array[begin];

  return sum;
}


struct SumArgs {
  int *array;
  int begin;
  int end;
};


void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)sum_array(sum_args->array,sum_args->begin,sum_args->end);
}

int main(int argc, char **argv) {


  int threads_num = -1;
  int array_size = -1;
  int seed = -1;
  int i;
while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"threads_num", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;
    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);

		if(seed<=0){
		   printf("seed can't be negative \n");
		   return -1;
		}
            // error handling
            break;
          case 1:
            array_size = atoi(optarg);

		if(array_size<=0){
		   printf("size can't be negative \n");
		   return -1;
		}
            // error handling
            break;
          case 2:
           threads_num = atoi(optarg);
		if(threads_num<=0){
		   printf("amount of threads can't be negative\n");
		   return -1;
		}
            break;

          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }


  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || threads_num == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  if (threads_num > array_size) {
  printf("The number of threads cant be greater than size of array \n");
   return 1;
}

  int *array = (int*)malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  // int ip = 0;
  // while (ip < array_size)
  //   printf("arr check = %d\n", array[ip++]);
  int sizeforthread= threads_num<=array_size ? (array_size/threads_num) : 1;
  pthread_t threads[threads_num];

  /*
   * TODO:
   * your code here
   * Generate array here
   */




  struct SumArgs args[threads_num];

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (i = 0; i < threads_num; i++) {

     args[i].begin=sizeforthread*i;
     args[i].end=args[i].begin+sizeforthread-1;
//printf("arr = %d cbegin = %d cend = %d\n", array[i], args[i].begin, args[i].end);
     if(i==threads_num-1)
	   args[i].end=array_size-1;
     args[i].array=array;
    //printf("check i = %d\n", i);
      if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
        printf("Error: pthread_create failed!\n");
      return 1;
    }


  }

  int total_sum = 0;
  for (i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}

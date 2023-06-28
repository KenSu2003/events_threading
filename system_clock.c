#include <stdio.h>
#include <unistd.h>
#include <time.h>
/* prints n fibonacci numbers*/
void fibonacci(int n){
    /* stores last two numbers in fibonacci series*/
    long n_2=0;
    long n_1=1;
    long current;
    
    printf("%ld, ", n_2);
    printf("%ld, ", n_1);
    for(int i=0; i<n-2; i++){
        current = n_1 + n_2;
        printf("%ld, ", current);
        n_2 = n_1;
        n_1 = current;
    } 
}
int main(){
    /* prints time taken to print n fibonacci numbers
     * measured using MONOTONIC CLOCK */
    int n = 40;
    struct timespec start, end;
    long time_ns;
    double time_sec;

    clock_gettime(CLOCK_MONOTONIC, &start);
    fibonacci(n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    time_ns = (end.tv_sec - start.tv_sec)*1000000000 + (end.tv_nsec - start.tv_nsec);
    time_sec = time_ns/1000000000.0;
    printf("\n Time taken to print %d numbers: %lf sec (%ld nsec)\n", n, time_sec, time_ns);
    return 0;
}
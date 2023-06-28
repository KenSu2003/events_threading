#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/epoll.h>
#include <signal.h>

//configures the gpio pin as input
void configure_gpio_input(int gpio_number){

    //converting gpio number from integer to string
    char gpio_num[10];
    sprintf(gpio_num, "export%d", gpio_number);
    const char* GPIOExport="/sys/class/gpio/export";

    // exporting the GPIO to user space
    FILE* fp = fopen(GPIOExport, "w");
    fwrite(gpio_num, sizeof(char), sizeof(gpio_num), fp);
    fclose(fp);

    // setting gpio direction as input
    char GPIODirection[40];
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", gpio_number);

    // setting GPIO as input
    fp = fopen(GPIODirection, "w");
    fwrite("in", sizeof(char), 2, fp);
    fclose(fp);

}

// sets up an interrupt on the given GPIO pin
void configure_interrupt(int gpio_number){
    configure_gpio_input(gpio_number);          /// set gpio as input

    // configuring interrupts on rising, falling or both edges
    char InterruptEdge[40];
    sprintf(InterruptEdge, "/sys/class/gpio/gpio%d/edge", gpio_number);

    // configures interrupt on falling edge
    FILE* fp = fopen(InterruptEdge, "w");
    // fwrite("falling", sizeof(char), 7, fp);
    // change from "falling —–> "rising" to detect rising edge
    fwrite("rising", sizeof(char), 7, fp);                 
 
    // configures interrupt on both edges
    // fwrite("both", sizeof(char), 4, fp);

    fclose(fp);
}


void event_handler_0(){
    // Handles Event 0
    // will be passed to event_callback
    // for event 0
    printf("Button 0 has been pressed.\n");
}

void event_handler_1(){
    // Handles Event 1
    // will be passed to event_callback
    // for event 1
    printf("Button 1 has been pressed.\n");
}

void event_callback(void handler()){
    /* This function gets invoked every time an
        Event is detected */
    handler(); 
}

int main(){
    // configure pin P8_8 as input with internal pull-up enabled [Button 0]
    int gpio_number_0 = 67;
    configure_gpio_input(gpio_number_0);
    //  file path to read button status
    char valuePath_0[40];
    sprintf(valuePath_0, "/sys/class/gpio/gpio%d/value", gpio_number_0);
    // configure pin P8_9 as input with internal pull-up enabled [Button 1]
    int gpio_number_1 = 69;
    configure_gpio_input(gpio_number_1);
    //  file path to read button status
    char valuePath_1[40];
    sprintf(valuePath_1, "/sys/class/gpio/gpio%d/value", gpio_number_1);
    // wait before first readings to avoid faulty readings
    sleep(1);    
    
    int state[2];
    FILE *fp_0, *fp_1;
    // loop to monitor events
    while(1){
        fp_0 = fopen(valuePath_0, "r");
        fp_1 = fopen(valuePath_1, "r");
        // default state is 1 since buttons are configured with
        // internal pull-up resistors. So, reading 0 means button
        // is pressed
        fscanf(fp_0, "%d", &(state[0]));
        fscanf(fp_1, "%d", &(state[1]));
        
        fclose(fp_0);
        fclose(fp_1);
        // event 0 detected
        if( state[0] == 0 ){
            event_callback(&event_handler_0);
        }
         // event 1 detected
         if( state[1] == 0 ){
            event_callback(&event_handler_1);
        } 
    }
    return 0; 
}
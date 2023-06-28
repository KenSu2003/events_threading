#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <poll.h>

// setup input pin in given mode
void config_pin(char* pin_number, char* mode){
    // creates environment to execute shell command
    if(!vfork()){
         // execute shell command for pin configuration
         int ret = execl("/usr/bin/config-pin", "config-pin", pin_number, mode, NULL);
         if (ret < 0){
        } 
    }
}

// set PWM duty cycle
void set_pwm_duty_cycle(char* pwmchip, char* channel, char* duty_cycle){
    // export file path
    char PWMDutyCycle[60];
    sprintf(PWMDutyCycle, "/sys/class/pwm/%s/pwm-7:%s/duty_cycle", pwmchip, channel);
    // configure PWM device
    FILE* fp = fopen(PWMDutyCycle, "w");
    fwrite(duty_cycle, sizeof(char), strlen(duty_cycle), fp);
    fclose(fp);
}

// set PWM period
void set_pwm_period(char* pwmchip, char* channel, char* period){
    long duty_cycle_int, period_int;
    // before setting up the period read old duty cycle
    char PWMDutyCycle[60], duty_cycle_str[20];
    sprintf(PWMDutyCycle, "/sys/class/pwm/%s/pwm-7:%s/duty_cycle", pwmchip, channel);
    FILE* fp = fopen(PWMDutyCycle, "r");
    fscanf(fp, "%ld", &duty_cycle_int);
    fclose(fp);

    period_int = atol(period);
    // If the old duty_cycle value is greater than the new period
    // update the dummy_duty_cycle first to avoid errors with setting up
    // the period
    if( duty_cycle_int >= period_int){
         duty_cycle_int = period_int/2;
         // converting long to char data type
         sprintf(duty_cycle_str, "%ld", duty_cycle_int);
         // setup dummy duty cycle
         set_pwm_duty_cycle(pwmchip, channel, duty_cycle_str);
    }
    // export file path
    char PWMPeriod[60];
    sprintf(PWMPeriod, "/sys/class/pwm/%s/pwm-7:%s/period", pwmchip, channel);
    fp = fopen(PWMPeriod, "w");
    fwrite(period, sizeof(char), strlen(period), fp);
    fclose(fp);
}

void stop_pwm(char* pin_number, char* pwmchip, char* channel){
    char PWMDisable[40];
    sprintf(PWMDisable, "/sys/class/pwm/%s/pwm-7:%s/enable", pwmchip, channel);
    // stop generating PWM
    FILE* fp = fopen(PWMDisable, "w");
    fwrite("0", sizeof(char), 1, fp);
    fclose(fp);
}

// starts a PWM
void start_pwm(char* pin_number, char* pwmchip, char* channel, char* period, char* duty_cycle){
    /* Input:
    pin_number: pin_number to generate PWM on
    pwmchip: the device folder to generate PWM
    channel: pwm device channel
    perod: pwm period
    duty_cycle: pwm duty cyle
    */
    // configure the pin in PWM mode 
    config_pin(pin_number, "pwm");
    
    // export PWM device
    FILE* fp;
    char PWMExport[40];
    sprintf(PWMExport, "/sys/class/pwm/%s/export", pwmchip);
    fp = fopen(PWMExport, "w");
    fwrite(channel, sizeof(char), sizeof(channel), fp);
    fclose(fp);

    // configure PWM Period
    set_pwm_period(pwmchip, channel, period);
    // configure PWM Duty Cycle
    set_pwm_duty_cycle(pwmchip, channel, duty_cycle);

    // enable PWM
    char PWMEnable[40];
    sprintf(PWMEnable, "/sys/class/pwm/%s/pwm-7:%s/enable", pwmchip, channel);
    // configure generating PWM
    fp = fopen(PWMEnable, "w");
    fwrite("1", sizeof(char), 1, fp);
    fclose(fp);
}

int main(){
    char pin_number[32] = "P8_13";
    char pwmchip[32] = "pwmchip7";
    char channel[32] = "1";
    char period[32] = "1000000";
    char duty_cycle[32] = "500000";

    stop_pwm(pin_number, pwmchip, channel);  // Make sure the pwm pin and channel are cleared first
    start_pwm(pin_number, pwmchip, channel, period, duty_cycle); // start pwm

    // wait for 60 seconds
    sleep(60);

    stop_pwm(pin_number, pwmchip, channel);
    return 0; 
}
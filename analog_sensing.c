#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
// Read the digital value from the ADC interface.
const char AIN_DEV[] = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";
// function to convert Celcius to Farenheit
double CtoF(double c){
  return (c * 9.0 / 5.0) + 32.0;
}
// Function to extract temperature from TMP36 data
double temperature(char *string){
  int value = atoi(string);
  // Convert the digital value to millivolts.
  double millivolts = (value / 4096.0) * 1800;
  // Convert the millivolts to Celsius temperature.
  double temperature = (millivolts - 500.0) / 10.0;
  return temperature;
}
int main(){
    int fd = open(AIN_DEV, O_RDONLY);
    while (1)
    {
        char buffer[1024];
        // Read the temperature sensor data
        int ret = read(fd, buffer, sizeof(buffer));
        if (ret != -1)
        {
            buffer[ret] = '\0';
            double celsius = temperature(buffer);
            double fahrenheit = CtoF(celsius);
            printf("digital value: %s  celsius: %f  fahrenheit: %f\n", buffer, celsius, fahrenheit);
            lseek(fd, 0, 0);
        }
        sleep(1); 
    }
    close(fd);
    return 0; 
}
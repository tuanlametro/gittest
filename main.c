#include "project.h"
#include <stdio.h>
#define coeff 5/4095; /*Vref divided by the number of steps between 0 and max*/
#define realv 5/3; /*Used to find real voltage, as the ADC reading is 3/5ths of the multimeter reading*/

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_1_Start();
    ADC_Battery_Start();
    ADC_Battery_StartConvert();
    printf("Battery Test...\n");
    float value = 0;
    float voltage = 0;
    
    for(;;)
    {
        ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT);
        value = ADC_Battery_GetResult16() * coeff;
        voltage = value * realv;       
        printf("%.1f\n", voltage);
    }
    
    return 0;
}

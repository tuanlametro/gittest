/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/
#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define coeff 5/4095 /*Vref divided by the number of steps between 0 and max*/
#define realv 5/3 /*Used to find real voltage, as the ADC reading is 3/5ths of the multimeter reading*/
#define SIZE 6
#define DIR 0 // 0 is left, 1 is right
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/
void motor_tank_turn(uint8 dir, uint8 l_speed, uint8 r_speed, uint32 delay);

#if 0
                vTaskDelay(500);
            }
        
            for (int i = 0; i < 3; i++){  
                BatteryLed_Write(1);
                vTaskDelay(1500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
        
            for (int i = 0; i < 3; i++){
                BatteryLed_Write(1);
                vTaskDelay(500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
        }
    }
}    
#endif

#if 0
>>>>>>> master
// Week 2 Assignment 2, by Lily
void zmain(void)
{
    char name[32];
    int age;
    
    TickType_t time_start, time_end, total_time;
    
    printf("\n\n");

    printf("Enter your age: ");
    time_start = xTaskGetTickCount();
    scanf("%d", &age);
    time_end = xTaskGetTickCount();
    total_time = time_end - time_start;
    if(total_time<3000) 
    {
        if(age <= 21)
        {
            printf("Super fast dude!");
        }
        else if(age >= 22 && age <= 50)
        {
            printf("Be quick or be dead");
        }
        else 
        {
            printf("Still going strong");
        }
    }
    else if(total_time >=3000 && total_time <=5000) 
    {
        if(age <= 21)
        {
            printf("So mediocre.");
        }
        else if(age >= 22 && age <= 50)
        {
            printf("You’re so average.");
        }
        else 
        {
            printf("You are doing ok for your age.");
        }
    }
    else 
    {
        if(age <= 21)
        {
            printf("My granny is faster than you!");
        }
        else if(age >= 22 && age <= 50)
        {
            printf("Have you been smoking something illegal? ");
        }
        else 
        {
            printf("Do they still allow you to drive?");
        }
    }

    /*while(true)
    {
        BatteryLed_Write(!SW1_Read());
        vTaskDelay(100);
    }*/
 } 
#endif

#if 1
// Week 2 Assignment 3, by Joshua
void batcheck();
void batterytest();
void ledloop();
int16 adcresult = 0;
float volts = 0.0;
bool warning = false;

void zmain(void)
{   

    ADC_Battery_Start();
    ADC_Battery_StartConvert();
    while(1)
    {
        if (warning == false)
        {
            batcheck();
        }
        else if (warning == true)
        {
            ledloop();
        }
    }
}

void batcheck()
{
    while(warning == false)
    {
        ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT); // wait for ADC converted value
        adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
        float converted = adcresult*coeff;
        volts = converted * realv;
        printf("%d %f\r\n",adcresult, volts);
        if (volts <= 4.0)
        {
            warning = true;
        }
        vTaskDelay(500);
    }
}

void ledloop()
{
    uint8 i = 0;
    
    while(warning == true)
    {
        Beep(500, i);
        i++;
        BatteryLed_Write(1);
        vTaskDelay(500);
        Beep(500, i);
        i++;
        BatteryLed_Write(0);
        vTaskDelay(500);
        if(SW1_Read() == 0)
        {
            warning = false;
        }
    }
}

#endif

#if 0
// button
void zmain(void)
{
    uint8 button = SW1_Read();
    void S()
    {
        for(int i = 0; i <= 2; i++)
        {
            printf("S");
            BatteryLed_Write(1);
            vTaskDelay(100);
            BatteryLed_Write(0);
            vTaskDelay(100);  
        }
    }
    
    void O()
    {
        for(int i = 0; i <= 2; i++)
        {
            printf("O");
            BatteryLed_Write(1);
            vTaskDelay(300);
            BatteryLed_Write(0);
            vTaskDelay(100);  
        }
    }
    while(button == 0) 
    {
        /* printf("Press button within 5 seconds!\n");
        int i = 50;
        while(i > 0) {
            if(SW1_Read() == 0) {
                break;
            }
            vTaskDelay(100);
            --i;
        }
        if(i > 0) {
            printf("Good work\n");
            while(SW1_Read() == 0) vTaskDelay(10); // wait until button is released
        }
        else {
            printf("You didn't press the button\n");
        }*/
        S();
        O();
        S();
    }
}
#endif

#if 0
// button
void zmain(void)
{
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    for(;;)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) vTaskDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif

#if 0

void zmain(void)
{
    uint8_t button = 0;
    while(1)
    {
        Ultra_Start(); 
        motor_start();
        motor_forward(0,0);
        if(SW1_Read() == 0)
        {
            button = !button;
        }
        
                                
        while(button == 1) 
        {
            int d = Ultra_GetDistance(); // d is distance in cm
            printf("distance = %d\r\n", d);
            if( d <= 10 )
            {
                Beep(100, 100);
                motor_backward(0,0);
                motor_turn(150, 75, 500);
                //motor_tank_turn('l', 100, 100, 500);
            }
            else 
            {
                motor_forward(150, 0);
            }
        }
    }
}  
    

#endif

#if 0
//IR receiverm - how to wait for IR remote commands
void zmain(void)
{
    IR_Start();
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    bool led = false;
    // Toggle led when IR signal is received
    for(;;)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        if(led) printf("Led is ON\n");
        else printf("Led is OFF\n");
    }    
 }   
#endif

#if 0
//IR receiver - read raw data
void zmain(void)
{
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    // print received IR pulses and their lengths
    for(;;)
    {
        if(IR_get(&IR_val, portMAX_DELAY)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
        }
    }    
 }   
#endif

#if 0
//reflectance
void zmain(void)
{
    struct sensors_ ref;
    struct sensors_ dig;

    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    

    for(;;)
    {
        // read raw sensor values
        reflectance_read(&ref);
        // print out each period of reflectance sensors
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        //print out 0 or 1 according to results of reflectance period
        printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);        
        
        vTaskDelay(200);
    }
}   
#endif

#if 0
//motor
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors

    vTaskDelay(3000);
    
    motor_forward(100,2000);     // moving forward
    motor_turn(200,50,2000);     // turn
    motor_turn(50,200,2000);     // turn
    motor_backward(100,2000);    // moving backward
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
    
    for(;;)
    {

    }
}
#endif

#if 1
/* Example of how to use the Accelerometer!!!*/
struct accData_ data;
int thresh; // Acts as both the threshold to trigger a reverse, and also as the seed to 
void acceltest(void);
int randn(void);
int seed(void);

void zmain(void)
<<<<<<< HEAD
{
    struct accData_ data;
    uint16_t last = 0, current = 0, thresh = 0; 

    motor_start();              // enable motor controller
    
    
    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok!\n");
    }
    
    while(1)
    {  
        while(thresh == 0)
        { 
            for(int i = 0; i < 4; i++)
            {
                motor_forward(50, 0);
                LSM303D_Read_Acc(&data);
                if(data.accX > last)
                {
                    thresh = data.accX;
                }
                last = data.accX;
                printf("%8d", thresh);
                vTaskDelay(50);
            }   
        }
        /*motor_turn(180, 200, 50);
        LSM303D_Read_Acc(&data);
        if((data.accX - last) > 5000)
        {
            Beep(100, 50);
            motor_backward(0,0);
            motor_turn(150, 75, 500);
            motor_forward(0,0);
        }
        last = data.accX;
        vTaskDelay(50);*/
    }
   
 }   
=======
{ 
    acceltest();
    thresh = -5000;
    srand(seed());
    
    while(1)
    {   
        LSM303D_Read_Acc(&data);
           
        if(randn() == 5)
        {
            Beep(50,100);
            if(data.accX % 2 == DIR)
            {
                motor_tank_turn(0, 100, 100, 500);
            }
            else if(data.accX % 2 != DIR)
            {
                motor_tank_turn(1, 100, 100, 500);
            } 
        }
        
        else
        {
            if(data.accX > thresh)
            {
            motor_forward(100,0);
            }
            
            else if(data.accX < thresh)
            {
                motor_backward(100, 200);
                
                if(data.accX % 2 == DIR)
                {
                    motor_tank_turn(0, 100, 100, 500);
                }
                else if(data.accX % 2 != DIR)
                {
                    motor_tank_turn(1, 100, 100, 500);
                }
            }
        }
        vTaskDelay(50);
    }
}   
  
void acceltest(void)
{
        if(!LSM303D_Start())
    {
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else 
    {
        printf("Device Ok...\n");
    }
}

int randn(void)
{
    int i = 0, count = 0;
    for(i = 0; i < 5; i++)
    {
        if((rand() % 2) == 1)
        {
            count += 1;
        }
    }
    return count;
}
int seed(void)
{
    int x = 0;
    motor_start();
    motor_forward(100, 0);
    for(int i = 0; i < 10; i++)
    {
        LSM303D_Read_Acc(&data);
        if(data.accX > x)
        {
           x = (data.accX);
        }
        vTaskDelay(50);
    }
    return x;
}
    
    
    
>>>>>>> joshua-branch
#endif    

#if 0
// MQTT test
void zmain(void)
{
    int ctr = 0;

    printf("\nBoot\n");
    send_mqtt("Zumo01/debug", "Boot");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 

    for(;;)
    {
        printf("Ctr: %d, Button: %d\n", ctr, SW1_Read());
        print_mqtt("Zumo01/debug", "Ctr: %d, Button: %d", ctr, SW1_Read());

        vTaskDelay(1000);
        ctr++;
    }
 }   
#endif

#if 0
void zmain(void)
{    
    struct accData_ data;
    struct sensors_ ref;
    struct sensors_ dig;
    
    printf("MQTT and sensor test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Accelerometer Ok...\n");
    }
    
    int ctr = 0;
    reflectance_start();
    for(;;)
    {
        LSM303D_Read_Acc(&data);
        // send data when we detect a hit and at 10 second intervals
        if(data.accX > 1500 || ++ctr > 1000) {
            printf("Acc: %8d %8d %8d\n",data.accX, data.accY, data.accZ);
            print_mqtt("Zumo01/acc", "%d,%d,%d", data.accX, data.accY, data.accZ);
            reflectance_read(&ref);
            printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       
            print_mqtt("Zumo01/ref", "%d,%d,%d,%d,%d,%d", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);
            reflectance_digital(&dig);
            printf("Dig: %8d %8d %8d %8d %8d %8d\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            print_mqtt("Zumo01/dig", "%d,%d,%d,%d,%d,%d", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            ctr = 0;
        }
        vTaskDelay(10);
    }
 }   

#endif

#if 0
void zmain(void)
{    
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now;

    // set current time
    now.Hour = 12;
    now.Min = 34;
    now.Sec = 56;
    now.DayOfMonth = 25;
    now.Month = 9;
    now.Year = 2018;
    RTC_WriteTime(&now); // write the time to real time clock

    for(;;)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            printf("%2d:%02d.%02d\n", now.Hour, now.Min, now.Sec);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif

// Put functions here for now

void motor_tank_turn(uint8 dir, uint8 l_speed, uint8 r_speed, uint32 delay)
{
    if(dir == DIR) 
    {
        MotorDirLeft_Write(1);      // Sets left tread to reverse
        MotorDirRight_Write(0);
    }
    if(dir == !DIR) 
    {
        MotorDirLeft_Write(0);      
        MotorDirRight_Write(1);     // Sets right tread to reverse
    }
    PWM_WriteCompare1(l_speed); 
    PWM_WriteCompare2(r_speed); 
    vTaskDelay(delay);
    
    MotorDirLeft_Write(0);          // Returns both motors to forward after turn is complete    
    MotorDirRight_Write(0); 
}

<<<<<<< HEAD
#if 0
// Assignment 1
    
void zmain(void){
    uint8 button = 1;
    while(1){ 
        button = SW1_Read();
        if (button == 0){
            for (int i = 0; i < 3; i++){
                BatteryLed_Write(1);
                vTaskDelay(500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
        
            for (int i = 0; i < 3; i++){  
                BatteryLed_Write(1);
                vTaskDelay(1500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
        
            for (int i = 0; i < 3; i++){
                BatteryLed_Write(1);
                vTaskDelay(500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
        }
    }
}    
#endif
=======

>>>>>>> master

/* [] END OF FILE */

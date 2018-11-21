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
bool power(void);
bool button = false;

// Week 2 Assignment 1, by Tuan
#if 0
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

// Week 2 Assignment 2, by Lily
#if 0
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
 } 
#endif

// Week 2 Assignment 3, by Joshua
#if 0
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

//Week 3 Assignment 1, by Lily
#if 0
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors

    vTaskDelay(5000);
    motor_turn(177,200,2000);     // moving forward
    motor_tank_turn(1,150,150,330); 
    motor_turn(177,200,1700);     // moving forward
    motor_tank_turn(1,150,150,330);     // turn right
    motor_turn(177,200,1800);     // moving forward
    motor_tank_turn(1,150,150,355);     // turn right   
    motor_turn(140,80,2700);     // turn right
    motor_turn(177,200,500);     // turn right
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
}
#endif

// Week 3 Assignment 2
#if 0
void zmain(void)
{
    while(1)
    {
        Ultra_Start(); 
        motor_start();
        power();
        motor_forward(0,0);
        
        while(button == true) 
        {
            int d = Ultra_GetDistance(); // d is distance in cm
            printf("distance = %d\r\n", d);
            if( d <= 10 )
            {
                Beep(100, 100);
                motor_backward(0,0);
                motor_turn(150, 75, 500);
                //motor_tank_turn(0, 100, 100, 500); Commented out because it isn't strictly what the assignment wanted.
            }
            else 
            {
                motor_forward(150, 0);
            }
        }
    }
}  
#endif

// Week 3 Assignment 3
#if 0
struct accData_ data;
int thresh = -5000;
void acceltest(void);
int randn(void);
int seed(void);

void zmain(void)
{

    acceltest();
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
#endif 

// Week 4 Assignment 2
#if 0
struct sensors_ dig;
int count = 0;
int black();
void fwhite();
float light_ratio = 0;
TickType_t tid = 0, tid2 = 0;
bool white = false, mid_white = false;


void zmain(void) {
    setup_motor();
    power();

    while (count < 5) {
        reflectance_read(&ref);
        reflectance_digital(&dig);
        motor_forward(SLOWSPEED, 0);
        if(dig.l3 == 1 && dig.r3 == 1) {
            count++;
            black();
            fwhite();
        }
        else 
        {
            light_ratio = (float)ref.l1 / ref.r1; 
            if(light_ratio > 1.0)
                motor_turn(SLOWSPEED/light_ratio, SLOWSPEED, 0);
            else if(light_ratio < 1.0 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
                motor_turn(SLOWSPEED, SLOWSPEED * light_ratio, 0); 
        }
    }
}

  
int black()
{
    if(count == 1) 
    {
        motor_forward(0,0);
        Beep(100, 50);
        IR_wait();
        tid = xTaskGetTickCount();
    
        while(dig.l3 != 0 || dig.r3 != 0) 
        {
            reflectance_digital(&dig);
            motor_forward(SLOWSPEED, 0);
        }
        tid2 = xTaskGetTickCount() - tid;
    }
    
    else
    {
        while(dig.l3 != 0 || dig.r3 != 0) 
        {
            reflectance_digital(&dig);
            motor_forward(SLOWSPEED, 0);
        }
    }
    
    return tid2;
}

void fwhite()
{
    if(count < 5) 
    {
        motor_forward(SLOWSPEED, tid2); 
        if(count == 2) motor_tank_turn(0, SLOWSPEED, SLOWSPEED, 500);
        else if(count > 2) motor_tank_turn(1, SLOWSPEED, SLOWSPEED, 500);
        
        while(1) {
            reflectance_digital(&dig);
            if(count == 2) motor_tank_turn(0, SLOWSPEED, SLOWSPEED, 0);
            else if(count > 2) motor_tank_turn(1, SLOWSPEED, SLOWSPEED, 0);
        
            if(dig.l1 == 1 && dig.r1 == 1) break;
        }
    }
    
    else motor_forward(0,0);
}
#endif

// Week 4 Assignment  3
#if 0
struct sensors_ dig;
struct sensors_ ref;

int count = 0;
int last = 0, most = 18000;
float light_ratio = 0;
bool white = false;
TickType_t time_start;

void zmain(void) 
{
    setup_motor();
    while(button == false) if(SW1_Read() == 0) button = true;
            
        while(count < 2)
        {
            while(count == 0)
            {
                reflectance_read(&ref);
                reflectance_digital(&dig);
                if(dig.l3 == 1 && dig.r3 == 1 && white == true)
                {
                    motor_forward(0,0);
                    IR_wait(); 
                    time_start = xTaskGetTickCount();
                    count++;
                    white = false;
                }
                else if(dig.l3 == 0 || dig.r3 == 0) white = true;
                
                light_ratio = (float)ref.l1 / ref.r1; 
                // Left Turns    
                if(light_ratio > 1.0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0)
                    motor_turn(SLOWSPEED/light_ratio, SLOWSPEED, 0);
                else if(light_ratio > 1.0 && dig.l1 == 1 && (dig.l2 == 1 || dig.l3 == 1))
                    motor_turn(SLOWSPEED*0.7/light_ratio, SLOWSPEED, 0);
                else if(light_ratio > 1.0 && dig.l1 == 0 && (dig.l2 == 1 || dig.l3 == 1))
                    motor_turn(0, SLOWSPEED, 0);
                // Right Turns
                else if(light_ratio < 1.0 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
                    motor_turn(SLOWSPEED, SLOWSPEED * light_ratio, 0);
                else if(light_ratio < 1.0 && dig.r1 == 1 && (dig.r2 == 1 || dig.r3 == 1))      
                    motor_turn(SLOWSPEED, SLOWSPEED*0.7*light_ratio, 0);
                else if(light_ratio < 1.0 && dig.r1 == 0 && dig.r2 == 1 && dig.r3 == 1)  
                    motor_turn(SLOWSPEED, 0, 0); 
                // Going Straight
                else if(light_ratio == 1.0)
                    motor_turn(SLOWSPEED, SLOWSPEED, 0);
            }
            
            reflectance_read(&ref);
            reflectance_digital(&dig); 

            if(dig.l3 == 1 && dig.r3 == 1 && white == true){
                count++;
                white = false;
            }
            
            else if(dig.l3 == 0 || dig.r3 == 0) white = true;
               
            if(ref.l1 >= most) ref.l1 = most;
            else if(ref.r1 >= most) ref.r1 = most;
            light_ratio = (float)ref.l1 / ref.r1; 
            
            // Left Turns    
            if(light_ratio > 1.0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0)
                motor_turn(MAXSPEED/light_ratio, MAXSPEED, 0);
            else if(light_ratio > 1.0 && dig.l1 == 1 && (dig.l2 == 1 || dig.l3 == 1))
                motor_turn(MAXSPEED*0.7/light_ratio, MAXSPEED, 0);
            else if(light_ratio > 1.0 && dig.l1 == 0 && (dig.l2 == 1 || dig.l3 == 1))
                motor_turn(0, MAXSPEED, 0);
                
            // Right Turns
            else if(light_ratio < 1.0 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
                motor_turn(MAXSPEED, MAXSPEED * light_ratio, 0);
            else if(light_ratio < 1.0 && dig.r1 == 1 && (dig.r2 == 1 || dig.r3 == 1))      
                motor_turn(MAXSPEED, MAXSPEED*0.7*light_ratio, 0);
            else if(light_ratio < 1.0 && dig.r1 == 0 && dig.r2 == 1 && dig.r3 == 1)  
                motor_turn(MAXSPEED, 0, 0); 
                
            // Going Straight
            else if(light_ratio == 1.0)
                motor_turn(MAXSPEED, MAXSPEED, 0);
        }
    print_mqtt("Zumo018", "Time: %d", xTaskGetTickCount() - time_start);
    motor_forward(0,0);
}
#endif
  
#if 1
//reflectance
void zmain(void)
{
    int count = 0, last = 0, least = 6000;
    float light_ratio = 0;
    struct sensors_ ref;
    struct sensors_ dig; // We currently are not using this
    motor_start();
    motor_forward(0,0);
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // We currently are not using this
    
    while(count != 2)
    {
        reflectance_read(&ref);
        if(ref.l3 < least || ref.r3 < least) // Constantly checks for lowest white value to avoid negative numbers.
        {
            least = ref.l3;
            if(ref.r3 < least)
            {
                least = ref.r3 - 10; // We minus 10 or so to try and avoid the middle sensors ever reading a lower white value.
            }
        }

        light_ratio = ((float)ref.l1 - least) / (ref.r1 - least); //If least is ever bigger than or equal to r1, we have a problem.
        motor_turn(150, 150, 0);
        /*if(ref.l3 > 15000 && ref.r3 > 15000)
        {
            count++;
        }*/
        //else
        //{
            if(light_ratio > 1)
            { 
                motor_turn(150/light_ratio, 150, 0); 
            }
            else if(light_ratio < 1)
            {        
                motor_turn(150, 150/light_ratio, 0); 
            }
        //}
        // last = Will be useful for tracking black starting and stoppung lines.
    }
}   
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

// Our own functions

void motor_tank_turn(uint8 dir, uint8 l_speed, uint8 r_speed, uint32 delay)
{
    MotorDirLeft_Write(!dir); // dir 0 turns left, dir 1 turns right.
    MotorDirRight_Write(dir);
    PWM_WriteCompare1(l_speed); 
    PWM_WriteCompare2(r_speed); 
    vTaskDelay(delay);
    
    MotorDirLeft_Write(0);          // Returns both motors to forward after turn is complete    
    MotorDirRight_Write(0); 
}

bool power(void)
{
    if(SW1_Read() == 0)
        {
            button = !button;
        }
    return button;
}
/* [] END OF FILE */

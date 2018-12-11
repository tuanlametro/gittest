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
#define MAXSPEED 255
#define SPEED 255
#include <math.h>
#define PI 3.141592654

/**
 * @file    main.c
 * @brief
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/
void motor_tank_turn(uint8 dir, uint8 l_MAXSPEED, uint8 r_MAXSPEED, uint32 delay);
void power(void);
bool button = false;
void setup_motor();
struct sensors_ dig;
struct sensors_ ref;
int count = 0;
float light_ratio = 0;
TickType_t time_start;
void drive_to_black();
struct accData_ data;
void acceltest(void);
int rand(void);
int seed(void);
int first_dataX;
int second_dataX;
int first_dataY;
int second_dataY;
int diff_dataX;
int diff_dataY;


// Week 2 Assignment 1
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

// Week 2 Assignment 2
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

// Week 2 Assignment 3
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
            batcheck();

        else if (warning == true)
            ledloop();
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
            warning = true;
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
            warning = false;
    }
}
#endif

// Week 3 Assignment 1
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
    power();
    acceltest();
    srand(seed());

    while(1)
    {
        LSM303D_Read_Acc(&data);

        if(randn() == 5)
        {
            Beep(50,100);
            if(data.accX % 2 == 0)
            {
                motor_tank_turn(0, 100, 100, 500);
            }
            else if(data.accX % 2 != 0)
            {
                motor_tank_turn(1, 100, 100, 500);
            }
            motor_forward(0,0);
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

                if(data.accX % 2 == 0)
                {
                    motor_tank_turn(0, 100, 100, 500);
                }
                else if(data.accX % 2 != 0)
                {
                    motor_tank_turn(1, 100, 100, 500);
                }
                motor_forward(0,0);
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

// Week 4 Assignment 1
#if 0

void drive_to_color(int i)
{
    reflectance_digital(&dig);
    while(dig.l3 != i || dig.r3 != i)
    {
        reflectance_digital(&dig);
        motor_forward(40,0);
    }
    motor_forward(0,0);
}

void zmain(void)
{
    setup_motor();
    power();
    drive_to_color(1);
    motor_forward(0,0);
    IR_wait();

    for(int i=0;i<3;i++)
    {
        drive_to_color(0);
        drive_to_color(1);
    }
    motor_forward(0,0);
}
#endif

// Week 4 Assignment 2
#if 0
int black();
void fwhite();
TickType_t tid = 0, tid2 = 0;
bool white = false, was_white;

void zmain(void) {
    setup_motor();
    power();

    while (count < 5) {
        was_white = false;
        reflectance_read(&ref);
        reflectance_digital(&dig);
        motor_forward(SPEED, 0);
        if(dig.l3 == 1 && dig.r3 == 1)
        {
            count++;
            black();
            fwhite();
        }
        else
        {
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
            motor_forward(SPEED, 0);
        }
        tid2 = xTaskGetTickCount() - tid;
    }

    else
    {
        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
    }

    return tid2;
}

void fwhite()
{
    if(count < 5)
    {
        motor_forward(SPEED, tid2); // Drives forward for the same time it takes to travel the width of a black line
        if(count == 2) motor_tank_turn(0, SPEED, SPEED, 500); // Turn left on the first intersection
        else if(count > 2) motor_tank_turn(1, SPEED, SPEED, 500); // Turn right for all others
        while(1)
        {
            reflectance_digital(&dig);
            if(count == 2) motor_tank_turn(0, SPEED, SPEED, 0);
            else if(count > 2) motor_tank_turn(1, SPEED, SPEED, 0);

            if(dig.l1 == 1 && dig.r1 == 1 && dig.l2 == 0 && dig.r2 == 0) break;
        }
        motor_forward(0,0);
    }
    else motor_forward(0,0);
}
#endif

// Week 4 Assignment 3
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
                    motor_turn(speed/light_ratio, speed, 0);
                else if(light_ratio > 1.0 && dig.l1 == 1 && (dig.l2 == 1 || dig.l3 == 1))
                    motor_turn(speed*0.7/light_ratio, speed, 0);
                else if(light_ratio > 1.0 && dig.l1 == 0 && (dig.l2 == 1 || dig.l3 == 1))
                    motor_turn(0, speed, 0);
                // Right Turns
                else if(light_ratio < 1.0 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
                    motor_turn(speed, speed * light_ratio, 0);
                else if(light_ratio < 1.0 && dig.r1 == 1 && (dig.r2 == 1 || dig.r3 == 1))
                    motor_turn(speed, speed*0.7*light_ratio, 0);
                else if(light_ratio < 1.0 && dig.r1 == 0 && dig.r2 == 1 && dig.r3 == 1)
                    motor_turn(speed, 0, 0);
                // Going Straight
                else if(light_ratio == 1.0)
                    motor_turn(speed, speed, 0);
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
    motor_forward(0,0);
}
#endif

// Maze stuff
#if 0
int black();
void fwhite();
void intersect(int i);
void drive_to_line();
TickType_t tid = 0, tid2 = 0;
bool white = false, flag = false;
int dir = 0, column = 0, row = 0;
//Dir 0 is N, Dir 1 is E, Dir -1 is W, everything else doesn't work

void zmain(void) {
    setup_motor();
    power();
    drive_to_line();

    while(row < 15) {
        reflectance_read(&ref);
        reflectance_digital(&dig);
        light_ratio = (float)ref.l1 / ref.r1;
        int d = Ultra_GetDistance(); // d is distance in cm
        if(d <= 25) flag = true;

        if(dig.l3 == 1 && dig.r3 == 1)
        {
            if(dir == 0)
                row++;
            else if(dir == -1)
                column--;
            else if(dir == 1)
                column++;
            black();
            fwhite();
        }
        else
        {
            // Left Turns
            if(light_ratio > 1.0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0)
                motor_turn(SPEED/light_ratio, SPEED, 0);
            else if(light_ratio > 1.0 && dig.l1 == 1 && (dig.l2 == 1 || dig.l3 == 1))
                motor_turn(SPEED*0.7/light_ratio, SPEED, 0);
            else if(light_ratio > 1.0 && dig.l1 == 0 && (dig.l2 == 1 || dig.l3 == 1))
                motor_turn(0, SPEED, 0);

            // Right Turns
            else if(light_ratio < 1.0 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
                motor_turn(SPEED, SPEED * light_ratio, 0);
            else if(light_ratio < 1.0 && dig.r1 == 1 && (dig.r2 == 1 || dig.r3 == 1))
                motor_turn(SPEED, SPEED*0.7*light_ratio, 0);
            else if(light_ratio < 1.0 && dig.r1 == 0 && dig.r2 == 1 && dig.r3 == 1)
                motor_turn(SPEED, 0, 0);

            // Going Straight
            else if(light_ratio == 1.0)
                motor_turn(SPEED, SPEED, 0);
        }
    }
    motor_forward(0, 0);
}

int black()
{
    if(row == 1 && dir == 0)
    {
        motor_forward(0,0);
        Beep(100, 50);
        IR_wait();
        tid = xTaskGetTickCount();

        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
        tid2 = xTaskGetTickCount() - tid;
        return tid2;
    }

    else
    {
        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
        count--;
    }
    return count;
}

void fwhite()
{
    motor_forward(SPEED, tid2);
    if(flag == true) // If the robot has detected an obstacle
    {
        count = 2; // Set the count to 2 so it knows to stop after travelling 2 rows/columns
        if(dir == 0)
        {
            if(column >= 0) //If we're in the middle or to the right of the middle, turn left
            {
                dir--;
                intersect(0);
                    /*
                    reflectance_digital(&dig);
                    motor_tank_turn(0, SPEED, SPEED, 0);
                    if(dig.l1 == 0 && dig.r1 == 0) white = true;
                    if(dig.l1 == 1 && dig.r1 == 1 && dig.l2 == 0 && dig.r2 == 0 && white == true) break;
                    */
            }

            if(column < 0) //Turn right
            {
                dir++;
                intersect(1);
            }
        }
        else if(dir == -1)
        {
            dir++;
            intersect(1);
        }

        else if(dir == 1)
        {
            dir--;
            intersect(0);
        }
        flag = false;

        motor_forward(0,0);
    }
    else if(flag == false) //Code to stop after 2 intersections
    {
        if(dir == -1 && count == 0)
        {
            dir++;
            intersect(1);
        }

        else if(dir == 1 && count == 0)
        {
            dir--;
            intersect(0);
        }
        motor_forward(0,0);
    }
}

void intersect(int i)
{
    while(1)
    {
        reflectance_digital(&dig);
        motor_tank_turn(i, SPEED, SPEED, 0);
        if(dig.l2 == 1 || dig.r2 == 1) white = true;
        if(dig.l1 == 1 && dig.r1 == 1 && white == true) break;
    }
    white = false;
}

void drive_to_line()
{
    while(1)
    {
        reflectance_digital(&dig);
        motor_forward(50,0);
        if(dig.l3 == 1 && dig.r3 == 1) break;
    }
}
#endif

// Week 5 Assignment 1
#if 0
void zmain(void)
{
    int hour = 0, min = 0;
    RTC_Start(); // start real time clock
    RTC_TIME_DATE now;

    // set current time
    now.Sec = 0;
    now.Min = 0;
    now.Hour = 0;
    now.DayOfMonth = 29;
    now.Month = 11;
    now.Year = 2018;
    printf("Enter the hour \n");
    scanf("%d", &hour);
    now.Hour = hour;
    printf("Enter the minutes \n");
    scanf("%d", &min);
    now.Min = min;

    RTC_WriteTime(&now);


    for(;;)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); // Disable Interrupt of RTC Component
            now = *RTC_ReadTime(); // copy the current time to a local variable
            RTC_EnableInt(); // Enable Interrupt of RTC Component

            // print the current time
            print_mqtt("Zumo018/time", "%2d:%02d.%02d\n", now.Hour, now.Min, now.Sec);

            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }
#endif

// Week 5 Assignment 2
#if 0
void zmain(void)
{
    TickType_t time = xTaskGetTickCount();
    setup_motor();
    power();

    while(1)
    {
        int d = Ultra_GetDistance(); // d is distance in cm
        printf("distance = %d\r\n", d);
        if( d <= 10 )
        {
            if((xTaskGetTickCount() - time) % 2 == 0)
            {
                motor_backward(150,300);
                motor_tank_turn(0, 150, 150, 400);
                print_mqtt("Zumo018/turn", "Turned left!");
            }
            else
            {
                motor_backward(150,300);
                motor_tank_turn(1, 150, 150, 400);
                print_mqtt("Zumo018/turn", "Turned right!");
            }
        }
        else motor_forward(150, 0);
    }
}
#endif

// Week 5 Assignment 3
#if 0

int black();
void fwhite();
TickType_t tid = 0, tid2 = 0;
bool white = false, flag = false;
int dir = 0, column = 0, row = 0;

void zmain(void)
{
    setup_motor();
    power();

    while (1) {
        reflectance_read(&ref);
        reflectance_digital(&dig);
        motor_forward(SPEED, 0);

        if(dig.l3 == 1 && dig.r3 == 1)
        {
            black();
        }
        else //line follow junk
        {
            light_ratio = (float)ref.l1 / ref.r1;
            // Left Turns
            if(light_ratio > 1.0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0)
                motor_turn(SPEED/light_ratio, SPEED, 0);
            else if(light_ratio > 1.0 && dig.l1 == 1 && (dig.l2 == 1 || dig.l3 == 1))
                motor_turn(SPEED*0.7/light_ratio, SPEED, 0);
            else if(light_ratio > 1.0 && dig.l1 == 0 && (dig.l2 == 1 || dig.l3 == 1))
                motor_turn(0, SPEED, 0);

            // Right Turns
            else if(light_ratio < 1.0 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
                motor_turn(SPEED, SPEED * light_ratio, 0);
            else if(light_ratio < 1.0 && dig.r1 == 1 && (dig.r2 == 1 || dig.r3 == 1))
                motor_turn(SPEED, SPEED*0.7*light_ratio, 0);
            else if(light_ratio < 1.0 && dig.r1 == 0 && dig.r2 == 1 && dig.r3 == 1)
                motor_turn(SPEED, 0, 0);

            // Going Straight
            else if(light_ratio == 1.0)
                motor_turn(SPEED, SPEED, 0);
        }
    }
}

int black()
{
    count++;
    motor_forward(0, 0);
    IR_flush();
    IR_wait();
    if(count == 1) tid = xTaskGetTickCount();
    if(count > 1) print_mqtt("Zumo018/lap", "Elapsed time: %d", xTaskGetTickCount() - tid);
    while(dig.l3 != 0 || dig.r3 != 0)
    {
        reflectance_digital(&dig);
        motor_forward(SPEED, 0);
    }
    return tid;
}
#endif

#if 0
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


    while(1)
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

#if 1
    

void zmain(void)
{
    TickType_t timestart, timeend;
    setup_motor();
    power();
    drive_to_black();
    print_mqtt("Zumo018/ready", "zumo");
    IR_wait();
    print_mqtt("Zumo018/start", "start");
    timestart=xTaskGetTickCount();
    motor_forward(SPEED, 500); 
   
    
    while(button == true) 
    {
        reflectance_digital(&dig);
        if(SW1_Read() == 0) button = false;
        
        if (dig.l3 == 1 || dig.l2 == 1 || dig.l1 == 1){
            motor_backward(SPEED, 180);
            motor_tank_turn(1, MAXSPEED, MAXSPEED, 180);
        }
        else if (dig.r3 == 1 || dig.r2 == 1 || dig.r1 == 1){
            motor_backward(SPEED, 180);
            motor_tank_turn(0, MAXSPEED, MAXSPEED, 200);
        }
        else if ((dig.r1 == 1 && dig.l1 == 1) || (dig.r2 == 1 && dig.l2 == 1) || (dig.r3 == 1 && dig.l3 == 1)){
            motor_backward(SPEED, 200);
            motor_tank_turn(0, MAXSPEED, MAXSPEED, 125);
        } 
        else {
            motor_forward(SPEED,0);
        }
        
        LSM303D_Read_Acc(&data);
        

        diff_dataX = data.accX - second_dataX;
        diff_dataY = data.accY - second_dataY;
        
        //print_mqtt("Zumo018/diff", "X %d Y %d", diff_dataX, diff_dataY);  
        
        float tan = data.accY / data.accX;
        float result;

        result = atan(tan); //angle in radians
        result = (result * 180) / PI;  // Converting radians to degrees
        
        if (diff_dataX > 6000 || diff_dataY > 6000){
            timeend = xTaskGetTickCount();
            print_mqtt("Zumo018/hit", "Zumo018/hit %d %.2f\n", timeend, result);
        }
        else if (diff_dataX < -6000 || diff_dataY < -6000) {
            timeend = xTaskGetTickCount();
            print_mqtt("Zumo018/hit", "Zumo018/hit %d %.2f\n", timeend, result);
        }
        motor_forward(0,0);
        second_dataX = data.accX;
        second_dataY = data.accY;
    }
    timeend = xTaskGetTickCount();
    print_mqtt("Zumo018/stop", "%d", timeend);
    print_mqtt("Zumo018/time", "Time is %d\n", timeend - timestart);    
}   

#endif

// Our own functions

void motor_tank_turn(uint8 dir, uint8 l_MAXSPEED, uint8 r_MAXSPEED, uint32 delay)
{
    MotorDirLeft_Write(!dir); // dir 0 turns left, dir 1 turns right.
    MotorDirRight_Write(dir);
    PWM_WriteCompare1(l_MAXSPEED);
    PWM_WriteCompare2(r_MAXSPEED);
    vTaskDelay(delay);
}

void power(void)
{
    while (button == false)
        if (SW1_Read() == 0) button = true;
}

void setup_motor()
{
    motor_start();
    motor_forward(0,0);
    Ultra_Start();
    IR_Start();
    IR_flush();
    reflectance_start();
    reflectance_set_threshold(14000,8000,16000,16000,8000,14000);
    LSM303D_Start();
}
//void check_hits()
//{
  //  if (0 || 0) {
        // send hit message: zumo028/hit time   
  

void drive_to_black()
{
    while(1){
        motor_forward(50,0);
        reflectance_digital(&dig);
        if(dig.l3 == 1 || dig.r3 == 1){
            break;
        }
    }
    motor_forward(0,0);
}   
/* [] END OF FILE */

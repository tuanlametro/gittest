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
#define SPEED 100

/**
 * @file    main.c
 * @brief
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/
void motor_tank_turn(uint8 dir, uint8 l_MAXSPEED, uint8 r_MAXSPEED, uint32 delay);
void power(void);
void linefollow(int x);
void setup_motor();
void drive_to_line();
void finish();
bool button = false, white = false, flag = false;
struct sensors_ dig;
struct sensors_ ref;
struct accData_ data;
int count = 0;
float light_ratio = 0;
TickType_t time_start, time_end;

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
bool was_white;

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

// Sumo wrestling
#if 0
int turntime = 500;   
int d = 0;
void black();
void drive_to_line();
TickType_t tid = 0, tid2 = 0;

void zmain(void) 
{
    setup_motor();
    power();
    drive_to_line();
    
    while(1)
    {
        reflectance_digital(&dig);
        d = Ultra_GetDistance(); // d is distance in cm
        printf("distance = %d\r\n", d);
        while( d <= 10 )
        {
            reflectance_digital(&dig);
            if(dig.l3 == 1 || dig.r3 == 1) break;
            d = Ultra_GetDistance();
            motor_forward(MAXSPEED, 0);
        }
        LSM303D_Read_Acc(&data);
        
        if(dig.l3 == 1 || dig.r3 == 1)
        {
            count++;
            black();
        }
    }
}
    
void black()
{
    tid2 = xTaskGetTickCount();
    if(count == 1)
    {
        motor_forward(0,0);
        IR_wait();
        tid = xTaskGetTickCount();

        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
    }

    else
    {
        if(dig.l3 == 1)
        {
            while(tid < 500)
            {
                tid = xTaskGetTickCount() - tid2;
                motor_tank_turn(1, SPEED, SPEED, 0);
            }
        }
        
        else if(dig.r3 == 1)
        {
            while(tid < 500)
            {
                tid = xTaskGetTickCount() - tid2;
                motor_tank_turn(0, SPEED, SPEED, 0);
            }
        }
    }
}
#endif 

// Line Following
#if 1

void black();
void zmain(void) 
{
    setup_motor();
    power();
    drive_to_line();
            
        while(count < 3)
        {
            reflectance_read(&ref);
            reflectance_digital(&dig); 

            if(dig.l3 == 1 && dig.r3 == 1)
                black();
            
            linefollow(255); //Max speed
        }
    print_mqtt("Zumo018", "Time: %d", xTaskGetTickCount() - time_start);
    motor_forward(0,0);
}

void black()
{
    /* This function serves to move the robot over a black line while only counting it once. 
    If the count is at the starting position, the function waits for IR input. A timestamp is taken after IR input is given. */
    if(count == 0)
    {
        motor_forward(0,0);
        IR_wait();
        time_start = xTaskGetTickCount();
    }
        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
    
    
    /*else
    {
        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
    }*/
    count++;
}
    
#endif

// Maze stuff
#if 0
void black();
void pathfind();
void intersect(int i);
void block();
TickType_t tid = 0, tid2 = 0;
bool exception = false;
int dir = 0, dumdir = 0, x = 15, y = 4, dx = 0, dy = 0, d = 0;
// Dir 0 is N, Dir 1 is E, Dir -1 is W, and nothing is S. We avoid S at all costs.
int grid[15][9] = //0 - 14 rows, 0 - 8 columns 
{  
    {1, 1, 1, 0, 0, 0, 1, 1, 1}, 
    {1, 1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1}, //Wait for IR happens at [14][4]
};
    
void zmain(void) 
{
    setup_motor();
    power();
    drive_to_line();

    while(x > 0) 
    {
        reflectance_digital(&dig);
        
        if(exception == true)
            pathfind();
            
        if(dig.l3 == 1 && dig.r3 == 1)
        {                
            black();
            block();
            motor_forward(SPEED, tid); // Move past the black line to a distance equal to the width of the black line.
            pathfind(); 
        }
        else
            linefollow(150);
        }     
    finish();
}

void black()
{   
    /* Same as with the black() function in the line follow program, this function serves to move the robot over a black line
    while only counting it once. If the count is at the starting position, the function waits for IR input. */
    
    if(x == 15)
    {
        motor_forward(0,0);
        IR_wait();
        time_start = xTaskGetTickCount();
        print_mqtt("Zumo018/start", "Start time: %d", tid); 
        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
        tid = xTaskGetTickCount() - time_start; // Records how long it took the robot to travel over the black line.
    }

    else
    {
        while(dig.l3 != 0 || dig.r3 != 0)
        {
            reflectance_digital(&dig);
            motor_forward(SPEED, 0);
        }
    }
    
    if(dir == 0)
    {
        //motor_forward(0,0); // rem
        x--;
    }
    else if(dir == -1)
        y--;
    else if(dir == 1)
        y++;
    
    print_mqtt("Zumo018/position", "Current Pos: %d, %d", x, y);
}

void block() 
{ 
    if(dir == 0)
        dx = -1, dy = 0;
    else if(dir == -1)
        dx = 0, dy = -1;
    else if(dir == 1)
        dx = 0, dy = 1;
    /* This series of if/else statements expands current dir into two numbers (dx, dy) that can be added to 
    the current position (x, y) to find the position of the intersection ahead of the robot. */
    
    d = Ultra_GetDistance();
    
    if(d < 20) // If an object is within 20 centimetres of the robot...
    {
        grid[x+dx][y+dy] = 1; // The intersection ahead of the robot is switched to a 1 
        if(dir != 0)
        {
            /* This ensures that if the robot wanted to turn towards the centre column but was obstructed,
            then its final dir will be in the opposition direction of its initial dir.*/
            
            exception = true;
            dumdir = dir * -1;
            grid[x-1][y+dy] = 1; // This coordinate is flipped so it is no longer considered a possible pathway by pathfind()
        }     
    }
}

void pathfind()
{    
    /*if(dir == 0)
        Beep(50, 50);
    else if(dir == 1)
    {
        Beep(50, 50);
        vTaskDelay(50);
        Beep(50, 50);
    }
    else if(dir == -1)
    {
        Beep(50, 50);
        vTaskDelay(50);
        Beep(50, 50);
        vTaskDelay(50);
        Beep(50, 50);
    } rem */
        
    if(grid[x+dx][y+dy] == 0) // If the intersection in front of us has no obstacle...
    {
        if(dir == 0) 
            return; // and the robot is facing forwards towards the exit, then exit function.
        else if(dir == 1)
        {
            intersect(0); // and the robot is facing to the right, then turn left.
        }
        else if(dir == -1) 
        {
            intersect(1); // and the robot is facing to the left, then turn right.
        }
    }
            
    else if(grid[x+dx][y+dy] == 1) // If the intersection in front of us has a block on it...
    {
        motor_forward(0, 50);
        if(y <= 4) // and the robot is to the left of centre
        {
            if(grid[x-1][y+1] == 0 && grid[x][y+1] == 0) // then first check if path to the right of robot is clear.
            {    
                intersect(1);
            }
            else if(grid[x-1][y-1] == 0) // otherwise, check the left.
            {    
                intersect(0);
            }
        }
        else if(y > 4) // Vice versa to the above If's operations.
        {
            if(grid[x-1][y-1] == 0 && grid[x][y-1] == 0)
            {
                intersect(0);
            }
                
            else if(grid[x-1][y+1] == 0)
            {
                intersect(1);
            }
        }
    }
    
    if(exception == true) 
    {
        dir = dumdir; 
        exception = false; // Dir is replaced with the dumdir value from the block() function and exception is turned off.
    }
    block();
}

void intersect(int i)
{   
    if(i == 0)
        dir--;
    else if(i == 1)
        dir++;
    // If the robot is turning left, the direction must be decrementing and vice versa.

    while(1)
    {
        reflectance_digital(&dig);

        motor_tank_turn(i, SPEED, SPEED, 0);
        if(dig.l1 == 0 && dig.r1 == 0) white = true;
        
        if(dig.l1 == 1 && dig.r1 == 1 && white == true)
            break;
    }
    white = false;
    motor_forward(0,0); // Returns motors to normal after the tank turn.
}

#endif
        
// Our own functions

void motor_tank_turn(uint8 dir, uint8 l_MAXSPEED, uint8 r_MAXSPEED, uint32 delay)
{
    // Sets the motors in opposite directions so that the robot turns on the spot.
    // Dir 0 turns robot left, dir 1 turns robot right.
    MotorDirLeft_Write(!dir);
    MotorDirRight_Write(dir);
    PWM_WriteCompare1(l_MAXSPEED);
    PWM_WriteCompare2(r_MAXSPEED);
    vTaskDelay(delay);
}

void power(void)
{
    // Holds the robot in a loop until the button is pressed.
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
    reflectance_set_threshold(15000,15000,15000,15000,15000,15000); // was 16k
}

void drive_to_line()
{
    while(1)
    {
        reflectance_digital(&dig);
        if(dig.l3 == 1 && dig.r3 == 1) break;
        else linefollow(50);
        //motor_forward(50,0); // Can be changed to linefollow       
    }
    print_mqtt("Zumo018/line", "Ready!"); 
}

void linefollow(int x)
{
    reflectance_read(&ref);
    reflectance_digital(&dig);
    int most = 20000;          
    if(ref.l1 >= most) ref.l1 = most;
    else if(ref.r1 >= most) ref.r1 = most; 
    light_ratio = (float)ref.l1 / ref.r1; // A ratio is found between the two middle sensors and applied to the motor speed.
    
    // Left Turns
    if(light_ratio > 1.0 && dig.l1 == 1 && dig.l2 == 0 && dig.l3 == 0)
        motor_turn(x/light_ratio, x, 0);
    else if(light_ratio > 1.0 && dig.l1 == 1 && (dig.l2 == 1 || dig.l3 == 1))
        motor_turn(x*0.7/light_ratio, x, 0);
    else if(dig.l1 == 0 && (dig.l2 == 1 || dig.l3 == 1)) //light_ratio > 1.0 &&  
        motor_turn(0, x, 0);

    // Right Turns
    else if(light_ratio < 1.0 && dig.r1 == 1 && dig.r2 == 0 && dig.r3 == 0)
        motor_turn(x, x * light_ratio, 0);
    else if(light_ratio < 1.0 && dig.r1 == 1 && (dig.r2 == 1 || dig.r3 == 1))
        motor_turn(x, x*0.7*light_ratio, 0);
    else if(dig.r1 == 0 && (dig.r2 == 1 || dig.r3 == 1))
        motor_turn(x, 0, 0);

    // Going Straight
    else if(light_ratio == 1.0 && dig.l1 == 1 && dig.r1 == 1)
        motor_turn(x, x, 0);
}

void finish()
{
    // All the stuff the robot should do after it has reached the end of its task.
    motor_forward(0, 0);
    time_end = xTaskGetTickCount() - time_start;
    print_mqtt("Zumo018/stop", "Stop time: %d", time_end); 
    print_mqtt("Zumo018/time", "Run tim: %d", time_end - time_start);  
}

/* [] END OF FILE */

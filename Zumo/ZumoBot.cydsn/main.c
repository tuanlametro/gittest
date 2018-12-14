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
#include <math.h>
#define coeff 5/4095 /*Vref divided by the number of steps between 0 and max*/
#define realv 5/3 /*Used to find real voltage, as the ADC reading is 3/5ths of the multimeter reading*/
#define SIZE 6
#define MAXSPEED 255
#define SUMOSPEED 200
#define SPEED 150
#define PI 3.141592654

/**
 * @file    main.c
 * @brief
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/
void motor_tank_turn(uint8 dir, uint8 l_MAXSPEED, uint8 r_MAXSPEED, uint32 delay);
void power(void);
void linefollow(int x);
void setup_motor();
void drive_to_line(int i);
void finish();
bool button = false, white = false, flag = false;
struct sensors_ dig;
struct sensors_ ref;
struct accData_ data;
int count = 0;
float light_ratio = 0;
TickType_t time_start, time_end;;
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

// Sumo Fight
#if 0
int lastresult;
    
void zmain(void)
{
    setup_motor();
    power();
    drive_to_line(1);
    IR_wait();
    time_start=xTaskGetTickCount();
    print_mqtt("Zumo018/start", "%d", time_start);
    
    motor_forward(100, 300);


    while(button == true)
    {
        reflectance_digital(&dig);
        if(SW1_Read() == 0) button = false;

        if (dig.l3 == 1 || dig.l2 == 1){
            motor_backward(SUMOSPEED, 180);
            motor_tank_turn(1, SUMOSPEED, SUMOSPEED, 180);
        }
        else if (dig.r3 == 1 || dig.r2 == 1){
            motor_backward(SUMOSPEED, 180);
            motor_tank_turn(0, SUMOSPEED, SUMOSPEED, 200);
        }
        else if ((dig.r1 == 1 && dig.l1 == 1) || (dig.r2 == 1 && dig.l2 == 1) || (dig.r3 == 1 && dig.l3 == 1)){
            motor_backward(SUMOSPEED, 200);
            motor_tank_turn(0, SUMOSPEED, SUMOSPEED, 125);
        }
        else {
            motor_forward(MAXSPEED,0);
        }

        LSM303D_Read_Acc(&data);


        diff_dataX = data.accX - second_dataX;
        diff_dataY = data.accY - second_dataY;

        float tan = data.accY / data.accX;
        float result;

        result = atan(tan); //angle in radians
        result = (result * 180) / PI;  // Converting radians to degrees
        
        if (result != 0 && result != lastresult)
            print_mqtt("Zumo018/hit", "%d %.2f", xTaskGetTickCount(), result);
        second_dataX = data.accX;
        second_dataY = data.accY;
        lastresult = result;
    }
    finish();
}

#endif

// Line Following
#if 0
void black();
void zmain(void)
{
    setup_motor();
    power();
    drive_to_line(2);

        while(count < 3)
        {
            reflectance_read(&ref);
            reflectance_digital(&dig);

            if(dig.l3 == 1 && dig.r3 == 1)
                black();

            linefollow(255); //Max speed
        }
    finish();
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
        print_mqtt("Zumo018/start", "%d", time_start);
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
#if 1
void black();
void pathfind();
void intersect(int i);
void block();
TickType_t tid = 0;
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
    drive_to_line(3);

    while(x > 0)
    {
        reflectance_digital(&dig);

        if(dig.l3 == 1 || dig.r3 == 1)
        {
            black();
            motor_forward(SPEED, tid/2);
            block();
        }
        else
            linefollow(255);
    }
    
    if(y > 4)
        intersect(0);
    else if(y < 4)
        intersect(1);

    while(y != 4)
    {
        if(dig.l3 == 1 || dig.r3 == 1)
            black(); 
        else
            linefollow(255);
    }
    
    if(dir == 1)
        intersect(0);
    else if(dir == -1)
        intersect(1);
        
    while(dig.l1 == 1 || dig.r1 == 1)
        linefollow(255);
        
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
        print_mqtt("Zumo018/start", "%d", time_start);
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
        x--;
    else if(dir == -1)
        y--;
    else if(dir == 1)
        y++;
    print_mqtt("Zumo018/position", "%d, %d", y-4, 15-x);
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
            grid[x-1][y+dy] = 1; // This coordinate is flipped so it is no longer considered a possible pathway by pathfind()  
    }
    
    if(grid[x+dx][y+dy] == 1)
        pathfind();
    else if(dir != 0 && grid[x-1][y] == 0)
        pathfind();
    else
        return;
}

void pathfind()
{

    if(grid[x+dx][y+dy] == 0 && grid[x-1][y] == 0) // If the intersection in front of us has no obstacle...
    {
        if(dir == 0)
            return; // and the robot is facing forwards towards the exit, then exit function.
        else if(dir == 1)
            intersect(0); // and the robot is facing to the right, then turn left.
        else if(dir == -1)
            intersect(1);// and the robot is facing to the left, then turn right.
    }

    else if(grid[x+dx][y+dy] == 1) // If the intersection in front of us has a block on it...
    {
        if(y <= 4) // and the robot is to the left of centre
        {
            if((grid[x-1][y+1] == 0 && grid[x][y+1] == 0) || y == 1) // then first check if path to the right of robot is clear.
                intersect(1);
            else if(grid[x-1][y-1] == 0) // otherwise, check the left.
                intersect(0);
        }
        else if(y > 4) // Vice versa to the above If's operations.
        {
            if((grid[x-1][y-1] == 0 && grid[x][y-1] == 0) || y == 7)
                intersect(0);

            else if(grid[x-1][y+1] == 0)
                intersect(1);
        }
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

        if(dig.l1 == 1 && dig.r1 == 1 && dig.r2 == 0 && dig.l2 == 0 && white == true)
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
    LSM303D_Start();
}

void drive_to_line(int i)
{
    while(1)
    {
        reflectance_digital(&dig);
        if(dig.l3 == 1 && dig.r3 == 1) break;
        else
            motor_forward(50,0); // Can be changed to linefollow
    }
    if(i == 1)
        print_mqtt("Zumo018/ready", "zumo");
    else if(i == 2)
        print_mqtt("Zumo018/ready", "line");  
    else if(i == 3)
        print_mqtt("Zumo018/ready", "maze");
    
    motor_forward(0,0);
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
    time_end = xTaskGetTickCount();
    print_mqtt("Zumo018/stop", "%d", time_end);
    print_mqtt("Zumo018/time", "%d", time_end - time_start);
}

/* [] END OF FILE */
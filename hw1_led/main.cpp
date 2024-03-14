#include "mbed.h"

#define LD1_ON     {led1 = 1;}
#define LD1_OFF    {led1 = 0;}
#define LD1_TOG    {led1 = !led1;}

#define LD2_ON     {led2 = 1;}
#define LD2_OFF    {led2 = 0;}
#define LD2_TOG    {led2 = !led2;}

#define LD3_ON     {led34.output(); led34 = 1; led3_status = 1;} // led34 同時只能使用一個 =1代表控制led3 =0代表控制led4 led3_status控制燈亮
#define LD3_OFF    {led34.input(); led3_status = 0;} // 將led34設置為input模式 此時不能更改使用的燈 但可以切換現在使用當中的燈亮不亮
#define LD3_TOG    {if (led3_status) LD3_OFF else LD3_ON;} // 如果燈亮就關 否則打開, LD3_OFF = led34.input() + led3_status = 0

#define LD4_ON     {led34.output(); led34 = 0; led4_status = 1;}
#define LD4_OFF    {led34.input(); led4_status = 0;}
#define LD4_TOG    {if (led4_status) LD4_OFF else LD4_ON;}
#define LED_DELAY 1000ms

Semaphore led_sem(0);
Thread t1;
Thread t2;

DigitalOut led1(LED1);
DigitalOut led2(LED2);

// This object drives both LD3 and LD4 on the board.
// Only one of these LEDs can be driven at a time.
DigitalInOut led34(LED3);
//InterruptIn button(USER_BUTTON);
InterruptIn button(BUTTON1);

int led_nb = 0;
int led3_status = 0;
int led4_status = 0;

volatile int button_switch = -1;

void led_thread(void const *name) {
    while (1) {
        led_sem.acquire();
        while (1) {
            if (*((int*)name) == 1) { 
                LD1_TOG;
                ThisThread::sleep_for(LED_DELAY);
                printf("led1\n");
                if(button_switch % 2 == 1)
                    break;
            }
            else if (*((int*)name) == 2) {
                LD2_TOG;
                ThisThread::sleep_for(LED_DELAY);
                printf("led2\n");
                if (button_switch % 2 == 0)
                    break;
            }
        }
        LD1_OFF;
        LD2_OFF;
        LD3_OFF;
        LD4_OFF;
        led_sem.release();
    }
}

void button_pressed()
{
    // acts at the first press
    if (button_switch == -1) {
        led_sem.release();
    }
}

void button_released()
{
    ++button_switch;
}

int main()
{
    LD1_OFF;
    LD2_OFF;
    LD3_OFF;
    LD4_OFF;

    button.fall(&button_pressed); // button按下時先將semaphore release
    button.rise(&button_released); // ++button_switch; (初始=-1)
    const int a1 = 1;
    const int a2 = 2;
    t1.start(callback(led_thread, (void *)&a1));
    t2.start(callback(led_thread, (void *)&a2));
    while (1);
}
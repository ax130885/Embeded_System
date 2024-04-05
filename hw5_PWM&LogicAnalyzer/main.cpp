// #include "mbed.h"

// // Define two PWM pins for complementary output
// PwmOut pwm1(D9);
// PwmOut pwm2(D10);
// chrono::seconds dead_time = 1s;

// // Define dead time in microseconds (e.g., 1000us for 1ms)
// const int dead_time_us = 1000;

// // Define the period of the PWM signals in seconds
// const float pwm_period = 0.05f; // 20Hz frequency

// // Define duty cycle limits
// const float dtc_low = 0.5f;
// const float dtc_high = 0.8f;

// // Thread function for PWM1
// void pwm1_thread() {
//     pwm1.period(pwm_period);
//     printf("pwm1 initalized\n");
//     while (true) {
//         printf("pwm1 start loop\n");
//         pwm1.write(dtc_high);
//         ThisThread::sleep_for(chrono::seconds(1));
//         pwm1.write(0.0f);
//         ThisThread::sleep_for(chrono::seconds(1));
//         printf("pwm1 end loop\n");
//     }
// }

// // Thread function for PWM2
// void pwm2_thread() {
//     pwm2.period(pwm_period);
//     printf("pwm2 initalized\n");
//     while (true) {
//         printf("pwm2 start loop\n");
//         pwm2.write(dtc_low);
//         ThisThread::sleep_for(chrono::seconds(1));
//         pwm2.write(0.0f);
//         ThisThread::sleep_for(chrono::seconds(1));
//         printf("pwm2 end loop\n");
//     }
// }

// int main() {
//     // Start the threads
//     printf("------------------------------------------------------");
//     Thread thread1;
//     Thread thread2;
//     thread1.start(pwm1_thread);
//     thread2.start(pwm2_thread);

//     // Main thread does nothing and could be used for other tasks or put to sleep
//     while (true) {
//         ThisThread::sleep_for(chrono::seconds(0));
//     }
// }

#include "mbed.h"

void dead_2_H1L2();
void dead_2_L1H2();
void H1L2();
void L1H2();

// Define two PWM pins
PwmOut pwm1(D9);
PwmOut pwm2(D10);

// Set PWM frequency and dead time (in seconds)
const float pwm_period = 0.001f; // 50ms period
const float high = 1.0f; // 100% duty cycle
const float low = 0.0f; // 0% duty cycle

// Create an event queue
EventQueue queue;

void _init() {
    // Set the PWM signal period
    pwm1.period(pwm_period);
    pwm2.period(pwm_period);

    // Initialize PWM signals
    pwm1.write(low);
    pwm2.write(low);
}

void dead_2_H1L2() {

    pwm1.write(low);
    pwm2.write(low);
    queue.call_in(int(10), H1L2); // Convert dead_time to milliseconds
}

void H1L2() {
    pwm1.write(high);
    pwm2.write(low);
    queue.call_in(int(40), dead_2_L1H2);
}

void dead_2_L1H2() {

    pwm1.write(low);
    pwm2.write(low);
    queue.call_in(int(10), L1H2); // Convert dead_time to milliseconds
}

void L1H2() {
    pwm1.write(low);
    pwm2.write(high);
    queue.call_in(int(40), dead_2_H1L2); // Convert to milliseconds
}

int main() {

    // Schedule PWM start and stop events
    queue.call(_init);
    queue.call_in(int(1000), dead_2_H1L2); // Convert to milliseconds

    // Start the event queue dispatch loop
    queue.dispatch_forever();
}
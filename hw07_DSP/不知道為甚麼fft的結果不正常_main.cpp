#include "mbed.h"

// #include "stm32l475e_iot01_gyro.h"
#include "dsp.h"
#include "arm_math.h"

#define SAMPLE_SIZE     64    // 樣本大小，2 的冪次方
#define TEST_LENGTH_SAMPLES 64

float32_t sin_out[SAMPLE_SIZE] = {0};
float32_t fft_input[SAMPLE_SIZE] = {0};  // 傅立葉轉換的輸入陣列
float32_t fft_output[SAMPLE_SIZE] = {0};     // 傅立葉轉換的輸出陣列
float32_t inverse_fft_output[SAMPLE_SIZE] = {0};
float32_t mag_output[SAMPLE_SIZE/2] = {0};

// 創建傅立葉轉換的實例
arm_rfft_fast_instance_f32 fft_instance;
arm_status status = arm_rfft_fast_init_f32(&fft_instance, SAMPLE_SIZE);
// // 創建感測器的buffer
// float pGyroDataXYZ[3] = {0};

// main() runs in its own thread in the OS
int main()
{

    printf("arm_status = %d\n", status);

    for(uint16_t i = 0; i < SAMPLE_SIZE; i++)
    {
        // sin_out[i] = arm_sin_f32(i*2*3.1416f/128) + 0.3f*arm_sin_f32(i*2*3.1416f/16);
        sin_out[i] = arm_sin_f32(i*2*3.1416f/32);
        printf("%f \n", sin_out[i]);
    }
    printf("start fft @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n");

    arm_rfft_fast_f32(&fft_instance, sin_out, fft_output, 0);
    arm_rfft_fast_f32(&fft_instance, fft_output, inverse_fft_output, 1);

    for (int i = 0; i < SAMPLE_SIZE; i++){
        // printf("mag_output[%d] = %.1f \n", i, mag_output[i]);
        printf("%f\n", fft_output[i]);
    }
    printf("start reverse #######################################################################\n\n");

    for (int i = 0; i < SAMPLE_SIZE; i+=1){
        // printf("mag_output[%d] = %.1f \n", i, mag_output[i]);
        printf("%f\n", inverse_fft_output[i]);
    }
    printf("\n\n\n");










    // arm_cmplx_mag_squared_f32(fft_output, mag_output, SAMPLE_SIZE);
    // arm_cmplx_mag_f32(fft_output, mag_output, SAMPLE_SIZE);

    // for (int i = 0; i < SAMPLE_SIZE/2; i++){
    //     // printf("mag_output[%d] = %.1f \n", i, mag_output[i]);
    //     printf("%f\n", mag_output[i]);
    // }
    // printf("\n\n");


 





    //     ThisThread::sleep_for(10000ms); // 每次迴圈之間暫停 1000 毫秒（1 秒）
    // }
}


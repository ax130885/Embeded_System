
#include "mbed.h"
#include "arm_math.h"
#include <cmath>
#include <cstdint>
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"
#include "dsp.h"

// main() runs in its own thread in the OS

//=========macro_define ==============//
#define SAMPLE_SIZE 320
#define BLOCK_SIZE 32

//=========macro_define ==============//

//==============global parameters====================//
const uint8_t block_size = BLOCK_SIZE;
const uint16_t sample_size = SAMPLE_SIZE;
const uint16_t blockNumber = SAMPLE_SIZE / BLOCK_SIZE;
static float32_t firStateF32[2 * BLOCK_SIZE + 29 - 1];
const float32_t firCoeffs32[29] = {
    -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
    -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
    +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
    +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f};

const float32_t firCoeffs32_mk2[29] = {
    -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
    -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
    +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
    +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f};
//==============global parameters====================//

int main()
{
    float sensor_value = 0;
    int16_t pDataXYZ[3] = {0};
    float pGyroDataXYZ[3] = {0};

    uint32_t i;
    uint32_t mem = 0;
    arm_fir_instance_f32 S;
    arm_status arm_status;
    float32_t *input_buff, *output_buff;
    input_buff = new float32_t[sample_size];  // 320
    output_buff = new float32_t[sample_size]; // 320
    float32_t *input_ptr = &input_buff[0];
    float32_t *output_ptr = &output_buff[0];

    BSP_TSENSOR_Init();
    BSP_HSENSOR_Init();
    BSP_PSENSOR_Init();

    BSP_MAGNETO_Init();
    BSP_GYRO_Init();
    BSP_ACCELERO_Init();

    arm_fir_instance_f32 FIR_insrance;

    float32_t data_x = 0;
    float32_t data_y = 0;
    float32_t data_z = 0;
    arm_fir_init_f32(&FIR_insrance, 29, (float32_t *)&firCoeffs32[0], &firStateF32[0], block_size);
    while (mem == 0)
    {
        printf(" =================data start collecting ==================== \n");
        for (i = 0; i < sample_size; i++) // input collecting data
        {
            BSP_ACCELERO_AccGetXYZ(pDataXYZ);
            data_x = float32_t(pDataXYZ[0]);
            data_y = float32_t(pDataXYZ[1]);
            data_z = float32_t(pDataXYZ[2]);
            float32_t temp = sqrt(data_z * data_z + data_y * data_y + data_x * data_x);
            input_buff[i] = temp;
            // printf("the data number %d --> X_MAG : %f, Y_MAG: %f, Z_MAG: %f ,result : %f\n",i,data_x,data_y,data_z,temp);
            printf("%f\n", temp);
        }

        for (i = 0; i < blockNumber; i++)
        {
            arm_fir_f32(&FIR_insrance, input_ptr + (i * block_size), output_ptr + (i * block_size), block_size);

            // printf("X_MAG : %f, Y_MAG: %f, Z_MAG: %f\n",result);
        }
        printf(" =================result start printing==================== \n");
        for (i = 0; i < sample_size; i++)
        {
            printf(" %f \n", output_buff[i]);
        }
        mem++;
        // ThisThread::sleep_for(10000);
    }
}


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>



#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable


void I2C_Init();
void Accele_Init();

long getXRawdata();
long getYRawdata();
long getXRawdata();

float getXdata();
float getYdata();
float getZdata();


uint32_t I2C_Send1(int8_t slave, uint8_t data1);
uint32_t I2C_Send2(int8_t slave, uint8_t data1, uint8_t data2);
uint8_t I2C_Recv(int8_t slave);

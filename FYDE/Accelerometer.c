
#include "Accelerometer.h"

void I2C_Init(void){
    SYSCTL_RCGCI2C_R |= 0x0001;           // activate I2C0
    SYSCTL_RCGCGPIO_R |= 0x0002;          // activate port B
    while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?
    
    GPIO_PORTB_AFSEL_R |= 0x0C;           // 3) enable alt funct on PB2,3
    GPIO_PORTB_ODR_R |= 0x08;             // 4) enable open drain on PB3 only
    GPIO_PORTB_DEN_R |= 0x0C;             // 5) enable digital I/O on PB2,3
    // 6) configure PB2,3 as I2C
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFF00FF)+0x00003300;
    GPIO_PORTB_AMSEL_R &= ~0x0C;          // 7) disable analog functionality on PB2,3
    I2C0_MCR_R = I2C_MCR_MFE;      // 9) master function enable
    I2C0_MTPR_R = 24;              // 8) configure for 100 kbps clock
    // 20*(TPR+1)*20ns = 10us, with TPR=24
}

void Accele_Init(){
    I2C_Send2(104, 0x6B, 0);        //Power Config
    I2C_Send2(104, 0x1C, 0);        //Accelerometer Config
}

long getXRawData(){
    long acceleX;
    I2C_Send1(104, 0x3B);
    acceleX = (I2C_Recv(104) << 8) | I2C_Recv(104);
    return acceleX;
}

long getYRawData(){
    long acceleY;
    I2C_Send1(104, 0x3D);
    acceleY = (I2C_Recv(104) << 8) | I2C_Recv(104);
    return acceleY;
    
}

long getZRawData(){
    long acceleZ;
    I2C_Send1(104, 0x3F);
    acceleZ = (I2C_Recv(104) << 8) | I2C_Recv(104);
    return acceleZ;
}

float getXdata(){
    return getXRawData() / 16384.0;
}

float getYdata(){
    return getYRawData() / 16384.0;
}

float getZdata(){
    return getYRawData() / 16384.0;
}

uint8_t I2C_Recv(int8_t slave){
    int retryCounter = 1;
    do{
        while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
        I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
        I2C0_MSA_R |= 0x01;              // MSA[0] is 1 for receive
        I2C0_MCS_R = (0
                      // & ~I2C_MCS_ACK     // negative data ack (last byte)
                      | I2C_MCS_STOP     // generate stop
                      | I2C_MCS_START    // generate start/restart
                      | I2C_MCS_RUN);    // master enable
        while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
        retryCounter = retryCounter + 1;        // increment retry counter
    }                                         // repeat if error
    while(((I2C0_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0) && (retryCounter <= 5));
    return (I2C0_MDR_R&0xFF);          // usually returns 0xFF on error
}

uint32_t I2C_Send1(int8_t slave, uint8_t data1){
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
    I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
    I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
    I2C0_MDR_R = data1&0xFF;         // prepare first byte
    I2C0_MCS_R = (0
                  //   & ~I2C_MCS_ACK     // no data ack (no data on send)
                  | I2C_MCS_STOP     // generate stop
                  | I2C_MCS_START    // generate start/restart
                  | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
    // return error bits
    return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}


uint32_t I2C_Send2(int8_t slave, uint8_t data1, uint8_t data2){
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
    I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
    I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
    I2C0_MDR_R = data1&0xFF;         // prepare first byte
    I2C0_MCS_R = (0
                  //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                  //   & ~I2C_MCS_STOP    // no stop
                  | I2C_MCS_START    // generate start/restart
                  | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
    // check error bits
    if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
        I2C0_MCS_R = (0                // send stop if nonzero
                      //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                      | I2C_MCS_STOP     // stop
                      //  & ~I2C_MCS_START   // no start/restart
                      //  & ~I2C_MCS_RUN    // master disable
                      );
        // return error bits if nonzero
        return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
    }
    I2C0_MDR_R = data2&0xFF;         // prepare second byte
    I2C0_MCS_R = (0
                  // & ~I2C_MCS_ACK     // no data ack (no data on send)
                  | I2C_MCS_STOP     // generate stop
                  // & ~I2C_MCS_START   // no start/restart
                  | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
    // return error bits
    return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}



#ifndef HP203B_H
#define HP203B_H


#define SOFT_RESET 0X06  //软件复位

#define CMD_READ_PT  0x10     //读取温度与气压
#define CMD_READ_AT  0x11     //读取海拔与温度
#define CMD_ANA_CAL 0x28  //内部模拟电路校准
#define CMD_REDD_P   0x30     //读取气压
#define CMD_READ_A   0x31     //读取海拔
#define CMD_READ_T   0x32     //读取温度


/*定义HP203B对象类型*/
typedef struct Hp203bObject {
    uint8_t deviceAddress;
    uint32_t cTemperature;
    uint32_t cPressure;
    uint32_t cAltitude;
    float fTemperature;
    float fPressure;
    float fAltitude;
    void (*Receive)(struct Hp203bObject *hp,uint8_t *rData,uint16_t rSize);    //接收数据操作指针
    void (*Transmit)(struct Hp203bObject *hp,uint8_t *tData,uint16_t tSize);   //发送数据操作指针
}Hp203bObjectType;

//typedef struct Hp203bRegister{
//    uint8_t CMD_ANA_CAL;
//
//}Hp203bRegisterType;


/*接收数据操作指针*/
typedef void (*Ph203bReceiveType)(struct Hp203bObject *hp,uint8_t *rData,uint16_t rSize);
/*发送数据操作指针*/
typedef void (*Ph203bTransmitType)(struct Hp203bObject *hp,uint8_t *tData,uint16_t tSize);

void ReceiveFromHp203b(Hp203bObjectType *hp,uint8_t *rData,uint16_t rSize);
void TransmitToHp203b(Hp203bObjectType *hp,uint8_t *tData,uint16_t tSize);

void Hp203bSoftReset(Hp203bObjectType *hp);
void Hp203bInitialization(Hp203bObjectType *hp,
                          uint8_t deviceAddress,
                          Ph203bReceiveType recieve,
                          Ph203bTransmitType transmit);


void Hp203bReadTemperaturePressure(Hp203bObjectType *hp);

#endif

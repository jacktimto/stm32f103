#include "main.h"
#include "i2c.h"
#include "hp203b.h"



/*从Hp203b接收数据*/
void ReceiveFromHp203b(Hp203bObjectType *hp,uint8_t *rData,uint16_t rSize)
{
    HAL_I2C_Master_Receive(&hi2c1,hp->deviceAddress,rData,rSize,1000);
}

/*向Hp203b传送数据*/
void TransmitToHp203b(Hp203bObjectType *hp,uint8_t *tData,uint16_t tSize)
{
    HAL_I2C_Master_Transmit(&hi2c1,hp->deviceAddress,tData,tSize,1000);
}

/*软件复位*/
void Hp203bSoftReset(Hp203bObjectType *hp)
{
    uint8_t pData[] = {SOFT_RESET};
    HAL_I2C_Master_Transmit(&hi2c1, hp->deviceAddress, pData, 1,1000);
    HAL_Delay(100); //need delay after init
}


/*HP203B对象初始化*/
void Hp203bInitialization(Hp203bObjectType *hp,
                          uint8_t deviceAddress,
                          Ph203bReceiveType recieve,
                          Ph203bTransmitType transmit)
{
    if((hp==NULL)||(recieve==NULL)||(transmit==NULL))
    {
        return;
    }

    hp->Receive=recieve;
    hp->Transmit=transmit;

    if((deviceAddress==0xEE)||(deviceAddress==0xEC))
    {
        hp->deviceAddress=deviceAddress;
    }
    else if((deviceAddress==0x77)||(deviceAddress==0x76))
    {
        hp->deviceAddress=(deviceAddress<<1);
    }
    else
    {
        hp->deviceAddress=0;
    }

    /*软复位命令*/
    Hp203bSoftReset(hp);
}

/*读取温度和压力值*/
void Hp203bReadTemperaturePressure(Hp203bObjectType *hp)
{
    uint8_t cmd=CMD_READ_PT;
    uint8_t rData[6];

    hp->Transmit(hp,&cmd,1);

    hp->Receive(hp,rData,6);

    hp->cTemperature=(rData[0]<<16)+(rData[1]<<8)+rData[2];

    if(rData[0]>0x0F)
    {
        hp->cTemperature=hp->cTemperature|0xFFF00000;
        temp=~(hp->cTemperature-0x01);
        hp->fTemperature=0.0f-(float)temp/100.0f;
    }
    else
    {
        hp->fTemperature=(float)hp->cTemperature/100.0f;
    }

    hp->cPressure=(rData[3]<<16)+(rData[4]<<8)+rData[5];
    hp->fPressure=(float)(hp->cPressure)/100.0f; //编译器会认为小数是double类型
}


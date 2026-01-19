/*
 * thermal.h
 *
 *  Created on: Jan 6, 2026
 *      Author: luan
 */

#ifndef INC_TMP468_HO_
#define INC_TMP468_HO_

#include<stdint.h>

/******************************** Register map ********************************/

#define TMP468_REG_LOCAL_TMP   0x00
#define TMP468_REG_REMOTE1_TMP 0x01
#define TMP468_REG_REMOTE2_TMP 0x02
#define TMP468_REG_REMOTE3_TMP 0x03
#define TMP468_REG_REMOTE4_TMP 0x04
#define TMP468_REG_REMOTE5_TMP 0x05
#define TMP468_REG_REMOTE6_TMP 0x06
#define TMP468_REG_REMOTE7_TMP 0x07
#define TMP468_REG_REMOTE8_TMP 0x08

#define TMP468_REG_SOFTWARE_RESET 0x20

#define TMP468_REG_THERM1_STATUS 0x21
#define TMP468_REG_THERM2_STATUS 0x22

#define TMP468_REG_REMOTE_CHANNEL_OPEN_STATUS 0x23
#define TMP468_REG_CONFIGURATION 0x30

#define TMP468_REG_THERM_HYSTERESIS 0x38

#define TMP468_REG_LOCAL_THERM1_LIMIT 0x39
#define TMP468_REG_LOCAL_THERM2_LIMIT 0x3A

/**************************** Remote Configuratoin ****************************/

#define TMP468_REG_REMOTE1_OFFSET 0x40              
#define TMP468_REG_REMOTE1_IDEALITY_CORRECTION 0x41/*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE1_THERM1_LIMIT 0x42
#define TMP468_REG_REMOTE1_THERM2_LIMIT 0x43


#define TMP468_REG_REMOTE2_OFFSET 0x48
#define TMP468_REG_REMOTE2_IDEALITY_CORRECTION 0x49/*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE2_THERM1_LIMIT 0x4a
#define TMP468_REG_REMOTE2_THERM2_LIMIT 0x4b


#define TMP468_REG_REMOTE3_OFFSET 0x50
#define TMP468_REG_REMOTE3_IDEALITY_CORRECTION 0x51/*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE3_THERM1_LIMIT 0x52
#define TMP468_REG_REMOTE3_THERM2_LIMIT 0x53


#define TMP468_REG_REMOTE4_OFFSET 0x58
#define TMP468_REG_REMOTE4_IDEALITY_CORRECTION 0x59/*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE4_THERM1_LIMIT 0x5a
#define TMP468_REG_REMOTE4_THERM2_LIMIT 0x5b


#define TMP468_REG_REMOTE5_OFFSET 0x60
#define TMP468_REG_REMOTE5_IDEALITY_CORRECTION 0x61 /*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE5_THERM1_LIMIT 0x62
#define TMP468_REG_REMOTE5_THERM2_LIMIT 0x63


#define TMP468_REG_REMOTE6_OFFSET 0x68
#define TMP468_REG_REMOTE6_IDEALITY_CORRECTION 0x69 /*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE6_THERM1_LIMIT 0x6a
#define TMP468_REG_REMOTE6_THERM2_LIMIT 0x6b


#define TMP468_REG_REMOTE7_OFFSET 0x70
#define TMP468_REG_REMOTE7_IDEALITY_CORRECTION 0x71/*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE7_THERM1_LIMIT 0x72
#define TMP468_REG_REMOTE7_THERM2_LIMIT 0x73


#define TMP468_REG_REMOTE8_OFFSET 0x78
#define TMP468_REG_REMOTE8_IDEALITY_CORRECTION 0x79/*on datasheet represented by greek leter eta (η)*/
#define TMP468_REG_REMOTE8_THERM1_LIMIT 0x7a
#define TMP468_REG_REMOTE8_THERM2_LIMIT 0x7b

#define TMP468_REG_REMOTE_IDEALITY_CORRECTION_pos 8U /*position to shift before writing*/

#define TMP468_REG_BLOCKREAD_START 0x80 /* reading from this address trigger autoincrement of pointer register*/
#define TMP468_REG_BLOCKREAD_END 0x88 /*inclusive*/

#define TMP468_REG_REMOTE_CONFIG_BASE_ADDR TMP468_REG_REMOTE1_OFFSET /*!< address of REMOTE1 sensor*/
#define TMP468_REG_REMOTE_CONFIG_REMOTE_REMOTE_OFFSET 0x8 /*!< address offset from REMOTE<n> to REMOTE<n+1> sensor*/
#define TMP468_REG_REMOTE_CONFIG_REMOTE_OFFSET_OFFSET 0x0 /*!< address offset from the REMOTE sensor address to the offset config register*/
#define TMP468_REG_REMOTE_CONFIG_REMOTE_ETA_OFFSET 0x1    /*!< address offset from the REMOTE sensor address to the eta config register*/
#define TMP468_REG_REMOTE_CONFIG_REMOTE_THERM1_OFFSET 0x2 /*!< address offset from the REMOTE sensor address to the THERM1 config register*/
#define TMP468_REG_REMOTE_CONFIG_REMOTE_THERM2_OFFSET 0x3 /*!< address offset from the REMOTE sensor address to the THERM! config register*/


/******************************* remote masking *******************************/

#define TMP468_REMOTE_bitmask (0xff<<8)
#define TMP468_REMOTE8_MASK   0b1000000000000000
#define TMP468_REMOTE7_MASK   0b0100000000000000
#define TMP468_REMOTE6_MASK   0b0010000000000000
#define TMP468_REMOTE5_MASK   0b0001000000000000
#define TMP468_REMOTE4_MASK   0b0000100000000000
#define TMP468_REMOTE3_MASK   0b0000010000000000
#define TMP468_REMOTE2_MASK   0b0000001000000000
#define TMP468_REMOTE1_MASK   0b0000000100000000
#define TMP468_LOCAL_MASK     0b0000000010000000

#define TMP468_CONFIG_ENABLE_REMOTE_BASE   TMP468_REMOTE1_MASK /* shift left to remote index*/
#define TMP468_CONFIG_ENABLE_LOCAL         (1U<<7)
#define TMP468_CONFIG_ONESHOT_START        (1U<<6)
#define TMP468_CONFIG_SHUTDOWN_mask        (1U<<5)
#define TMP468_CONFIG_CONVERSION_RATE_mask (7U<<2)
#define TMP468_CONFIG_CONVERSION_RATE_pos  (2U)
#define TMP468_CONFIG_BUSY                 (1U<<1)

#define TMP468_CONVERSION_RATE_16s         (0U<<TMP468_CONFIG_CONVERSION_RATE_pos)
#define TMP468_CONVERSION_RATE_8s          (1U<<TMP468_CONFIG_CONVERSION_RATE_pos)
#define TMP468_CONVERSION_RATE_4s          (2U<<TMP468_CONFIG_CONVERSION_RATE_pos)
#define TMP468_CONVERSION_RATE_2s          (3U<<TMP468_CONFIG_CONVERSION_RATE_pos)
#define TMP468_CONVERSION_RATE_1s          (4U<<TMP468_CONFIG_CONVERSION_RATE_pos)
#define TMP468_CONVERSION_RATE_2hz         (5U<<TMP468_CONFIG_CONVERSION_RATE_pos)
#define TMP468_CONVERSION_RATE_3hz         (6U<<TMP468_CONFIG_CONVERSION_RATE_pos)
#define TMP468_CONVERSION_RATE_CONTINUOUS  (7U<<TMP468_CONFIG_CONVERSION_RATE_pos)

#define TMP468_THERM_mask           0b0111111111000000
#define TMP468_THERM_DISABLE        TMP468_THERM_mask
#define TMP468_THERM_pos            (6U)
#define TMP468_THERM_SIGNAL_mask    1U<<15
#define TMP468_SELECT_THERM1        0
#define TMP468_SELECT_THERM2        1

#define TMP468_HYSTERESIS_pos       7U
#define TMP468_HYSTERESIS_mask      (0xff << TMP468_HYSTERESIS_pos)

/******************************* Info Registers *******************************/

#define TMP468_REG_LOCK  0xC4  /* Lock register */
#define TMP468_REG_MANID 0xFE  /* Manufacturer identification 0x5449 */
#define TMP468_REG_DEVID 0xFF  /* Device identification 0x0468 */

/****************************** Resolution info *******************************/
#define TMP468_HYSTERESIS_RESOLUTION 1        /*!< one degree per bit*/
#define TMP468_THERM_RESOLUTION      0.5f     /*!< 0.5 degrees per bit*/
#define TMP468_TEMP_RESOLUTION       0.0625f  /*!< 0.0625 degrees per bit*/

/********************************* Constants **********************************/
#define TMP468_DO_UNLOCK       0xEB19
#define TMP468_DO_LOCK         0x5CA6
#define TMP468_LOCKED       0x8000
#define TMP468_UNLOCKED     0x0000

#define TMP468_REMOTE_CONFIG_STRUCT

#ifdef TMP468_REMOTE_CONFIG_STRUCT
struct TMP468_remoteConfig{
    int32_t offset;
    int8_t idealityCorrection;
    int32_t THERM1_limit; 
    int32_t THERM2_limit;
};
#endif //TMP468_REMOTE_CONFIG_STRUCT

typedef struct TMP468_t{
    uint16_t addr;
    // uint16_t config;
    uint16_t hysteresis;
    uint16_t localTHERMLimit1, localTHERMLimit2;
    #ifdef TMP468_REMOTE_CONFIG_STRUCT
    struct TMP468_remoteConfig remote[8];
    #endif //TMP468_REMOTE_CONFIG_STRUCT
    void (*i2c_write)(uint16_t, uint8_t, uint16_t); //addr, reg, pData
    uint16_t (*i2c_read )(uint16_t, uint8_t); //addr, reg
} TMP468_t;

// Python reference, table12 of the datasheet (revB)
// etatoi = lambda x: round( ((1.008*2088)/x)-2088 )
// itoeta = lambda x: (1.008*2088)/(2088+x)
#define TMP468_etatoi(eta) ((int)((((1.008f * 2088.0f) / (eta)) - 2088.0f) + 0.5f))
#define TMP468_itoeta(i)   ((1.008f * 2088.0f) / (2088.0f + (float)(i)))

int32_t     TMP468_getRemoteTemperature(TMP468_t target, int idx);
uint16_t    TMP468_getTHERM1Status(TMP468_t target);
uint16_t    TMP468_getTHERM2Status(TMP468_t target);
uint16_t    TMP468_getOpenStatus(TMP468_t target);
int         TMP468_isbusy(TMP468_t target);
TMP468_t    TMP468_init(uint16_t addr, uint16_t conversionRate, uint16_t hysteresis, uint32_t THERM1, uint32_t THERM2, 
                        void (*i2c_write)(uint16_t, uint8_t, uint16_t), 
                        uint16_t (*i2c_read )(uint16_t, uint8_t) );
void        TMP468_setRemoteEnabled(TMP468_t target, uint16_t flags);
uint16_t    TMP468_getRemoteEnabled(TMP468_t target);
void        TMP468_enableLocal(TMP468_t target);
void        TMP468_disableLocal(TMP468_t target);
uint16_t    TMP468_getHysteresis(TMP468_t target);
void        TMP468_setHysteresis(TMP468_t target, uint16_t hysteresis);
int16_t     TMP468_getThermlimit(TMP468_t target, int idx, int thermSelect);
void        TMP468_setThermLimit(TMP468_t target, int idx, int16_t thermlimit, int thermSelect);
void        TMP468_disableTherm(TMP468_t target, int idx, int thermSelect);
void        TMP468_configRemote(TMP468_t *target, int idx, uint32_t offset, int8_t eta, uint32_t THERM1, uint32_t THERM2);
void        TMP468_wakeup(TMP468_t target);
void        TMP468_sleep(TMP468_t target);
void        TMP468_lock(TMP468_t target);
void        TMP468_unlock(TMP468_t target);
uint16_t    TMP468_getLock(TMP468_t target);


#endif /* INC_TMP468_HO_ */
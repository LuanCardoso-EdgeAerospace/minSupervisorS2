/*
 * INA230.h
 *
 *  Created on: Jan 8, 2026
 *      Author: luan
 */

#ifndef INC_INA230_H_
#define INC_INA230_H_

#include <stdint.h>

/******************************* Register Map  ********************************/
#define INA230_REG_CONFIG        0x00  /*!< Configuration Register      - RW     */
#define INA230_REG_VSHUNT        0x01  /*!< Shunt Voltage Register      - R      */
#define INA230_REG_VBUS          0x02  /*!< Bus Voltage Register        - R      */
#define INA230_REG_PWR           0x03  /*!< Power Register              - R      */
#define INA230_REG_CURRENT       0x04  /*!< Current Register            - R      */
#define INA230_REG_CALIBRATION   0x05  /*!< Calibration Register        - RW     */
#define INA230_REG_MASK_ENABLE   0x06  /*!< Mask/Enable Register        - RW     */
#define INA230_REG_ALERT_LIMIT   0x07  /*!< Alert Limit Register        - RW     */
#define INA230_REG_ID            0xFF  /*!< Die identification register - R      */

/*******************  Bit definition for Configuration register  **************/
#define INA230_REG_CFG_RESET_Pos        (15U)
#define INA230_REG_CFG_RESET_Msk        (0x1U << INA230_REG_CFG_RESET_Pos)      /* 0x8000 */
#define INA230_REG_CFG_RESET            INA230_REG_CFG_RESET_Msk                /*!< Reset Bit */
#define INA230_REG_CFG_AVG_Pos          (9U)
#define INA230_REG_CFG_AVG_Msk          (0x7U << INA230_REG_CFG_AVG_Pos)        /* 0x0E00 */
#define INA230_REG_CFG_AVG              INA230_REG_CFG_AVG_Msk                  /*!< Averaging Mode */
#define INA230_REG_CFG_AVG_0            (0x1U << INA230_REG_CFG_AVG_Pos)        /* 0x0200 */
#define INA230_REG_CFG_AVG_1            (0x2U << INA230_REG_CFG_AVG_Pos)        /* 0x0400 */
#define INA230_REG_CFG_AVG_2            (0x4U << INA230_REG_CFG_AVG_Pos)        /* 0x0800 */
#define INA230_REG_CFG_VBUSCT_Pos       (6U)
#define INA230_REG_CFG_VBUSCT_Msk       (0x7U << INA230_REG_CFG_VBUSCT_Pos)     /* 0x01C0 */
#define INA230_REG_CFG_VBUSCT           INA230_REG_CFG_VBUSCT_Msk               /*!< Bus Voltage Conversion Time */
#define INA230_REG_CFG_VBUSCT_0         (0x1U << INA230_REG_CFG_VBUSCT_Pos)     /* 0x0040 */
#define INA230_REG_CFG_VBUSCT_1         (0x2U << INA230_REG_CFG_VBUSCT_Pos)     /* 0x0080 */
#define INA230_REG_CFG_VBUSCT_2         (0x4U << INA230_REG_CFG_VBUSCT_Pos)     /* 0x0100*/
#define INA230_REG_CFG_VSHUNTCT_Pos     (3U)
#define INA230_REG_CFG_VSHUNTCT_Msk     (0x7U << INA230_REG_CFG_VSHUNTCT_Pos)   /* 0x0068 */
#define INA230_REG_CFG_VSHUNTCT         INA230_REG_CFG_VSHUNTCT_Msk             /*!< Shunt Voltage Conversion Time */
#define INA230_REG_CFG_VSHUNTCT_0       (0x1U << INA230_REG_CFG_VSHUNTCT_Pos)   /* 0x0008 */
#define INA230_REG_CFG_VSHUNTCT_1       (0x2U << INA230_REG_CFG_VSHUNTCT_Pos)   /* 0x0020 */
#define INA230_REG_CFG_VSHUNTCT_2       (0x4U << INA230_REG_CFG_VSHUNTCT_Pos)   /* 0x0040 */
#define INA230_REG_CFG_MODE_Pos         (0U)
#define INA230_REG_CFG_MODE_Msk         (0x7U << INA230_REG_CFG_MODE_Pos)       /* 0x0007 */
#define INA230_REG_CFG_MODE             INA230_REG_CFG_MODE_Msk                 /*!< Operating Mode */
#define INA230_REG_CFG_MODE_0           (0x1U << INA230_REG_CFG_MODE_Pos)       /* 0x0001 */
#define INA230_REG_CFG_MODE_1           (0x2U << INA230_REG_CFG_MODE_Pos)       /* 0x0002 */
#define INA230_REG_CFG_MODE_2           (0x4U << INA230_REG_CFG_MODE_Pos)       /* 0x0004 */

/******************  Bit definition for Shunt Voltage register  ***************/
#define INA230_REG_VSHUNT_DATA_Pos      (0U)
#define INA230_REG_VSHUNT_DATA_Msk      (0x7FFFU << INA230_REG_VSHUNT_DATA_Pos) /* 0x7FFF */
#define INA230_REG_VSHUNT_DATA          INA230_REG_VSHUNT_DATA_Msk              /*!< Measured shunt voltage */
#define INA230_REG_VSHUNT_SIGN_Pos      (15U)
#define INA230_REG_VSHUNT_SIGN_Msk      (0x1U << INA230_REG_VSHUNT_SIGN_Pos)    /* 0x8000 */
#define INA230_REG_VSHUNT_SIGN          INA230_REG_VSHUNT_SIGN_Msk              /* Sign */

/*******************  Bit definition for Bus Voltage register  ****************/
#define INA230_REG_VBUS_DATA_Pos        (0U)
#define INA230_REG_VBUS_DATA_Msk        (0x7FFFU << INA230_REG_VBUS_DATA_Pos)   /* 0x7FFF */
#define INA230_REG_VBUS_DATA            INA230_REG_VBUS_DATA_Msk                /*!< Measured bus voltage */

/*********************  Bit definition for Power register  ********************/
#define INA230_REG_PWR_DATA_Pos         (0U)
#define INA230_REG_PWR_DATA_Msk         (0xFFFFU << INA230_REG_PWR_DATA_Pos)    /* 0xFFFF */
#define INA230_REG_PWR_DATA             INA230_REG_PWR_DATA_Msk                 /*!< Measured power */

/*****************  Bit definition for Current register  **********************/
#define INA230_REG_CURRENT_DATA_Pos     (0U)
#define INA230_REG_CURRENT_DATA_Msk     (0x7FFFU << INA230_REG_CURRENT_DATA_Pos) /* 0x7FFF */
#define INA230_REG_CURRENT_DATA         INA230_REG_CURRENT_DATA_Msk              /*!< Measured current */
#define INA230_REG_CURRENT_SIGN_Pos     (15U)
#define INA230_REG_CURRENT_SIGN_Msk     (0x1U << INA230_REG_CURRENT_SIGN_Pos)   /* 0x8000 */
#define INA230_REG_CURRENT_SIGN         INA230_REG_CURRENT_SIGN_Msk             /* Sign */

/*******************  Bit definition for Calibration register  ****************/
#define INA230_REG_CALIBRATION_DATA_Pos (0U)
#define INA230_REG_CALIBRATION_DATA_Msk (0x7FFFU << INA230_REG_CALIBRATION_DATA_Pos) /* 0x7FFF */
#define INA230_REG_CALIBRATION_DATA     INA230_REG_CALIBRATION_DATA_Msk              /* !< calibration value */

/*******************  Bit definition for Mask/Enable register  ****************/
#define INA230_REG_MASK_ENABLE_SOL_Pos  (15U)
#define INA230_REG_MASK_ENABLE_SOL_Msk  (0x1U << INA230_REG_MASK_ENABLE_SOL_Pos)  /* 0x8000 */
#define INA230_REG_MASK_ENABLE_SOL      INA230_REG_MASK_ENABLE_SOL_Msk            /*!< Shunt Voltage Over-Voltage */
#define INA230_REG_MASK_ENABLE_SUL_Pos  (14U)
#define INA230_REG_MASK_ENABLE_SUL_Msk  (0x1U << INA230_REG_MASK_ENABLE_SUL_Pos)  /* 0x4000 */
#define INA230_REG_MASK_ENABLE_SUL      INA230_REG_MASK_ENABLE_SUL_Msk            /*!< Shunt Voltage Under-Voltage */
#define INA230_REG_MASK_ENABLE_BOL_Pos  (13U)
#define INA230_REG_MASK_ENABLE_BOL_Msk  (0x1U << INA230_REG_MASK_ENABLE_BOL_Pos)  /* 0x2000 */
#define INA230_REG_MASK_ENABLE_BOL      INA230_REG_MASK_ENABLE_BOL_Msk            /*!< Bus Voltage Over-Voltage */
#define INA230_REG_MASK_ENABLE_BUL_Pos  (12U)
#define INA230_REG_MASK_ENABLE_BUL_Msk  (0x1U << INA230_REG_MASK_ENABLE_BUL_Pos)  /* 0x1000 */
#define INA230_REG_MASK_ENABLE_BUL      INA230_REG_MASK_ENABLE_BUL_Msk            /*!< Bus Voltage Under-Voltage */
#define INA230_REG_MASK_ENABLE_POL_Pos  (11U)
#define INA230_REG_MASK_ENABLE_POL_Msk  (0x1U << INA230_REG_MASK_ENABLE_POL_Pos)  /* 0x0800 */
#define INA230_REG_MASK_ENABLE_POL      INA230_REG_MASK_ENABLE_POL_Msk            /*!< Over-Limit Power */
#define INA230_REG_MASK_ENABLE_CNVR_Pos (10U)
#define INA230_REG_MASK_ENABLE_CNVR_Msk (0x1U << INA230_REG_MASK_ENABLE_CNVR_Pos) /* 0x0400 */
#define INA230_REG_MASK_ENABLE_CNVR     INA230_REG_MASK_ENABLE_CNVR_Msk           /*!< Conversion Ready */
#define INA230_REG_MASK_ENABLE_AFF_Pos  (4U)
#define INA230_REG_MASK_ENABLE_AFF_Msk  (0x1U << INA230_REG_MASK_ENABLE_AFF_Pos)  /* 0x0001 */
#define INA230_REG_MASK_ENABLE_AFF      INA230_REG_MASK_ENABLE_AFF_Msk            /*!< Alert Function Flag */
#define INA230_REG_MASK_ENABLE_CVRF_Pos (3U)
#define INA230_REG_MASK_ENABLE_CVRF_Msk (0x1U << INA230_REG_MASK_ENABLE_CVRF_Pos) /* 0x0008 */
#define INA230_REG_MASK_ENABLE_CVRF     INA230_REG_MASK_ENABLE_CVRF_Msk           /*!< Conversion Ready Flag */
#define INA230_REG_MASK_ENABLE_OVF_Pos  (2U)
#define INA230_REG_MASK_ENABLE_OVF_Msk  (0x1U << INA230_REG_MASK_ENABLE_OVF_Pos)  /* 0x0004 */
#define INA230_REG_MASK_ENABLE_OVF      INA230_REG_MASK_ENABLE_OVF_Msk            /*!< Math Overflow Flag */
#define INA230_REG_MASK_ENABLE_APOL_Pos (1U)
#define INA230_REG_MASK_ENABLE_APOL_Msk (0x1U << INA230_REG_MASK_ENABLE_APOL_Pos) /* 0x0002 */
#define INA230_REG_MASK_ENABLE_APOL     INA230_REG_MASK_ENABLE_APOL_Msk           /*!< Alert Polarity */
#define INA230_REG_MASK_ENABLE_LEN_Pos  (0U)
#define INA230_REG_MASK_ENABLE_LEN_Msk  (0x1U << INA230_REG_MASK_ENABLE_LEN_Pos)  /* 0x0001 */
#define INA230_REG_MASK_ENABLE_LEN      INA230_REG_MASK_ENABLE_LEN_Msk            /*!< Alert Latch Enable */
#define INA230_REG_MASK_ENABLE_Func_Msk (0xfc00U)

/*******************  Bit definition for Alert Limit register  ****************/
#define INA230_REG_ALERT_LIMIT_DATA_Pos (0U)
#define INA230_REG_ALERT_LIMIT_DATA_Msk (0xFFFFU << INA230_REG_ALERT_LIMIT_DATA_Pos) /* 0xFFFF */
#define INA230_REG_ALERT_LIMIT_DATA     INA230_REG_ALERT_LIMIT_DATA_Msk              /*!< Alert Limit */

/******************************* Operating mode *******************************/
#define INA230_MODE_POWER_DOWN       (uint16_t)0x0000U                                 /*!< Turn off current into the INA230 inputs               */
#define INA230_MODE_TRIGGERED_VSHUNT INA230_REG_CFG_MODE_0                             /*!< Shunt Voltage, Single shot conversion                 */
#define INA230_MODE_TRIGGERED_VBUS   INA230_REG_CFG_MODE_1                             /*!< Bus voltage, Single shot conversion                   */
#define INA230_MODE_TRIGGERED_ALL    (INA230_REG_CFG_MODE_1 | INA230_REG_CFG_MODE_0)   /*!< Shunt Voltage and Bus voltage, Single shot conversion */
#define INA230_MODE_CONTINOUS_VSHUNT (INA230_REG_CFG_MODE_2 | INA230_REG_CFG_MODE_0)   /*!< Shunt Voltage, Continuous conversion                  */
#define INA230_MODE_CONTINOUS_VBUS   (INA230_REG_CFG_MODE_2 | INA230_REG_CFG_MODE_1)   /*!< Bus voltage, Continuous conversion                    */
#define INA230_MODE_CONTINOUS_ALL    INA230_REG_CFG_MODE

/*************************** Shunt conversion time ****************************/
#define INA230_SHUNT_ADC_CT_140     (uint16_t)0x0000U                                       /*!< ADC conversion time: 140 us  */
#define INA230_SHUNT_ADC_CT_204     INA230_REG_CFG_VSHUNTCT_0                               /*!< ADC conversion time: 204 us  */
#define INA230_SHUNT_ADC_CT_332     INA230_REG_CFG_VSHUNTCT_1                               /*!< ADC conversion time: 332 us  */
#define INA230_SHUNT_ADC_CT_588     (INA230_REG_CFG_VSHUNTCT_1 | INA230_REG_CFG_VSHUNTCT_0) /*!< ADC conversion time: 588 us  */
#define INA230_SHUNT_ADC_CT_1100    INA230_REG_CFG_VSHUNTCT_2                               /*!< ADC conversion time: 1100 us */
#define INA230_SHUNT_ADC_CT_2116    (INA230_REG_CFG_VSHUNTCT_2| INA230_REG_CFG_VSHUNTCT_0)  /*!< ADC conversion time: 2116 us */
#define INA230_SHUNT_ADC_CT_4156    (INA230_REG_CFG_VSHUNTCT_2| INA230_REG_CFG_VSHUNTCT_1)  /*!< ADC conversion time: 4156 us */
#define INA230_SHUNT_ADC_CT_8244    INA230_REG_CFG_VSHUNTCT                                 /*!< ADC conversion time: 8244 us */

/**************************** VBus conversion time ****************************/
#define INA230_VBUS_ADC_CT_140     (uint16_t)0x0000U                                       /*!< ADC conversion time: 140 us  */
#define INA230_VBUS_ADC_CT_204     INA230_REG_CFG_VBUSCT_0                                 /*!< ADC conversion time: 204 us  */
#define INA230_VBUS_ADC_CT_332     INA230_REG_CFG_VBUSCT_1                                 /*!< ADC conversion time: 332 us  */
#define INA230_VBUS_ADC_CT_588     (INA230_REG_CFG_VBUSCT_1 | INA230_REG_CFG_VBUSCT_0)     /*!< ADC conversion time: 588 us  */
#define INA230_VBUS_ADC_CT_1100    INA230_REG_CFG_VBUSCT_2                                 /*!< ADC conversion time: 1100 us */
#define INA230_VBUS_ADC_CT_2116    (INA230_REG_CFG_VBUSCT_2| INA230_REG_CFG_VBUSCT_0)      /*!< ADC conversion time: 2116 us */
#define INA230_VBUS_ADC_CT_4156    (INA230_REG_CFG_VBUSCT_2| INA230_REG_CFG_VBUSCT_1)      /*!< ADC conversion time: 4156 us */
#define INA230_VBUS_ADC_CT_8244    INA230_REG_CFG_VBUSCT                                   /*!< ADC conversion time: 8244 us */

/***************************** Number of averages *****************************/
#define INA230_AVG_1      (uint16_t)0x0000U                              					/*!< Number of averages: 1    */
#define INA230_AVG_4      INA230_REG_CFG_AVG_0                               				/*!< Number of averages: 4    */
#define INA230_AVG_16     INA230_REG_CFG_AVG_1                               				/*!< Number of averages: 16   */
#define INA230_AVG_64     (INA230_REG_CFG_AVG_1 | INA230_REG_CFG_AVG_0)          			/*!< Number of averages: 64   */
#define INA230_AVG_128    INA230_REG_CFG_AVG_2                               				/*!< Number of averages: 128  */
#define INA230_AVG_256    (INA230_REG_CFG_AVG_2| INA230_REG_CFG_AVG_0)           			/*!< Number of averages: 256  */
#define INA230_AVG_512    (INA230_REG_CFG_AVG_2| INA230_REG_CFG_AVG_1)           			/*!< Number of averages: 512  */
#define INA230_AVG_1024   INA230_REG_CFG_AVG                                 				/*!< Number of averages: 1024 */

/*********************************** Flags ************************************/
#define INA230_AFF        INA230_REG_MASK_ENABLE_AFF                  /*!< Alert Function Flag   */
#define INA230_CVRF       INA230_REG_MASK_ENABLE_CVRF                 /*!< Conversion Ready Flag */
#define INA230_OVRF       INA230_REG_MASK_ENABLE_OVF                  /*!< Math Overflow Flag    */

/*********************************** alerts ***********************************/
#define INA230_ALERTFUNC_SOL       INA230_REG_MASK_ENABLE_SOL         /*!< Shunt Voltage Over Limit  */
#define INA230_ALERTFUNC_SUL       INA230_REG_MASK_ENABLE_SUL         /*!< Shunt Voltage Under Limit */
#define INA230_ALERTFUNC_BOL       INA230_REG_MASK_ENABLE_BOL         /*!< Bus Voltage Over Limit    */
#define INA230_ALERTFUNC_BUL       INA230_REG_MASK_ENABLE_BUL         /*!< Bus Voltage Under Limit   */
#define INA230_ALERTFUNC_POL       INA230_REG_MASK_ENABLE_POL         /*!< Power Over Limit          */



/******************************************************************************/
/********************************** Exports ***********************************/
/******************************************************************************/
typedef struct INA230{
	uint16_t address;
	uint16_t configReg;
	uint16_t calibrationReg;
	uint16_t current_LSB;
	uint16_t shuntResistor;
    void     (*i2c_write)(uint16_t, uint8_t, uint16_t); //addr, reg, pData
    uint16_t (*i2c_read )(uint16_t, uint8_t); //addr, reg
} INA230_t;

INA230_t INA230_init(uint16_t address, 			/*!< Address in the ic2 bus */
					 uint16_t current_LSB, 		/*!< resolution of the current resistor in uA (usually 1000 uA) */
					 uint16_t shuntResistor, 	/*!< shunt resistor value in milli Ohm */
					 uint16_t shuntConvertTime, /*!< ADC sampling time for the shunt resistor (datasheet table idx)*/
					 uint16_t busConvertTime,   /*!< ADC sampling time for the bus (datasheet table idx)*/
					 uint16_t averagingMode,    /*!< Number of measurements averaged together (datasheet table idx)*/
					 void (*i2c_write)(uint16_t, uint8_t, uint16_t), //addr, reg, pData
					 uint16_t (*i2c_read )(uint16_t, uint8_t) //addr, reg
					);

void         INA230_start(INA230_t target, unsigned opMode);
void         INA230_stop(INA230_t target);
int16_t      INA230_getCurrent(INA230_t target);
uint16_t     INA230_getPower(INA230_t target);
int16_t      INA230_getVoltageShunt(INA230_t target);
int16_t      INA230_getVoltageShunt_microvolt(INA230_t target);
uint16_t     INA230_getVoltageBus(INA230_t target);
uint16_t     INA230_getID(INA230_t target);
void         INA230_reset(INA230_t target);
uint16_t     INA230_getCalibration(INA230_t target);
void         INA230_setCalibration(INA230_t target, uint16_t calibration);
uint16_t     INA230_getConfiguration(INA230_t target);
void         INA230_setConfiguration(INA230_t target, uint16_t cfg);
uint16_t     INA230_ConversionReady(INA230_t target);
// Alarm functions
void		 INA230_setAlertFunction(INA230_t target, uint16_t mode);
uint16_t     INA230_getAlertFunction(INA230_t target);
uint16_t 	 INA230_getAlertFunctionFlag(INA230_t target);
void         INA230_getMathOverflowFlag(INA230_t target);
void         INA230_setAlertPolarity(INA230_t target, uint16_t polarity);
uint16_t     INA230_getAlertPolarity(INA230_t target);
void         INA230_setAlertLatch(INA230_t target, uint16_t latch);
uint16_t     INA230_getAlertLatch(INA230_t target);
void         INA230_setAlertLimit(INA230_t target, uint16_t limit);
int16_t      INA230_getAlertLimit(INA230_t target);

#endif /* INC_INA230_H_ */

/*
 * INA230.c
 *
 *	Driver for Texas Instruments INA230 Bidirectional Current/power Monitor with I2C interface
 *  Created on: Jan 8, 2026
 *      Author: luan
 */

#include "INA230.h"

/**
 * @brief Initializes the configuration of an INA230 sensor, and returns a handle
 * to use with the other functions on this driver.
 * 
 * @param address address of the device in the i2c bus. 
 * @param current_LSB resolution of the current register, in microamperes.
 * @param shuntResistor value of the shunt resistor, in milli ohmns. 
 * @param shuntConvertTime sampling time for the shunt measurement (in header as INA230_CT_*).
 * @param busConvertTime sampling time for the bus measurement (in header as INA230_CT_*).
 * @param averagingMode number of samples to be averaged for the measurement.
 * @param i2c_write function pointer to write to the i2c bus. parameters are address, register, data
 * @param i2c_read 
 * @return INA230_t 
 */
INA230_t INA230_init(uint16_t address, /* address in the ic2 bus*/
                     uint16_t current_LSB, /* uA (usually 1000 uA)*/
                     uint16_t shuntResistor, /* milli Ohm */
                     uint16_t shuntConvertTime,
                     uint16_t busConvertTime,
                     uint16_t averagingMode,
                     void (*i2c_write)(uint16_t, uint8_t, uint16_t), //addr, reg, pData
                     uint16_t (*i2c_read )(uint16_t, uint8_t) //addr, reg
                    ){
    uint32_t denominator = current_LSB*(shuntResistor*1000);
    uint16_t calibration = (uint16_t)(5120000/denominator); //using softeabi division should keep precision.

    INA230_t handle = {
        .address = address,
        .calibrationReg = calibration,
        .current_LSB = current_LSB,
        .shuntResistor = shuntResistor,
        .i2c_write = i2c_write,
        .i2c_read = i2c_read
    };

    //Reset all the registers to default values
    INA230_reset(handle);

    //configuration values
    uint16_t cfg = shuntConvertTime | busConvertTime | averagingMode;
//	cfg |= ( ( (uint16_t)shuntConvertTime << INA230_REG_CFG_VSHUNTCT_Pos ) & INA230_REG_CFG_VSHUNTCT_Msk );
//	cfg |= ( ( (uint16_t)busConvertTime   << INA230_REG_CFG_VBUSCT_Pos   ) & INA230_REG_CFG_VBUSCT_Msk   );
//	cfg |= ( ( (uint16_t)averagingMode    << INA230_REG_CFG_AVG_Pos      ) & INA230_REG_CFG_AVG_Msk      );

    i2c_write(address, INA230_REG_CONFIG, cfg);

    //write calibration
    i2c_write(address, INA230_REG_CALIBRATION, calibration);

    return handle;
}

/**
 * @brief Start measurement conversion and writing to internal registers.
 * 
 * @param target 
 * @param opMode INA230_MODE_* defines the operation mode.
 */
void INA230_start(INA230_t target, unsigned opMode){
    uint16_t config = target.i2c_read(target.address, INA230_REG_CONFIG);
    config |= INA230_REG_CFG_MODE & opMode;
    target.i2c_write(target.address, INA230_REG_CONFIG, config);
}

/**
 * @brief Puts the device in a low power mode, pausing all the conversions.
 * 
 * @param target 
 */
void INA230_stop(INA230_t target){
    uint16_t config = target.i2c_read(target.address, INA230_REG_CONFIG);
    config |= INA230_MODE_POWER_DOWN;
    target.i2c_read(target.address, INA230_REG_CONFIG);
}

 /**
  * @brief returns the conversion ready flag.
  * WARNING: This bit clears under the following conditions in single-shot mode
  * 1) Writing to the Configuration register (except for power-down or disable selections)
  * 2.) Reading the Mask/Enable register
  * @param target 
  * @return uint16_t 
  */
uint16_t INA230_ConversionReady(INA230_t target){
    uint16_t regMask = target.i2c_read(target.address, INA230_REG_MASK_ENABLE);
    return ((regMask & INA230_REG_MASK_ENABLE_CVRF_Msk)>>INA230_REG_MASK_ENABLE_CVRF_Pos);
}

 /**
  * @brief Return the Current register, scaled to milliamps
  * 
  * @param target 
  * @return int16_t 
  */
int16_t INA230_getCurrent(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_CURRENT) * target.current_LSB;
}

/**
 * @brief Returns the contents of the power register, scaled to milliwatts.
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t INA230_getPower(INA230_t target){
    uint16_t power = target.i2c_read(target.address, INA230_REG_PWR);
    //The Power register LSB is internally programmed to equal 25 times the programmed value of the Current_LSB
    //return (power*target.current_LSB*25);
    return (power*target.current_LSB)/40; //already scaled
}

/**
 * @brief Returns the contents of the VSHUNT register, scaled to microvolts
 * 
 * The scale of this register is smaller since the measured voltage across the shunt register tends to be quite small.
 * 
 * @param target 
 * @return int16_t 
 */
int16_t INA230_getVoltageShunt(INA230_t target){
    int16_t voltage = target.i2c_read(target.address, INA230_REG_VSHUNT);
    //Internally the shunt is scaled to LSB=2.5 uV
    return voltage*2.5;

}

 /**
  * @brief Returns the contents of the VBUS registers, scaled to mV
  * 
  * Measurement is done between IN- and GND pins.
  * 
  * @param target 
  * @return uint16_t 
  */
uint16_t INA230_getVoltageBus(INA230_t target){
    int16_t voltage = target.i2c_read(target.address, INA230_REG_VBUS);
    //converted = voltage*1.25f;
    //converted = voltage + (voltage / 4)
    return voltage*1.25f;
}

/**
 * @brief Returns the chip ID, usually 0x2260 for the INA230
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t INA230_getID(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_ID);
}

/**
 * @brief generates a system reset, all registers reset to default values
 * 
 * @param target 
 * @return uint16_t 
 */
void INA230_reset(INA230_t target){
    uint16_t cfg = INA230_getConfiguration(target);
    cfg |= INA230_REG_CFG_RESET;
    INA230_setConfiguration(target, cfg);
}

/**
 * @brief Returns the calibration value, used to scale VBUS and calculate CURRENT and POWER.
 * This value also encodes the resistance of the shunt resistor, and correlates it to the
 * voltage measure in the ADC. See datasheet for more details.
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t INA230_getCalibration(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_CALIBRATION);
}

/**
 * @brief Set the calibration register, used to scale VBUS and calculate CURRENT and POWER.
 * This value also encodes the resistance of the shunt resistor, and correlates it to the
 * voltage measure in the ADC. See datasheet for more details.
 * 
 * @param target 
 * @param calibration 
 */
void INA230_setCalibration(INA230_t target, uint16_t calibration){
    target.i2c_write(target.address, INA230_REG_CALIBRATION, calibration);
}

/**
 * @brief returns the configuration string of the target device.
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t INA230_getConfiguration(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_CONFIG);
}

/**
 * @brief Overwites the calibration string with a new one.
 * 
 * @param target 
 * @param cfg 
 */
void INA230_setConfiguration(INA230_t target, uint16_t cfg){
    target.i2c_write(target.address, INA230_REG_CONFIG, cfg);
}

/******************************************************************************/
/****************************** Alarm functions *******************************/
/******************************************************************************/

/**
 * @brief Sets the condition to assert the alarm on the device.
 * 
 * @param target 
 * @param mode One of INA230_ALERTFUNC_*
 */
void INA230_setAlertFunction(INA230_t target, uint16_t mode){
    uint16_t regMaskEnable = target.i2c_read(target.address, INA230_REG_MASK_ENABLE);
    //zero region
    regMaskEnable &=  ~INA230_REG_MASK_ENABLE_Func_Msk;
    regMaskEnable |= mode & INA230_REG_MASK_ENABLE_Func_Msk;
    target.i2c_write(target.address, INA230_REG_MASK_ENABLE, regMaskEnable);
}

/**
 * @brief Returns the currently set alarm function.
 * 
 * warning: reading from this register automatically clears the conversion ready flag.
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t INA230_getAlertFunction(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_MASK_ENABLE) & INA230_REG_MASK_ENABLE_Func_Msk;
}

 /**
  * @brief Get flag for current alarm trigger.
  * 
  * Although only one alert function at a time can be monitored at the ALERT pin, the Conversion Ready bit (CNVR, bit
  * 10) can also be enabled to assert the ALERT pin. Reading the Alert Function Flag bit after an alert can help
  * determine if the alert function was the source of the alert.
  * When the Alert Latch Enable bit is set to Latch mode, the Alert Function Flag bit clears only when the Mask/Enable
  * register is read. When the Alert Latch Enable bit is set to Transparent mode, the Alert Function Flag bit is cleared
  * after the next conversion that does not result in an alert condition.
  * 
  * @param target 
  * @return uint16_t 
  */
uint16_t INA230_getAlertFunctionFlag(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_MASK_ENABLE) & INA230_AFF;
}

/**
 * @brief Return Math overflow flag. Mask is cleared after read.
 * 
 * @param target 
 */
void INA230_getMathOverflowFlag(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_MASK_ENABLE) & INA230_OVRF;
}

 /**
  * @brief Configures the latching feature of the ALERT pin and the flag bits.
  * INA230_REG_MASK_ENABLE_APOL = Inverted (active-high open collector)
  * 0 = Normal (active-low open collector) (default)
  * 
  * @param target 
  * @param polarity 
  */
void INA230_setAlertPolarity(INA230_t target, uint16_t polarity){
    uint16_t regMaskEnable = target.i2c_read(target.address, INA230_REG_MASK_ENABLE);
    //zero region
    regMaskEnable &=  ~INA230_REG_MASK_ENABLE_APOL_Msk;
    regMaskEnable |= polarity & INA230_REG_MASK_ENABLE_APOL_Msk;
    target.i2c_write(target.address, INA230_REG_MASK_ENABLE, regMaskEnable);
}

uint16_t INA230_getAlertPolarity(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_MASK_ENABLE) & INA230_REG_MASK_ENABLE_APOL_Msk;
}

/**
 * Configures the latching feature of the ALERT pin and flag bits.
 *  1 = Latch enabled
 *  0 = Transparent (default)
 *  When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
 *  when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
 *  remain active following a fault until the Mask/Enable register has been read.
 */
void INA230_setAlertLatch(INA230_t target, uint16_t latch){
    uint16_t regMaskEnable = target.i2c_read(target.address, INA230_REG_MASK_ENABLE);
    //zero region
    regMaskEnable &=  ~INA230_REG_MASK_ENABLE_LEN_Msk;
    regMaskEnable |= latch & INA230_REG_MASK_ENABLE_LEN_Msk;
    target.i2c_write(target.address, INA230_REG_MASK_ENABLE, regMaskEnable);
}

/**
 * @brief Returns current state of the latch pin
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t INA230_getAlertLatch(INA230_t target){
    return target.i2c_read(target.address, INA230_REG_MASK_ENABLE) & INA230_REG_MASK_ENABLE_LEN_Msk;
}

/**
 * @brief Set value limit for the alarm.
 * 
 * must be called after INA230_setAlertFunction for correct conversion from microvolts/millivolts/milliwats
 * into the raw register value (taking into account the variable LSB)
 * 
 * shunt under/over voltage is set in microvolts
 * bus under/over voltage is set in millivolts
 * power overlimit is set in milliwatts
 * 
 * @param target 
 * @param limit 
 */
void INA230_setAlertLimit(INA230_t target, uint16_t limit){
    uint16_t mode =  INA230_getAlertFunction(target);
    switch (mode){
    case INA230_ALERTFUNC_SOL:
    case INA230_ALERTFUNC_SUL:
        target.i2c_write(target.address, INA230_REG_ALERT_LIMIT, (int16_t)(limit/2.5)); //value in microvolts
        break;
    case INA230_ALERTFUNC_BOL:
    case INA230_ALERTFUNC_BUL:
        target.i2c_write(target.address, INA230_REG_ALERT_LIMIT, (int16_t)(limit/1.25)); //value in millivolts
        break;
    case INA230_ALERTFUNC_POL:
        target.i2c_write(target.address, INA230_REG_ALERT_LIMIT, (limit*40)/target.current_LSB); //value milliwatts
        break;
    default:
        return;
    }
    return;
}

/**
 * @brief Returns the alarm limit.
 * 
 * shunt under/over voltage is returned in microvolts
 * bus under/over voltage is returned in millivolts
 * power overlimit is returned in milliwatts
 * 
 * @param target 
 * @return int16_t 
 */
int16_t INA230_getAlertLimit(INA230_t target){
    uint16_t mode =  INA230_getAlertFunction(target);
    int16_t limit = target.i2c_read(target.address, INA230_REG_ALERT_LIMIT);;

    switch (mode){
    case INA230_ALERTFUNC_SOL:
    case INA230_ALERTFUNC_SUL:
        return (int16_t)(limit*2.5); //value in microvolts
        break;
    case INA230_ALERTFUNC_BOL:
    case INA230_ALERTFUNC_BUL:
        return (int16_t)(limit*1.25); //value in millivolts
        break;
    case INA230_ALERTFUNC_POL:
        return (int16_t)((limit*target.current_LSB)/40); //value milliwats
        break;
    default:
        return 0;
    }

}









/*
 * thermal.h
 *
 *	Functions to interface with the TMP468
 *  Created on: Jan 6, 2026
 *      Author: luan
 */


#include "main.h"
#include "util.h"
#include "TMP468.h"

/**
 * @brief Converts the register-scale temperature into milli Celsius
 * 
 * @param reg 
 * @return int32_t 
 */
int32_t regToMillis(int16_t reg){
  // Using division here since bitwise shift with signal extend is not 
  // a guaranteed behavior of C.
  return (int32_t)(reg / 8) * 62;
}

/**
 * @brief transform a temperature in milidegrees into the format expecified by the register.
 * Since the actual resolution of the device is TMP468_TEMP_RESOLUTION per bit, for data storage
 * it is prefered to either convert to degrees, or into the register format, which fits in a 16 bit
 * signed.
 * 
 * @param millis 
 * @return int16_t 
 */
int16_t millisToReg(int32_t millis){
    int16_t reg = millis / 62; //truncate if necessary
    return reg<<3;
}

/**
 * @brief input value is degreeCelsius * 2 to keep resolution of register.
 * output can be directly written to THERM Limit register
 * 
 * @param hlvcelsius temperature in half degree resolution in range 
 * @return uint16_t bitfield on correct format for i2c write
 */
uint16_t halfDegreesToThermLimitreg(int hlvcelsius){
    return hlvcelsius << TMP468_THERM_pos;
}

/**
 * @brief returned value is degreeCelsius * 2 to keep resolution of register.
 * 
 * @param thermlimit raw register contents of THERM limit
 * @return int Temperature in half-degrees
 */
int thermLimitRegtoHalfDegrees(uint16_t thermlimit){
    int hlvcelsius = (thermlimit & TMP468_THERM_mask) >> TMP468_THERM_pos;
    return (thermlimit&TMP468_THERM_SIGNAL_mask) ? -hlvcelsius : hlvcelsius;
}

/********************************** Reading ***********************************/

/**
 * @brief Returns temperature of channel idx in thousandths of degrees
 * 
 * @param target 
 * @param idx Remote sensor channel. Zero if using local sensor
 * @return uint32_t 
 */
int32_t TMP468_getRemoteTemperature(TMP468_t target, int idx){
    if (idx<0 || idx>8) return -1;
    return regToMillis(target.i2c_read(target.addr, TMP468_REG_LOCAL_TMP + idx)); 
    //This is only possible since the registers for the remote register are in adjacent address.
}

/**
 * @brief Returns status of the therm alerts
 * 
 * Returns a bitfield with a bit set when a channel has exceeded the THERM1 limit. 
 * Bit is reset when the temperature goes bellow THERM-Hysteresis.
 * Use the masks TMP468_REMOTE*_MASK and TMP468_LOCAL_MASK for checking each channel independently.
 * 
 * Warning: This is the inverse of the alert pin on the IC, which goes to a logical low when the sensor
 * detects overtemperature.
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t TMP468_getTHERM1Status(TMP468_t target){
        return target.i2c_read(target.addr, TMP468_REG_THERM1_STATUS); 
}


/**
 * @brief Returns status of the therm alerts
 * 
 * Returns a bitfield with a bit set when a channel has exceeded the THERM1 limit. 
 * Bit is reset when the temperature goes bellow THERM-Hysteresis.
 * Use the masks TMP468_REMOTE*_MASK and TMP468_LOCAL_MASK for checking each channel independently.
 * 
 * Warning: This is the inverse of the alert pin on the IC, which goes to a logical low when the sensor
 * detects overtemperature.
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t TMP468_getTHERM2Status(TMP468_t target){
    return target.i2c_read(target.addr, TMP468_REG_THERM2_STATUS); 
}

/**
 * @brief Check for connection state on the remote channels. 
 * 
 * Returns a bitfield with a bit set when an open circuit is detected. Use the masks
 * TMP468_REMOTE*_MASK and TMP468_LOCAL_MASK for checking each channel independently.
 * 
 * @param target 
 * @return uint16_t 0 if all circuits are closed, mask otherwise.
 */
uint16_t TMP468_getOpenStatus(TMP468_t target){
    return target.i2c_read(target.addr, TMP468_REG_REMOTE_CHANNEL_OPEN_STATUS); 
}

/**
 * @brief Busy status of the ADC in the TMP468
 * 
 * @param target 
 * @return bool
 */
int TMP468_isbusy(TMP468_t target){
    return target.i2c_read(target.addr, TMP468_REG_CONFIGURATION) & TMP468_CONFIG_BUSY;
}

/******************************* Configuration ********************************/

/**
 * @brief Initialization function for TMP468. Remote channels default to on, but are not configured.
 * 
 * @param addr address in the i2c bus
 * @param conversionRate One of TMP468_CONVERSION_RATE_*
 * @param hysteresis Histeresis value in degrees 
 * @param THERM1 Thermal alarm, in 0.5 degrees resolution
 * @param THERM2 Thermal alarm, in 0.5 degrees resolution
 * @return TMP468_t 
 */
TMP468_t TMP468_init(uint16_t addr, uint16_t conversionRate, uint16_t hysteresis, uint32_t THERM1, uint32_t THERM2, 
                     void (*i2c_write)(uint16_t, uint8_t, uint16_t), 
                     uint16_t (*i2c_read )(uint16_t, uint8_t) ){
    TMP468_t target = {
        .addr = addr,
        .hysteresis = hysteresis,
        .localTHERMLimit1 = THERM1,
        .localTHERMLimit2 = THERM2,
        .i2c_write = i2c_write,
        .i2c_read = i2c_read
    };

    uint16_t cfg = target.i2c_read(addr, TMP468_REG_CONFIGURATION);	
    cfg &= ~TMP468_CONFIG_CONVERSION_RATE_mask; //clears bits
    cfg |= conversionRate; //set bits to conversion rate
    target.i2c_write(addr, TMP468_REG_CONFIGURATION, cfg);

    target.i2c_write(target.addr, TMP468_REG_THERM_HYSTERESIS, hysteresis); 
    target.i2c_write(target.addr, TMP468_REG_LOCAL_THERM2_LIMIT, halfDegreesToThermLimitreg(THERM1));
    target.i2c_write(target.addr, TMP468_REG_LOCAL_THERM1_LIMIT, halfDegreesToThermLimitreg(THERM2));
    
    return target;
}

/**
 * @brief Enables the remote sensors.
 * 
 * @param target 
 * @param flags Multiple sensors can be comined using bitwise OR
 */
void TMP468_setRemoteEnabled(TMP468_t target, uint16_t flags){
    uint16_t cfg = target.i2c_read(target.addr, TMP468_REG_CONFIGURATION);
    //clears bits
    cfg &= ~TMP468_REMOTE_bitmask;
    cfg |= flags & TMP468_REMOTE_bitmask;
    target.i2c_write(target.addr, TMP468_REG_CONFIGURATION, cfg);
}

/**
 * @brief Return list of enabled remote sensors, as bit flags
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t TMP468_getRemoteEnabled(TMP468_t target){
    uint16_t cfg = target.i2c_read(target.addr, TMP468_REG_CONFIGURATION);
    cfg &= TMP468_REMOTE_bitmask;
    return cfg;
}

/**
 * @brief Enable log on the local channel.
 * 
 * @param target 
 */
void TMP468_enableLocal(TMP468_t target){
    uint16_t cfg = target.i2c_read(target.addr, TMP468_REG_CONFIGURATION);
    cfg |= TMP468_LOCAL_MASK; //sets bit to 1
    target.i2c_write(target.addr, TMP468_REG_CONFIGURATION, cfg);
}

/**
 * @brief Disable log on the local channel.
 * 
 * @param target 
 */
void TMP468_disableLocal(TMP468_t target){
    uint16_t cfg = target.i2c_read(target.addr, TMP468_REG_CONFIGURATION);	
    cfg &= ~TMP468_LOCAL_MASK; //sets bit to 1
    target.i2c_write(target.addr, TMP468_REG_CONFIGURATION, cfg);
}


/**
 * @brief Get value of Histeresis in degrees C
 * 
 * @param target 
 * @return int16_t 
 */
uint16_t TMP468_getHysteresis(TMP468_t target){
    return target.i2c_read(target.addr, TMP468_REG_THERM_HYSTERESIS) >> TMP468_HYSTERESIS_pos;
}

/**
 * @brief Sets hysteresis value in degrees C
 * 
 * @param target 
 * @param hysteresis Hysteresis temperature in degrees
 */
void TMP468_setHysteresis(TMP468_t target, uint16_t hysteresis){
    target.i2c_write(target.addr, TMP468_REG_THERM_HYSTERESIS, hysteresis << TMP468_HYSTERESIS_pos);
}

/** internal function, allows one to refer to the internal register as idx0 */
int16_t thermRegisterOffset(int idx, int thermSelect){
    if (idx < 0 || idx > 8 || thermSelect < 0 || thermSelect > 1) {
        return -1;
    }

    int16_t reg;
    if (idx==0){
        reg = TMP468_REG_LOCAL_THERM2_LIMIT + thermSelect;
    } else {
        reg = TMP468_REG_REMOTE_CONFIG_BASE_ADDR + \
              (TMP468_REG_REMOTE_CONFIG_REMOTE_REMOTE_OFFSET * idx) + \
              TMP468_REG_REMOTE_CONFIG_REMOTE_THERM1_OFFSET + \
              thermSelect;
    }
    return reg;
}

/**
 * @brief Returns the value of the THERM limit
 * 
 * @param target 
 * @param idx Measurement channel, with 0 being the local channel.
 * @param thermSelect TMP468_SELECT_THERM1 or TMP468_SELECT_THERM2
 * @return int16_t 
 */
int16_t TMP468_getThermlimit(TMP468_t target, int idx, int thermSelect){
    int16_t reg = thermRegisterOffset(idx, thermSelect);
    if (reg == -1){ 
        return -1;
    }
    else{
        return target.i2c_read(target.addr, reg);
    }
}

/**
 * @brief Sets the value of the THERM limit
 * 
 * @param target 
 * @param idx Measurement channel, with 0 being the local channel.
 * @param thermlimit 
 * @param thermSelect TMP468_SELECT_THERM1 or TMP468_SELECT_THERM2
 */
void TMP468_setThermLimit(TMP468_t target, int idx, int16_t thermlimit, int thermSelect){
    int16_t reg = thermRegisterOffset(idx, thermSelect);
    if (reg == -1) return;
    target.i2c_write(target.addr, reg, thermlimit);
}

/**
 * @brief Disable alarm on THERM
 * 
 * @param target 
 * @param idx Measurement channel, with 0 being the local channel.
 * @param thermSelect TMP468_SELECT_THERM1 or TMP468_SELECT_THERM2
 */
void TMP468_disableTherm(TMP468_t target, int idx, int thermSelect){
    TMP468_setThermLimit(target, idx, TMP468_THERM_DISABLE, thermSelect);
}

/**
 * @brief Configure the remote temperature sensor number IDX.
 * 
 * @param target Target Temperature sensor
 * @param idx Target remote sensor index (from 1 to 8)
 * @param offset Temperature offset to apply to measurement, in mili degrees [–128000, +127937]
 * @param eta ideality factor correction. Convert from float using TMP468_etatoi for compile time optimization.
 * @param THERM1 Temperature alarm 1, in mili degrees
 * @param THERM2 Temperature alarm 2, in mili degrees
 */
void TMP468_configRemote(TMP468_t *target, int idx, uint32_t offset, int8_t eta, uint32_t THERM1, uint32_t THERM2){
    //base address
    unsigned baseRegister = TMP468_REG_REMOTE_CONFIG_BASE_ADDR + (TMP468_REG_REMOTE_CONFIG_REMOTE_REMOTE_OFFSET * idx);
    unsigned offsetRegister = baseRegister + TMP468_REG_REMOTE_CONFIG_REMOTE_OFFSET_OFFSET;
    unsigned etaResister = baseRegister + TMP468_REG_REMOTE_CONFIG_REMOTE_ETA_OFFSET;
    unsigned THERM1Register = baseRegister + TMP468_REG_REMOTE_CONFIG_REMOTE_THERM1_OFFSET;
    unsigned THERM2Register = baseRegister + TMP468_REG_REMOTE_CONFIG_REMOTE_THERM2_OFFSET;

    target->i2c_write(target->addr, offsetRegister, millisToReg(offset));
    target->i2c_write(target->addr, etaResister, eta << TMP468_REG_REMOTE_IDEALITY_CORRECTION_pos);
    target->i2c_write(target->addr, THERM1Register, halfDegreesToThermLimitreg(THERM1/2000));
    target->i2c_write(target->addr, THERM2Register, halfDegreesToThermLimitreg(THERM2/2000));

    //set the configured remote as enabled
    uint16_t enabled = TMP468_getRemoteEnabled(*target);
    enabled |= TMP468_CONFIG_ENABLE_REMOTE_BASE << idx;
    TMP468_setRemoteEnabled(*target, enabled);
    
    #ifdef TMP468_REMOTE_CONFIG_STRUCT
    target->remote[idx-1].offset = offset;
    target->remote[idx-1].idealityCorrection = eta << TMP468_REG_REMOTE_IDEALITY_CORRECTION_pos;
    target->remote[idx-1].THERM1_limit = THERM1;
    target->remote[idx-1].THERM2_limit = THERM2;
    #endif //TMP468_REMOTE_CONFIG_STRUCT
}

/**
 * @brief enters low power mode
 * 
 * @param target 
 */
void TMP468_sleep(TMP468_t target){
    uint16_t cfg = target.i2c_read(target.addr, TMP468_REG_CONFIGURATION);
    cfg |= TMP468_CONFIG_SHUTDOWN_mask; // 1 = enables device shutdown
    target.i2c_write(target.addr, TMP468_REG_CONFIGURATION, cfg);
}


/**
 * @brief Enable temperature gathering according to configuration
 * 
 * @param target 
 */
void TMP468_wakeup(TMP468_t target){
    uint16_t cfg = target.i2c_read(target.addr, TMP468_REG_CONFIGURATION);
    cfg &= ~TMP468_CONFIG_SHUTDOWN_mask; //clear bit
    target.i2c_write(target.addr, TMP468_REG_CONFIGURATION, cfg);
}

/******************************** Lock control ********************************/

/**
 * @brief Write protect the internal registers.
 * 
 * @param target 
 */
void TMP468_lock(TMP468_t target){
    target.i2c_write(target.addr, TMP468_REG_LOCK, TMP468_DO_LOCK);
}

/**
 * @brief Disable write protection on internal registers
 * 
 * @param target 
 */
void TMP468_unlock(TMP468_t target){
    target.i2c_write(target.addr, TMP468_REG_LOCK, TMP468_DO_UNLOCK);
}

/**
 * @brief Returns current lock status
 * 
 * @param target 
 * @return uint16_t 
 */
uint16_t TMP468_getLock(TMP468_t target){
    return target.i2c_read(target.addr, TMP468_REG_LOCK);
}


#ifndef _MOTOR_CONTROLLER_CAN_H
#define _MOTOR_CONTROLLER_CAN_H

#include "stdint.h"

//Defines

#define MC_GEN5             0

#define TORQUE_MODE			0
#define SPEED_MODE			1

#define BUS_DISCHARGED 		0
#define BUS_CHARGED    		1

#define MC_COMMAND_READ 	0
#define MC_COMMAND_WRITE 	1

#define MC_COMMAND_MSG			 0x0C0
#define MC_PARAM_COMMAND_MSG	 0x0C1

#define MC_ENABLE_BYTE_4 		 0b11100101
#define MC_ENABLE_BYTE_5		 0b00000000

#define CAN_MC_RX_TEMP1_ID					0x0A0
#define CAN_MC_RX_TEMP2_ID					0x0A1
#define CAN_MC_RX_TEMP3_ID					0x0A2
#define CAN_MC_RX_ANALOG_INPUTS_VOLTAGE		0x0A3
#define CAN_MC_RX_DIGITAL_INPUT_STATUS		0x0A4
#define CAN_MC_RX_MOTOR_ID					0x0A5
#define CAN_MC_RX_CURRENT_ID				0x0A6
#define CAN_MC_RX_VOLT_ID					0x0A7
#define CAN_MC_RX_FAULT_ID					0x0AB
#define CAN_MC_RX_INTERNAL_VOLTAGES 		0x0A9
#define CAN_MC_RX_INTERNAL_STATES			0x0AA
#define CAN_MC_RX_TORQUE_TIMER_INFO			0x0AC
#define CAN_MC_RX_MODULATION_INDEX			0x0AD
#define CAN_MC_RX_FIRMWARE_INFO				0x0AE
#define CAN_MC_RX_DIAGNOSTIC_DATA			0x0AF
#define CAN_MC_RX_HIGHSPEED					0x0B0
#define CAN_MC_RX_TORQUE_CAPABILITY			0x0B1

#define PEAK_TORQUE			230	//Peak torque for EMRAX 228 motor

//Static Variables
static int16_t bus_voltage;
static int16_t bus_current;
static int16_t mc_currentA;
static int16_t mc_currentB;
static int16_t mc_currentC;

//Temp 1
static int16_t mc_igbtA_temp;
static int16_t mc_igbtB_temp;
static int16_t mc_igbtC_temp;

//Temp 2
static int16_t mc_controlboard_temp;
static int16_t mc_RTD_temp_1;
static int16_t mc_RTD_temp_2;
static int16_t mc_RTD_temp_3;

//Temp 3
static int16_t mc_coolant_temp;
static int16_t mc_hot_spot_temp;
static int16_t mc_motor_temp;
static int16_t mc_torque_shudder;

//Digital input status
static int8_t mc_forward_switch;
static int8_t mc_reverse_switch;
static int8_t mc_brake_switch;
static int8_t mc_REGEN_disable;
static int8_t mc_ignition_switch;
static int8_t mc_Start_switch;
static int8_t mc_valet_mode;
static int8_t mc_digital_input8;

//internal voltages
static int16_t mc_onehalf_volt_ref;
static int16_t mc_twohalf_volt_ref;
static int16_t mc_five_volt_ref;
static int16_t mc_twelve_volt_ref;

//internal states
static int8_t mc_VSM_state;
static int8_t mc_PWM_freq;
static int8_t mc_inverter_state;
static int8_t mc_relay_state;
static int8_t mc_inverter_run_mode;
static int8_t mc_self_sensing_assist_enable;
static int8_t mc_inverter_active_discharge_state;
static int8_t mc_inverter_command_mode;
static int8_t mc_rolling_counter_value;
static int8_t mc_inverter_enable_state;
static int8_t mc_burst_model_mode;
static int8_t mc_start_mode_active;
static int8_t mc_inverter_enable_lockout;
static int8_t mc_direction_command;
static int8_t mc_BMS_active;
static int8_t mc_BMS_limiting_torque;
static int8_t mc_limit_max_speed;
static int8_t mc_limit_hot_spot;
static int8_t mc_low_speed_limiting;
static int8_t mc_coolant_temperature_limiting;
static int8_t mc_limit_stall_burst_model;

//torque timer info
static int16_t mc_torque_command;
static int16_t mc_torque_feedback;
static int32_t mc_power_on_timer;

//modulation index & flux
static int16_t mc_modulation_index;
static int16_t mc_flux_weakening_output;
static int16_t mc_id_command;
static int16_t mc_iq_command;

//Motor position info
static int16_t mc_angle;
static int16_t mc_rpm;
static int16_t mc_Electrical_output_freq;
static int16_t mc_delta_resolver_filtered;

static int16_t mc_output_voltage;

static int16_t mc_vd;
static int16_t mc_vq;

static int8_t  mc_direction;
static int8_t  mc_enable_inverter;
static int8_t  mc_enable_discharge;
static int16_t mc_torque_limit;
static int16_t mc_torque;
static int16_t mc_speed;

//Firmware information
static int16_t mc_EEPROM_ver;
static int16_t mc_software_ver;
static int16_t mc_datecode_mmdd;
static int16_t mc_datecode_yyyy;

//torque capability
static int16_t mc_torque_capability;


//Getters
float mc_getBusVoltage();
float mc_getBusCurrent();
float mc_getIGBTACurrent();
float mc_getIGBTBCurrent();
float mc_getIGBTCCurrent();
float mc_getAverageIGBTTemp();
float mc_getIGBTATemp();
float mc_getIGBTBTemp();
float mc_getIGBTCTemp();
float mc_getMotorTemp();
int mc_getRPM();
float mc_getOutputVoltage();
float mc_getCommandedTorque();
float mc_getFeedbackTorque();
float mc_get_vd();
float mc_get_vq();

//Setters
void mc_set_torque_limit(int setTorque);
void mc_set_torque(int setTorque);
void mc_set_speed(int setSpeed);
void mc_set_direction(uint8_t setDirection);
void mc_set_inverter_enable(uint8_t setEnable);
void mc_set_inverter_discharge(uint8_t setEnable);
uint8_t isMCBusCharged();

//Can messaging processing
void mc_process_temp1_can(uint8_t * data);
void mc_process_temp2_can(uint8_t * data);
void mc_process_temp3_can(uint8_t * data);
void mc_process_analog_inputs_voltage_can(uint8_t * data);
void mc_process_digital_input_status_can(uint8_t * data);
void mc_process_motor_can(uint8_t * data);
void mc_process_current_can(uint8_t * data);
void mc_process_volt_can(uint8_t * data);
void mc_process_fault_can(uint8_t * data);
void mc_process_internal_volt_can(uint8_t * data);
void mc_process_internal_states_can(uint8_t * data);
void mc_process_torque_timer_info_can(uint8_t * data);
void mc_process_modulation_index_can(uint8_t * data);
void mc_process_firmware_info_can(uint8_t * data);
void mc_process_diagnostic_data_can(uint8_t * data);
void mc_process_fast_can(uint8_t * data);
void mc_process_torque_capability_can(uint8_t * data);

void sendTorque(int16_t);

// Motor Controller send message
void mc_send_command_msg(uint8_t mode);
void mc_send_param_command_message(uint8_t param_address, uint8_t RW, uint8_t * Data);

// Motor controller broadcasting messaging
void mc_enable_broadcast_msgs();
void mc_disable_broadcast_msgs();

//Motor controller modifying states
void UpdateMCState(int16_t mc_trottle_val);
void EnableMC();
void DisableMC();

//Clear faults in motor controller
void fixFaults();

//Enumerated Values for changing motor controller state
typedef enum{
    MC_DISABLED = 0,
    MC_ENABLED = 1
}mc_state_t;
#endif

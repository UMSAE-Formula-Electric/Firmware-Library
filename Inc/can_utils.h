/*
 * can_utils.h
 *
 * Created on: May 25, 2026
 * Author: cedric
 */

#ifndef INC_CAN_UTILS_H_
#define INC_CAN_UTILS_H_

/* Defines */
#include "main.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

#define CAN_MC_QUEUE_LENGTH 		64
#define CAN_AMS_QUEUE_LENGTH 		64
#define CAN_ACB_VCU_QUEUE_LENGTH 	64
#define CAN_QUEUE_LENGTH 			64

#define CAN_QUEUE_ITEM_SIZE sizeof( CanRxMsg )

#define CANTXTIMEOUT 			10 //number of milleseconds to wait for the can packet to send
#define CANTXMBTIMEOUT			10 //number of retires to get a mailbox on send

#define CAN_NO_RTR 				0
#define CAN_RTR 				1

#define CAN_SUB_Q_DELAY_MS 		100

// ==============================================================================
// INTERNAL NODE IDs
// ==============================================================================
#define CAN_ACU_CAN_ID			0x69
#define CAN_VCU_CAN_ID			0x88
#define CAN_SCU_CAN_ID			0x89
#define CAN_AMS_CAN_ID          0x70
#define CAN_VCU_LOG_ID			0x71

#define CAN_ACU_TO_VCU_ID           0x002 // Dec: 2
#define CAN_VCU_TO_ACU_ID           0x003 // Dec: 3
#define CAN_VCU_SET_ACB_STATE_ID	0x001 // Dec: 1

// ==============================================================================
// MOTOR CONTROLLER CAN IDs (Cascadia Motion)
// ==============================================================================

// TX: Messages sent TO the Motor Controller
#define MC_COMMAND_MSG			            0x0C0 // Dec: 192
#define MC_PARAM_COMMAND_MSG	            0x0C1 // Dec: 193

// Motor Controller EEPROM parameter addresses (used with mc_send_param_command_message)
#define CAN_MC_ACTIVE_MESSAGES              148   // Enable/disable broadcast messages (PM100 manual p.29)

// RX: Messages received FROM the Motor Controller
#define CAN_MC_RX_TEMP1_ID					0x0A0 // Dec: 160
#define CAN_MC_RX_TEMP2_ID					0x0A1 // Dec: 161
#define CAN_MC_RX_TEMP3_ID					0x0A2 // Dec: 162
#define CAN_MC_RX_ANALOG_INPUTS_VOLTAGE		0x0A3 // Dec: 163
#define CAN_MC_RX_DIGITAL_INPUT_STATUS		0x0A4 // Dec: 164
#define CAN_MC_RX_MOTOR_ID					0x0A5 // Dec: 165
#define CAN_MC_RX_CURRENT_ID				0x0A6 // Dec: 166
#define CAN_MC_RX_VOLT_ID					0x0A7 // Dec: 167
#define CAN_MC_RX_FLUX_ID					0x0A8 // Dec: 168
#define CAN_MC_RX_INTERNAL_VOLTAGES 		0x0A9 // Dec: 169
#define CAN_MC_RX_INTERNAL_STATES			0x0AA // Dec: 170
#define CAN_MC_RX_FAULT_ID					0x0AB // Dec: 171
#define CAN_MC_RX_TORQUE_TIMER_INFO			0x0AC // Dec: 172
#define CAN_MC_RX_MODULATION_INDEX			0x0AD // Dec: 173
#define CAN_MC_RX_FIRMWARE_INFO				0x0AE // Dec: 174
#define CAN_MC_RX_DIAGNOSTIC_DATA			0x0AF // Dec: 175
#define CAN_MC_RX_HIGHSPEED					0x0B0 // Dec: 176
#define CAN_MC_RX_TORQUE_CAPABILITY			0x0B1 // Dec: 177
#define CAN_MC_RX_READ_WRITE_PARAM			0x0C2 // Dec: 194
#define CAN_MC_RX_U2C_COMMAND				0x1D7 // Dec: 471
#define CAN_MC_RX_INV_DIAG_BUFFER			0x80000BA5 // Dec: 2147486373

// ==============================================================================
// EMUS BMS CAN IDs (Base ID + Offset Logic)
// ==============================================================================
#define CAN_BMS_BASE_ID         				0x100 // Dec: 256

#define CAN_BMS_OVERALL_ID		                (CAN_BMS_BASE_ID + 0) // Dec: 256
#define CAN_BMS_VOLTAGE_ID		                (CAN_BMS_BASE_ID + 1) // Dec: 257
#define CAN_BMS_MODULE_TEMPERATURE				(CAN_BMS_BASE_ID + 2) // Dec: 258
#define CAN_BMS_CELL_BALANCING_RATE				(CAN_BMS_BASE_ID + 3) // Dec: 259
#define CAN_BMS_STATE_OF_CHARGE					(CAN_BMS_BASE_ID + 5) // Dec: 261
#define CAN_BMS_ENERGY_PARAM					(CAN_BMS_BASE_ID + 6) // Dec: 262
#define CAN_BMS_DIAGNOSTIC_ID	                (CAN_BMS_BASE_ID + 7) // Dec: 263
#define CAN_BMS_CELL_TEMPERATURE				(CAN_BMS_BASE_ID + 8) // Dec: 264

/* TYPE-B CAN IDs */
#define CAN_BMS_INDIVIDUAL_CELL_VOLTAGES_TYPE_B			(CAN_BMS_BASE_ID + 17) // Dec: 273 (Fixed offset from 11)
#define CAN_BMS_INDIVIDUAL_CELL_MODULE_TEMPS_TYPE_B		(CAN_BMS_BASE_ID + 18) // Dec: 274 (Fixed offset from 12)
#define CAN_BMS_INDIVIDUAL_CELL_BALANCING_RATE_TYPE_B	(CAN_BMS_BASE_ID + 19) // Dec: 275 (Fixed offset from 13)
#define CAN_BMS_INDIVIDUAL_CELL_TEMPS_TYPE_B			(CAN_BMS_BASE_ID + 20) // Dec: 276 (Fixed offset from 14)

#define CAN_BMS_CONTACTOR_CONTROL 				(CAN_BMS_BASE_ID + 297) // Dec: 553 (Fixed offset from 129)
#define CAN_BMS_STATS							(CAN_BMS_BASE_ID + 306) // Dec: 562 (Fixed offset from 132)
#define CAN_BMS_EVENTS							(CAN_BMS_BASE_ID + 307) // Dec: 563 (Fixed offset from 133)

// ==============================================================================
// EMUS BMS CAN IDs (Standard - Extended variants)
// ==============================================================================
#define CAN_BMS_CURRENT_LIMIT_STD_ID            (CAN_BMS_BASE_ID + 258) // Dec: 514
#define CAN_BMS_CHARGER_CONTROL_STD_ID          (CAN_BMS_BASE_ID + 317) // Dec: 573
#define CAN_BMS_SOC_HEALTH_PARAM_STD_ID         (CAN_BMS_BASE_ID + 197) // Dec: 453
#define CAN_BMS_OVERALL_PARAMS2_STD_ID          (CAN_BMS_BASE_ID + 185) // Dec: 441
#define CAN_BMS_SERIAL_NUMBER_STD_ID            (CAN_BMS_BASE_ID + 421) // Dec: 677
#define CAN_BMS_FIRMWARE_VERSION_STD_ID         (CAN_BMS_BASE_ID + 405) // Dec: 661
#define CAN_BMS_BATTERY_VOLT_2_OVERALL_STD_ID   (CAN_BMS_BASE_ID + 190) // Dec: 446
#define CAN_BMS_INDIV_CELLS_VOLT_STD_A1_ID      (CAN_BMS_BASE_ID + 213) // Dec: 469
#define CAN_BMS_INDIV_CELL_MOD_TEMP_STD_A1_ID   (CAN_BMS_BASE_ID + 245) // Dec: 501
#define CAN_BMS_CONFIG_PARAMS_STD_ID            (CAN_BMS_BASE_ID + 309) // Dec: 565
#define CAN_BMS_INDIV_CELL_TEMP_STD_A1_ID       (CAN_BMS_BASE_ID + 437) // Dec: 693
#define CAN_BMS_INDIV_CELL_BAL_RATE_STD_A1_ID   (CAN_BMS_BASE_ID + 277) // Dec: 533
#define CAN_BMS_LOGIN_LEVEL_STD_ID              (CAN_BMS_BASE_ID + 311) // Dec: 567
#define CAN_BMS_INDIV_CELLS_VOLT_STD_A2_ID      (CAN_BMS_BASE_ID + 214) // Dec: 470
#define CAN_BMS_INDIV_CELL_BAL_RATE_STD_A2_ID   (CAN_BMS_BASE_ID + 278) // Dec: 534
#define CAN_BMS_INDIV_CELL_MOD_TEMP_STD_A2_ID   (CAN_BMS_BASE_ID + 246) // Dec: 502
#define CAN_BMS_INDIV_CELL_TEMP_STD_A2_ID       (CAN_BMS_BASE_ID + 438) // Dec: 694

#define CAN_BMS_NEURO1_BATTERY_DATA_ID          0x0521 // Dec: 1313
#define CAN_BMS_NEURO2_BATTERY_DATA_ID          0x0522 // Dec: 1314
#define CAN_BMS_NEURO3_BATTERY_DATA_ID          0x0523 // Dec: 1315
#define CAN_BMS_NEURO4_BIMDATA_ID               0x0530 // Dec: 1328

// BMS Extended Frame IDs
#define CAN_BMS_CHARGER_CONTROL_EXT_ID          0x99B52408 // Dec: 2578777096
#define CAN_BMS_SOC_HEALTH_PARAM_EXT_ID         0x99B52010 // Dec: 2578776080
#define CAN_BMS_RX_CHG_TRANS_MSGS_ID            0x98FEE0E5 // Dec: 2566869221
#define CAN_BMS_TX_CHG_RECEIVE_MSGS_ID          0x980605F4 // Dec: 2550588916
#define CAN_BMS_INDIV_CELL_BAL_EXT_EXTD_B_ID    0x99B5200D // Dec: 2578776077
#define CAN_BMS_INDIV_CELL_TEMP_EXT_B_ID        0x99B5200E // Dec: 2578776078
#define CAN_BMS_INDIV_CELL_MOD_TEMP_EXT_A1_ID   0x99B52200 // Dec: 2578776576
#define CAN_BMS_INDIV_CELLS_VOLT_EXT_B_ID       0x99B5200B // Dec: 2578776075
#define CAN_BMS_BATTERY_VOLT_1_OVERALL_EXT_ID   0x99B52001 // Dec: 2578776065
#define CAN_BMS_OVERALL_PARAMS2_EXT_ID          0x99B52004 // Dec: 2578776068
#define CAN_BMS_SERIAL_NUMBER_EXT_ID            0x99B52710 // Dec: 2578777872
#define CAN_BMS_FIRMWARE_VERSION_EXT_ID         0x99B52700 // Dec: 2578777856
#define CAN_BMS_ENERGY_PARAMS_2_EXT_ID          0x99B52006 // Dec: 2578776070
#define CAN_BMS_BATTERY_VOLT_2_OVERALL_EXT_ID   0x99B52009 // Dec: 2578776073
#define CAN_BMS_OVERALL_PARAMS_EXT_ID           0x99B52000 // Dec: 2578776064
#define CAN_BMS_DIAGNOSTIC_CODES_EXT_ID         0x99B52007 // Dec: 2578776071
#define CAN_BMS_CELL_MOD_TEMP_OVERALL_EXT_ID    0x99B52002 // Dec: 2578776066
#define CAN_BMS_CELL_TEMP_OVERALL_PARAMS_EXT_ID 0x99B52008 // Dec: 2578776072
#define CAN_BMS_CELL_BALANCING_OVERALL_EXT_ID   0x99B52003 // Dec: 2578776067
#define CAN_BMS_CONFIG_PARAMS_EXT_ID            0x99B52400 // Dec: 2578777088
#define CAN_BMS_LOGIN_LEVEL_EXT_ID              0x99B52402 // Dec: 2578777090
#define CAN_BMS_CONTACTOR_CONTROL_EXT_ID        0x99B52401 // Dec: 2578777089
#define CAN_BMS_ENERGY_PARAMS_EXT_ID            0x99B52600 // Dec: 2578777600
#define CAN_BMS_SOC_PARAMS_EXT_ID               0x99B52500 // Dec: 2578777344
#define CAN_BMS_STATISTICS_EXT_ID               0x99B52404 // Dec: 2578777092
#define CAN_BMS_EVENTS_EXT_ID                   0x99B52405 // Dec: 2578777093
#define CAN_BMS_INDIV_CELLS_VOLT_EXT_A1_ID      0x99B52100 // Dec: 2578776320
#define CAN_BMS_INDIV_CELL_MOD_TEMP_EXT_B_ID    0x99B5200C // Dec: 2578776076
#define CAN_BMS_INDIV_CELL_TEMP_EXT_A1_ID       0x99B52800 // Dec: 2578778112
#define CAN_BMS_INDIV_CELLS_VOLT_EXT_A2_ID      0x99B52101 // Dec: 2578776321
#define CAN_BMS_INDIV_CELL_BAL_EXT_EXTD_A2_ID   0x99B52301 // Dec: 2578776833
#define CAN_BMS_INDIV_CELL_BAL_EXT_EXTD_A1_ID   0x99B52300 // Dec: 2578776832
#define CAN_BMS_INDIV_CELL_MOD_TEMP_EXT_A2_ID   0x99B52201 // Dec: 2578776577
#define CAN_BMS_INDIV_CELL_TEMP_EXT_A2_ID       0x99B52801 // Dec: 2578778113

// ==============================================================================
// SENSOR CONTROL UNIT CAN IDs
// ==============================================================================
#define CAN_SCU_BRAKE_TEMPS_ID                 0x265 // Dec: 613
#define CAN_SCU_TWO_FOOT_RULE_ID               0x257 // Dec: 599
#define CAN_SCU_WARNING_0_ID                   0x249 // Dec: 585
#define CAN_SCU_WARNING_1_ID                   0x250 // Dec: 592
#define CAN_SCU_APPS_TO_TORQUE_REQUEST_ID      0x220 // Dec: 544
#define CAN_SCU_BRAKE_PRESSED_ID               0x219 // Dec: 537
#define CAN_SCU_APPS_PRESSED_ID                0x218 // Dec: 536
#define CAN_SCU_SAFETY_LOOP_STATUS_ID          0x217 // Dec: 535
#define CAN_SCU_FLOW_METER_ID                  0x256 // Dec: 598
#define CAN_SCU_COOLING_LOOP_TEMPERATURE_ID    0x255 // Dec: 597
#define CAN_SCU_SHOCK_POTENTIOMETERS_ID        0x254 // Dec: 596
#define CAN_SCU_LV_BATTERY_DATA_ID             0x253 // Dec: 595
#define CAN_SCU_WHEEL_SPEED_ID                 0x252 // Dec: 594
#define CAN_SCU_VEHICLE_SPEED_ID               0x251 // Dec: 593

// ==============================================================================
// ENERGY METER CAN IDs
// ==============================================================================
#define CAN_EM_TEAM_DATA_2_ID                  0x30E // Dec: 782
#define CAN_EM_TEAM_DATA_1_ID                  0x30D // Dec: 781
#define CAN_EM_TEMPERATURE_ID                  0x60D // Dec: 1549
#define CAN_EM_STATUS_ID                       0x40D // Dec: 1037
#define CAN_EM_MEASUREMENT_ID                  0x10D // Dec: 269

// ==============================================================================
// INERTIAL MEASUREMENT UNIT CAN IDs
// ==============================================================================
#define CAN_IMU_SLOPE_SENSOR_ID                0x98EFA980 // Dec: 2565876096
#define CAN_IMU_ANGULAR_RATE_ID                0x98EFAA80 // Dec: 2565876352
#define CAN_IMU_ACCELERATION_ID                0x98EFAD80 // Dec: 2565877120
#define CAN_IMU_MAGNETOMETER_ID                0x98FEEA80 // Dec: 2566875776

// ==============================================================================
// VEHICLE CONTROL UNIT CAN IDs
// ==============================================================================
#define CAN_VCU_PEDAL_PRESS_PERCENT_ID         0x0C3 // Dec: 195
#define CAN_VCU_LOGIN_SET_PASSWORD_ID          0x238 // Dec: 568
#define CAN_VCU_CHARGING_STATION_STD_ID        0x23C // Dec: 572
#define CAN_VCU_CHARGING_STATION_EXT_ID        0x99B52407 // Dec: 2578777095
#define CAN_VCU_LOGIN_SET_PWD_STATUS_EXT_ID    0x99B52403 // Dec: 2578777091

// ==============================================================================
// U2C / DELPHI DC/DC CONVERTER CAN IDs
// ==============================================================================
#define CAN_U2C_MESSAGE_RXD_ID                 0x0BC // Dec: 188

// ==============================================================================
// STEERING ANGLE SENSOR CAN IDs
// ==============================================================================
#define CAN_LWS_STANDARD_ID                    0x2B0 // Dec: 688
#define CAN_LWS_CONFIG_ID                      0x7C0 // Dec: 1984

#define CAN_EXT 				1
#define CAN_NO_EXT 				0

extern osMessageQueueId_t canRxPacketQueueHandle;
extern osMessageQueueId_t canTxPacketQueueHandle;
extern CAN_HandleTypeDef hcan1;

typedef struct {
    CAN_RxHeaderTypeDef rxPacketHeader;
    uint8_t rxPacketData[8];
} CAN_RxPacketTypeDef;

typedef struct {
    CAN_TxHeaderTypeDef txPacketHeader;
    uint8_t txPacketData[8];
} CAN_TxPacketTypeDef;

enum STARTUP_STATUS_NOTIFY_MSG{
    CAN_ACB_TSA_ACK = 0,
    CAN_ACB_TSA_NACK,
    CAN_ACB_RTD_ACK,
    CAN_ACB_RTD_NACK,
    CAN_GO_IDLE_REQ,  //Request to go idle
    CAN_NO_SAFETY_LOOP_SET,  //Message to VCU to indicate that the safety loop is open at the VCU. Used when the car is idle
    CAN_NO_SAFETY_LOOP_CLEAR,//Message to VCU to indicate that the safety loop is closed at the VCU. Used when the car is idle
    CAN_AIR_WELD_SET,
    CAN_HEARTBEAT_REQUEST,
    CAN_HEARTBEAT_RESPONSE,
    CAN_BATTERY_VOLTAGE_REQUEST,
    CAN_BATTERY_VOLTAGE_RESPONSE
};

/* End Defines */

/* Prototypes */
uint8_t sendCan(CAN_HandleTypeDef* hcan, uint8_t const * data, uint32_t length, uint32_t dest, uint32_t canRTR, uint8_t isExtended);
/* End Prototypes */
#endif /* INC_CAN_UTILS_H_ */

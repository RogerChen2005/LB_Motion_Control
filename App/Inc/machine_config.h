/* File: machine_config.h
 * Purpose: Holds lead screw, DM542 pulse, speed, and acceleration configuration.
 */
#ifndef MACHINE_CONFIG_H
#define MACHINE_CONFIG_H
/* Mechanical configuration: directly coupled 4 mm lead screw. */
#define LEAD_UM_PER_REV 4000U
#define PULSES_PER_REV 25600U /* Driver table: SW5-SW8 all OFF. */
#define MAX_SPEED_MM_MIN 1200U
#define DEFAULT_SPEED_MM_MIN 60U
#define JOG_SPEED_MM_MIN 1200U
#define ACCEL_MILLI_SPEED_PER_MS 480U /* 480 (mm/min)/s */
#define ENTER_STARTS_RUN 0
#endif

/* File: machine_config.h
 * Purpose: Holds lead screw, DM542 pulse, speed, and acceleration configuration.
 */
#ifndef MACHINE_CONFIG_H
#define MACHINE_CONFIG_H
/* Mechanical configuration: directly coupled 4 mm lead screw. */
#define LEAD_UM_PER_REV 4000U
#define PULSES_PER_REV 1600U /* DM542 SW5 OFF, SW6 OFF, SW7 ON, SW8 ON (1.8 deg motor). */
#define MAX_SPEED_MM_MIN 1200U
#define DEFAULT_SPEED_MM_MIN 60U
#define JOG_SPEED_MM_MIN 1200U
#define ACCEL_MILLI_SPEED_PER_MS 480U /* 480 (mm/min)/s */
#define ENTER_STARTS_RUN 0
#endif

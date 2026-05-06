#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

// Intitialize servo 
void servo_init(void);

// Move servo to position
void servo_open(void);

// Move servo to close position
void servo_close(void);

// Set servo angle (0- 180)
void servo_set_angle(uint8_t angle);

#endif
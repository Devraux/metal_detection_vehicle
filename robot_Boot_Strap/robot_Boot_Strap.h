#ifndef _ROBOT_BOOT_STRAP_
#define _ROBOT_BOOT_STRAP_

#include "../GPS/GPS.h"
#include "../metal_detection/metal_detection.h"
#include "../motion/motion.h"
#include "../motion/mpu6050.h"
#include "../pico_irq/pico_irq.h"
#include "../servo/servo.h"
#include "../UDP/UDP.h"


/// @brief robot all sensors and other code initialization
/// @param  --
void robot_Boot_Strap(void);

/// @brief period robot measurements 
/// @param timer Pi Pico necessary timer structure
/// @return true if measurements goes correct false otherwise
bool period_Robot_Measurements(struct repeating_timer *timer);

#endif
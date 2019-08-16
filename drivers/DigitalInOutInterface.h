/*
 * DigitalInOutInterface.h
 *
 *  Created on: 16.08.2019
 *      Author: tobias
 */

#ifndef MBED_DIGITALINOUTINTERFACE_H_
#define MBED_DIGITALINOUTINTERFACE_H_

#include "platform/platform.h"

namespace mbed {
/** \addtogroup drivers */

/** A digital input/output, used for setting or reading a bi-directional pin
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class DigitalInOutInterface {

public:
    /** Set the output, specified as 0 or 1 (int)
     *
     *  @param value An integer specifying the pin output value,
     *      0 for logical 0, 1 (or any other non-zero value) for logical 1
     */
	virtual void write(int value) = 0;

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    an integer representing the output setting of the pin if it is an output,
     *    or read the input if set as an input
     */
    virtual int read() {
        // Thread safe / atomic HAL call
        return 0;
    }

    /** Set as an output
     */
    virtual void output() = 0;

    /** Set as an input
     */
    virtual void input() = 0;

    /** Set the input pin mode
     *
     *  @param pull PullUp, PullDown, PullNone, OpenDrain
     */
    virtual void mode(PinMode pull) = 0;

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    Non zero value if pin is connected to uc GPIO
     *    0 if gpio object was initialized with NC
     */
    virtual int is_connected() {
        // Thread safe / atomic HAL call
        return 0;
    }
};

} // namespace mbed

#endif /* MBED_DIGITALINOUTINTERFACE_H_ */

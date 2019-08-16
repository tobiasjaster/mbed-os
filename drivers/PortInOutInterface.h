/*
 * PortInOutInterface.h
 *
 *  Created on: 16.08.2019
 *      Author: tobias
 */

#ifndef MBED_PORTINOUTINTERFACE_H_
#define MBED_PORTINOUTINTERFACE_H_

#include "platform/platform.h"

namespace mbed {
/** \addtogroup drivers */

/** A multiple pin digital in/out used to set/read multiple bi-directional pins
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class PortInOutInterface {
public:

    /** Write the value to the output port
     *
     *  @param value An integer specifying a bit to write for every corresponding port pin
     */
    virtual void write(int value) = 0;

    /** Read the value currently output on the port
     *
     *  @returns
     *    An integer with each bit corresponding to associated port pin setting
     */
    virtual int read() {
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
     *  @param mode PullUp, PullDown, PullNone, OpenDrain
     */
    virtual void mode(PinMode mode) = 0;
};

} // namespace mbed

#endif /* MBED_PORTINOUTINTERFACE_H_ */

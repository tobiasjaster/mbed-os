/*
 * PortInInterface.h
 *
 *  Created on: 16.08.2019
 *      Author: tobias
 */

#ifndef MBED_PORTININTERFACE_H_
#define MBED_PORTININTERFACE_H_

#include "platform/platform.h"

namespace mbed {
/** \addtogroup drivers */

/** A multiple pin digital input
 *
 * @note Synchronization level: Interrupt safe
 *
 *  Example:
 * @code
 * // Switch on an LED if any of mbed pins 21-26 is high
 *
 * #include "mbed.h"
 *
 * PortIn     p(Port2, 0x0000003F);   // p21-p26
 * DigitalOut ind(LED4);
 *
 * int main() {
 *     while(1) {
 *         int pins = p.read();
 *         if(pins) {
 *             ind = 1;
 *         } else {
 *             ind = 0;
 *         }
 *     }
 * }
 * @endcode
 * @ingroup drivers
 */
class PortInInterface {
public:

    /** Read the value currently output on the port
     *
     *  @returns
     *    An integer with each bit corresponding to associated port pin setting
     */
    virtual int read() {
        return 0;
    }

    /** Set the input pin mode
     *
     *  @param mode PullUp, PullDown, PullNone, OpenDrain
     */
    virtual void mode(PinMode mode) = 0;
};

} // namespace mbed

#endif /* MBED_PORTININTERFACE_H_ */

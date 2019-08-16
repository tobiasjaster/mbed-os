/*
 * PortOutInterface.h
 *
 *  Created on: 16.08.2019
 *      Author: tobias
 */

#ifndef MBED_PORTOUTINTERFACE_H_
#define MBED_PORTOUTINTERFACE_H_

#include "platform/platform.h"

namespace mbed {
/** \addtogroup drivers */
/** A multiple pin digital out
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Toggle all four LEDs
 *
 * #include "mbed.h"
 *
 * // LED1 = P1.18  LED2 = P1.20  LED3 = P1.21  LED4 = P1.23
 * #define LED_MASK 0x00B40000
 *
 * PortOut ledport(Port1, LED_MASK);
 *
 * int main() {
 *     while(1) {
 *         ledport = LED_MASK;
 *         wait(1);
 *         ledport = 0;
 *         wait(1);
 *     }
 * }
 * @endcode
 * @ingroup drivers
 */
class PortOutInterface {
public:

    /** Write the value to the output port
     *
     *  @param value An integer specifying a bit to write for every corresponding PortOut pin
     */
    virtual void write(int value) = 0;

    /** Read the value currently output on the port
     *
     *  @returns
     *    An integer with each bit corresponding to associated PortOut pin setting
     */
    virtual int read() {
        return 0;
    }
};

} // namespace mbed

#endif /* MBED_PORTOUTINTERFACE_H_ */

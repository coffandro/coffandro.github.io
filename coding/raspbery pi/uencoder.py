'''
uEncoder, a MicroPython library for reading rotary encoders
Copyright (C) 2018, Sean Lanigan

Based on C++ code originally by Ben Buxton, available on GitHub: 
https://github.com/buxtronix/arduino/tree/master/libraries/Rotary


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

from machine import Pin
from micropython import const

R_START =       const(0x00)
R_CW_FINAL =    const(0x01)
R_CW_BEGIN =    const(0x02)
R_CW_NEXT =     const(0x03)
R_CCW_BEGIN =   const(0x04)
R_CCW_FINAL =   const(0x05)
R_CCW_NEXT =    const(0x06)
R_MASK =        const(0x30)

# Values returned by 'process'
# No complete step yet.
DIR_NONE =      const(0x00)
# Clockwise step.
DIR_CW =        const(0x10)
# Counter-clockwise step.
DIR_CCW =       const(0x20)

ttable = \
(
    # R_START
    (R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START),
    # R_CW_FINAL
    (R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW),
    # R_CW_BEGIN
    (R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START),
    # R_CW_NEXT
    (R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START),
    # R_CCW_BEGIN
    (R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START),
    # R_CCW_FINAL
    (R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW),
    # R_CCW_NEXT
    (R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START)
)


class Encoder():
    def __init__(self, 
        pin_A, 
        pin_B, 
        callback, 
        use_interrupts = True):
        '''Encoder takes arguments of:
        pin_A, pin_B: Instance of Pin class, attached to the physical pins.
        callback: Function to call when a rotation is detected. Must take a 
        single argument, which will be DIR_CW or DIR_CCW.
        '''
        assert(isinstance(pin_A, Pin))
        assert(isinstance(pin_B, Pin))
        assert(callable(callback))
        self.pin_A = pin_A
        self.pin_B = pin_B
        self.state = R_START
        self.callback = callback
        if use_interrupts:
            mode = Pin.IRQ_RISING | Pin.IRQ_FALLING
            self.pin_A_irq = self.pin_A.irq(trigger=mode, handler=self.process)
            self.pin_B_irq = self.pin_B.irq(trigger=mode, handler=self.process)
        # TODO: Implement a polling-only method, with asyncio

    def process(self, line):
        '''Performs the polling operation, calls the callback indicating which 
        direction the encoder was turned
        '''
        # Read self.state into a local variable (should be faster)
        state = self.state
        # Read current pin states
        pinstate = ((self.pin_B.value() << 1) | self.pin_A.value())
        # Determine new state from the pins and state table.
        state = ttable[state & const(0x0f)][pinstate]
        # Return emit bits, ie the generated event.
        return_state = (state & R_MASK)
        # Store the updated state
        self.state = state
        # Call the callback, if it is defined
        if self.callback:
            self.callback(return_state)

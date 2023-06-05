/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Tomas Pecserke (github.com/tomas-pecserke)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <ctype.h>
#include <tusb.h>

#include "pico_tusb_reset_interface.h"
#include "usb_descriptors.h"

int main() {
    tusb_init();
    while (1) {
        tud_task();
    }
}

// Implementation of vendor interface callback
bool tud_vendor_control_xfer_cb(__unused uint8_t rhport, uint8_t stage, tusb_control_request_t const * request) {
    switch (request->wIndex) {
        case ITF_NUM_VENDOR_RESET:
            return reset_interface_cb(stage, request);
        // If you have more vendor interfaces, forward their calls here
    }

    return false;
}

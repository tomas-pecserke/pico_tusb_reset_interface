# Pico TinyUSB Reset Interface Library

[![CodeQL](https://github.com/tomas-pecserke/pico_tusb_reset_interface/actions/workflows/codeql.yml/badge.svg)](https://github.com/tomas-pecserke/pico_tusb_reset_interface/actions/workflows/codeql.yml)
[![Build example](https://github.com/tomas-pecserke/pico_tusb_reset_interface/actions/workflows/example.yml/badge.svg)](https://github.com/tomas-pecserke/pico_tusb_reset_interface/actions/workflows/example.yml)
[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)

Pico TinyUSB Reset Interface is a library for [Raspberry Pi Pico] and other [RP2040] based boards,
that makes it easy to implement a USB reset interface for custom [TinyUSB] setup.
This interface is compatible with default USB implementation provided by [Pico-SDK] in it's [`pico_stdio_usb`] module.
<!-- Hopefully coming soon 
It allows the user to use standard tools like [`picotool`] with custom USB setups.
-->

Reset interface is only supported in device USB mode.

:warning: **Warning:** :warning:
Never use this or stdio_usb reset interface in production.
It allows for arbitrary code execution on the device in bootloader mode.
As such it is only meant as development tool and is not secure for production use.

# Features

- Supports multiple vendor interfaces alongside the reset interface
- Provides reset interface description template for simple declaration
- Provides callback implementation for reset interface for simple integration

# Example

Example project with cmake support is provided [here](example).

# Dependencies

- Pico SDK
    - pico_stdlib
    - pico_stdio_usb
- TinyUSB (included in Pico SDK by default)
    - tinyusb_device

# Installation

1) Download or clone the library to `<project_root>/lib/pico_tusb_reset_interface` directory.
2) Include the library in your CMake configuration:
```cmake
# ...

set(TARGET_NAME "pico_tusb_reset_interface_example")

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include Pico TinyUSB Reset Interface Library
include(${CMAKE_CURRENT_SOURCE_DIR}/lib/pico_tusb_reset_interface/CMakeLists.txt)

# Pull in dependencies
target_link_libraries(${TARGET_NAME}
    pico_stdlib
    pico_stdio_usb
    pico_unique_id
    tinyusb_device
    pico_tusb_reset_interface
)

# ...
```

# Usage

These instructions assume knowledge of how to set up TinyUSB.
This is not a step-by-step manual.
For information about setting up TinyUSB stack please refer to [TinyUSB documentation].


First you need to declare a vendor interface in your TinyUSB config.
```c
// src/tusb_config.h

// ...

//------------- CLASS -------------//
#define CFG_TUD_CDC               0
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            1 // Here we declare single vendor interface

// VENDOR FIFO size of TX and RX
#define CFG_TUD_VENDOR_RX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)
#define CFG_TUD_VENDOR_TX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)
```

```c
// src/usb_descriptors.h

// Configuration Descriptor
enum {
    ITF_NUM_VENDOR_RESET = 0,
    // Other interfaces
    ITF_NUM_TOTAL
};

// String indices
enum {
    STR_IDX_CONFIG = 0,
    STR_IDX_MANUFACTURER,
    STR_IDX_PRODUCT,
    STR_IDX_SERIAL,
    // String for other interfaces
    STR_IDX_VENDOR_RESET
};
```

```c
// src/usb_descriptors.c

// ..

#include "pico_tusb_reset_interface.h"
#include "usb_descriptors.h"

// Include all interface definitions length
#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_RPI_RESET_DESC_LEN)

uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, STR_IDX_CONFIG, CONFIG_TOTAL_LEN, 0x00, 500),
    
    // Other interfaces
    
    // Reset VENDOR: Interface number, string index
    TUD_RPI_RESET_DESCRIPTOR(ITF_NUM_VENDOR_RESET, STR_IDX_VENDOR_RESET)
};

// String Descriptors
static char usbd_serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];
// array of pointer to string descriptors
char const *string_desc_arr[] = {
    [STR_IDX_CONFIG] = (const char[]) {0x09, 0x04}, // 0: is supported language is English (0x0409)
    [STR_IDX_MANUFACTURER] = "TinyUSB",   // 1: Manufacturer
    [STR_IDX_PRODUCT] = "TinyUSB Device", // 2: Product
    [STR_IDX_SERIAL] = usbd_serial_str,   // 3: Serials, should use chip ID
    [STR_IDX_VENDOR_RESET] = "Reset",     // Reset Interface
};

// ...
```

Then you can implement the vendor interface callback:

```c
// src/main.c

#include <ctype.h>
#include <tusb.h>
#include "pico_tusb_reset_interface.h"
#include "usb_descriptors.h"

int main() {
    // init your app here
    tusb_init();
    while (1) {
        tud_task();
        // call your code loop here
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
```

# Picotool support

[`picotool`] has a [pull request](https://github.com/raspberrypi/picotool/pull/83) in the works
and will hopefully soon support custom devices with compatible reset interface
with use of `--detect-reset-interface` or `-i` option.

<!-- Hopefully coming soon 
To upload firmware to your custom Pico board use this command.
Replace `program.uf2` with path to your actual compiled firmware.
```shell
picotool upload -x firmware.uf2 -f -i
```
-->

For more information see the [documentation](https://github.com/raspberrypi/picotool/#support-for-custom-stdio-usb-compatible-reset-interface).

# License

All sources are licensed under MIT license, the [Full license is here](LICENSE).
However, each file can be individually licensed especially those in `examples` folder.
Please make sure you understand all the license term for files you use in your project.

[Raspberry Pi Pico]: https://www.raspberrypi.com/products/raspberry-pi-pico/
[RP2040]: https://www.raspberrypi.com/products/rp2040/
[TinyUSB]: https://github.com/hathach/tinyusb
[Pico-SDK]: https://www.raspberrypi.com/documentation/pico-sdk/index_doxygen.html
[`pico_stdio_usb`]: https://www.raspberrypi.com/documentation/pico-sdk/runtime.html#pico_stdio_usb
[`picotool`]: https://github.com/raspberrypi/picotool
[TinyUSB documentation]: https://docs.tinyusb.org/en/latest/index.htmlľ

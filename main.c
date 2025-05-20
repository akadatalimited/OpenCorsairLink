/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2020  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "device.h"
#include "driver.h"
#include "logic/options.h"
#include "logic/scan.h"
#include "logic/print.h"
#include "logic/psu_status_output.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int
commanderpro_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings );

int
hydro_asetek_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings );

int
hydro_asetekpro_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings );

int
hydro_coolit_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings );

int
hydro_platinum_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings );

int
psu_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings );

int
main( int argc, char* argv[] )
{
    if (argc == 1) {
        options_print();
        return 0;
    }
    int rr; // result from libusb functions

    int8_t device_number = -1;

    int scanlist_count = 0;

    struct option_flags flags;
    struct option_parse_return settings;

    options_parse( argc, argv, &flags, &device_number, &settings );

    libusb_context* context = NULL;

    rr = libusb_init( &context );
    if ( rr < 0 )
    {
        msg_info( "Init Error %d\n", rr );
        return 1;
    }
    // rr = libusb_set_option( context, LIBUSB_OPTION_LOG_LEVEL, 2 );

    corsairlink_device_scanner( context, &scanlist_count, flags );
    msg_debug( "DEBUG: scan done, start routines\n" );
    msg_debug( "DEBUG: selected device_number = %d\n", device_number );

    if ( device_number >= 0 )
    {
        if ( device_number >= scanlist_count )
        {
            msg_info(
                "Detected %d device(s), submitted device %d is out of range\n", scanlist_count,
                device_number );
        }
        else
        {
            if ( scanlist[device_number].device->driver == &corsairlink_driver_rmi )
            {
                if (flags.show_status && !flags.show_json) {    
                // Just human-readable    
                    psu_status_print(scanlist[device_number]);
                } else if (flags.show_json && !flags.show_status) {    
                // Just JSON    
                    psu_status_json(scanlist[device_number]);
                } else if (flags.show_status && flags.show_json) {    
                // Both, but in a clean order    
                    psu_status_print(scanlist[device_number]);    
                    printf("\n");  // spacing    
                    psu_status_json(scanlist[device_number]);
                } else {    
                // Default full telemetry with vendor/product/firmware    
                    psu_settings(scanlist[device_number], flags, settings);
                }

            }
            else if ( scanlist[device_number].device->driver == &corsairlink_driver_commanderpro )
            {
                commanderpro_settings( scanlist[device_number], flags, settings );
            }
            else if ( scanlist[device_number].device->driver == &corsairlink_driver_asetek )
            {
                hydro_asetek_settings( scanlist[device_number], flags, settings );
            }
            else if ( scanlist[device_number].device->driver == &corsairlink_driver_asetekpro )
            {
                hydro_asetekpro_settings( scanlist[device_number], flags, settings );
            }
            else if ( scanlist[device_number].device->driver == &corsairlink_driver_coolit )
            {
                hydro_coolit_settings( scanlist[device_number], flags, settings );
            }
            else if ( scanlist[device_number].device->driver == &corsairlink_driver_platinum )
            {
                hydro_platinum_settings( scanlist[device_number], flags, settings );
            }
        }
    }

exit:
    corsairlink_close( context );
    return 0;
}

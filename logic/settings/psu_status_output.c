/*
 * PSU Status JSON and Text Output
 */

#include "device.h"
#include "driver.h"
#include <libusb.h>
#include <stdio.h>
#include <string.h>
#include "logic/scan.h"

void psu_status_print(struct corsair_device_scan scanned_device) {
    struct corsair_device_info* dev = scanned_device.device;
    struct libusb_device_handle* handle = scanned_device.handle;
    char name[32] = {0};
    uint32_t time = 0;
    double temp, volts, amps, watts;

    dev->lowlevel->init(handle, dev->write_endpoint);

    dev->driver->product(dev, handle, name, sizeof(name));
    printf("Device: %s\n", name);

    for (int i = 0; i < 2; i++) {
        dev->driver->temperature.read(dev, handle, i, &temp);
        printf("Temperature %d: %.2f C\n", i, temp);
    }

    dev->driver->psu_time.powered(dev, handle, &time);
    printf("Powered: %u (%dd %dh)\n", time, time / 86400, (time / 3600) % 24);

    dev->driver->psu_time.uptime(dev, handle, &time);
    printf("Uptime: %u (%dd %dh)\n", time, time / 86400, (time / 3600) % 24);

    dev->driver->power.supply_voltage(dev, handle, &volts);
    printf("Supply Voltage: %.2f V\n", volts);

    dev->driver->power.total_wattage(dev, handle, &watts);
    printf("Total Power: %.2f W\n", watts);

    for (int i = 0; i < 3; i++) {
        const char* label = (i == 0) ? "12V" : (i == 1) ? "5V" : "3.3V";
        dev->driver->power.sensor_select(dev, handle, i);
        dev->driver->power.voltage(dev, handle, i, &volts);
        dev->driver->power.amperage(dev, handle, i, &amps);
        dev->driver->power.wattage(dev, handle, i, &watts);
        printf("Output %s: %.2f V, %.2f A, %.2f W\n", label, volts, amps, watts);
    }

    dev->lowlevel->deinit(handle, dev->write_endpoint);
}


void psu_status_json(struct corsair_device_scan scanned_device) {
    struct corsair_device_info* dev = scanned_device.device;
    struct libusb_device_handle* handle = scanned_device.handle;
    char name[32] = {0};
    uint32_t time = 0;
    double temp, volts, amps, watts;

    dev->lowlevel->init(handle, dev->write_endpoint);

    dev->driver->product(dev, handle, name, sizeof(name));

    printf("{\n");
    printf("  \"device\": \"%s\",\n", name);

    printf("  \"temperatures\": [\n");
    for (int i = 0; i < 2; i++) {
        dev->driver->temperature.read(dev, handle, i, &temp);
        printf("    %.2f%s\n", temp, (i == 1) ? "" : ",");
    }
    printf("  ],\n");

    dev->driver->psu_time.powered(dev, handle, &time);
    printf("  \"powered_seconds\": %u,\n", time);

    dev->driver->psu_time.uptime(dev, handle, &time);
    printf("  \"uptime_seconds\": %u,\n", time);

    dev->driver->power.supply_voltage(dev, handle, &volts);
    printf("  \"supply_voltage\": %.2f,\n", volts);

    dev->driver->power.total_wattage(dev, handle, &watts);
    printf("  \"total_wattage\": %.2f,\n", watts);

    printf("  \"rails\": [\n");
    for (int i = 0; i < 3; i++) {
        dev->driver->power.sensor_select(dev, handle, i);
        dev->driver->power.voltage(dev, handle, i, &volts);
        dev->driver->power.amperage(dev, handle, i, &amps);
        dev->driver->power.wattage(dev, handle, i, &watts);

        const char* label = (i == 0) ? "12V" : (i == 1) ? "5V" : "3.3V";
        printf("    {\n");
        printf("      \"label\": \"%s\",\n", label);
        printf("      \"voltage\": %.2f,\n", volts);
        printf("      \"amperage\": %.2f,\n", amps);
        printf("      \"wattage\": %.2f\n", watts);
        printf("    }%s\n", (i < 2) ? "," : "");
    }
    printf("  ]\n");
    printf("}\n");

    dev->lowlevel->deinit(handle, dev->write_endpoint);
}
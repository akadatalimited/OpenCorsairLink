#ifndef _PSU_STATUS_OUTPUT_H
#define _PSU_STATUS_OUTPUT_H

	#include "driver.h"
	#include "logic/options.h"
	#include "device.h"

	void psu_status_print(struct corsair_device_scan scanned_device);


	void psu_status_json(struct corsair_device_scan scanned_device);

#endif

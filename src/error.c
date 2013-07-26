#include "atsha204.h"

const char *atsha_error_name(int err) {
	if (err == ATSHA_ERR_OK) {
		return "OK";
	} else if (err == ATSHA_ERR_MEMORY_ALLOCATION_ATSHA_ERROR) {
		return "Memory allocation ATSHA_ERRor. Some memory allocation failed.";
	} else if (err == ATSHA_ERR_COMMUNICATION) {
		return "Communication ATSHA_ERRor: is not possible to send packet to the device, receive packet from the device, or multiple times was delivered/received malformed packet.";
	} else if (err == ATSHA_ERR_WAKE_NOT_CONFIRMED) {
		return "Is not confirmed if device is wake up or not";
	} else {
		return "ATSHA_ERRor code is not in the list";
	}
}

#ifndef LIBATSHA204_H
#define LIBATSHA204_H

#include <stdlib.h>
#include <stdint.h>

/**********************************************************************
 ******** THIS FILE REPRESENTS PUBLIC INTERFACE OF LIBATSHA204 *********
***********************************************************************/

typedef struct {
	size_t bytes;
	unsigned char *data;
} big_int;

int dev_rev(uint32_t *revision);

static const int ATSHA_ERR_OK = 0;
static const int ATSHA_ERR_MEMORY_ALLOCATION_ATSHA_ERROR = 1;
static const int ATSHA_ERR_COMMUNICATION = 2;
static const int ATSHA_ERR_WAKE_NOT_CONFIRMED = 3;

static const int ATSHA_ERR_USBCMD_NOT_CONFIRMED = 6;

const char *atsha_error_name(int ATSHA_ERR);

#endif //LIBATSHA204_H

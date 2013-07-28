#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //close()
#include<sys/file.h> //open()
#include<fcntl.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#include<assert.h>

#include "atsha204.h"
#include "tools.h"
#include "configuration.h"
#include "atsha204consts.h"
#include "layer_usb.h"
#include "main.h"

extern atsha_configuration g_config;

int wake(int dev) {
	int status;
	int tries = TRY_SEND_RECV_ON_COMM_ERROR + 1; //+1 will be eliminated after first iteration
	unsigned char *answer = NULL;

	while (tries >= 0) {
		tries--;
////////////////////////////////////////////////////////////////////////
#ifdef USE_LAYER_USB
		status = usb_wake(dev, &answer);
#endif
////////////////////////////////////////////////////////////////////////
		if (status == ATSHA_ERR_OK) {
			//Check packet consistency and check wake confirmation
			bool packet_ok = check_packet(answer);
			if (!packet_ok || (answer[1] != ATSHA204_STATUS_WAKE_OK)) {
				free(answer);
				answer = NULL;
				if (!packet_ok && g_config.verbose) log_message("ERR: Wake: CRC doesn't match.");
				status = ATSHA_ERR_COMMUNICATION;
				usleep(TRY_SEND_RECV_ON_COMM_ERROR_TOUT);
				continue;
			}

			break;
		}
	}

	free(answer);
	return status;
}

int idle(int dev) {
	int status;
	int tries = TRY_SEND_RECV_ON_COMM_ERROR;

	while (true) {
#ifdef USE_LAYER_USB
		status = usb_idle(dev);
#endif
		if (status == ATSHA_ERR_OK) return status;
		if (tries < 0) return status;
	}
}

int command(int dev, unsigned char *raw_packet, unsigned char **answer) {
	int status;
	int tries = TRY_SEND_RECV_ON_COMM_ERROR + 1; //+1 will be eliminated after first iteration

	while (tries >= 0) {
		tries--;
////////////////////////////////////////////////////////////////////////
#ifdef USE_LAYER_USB
		status = usb_command(dev, raw_packet, answer);
#endif
////////////////////////////////////////////////////////////////////////
		if (status == ATSHA_ERR_OK) {
			//Check packet consistency and status code
			if (!check_packet(*answer)) {
				free(*answer);
				*answer = NULL;
				if (g_config.verbose) log_message("ERR: Command: CRC doesn't match.");
				status = ATSHA_ERR_COMMUNICATION;
				usleep(TRY_SEND_RECV_ON_COMM_ERROR_TOUT);
				continue;
			}

			if ((*answer)[0] == 4) { //Messages with length 4 are always status codes
				unsigned char atsha204_status = (*answer)[1];
				bool go_trough = true;
				if (atsha204_status == ATSHA204_STATUS_PARSE_ERROR) {
					if (g_config.verbose) log_message("ERR: Bad status: Parse error.");
					go_trough = false;
				} else if (atsha204_status == ATSHA204_STATUS_EXEC_ERROR) {
					if (g_config.verbose) log_message("ERR: Bad status: Execution error.");
					go_trough = false;
				} else if (atsha204_status == ATSHA204_STATUS_COMMUNICATION_ERROR) {
					if (g_config.verbose) log_message("ERR: Bad status: Communication error.");
					go_trough = false;
				} //The rest of status codes are distributed

				if (!go_trough) {
					free(*answer);
					*answer = NULL;
					status = ATSHA_ERR_BAD_COMMUNICATION_STATUS;
					usleep(TRY_SEND_RECV_ON_COMM_ERROR_TOUT);
					continue;
				}
			}

			break;
		}
	}

	return status;
}

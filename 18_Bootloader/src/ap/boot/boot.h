/*
 * boot.h
 *
 *  Created on: 2020. 12. 27.
 *      Author: MCES
 */

#ifndef SRC_AP_BOOT_BOOT_H_
#define SRC_AP_BOOT_BOOT_H_



#endif /* SRC_AP_BOOT_BOOT_H_ */

#include "hw.h"
#include "common/cmd/cmd.h"
#include "core/flash.h"


void bootInit(void);
void bootProcessCmd(cmd_t *p_cmd);
void bootJumpToFw(void);
bool bootCheckFw(void);

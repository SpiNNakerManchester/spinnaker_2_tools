#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "attributes.h"
#include "qpe-types.h"

extern uint8_t _MYCHIPID;
extern uint8_t _MYPEID;

//! @brief Get chipID where the code is running
__static_inline uint8_t getMyChipID ()
{
	return _MYCHIPID;
}

//! @brief Getting the Module of ID where the code is running
__static_inline uint8_t getMyPEID ()
{
	return _MYPEID;
}

//! @brief Get chipID where the code is running
__static_inline uint8_t _getMyChipID ()
{
	return COMMS->ID_field.MyChipID;
}

//! @brief Getting the Module of ID where the code is running
__static_inline uint8_t _getMyPEID ()
{
	return COMMS->ID_field.MyPEID;
}

//! @brief Enable/Disable buffered write
__static_inline void set_buffered_write (bool en)
{
    COMMS->GCTL_field.buffered_write = (en ? 1 : 0);
}

#define TO_NOC_ADDR(x) local_addr_to_noc_addr ((void *)(x))
__static_inline uintptr_t local_addr_to_noc_addr (void *ptr)
{
    return ((uintptr_t)ptr) | (QUADPE_NOC_SRAM_BASE | (getMyPEID () << 17));
}

#ifdef __cplusplus
}
#endif

#endif /* __COMMUNICATION_H__ */


#include <spin2_api.h>
#include <spin2_api_params.h>

void spin2_callback_on(uint32_t event_id, callback_t cback, int priority){

    callback[event_id].cback = cback;
    callback[event_id].priority = priority;
}

void spin2_callback_off(uint32_t event_id) {

    callback[event_id].cback = NULL;
}

uint32_t spin2_schedule_callback(callback_t cback, uint32_t arg0, uint32_t arg1, int priority);


void spin2_dma_transfer(uint32_t tag, void *system_address, void *tcm_address, uint32_t direction, uint32_t length);

void spin2_memcpy(void *dst, const void *src, uint32_t len) {

    char *d = (char *) dst;
    const char *s = (const char *) src;

    while(len--)
        *d++ = *s++;
}

static void deschedule(uint event_id) {

}

static void dispatch(void) {

}

uint32_t spin2_send_packet(uint32_t key, uint32_t data, uint32_t TCR);


uint32_t  spin2_get_id(void);

uint32_t  spin2_get_core_id(void);

uint32_t  spin2_get_chip_id(void);
#include <sark2.h>


void *sark2_xalloc(heap_t *heap, uint size, uint tag, uint flag) {

    if (size == 0 || tag > 255) {
        return NULL;
    }

    uint app_id = (flag & ALLOC_ID) ? (flag >> 8) : sark_vec->app_id;
    uint entry = (app_id << 8) + tag;

    if (tag != 0 && sv->alloc_tag[entry] != NULL) {
        if (flag & ALLOC_TAG_RETRY) {
            block_t* block = sv->alloc_tag[entry] - 1;
            uint alloc_size =
                    ((uchar *) block->next - (uchar *) block) - sizeof(block_t);
            if (size == alloc_size) {
                return sv->alloc_tag[entry];
            }
        }
        return NULL;
    }

    // Word align and add overhead

    size = ((size + 3) & ~3) + sizeof(block_t);

    uint cpsr = 0;

    if (flag & ALLOC_LOCK) {
        cpsr = sark_lock_get(LOCK_HEAP);
    }

    block_t *prev_blk = NULL;
    block_t *free_blk = heap->free;

    while (free_blk != NULL) {
        block_t *next_blk = free_blk->next;
        block_t *new_blk = (block_t *) ((uchar *) free_blk + size);

        if (new_blk <= free_blk) {      // Request way too big?
            break;
        }

        if (new_blk > next_blk) {       // Free block too small
            prev_blk = free_blk;
            free_blk = free_blk->free;
            continue;
        }

        if (new_blk + 1 < next_blk) {   // too big - split but don't make zero-size frag.
            new_blk->next = next_blk;
            free_blk->next = new_blk;
            new_blk->free = free_blk->free;
            free_blk->free = new_blk;
        }

        if (prev_blk) {
            prev_blk->free = free_blk->free;
        } else {
            heap->free = free_blk->free;
        }

        heap->free_bytes -= size;

        if (flag & ALLOC_LOCK) {
            sark_lock_free(cpsr, LOCK_HEAP);
        }

        free_blk->free = (block_t *) (0xffff0000 + entry);

        if (tag != 0) {
            sv->alloc_tag[entry] = free_blk + 1;
        }

        return free_blk + 1;
    }

    if (flag & ALLOC_LOCK) {
        sark_lock_free(cpsr, LOCK_HEAP);
    }

    return NULL;
}

void *sark2_alloc(uint count, uint size) {

    // @TODO: define the local heap position and change it instead of NULL
    return sark_xalloc(NULL, count * size, 0, 0);
}

void rt_error(uint code, ...) {

    //@TODO: implement RTE state
}

void sark2_memcpy(void *dst, const void *src, uint32_t len) {

    char *d = (char *) dst;
    const char *s = (const char *) src;

    while(len--)
        *d++ = *s++;
}

// Enable FIQ and IRQ interrupts
void inline sark2_int_enable(void) {

    asm volatile ("cpsie if");
}

// Disable all interrupts
void inline sark2_int_disable(void) {

    asm volatile ("cpsid if");
}

// Disable IRQ interrupts
void inline sark2_irq_disable(void) {

    asm volatile ("cpsid i");
}

// Restore interrupts after spin2_irq_disable
void inline sark2_mode_restore(void) {

    asm volatile ("cpsie i")
}

void sark2_wfi(void) {

    asm volatile ("wfi");
}

void sark2_wfe(void) {

    asm volatile ("wfe");
}

uint32_t inline sark2_pe_id(void) {

    // Alternatively use getMyPEID()
    return (COMMS->ID_field.MyPEID) & 0x03;
}

uint32_t inline sark2_qpe_id(void) {

    // Alternatively use getMyPEID()
    return (COMMS->ID_field.MyPEID) >> 2;
}

uint32_t inline sark2_node_id(void) {

    // Alternatively use getMyPEID()r
    eturn COMMS->ID_field.MyPEID;
}

uint32_t inline sark2_chip_id(void) {

    // Alternatively use getMyChipID()
    return COMMS->ID_field.MyChipID;
}

uint32_t sark2_count_bits(uint word) {

    uint32_t count = 0;

    while (word != 0) {
        count++;
        word &= word - 1;
    }

    return count;
}

uint32_t sark2_str_len(char *string) {

    uint32_t count = 0;

    while (*string++) {
        count++;
    }

    return count;
}

void sark2_str_cpy(char *dest, const char* src) {

    char *d = dest;
    while ((*d++ = *src++) != '\0');
}

int sark2_str_cmp(const char* s1, const char* s2) {

    while(*s1 && (*s1==*s2)) {
        s1++,s2++;
    }

    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}



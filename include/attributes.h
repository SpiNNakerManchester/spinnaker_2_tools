
#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef DOXYGEN

//! @internal
//! These marcos are just for doxygen.
//! @defgroup unused_macros Doxygen macros only
//! @{
//! @internal

//! @brief Hide GNU GCC attributes
#define __attribute__(x)

//! @brief Hide static inline
#define __static_inline

#define _begin_config_word(name)                                                                                    \
    /*! @brief name config - see @ref RO_##name##_field for a detailed description of configuration options. */     \
    /*! The name configuration word contains the all the configuration Fields defined by @ref RO_##name##_field. \n
        Keep in mind that all the data in the struct itself is read-only\! For modification you have to set the
        adequate bits (via bitshifting) in the @ref name variable.

	    @htmlinclude  arm-ahb/ahb_##name##.html */                                                                  \
    RW_32Bit_Configuration_Word name;                                                                               \
    /*! @brief The datastructures of name##_field are read only. */                                                 \
	/*!	In order to modify them you have to write the @ref name Variable.
	
	    @htmlinclude  arm-ahb/ahb_##name##.html */                                                                  \
		struct RO_##name##_field {

#define _begin_config_word_RO(name)                                                                                 \
    /*! @brief name config - see @ref RO_##name##_field for a detailed description of configuration options. */     \
    /*! The name configuration word contains the all the configuration Fields defined by @ref RO_##name##_field.

	    @htmlinclude  arm-ahb/ahb_##name##.html */                                                                  \
    RO_32Bit_Configuration_Word name;                                                                               \
    /*! @brief The datastructures of name##_field are read only. */                                                 \
    /*! In order to modify them you have to write the @ref name Variable.

	    @htmlinclude  arm-ahb/ahb_##name##.html
    */                                                                                                              \
        struct RO_##name##_field {

#define _field(name,bitlen)      RO_##bitlen##bit  name : bitlen;
#define _pad(num) pad_of_##num##bit      : num;

#define _end_config_word(name)  };                                                                                  \
        /*! @brief For ease of use RO_Only Bitfield to access Bits of the @ref name variable. */                    \
        struct RO_##name##_field name##_field;

#define _end_config_word_RO(name)  };                                                                               \
        /*! @brief For ease of use RO_Only Bitfield to access Bits of the @ref name variable. */                    \
        struct RO_##name##_field name##_field;

#define _config_word(name)     RW_Word_of_32bit  name;
#define _config_word_RO(name)  RO_Word_of_32bit  name;
#define _config_float_RO(name) RO_float_of_32bit name;


//! @}

#else

#define __static_inline static __attribute__((always_inline,unused)) inline

#define _begin_config_word(name) \
    union {                      \
        volatile uint32_t name;  \
        volatile struct {
#define _begin_config_word_RO(name) _begin_config_word(name)
#define _field(name, bitlen)  volatile uint32_t  name : bitlen;
#define _pad(num)  volatile uint32_t : num; \

#define _end_config_word(name) } name##_field; };
#define _end_config_word_RO(name) _end_config_word(name)
#define _config_word(name)     volatile uint32_t name;
#define _config_word_RO(name)  volatile uint32_t name;
#define _config_float_RO(name) volatile float    name;

#ifdef __arm__
#define __attr_interrupt void __attribute__((interrupt))
#else
#define __attr_interrupt void
#endif

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#endif

#ifdef __cplusplus
}
#endif

#endif /* __ATTRIBUTES_H__ */


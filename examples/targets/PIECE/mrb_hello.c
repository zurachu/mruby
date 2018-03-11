/* dumped in little endian order.
   use `mrbc -E` option for big endian CPU. */
#include <stdint.h>
extern const uint8_t mrb_hello[];
const uint8_t
#if defined __GNUC__
__attribute__((aligned(4)))
#elif defined _MSC_VER
__declspec(align(4))
#endif
mrb_hello[] = {
0x45,0x54,0x49,0x52,0x30,0x30,0x30,0x34,0xf7,0x29,0x00,0x00,0x00,0x5f,0x4d,0x41,
0x54,0x5a,0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x00,0x2e,0x30,0x30,
0x30,0x30,0x00,0x00,0x00,0x26,0x00,0x02,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x03,
0x03,0x1c,0x58,0x01,0x01,0x80,0x80,0x00,0x4a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x4c,0x56,0x41,0x52,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x01,
0x00,0x01,0x61,0x00,0x00,0x00,0x01,0x45,0x4e,0x44,0x00,0x00,0x00,0x00,0x08,
};

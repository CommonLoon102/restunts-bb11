#ifndef RESTUNTS_TEMPLE_LEGACY
#define RESTUNTS_TEMPLE_LEGACY

// The game keeps DOS field widths; pointers and allocation sizes are native.
#define legacy_void U0
#define legacy_char I8
#define legacy_s8 I8
#define legacy_u8 U8
#define legacy_s16 I16
#define legacy_u16 U16
#define legacy_s32 I32
#define legacy_u32 U32
#define legacy_s64 I64
#define legacy_u64 U64
#define legacy_int I32
#define legacy_uint U32
#define legacy_f64 F64

#define TEMPLE_LOGICAL_WIDTH 320
#define TEMPLE_LOGICAL_HEIGHT 200
#define TEMPLE_SCREEN_WIDTH 640
#define TEMPLE_SCREEN_HEIGHT 480
#define TEMPLE_SCREEN_PIXELS (TEMPLE_SCREEN_WIDTH*TEMPLE_SCREEN_HEIGHT)
#define TEMPLE_RENDER_SCALE 2
#define TEMPLE_RENDER_WIDTH (TEMPLE_LOGICAL_WIDTH*TEMPLE_RENDER_SCALE)
#define TEMPLE_RENDER_HEIGHT (TEMPLE_LOGICAL_HEIGHT*TEMPLE_RENDER_SCALE)
#define TEMPLE_RENDER_LEFT ((TEMPLE_SCREEN_WIDTH-TEMPLE_RENDER_WIDTH)/2)
#define TEMPLE_RENDER_TOP ((TEMPLE_SCREEN_HEIGHT-TEMPLE_RENDER_HEIGHT)/2)
#define TEMPLE_BYTE_BITS 8
#define TEMPLE_BYTE_MASK 255
#define TEMPLE_WORD_MASK 65535
#define TEMPLE_WORD_SIGN_BIT 32768
#define TEMPLE_WORD_MODULUS 65536
#define TEMPLE_PATH_CAPACITY 256
#define TEMPLE_EXIT_EXCEPTION 'Restunts'

#endif

#ifndef RESTUNTS_TEMPLE_RASTER_HH
#define RESTUNTS_TEMPLE_RASTER_HH

#include "LEGACY.HH"

extern legacy_void TempleRasterRegisterSprite(CSprite *sprite);
extern legacy_void TempleRasterSelect(CSprite *sprite);
extern legacy_void TempleRasterForget(legacy_void *bitmap);
extern legacy_void TempleRasterForgetRange(legacy_u8 *begin,legacy_u32 size);
extern legacy_void TempleRasterXorPixel(legacy_u8 *bitmap,legacy_u16 offset,legacy_u8 mask);
extern legacy_void TempleRasterShutdown();
extern legacy_void TempleRasterSource(legacy_void *shape);
extern legacy_void *TempleRasterResource(legacy_void *resource);
extern legacy_void *TempleRasterResourceParsed(legacy_void *resource);
extern legacy_void TempleRasterPresent(CDC *dc);
extern legacy_void TempleRasterNative(legacy_u8 enabled);
extern legacy_u8 TempleRasterScreenDirty();
extern legacy_void TempleRasterScreenPresented();

extern legacy_void RsVideoPagesInitialize();
extern legacy_u8 RsVideoPagesIsActive();
extern legacy_void RsVideoPagesBeginRace();
extern legacy_void RsVideoPagesEndRace();
extern legacy_void RsVideoPagesSelectBackbuffer();
extern legacy_void RsVideoPagesPresent();
extern legacy_u8 RsVideoPagesIsTarget(legacy_void *bitmap);
extern legacy_u8 RsVideoPagesUsesRasterHooks(legacy_void *bitmap);
extern legacy_u8 RsVideoPagesReadPixel(legacy_u8 *bitmap,legacy_u16 offset);
extern legacy_void RsVideoPagesWritePixel(legacy_u8 *bitmap,legacy_u16 offset,legacy_u8 color);
extern legacy_void RsVideoPagesFillSpan(
    legacy_u8 *bitmap,legacy_u16 offset,legacy_u16 count,legacy_u8 color);
extern legacy_void RsVideoPagesCopySpan(legacy_u8 *destination,legacy_u16 destination_offset,
    legacy_u8 *source,legacy_u16 source_offset,legacy_u16 count);
extern legacy_void RsVideoPagesRasterSpan(legacy_u8 *destination,legacy_u16 destination_offset,
    legacy_u8 *source,legacy_u16 source_offset,legacy_u16 count,legacy_s16 operation,
    legacy_u8 *palette);
extern legacy_void RsVideoPagesPatternSpan(legacy_u8 *bitmap,legacy_u16 offset,legacy_u16 count,
    legacy_u8 pattern,legacy_u8 color,legacy_u8 alternate_color,legacy_s16 two_colors);

#endif

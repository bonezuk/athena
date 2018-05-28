//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_WAVPACKOMEGA_WAVPACKBIOREADER_H
#define __ORCUS_ENGINE_WAVPACKOMEGA_WAVPACKBIOREADER_H
//-------------------------------------------------------------------------------------------

#include "wavpack/wavpack.h"

#include "common/inc/BIOStream.h"
#include "engine/wavpackomega/inc/WavPackOmegaDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace wavpackomega
{
//-------------------------------------------------------------------------------------------

WAVPACKOMEGA_EXPORT int32_t WavPackBIOReaderReadBytes(void *id, void *data, int32_t bCount);

WAVPACKOMEGA_EXPORT uint32_t WavPackBIOReaderGetPos(void *id);

WAVPACKOMEGA_EXPORT int WavPackBIOReaderSetPosAbs(void *id, uint32_t pos);

WAVPACKOMEGA_EXPORT int WavPackBIOReaderSetPosRel(void *id, int32_t delta, int mode);

WAVPACKOMEGA_EXPORT int WavPackBIOReaderPushBackByte(void *id,int c);

WAVPACKOMEGA_EXPORT uint32_t WavPackBIOReaderGetLength(void *id);

WAVPACKOMEGA_EXPORT int WavPackBIOReaderCanSeek(void *id);

WAVPACKOMEGA_EXPORT int32_t WavPackBIOReaderWriteBytes(void *id,void *data,int32_t bCount);

WAVPACKOMEGA_EXPORT void WavPackBIOReaderSetupReader(WavpackStreamReader *reader);

//-------------------------------------------------------------------------------------------
} // namespace wavpackomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

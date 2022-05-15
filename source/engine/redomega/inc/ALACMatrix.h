//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_ENGINE_REDOMEGA_ALACMATRIX_H
#define __OMEGA_ENGINE_REDOMEGA_ALACMATRIX_H
//-------------------------------------------------------------------------------------------

#include "engine/redomega/inc/ALACSequence.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
namespace redomega
{
//-------------------------------------------------------------------------------------------

class REDOMEGA_EXPORT ALACMatrix
{
	public:
		ALACMatrix();
		virtual ~ALACMatrix();
		
		void unMix16(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes);
		void unMix20(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes);
		void unMix24(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes,tuint16 *shiftUV,tint32 bytesShifted);
		void unMix32(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes,tuint16 *shiftUV,tint32 bytesShifted);

		void copyPredictorTo16(tint32 *in,sample_t *out,tint32 stride,tint32 numSamples);
		void copyPredictorTo20(tint32 *in,sample_t *out,tuint32 stride,tint numSamples);
		void copyPredictorTo24(tint32 *in,sample_t *out,tint32 stride,tint32 numSamples);
		void copyPredictorTo24Shift(tint32 *in,tuint16 *shift,sample_t *out,tuint32 stride,tint32 numSamples,tint32 bytesShifted);
		void copyPredictorTo32(tint32 *in,sample_t *out,tuint32 stride,tint32 numSamples);
		void copyPredictorTo32Shift(tint32 *in,tuint16 *shift,sample_t *out,tuint32 stride,tint32 numSamples,tint32 bytesShifted);
		
		void clip(sample_t *out,tint32 numSamples,tuint32 stride);
		void clipLR(sample_t *out,tint32 numSamples,tuint32 stride);

	protected:
};

//-------------------------------------------------------------------------------------------
} // namespace redomega
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------


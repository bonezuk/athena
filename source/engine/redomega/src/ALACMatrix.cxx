#include "engine/redomega/inc/ALACMatrix.h"
#include "engine/inc/FormatTypeFromFloat.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace redomega
{
//-------------------------------------------------------------------------------------------

ALACMatrix::ALACMatrix()
{}

//-------------------------------------------------------------------------------------------

ALACMatrix::~ALACMatrix()
{}

//-------------------------------------------------------------------------------------------

void ALACMatrix::unMix16(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes)
{
	tint32 j;
	sample_t *op = out;
	
	if(mixRes!=0)
	{
		for(j=0;j<numSamples;j++)
		{
			tint32 lCh,rCh;
			lCh = u[j] + v[j] - ((mixRes * v[j]) >> mixBits);
			rCh = lCh - v[j];
			op[0] = sampleFrom16Bit(lCh);
			op[1] = sampleFrom16Bit(rCh);
			op += stride;
		}
	}
	else
	{
		for(j=0;j<numSamples;j++)
		{
			op[0] = sampleFrom16Bit(u[j]);
			op[1] = sampleFrom16Bit(v[j]);
			op += stride;
		}
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::unMix20(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes)
{
	tint j;
	sample_t *op = out;
	
	if(mixRes!=0)
	{
		for(j=0;j<numSamples;j++)
		{
			tint32 l,r;
			
			l = u[j] + v[j] - ((mixRes * v[j]) >> mixBits);
			r = l - v[j];
			l <<= 4;
			r <<= 4;
			op[0] = sampleFrom24Bit(l);
			op[1] = sampleFrom24Bit(r);
			op += stride;
		}
	}
	else
	{
		for(j=0;j<numSamples;j++)
		{
			tint32 val;
			
			val = u[j] << 4;
			op[0] = sampleFrom24Bit(val);
			val = v[j] << 4;
			op[1] = sampleFrom24Bit(val);
			op += stride;
		}
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::unMix24(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes,tuint16 *shiftUV,tint32 bytesShifted)
{
	tint32 l,r,j,k;
	tint32 shift = bytesShifted << 3;
	sample_t *op = out;
	
	if(mixRes!=0)
	{
		if(bytesShifted!=0)
		{
			for(j=0,k=0;j<numSamples;j++,k+=2)
			{
				l = u[j] + v[j] - ((mixRes * v[j]) >> mixBits);
				r = l - v[j];
				l = (l << shift) | static_cast<tuint32>(shiftUV[k]);
				r = (r << shift) | static_cast<tuint32>(shiftUV[k+1]);
				op[0] = sampleFrom24Bit(l);
				op[1] = sampleFrom24Bit(r);
				op += stride;
			}
		}
		else
		{
			for(j=0;j<numSamples;j++)
			{
				l = u[j] + v[j] - ((mixRes * v[j]) >> mixBits);
				r = l - v[j];
				op[0] = sampleFrom24Bit(l);
				op[1] = sampleFrom24Bit(r);
				op += stride;
			}
		}
	}
	else
	{
		if(bytesShifted!=0)
		{
			for(j=0,k=0;j<numSamples;j++,k+=2)
			{
				l = (u[j] << shift) | static_cast<tuint32>(shiftUV[k]);
				r = (v[j] << shift) | static_cast<tuint32>(shiftUV[k+1]);
				op[0] = sampleFrom24Bit(l);
				op[1] = sampleFrom24Bit(r);
				op += stride;
			}
		}
		else
		{
			for(j=0;j<numSamples;j++)
			{
				l = u[j];
				r = v[j];
				op[0] = sampleFrom24Bit(l);
				op[1] = sampleFrom24Bit(r);
				op += stride;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::unMix32(tint32 *u,tint32 *v,sample_t *out,tuint32 stride,tint32 numSamples,tint32 mixBits,tint32 mixRes,tuint16 *shiftUV,tint32 bytesShifted)
{
	tint32 j,k,l,r;
	sample_t *op = out;
	tint32 shift = bytesShifted << 3;
	
	if(mixRes!=0)
	{
		for(j=0,k=0;j<numSamples;j++,k+=2)
		{
			tint32 lt = u[j];
			tint32 rt = v[j];
			l = lt + rt - ((mixRes * rt) >> mixBits);
			r = l - rt;
			lt = (l << shift) | static_cast<tuint32>(shiftUV[k]);
			rt = (r << shift) | static_cast<tuint32>(shiftUV[k+1]);
			op[0] = sampleFrom32Bit(lt);
			op[1] = sampleFrom32Bit(rt);
			op += stride;
		}
	}
	else
	{
		if(bytesShifted==0)
		{
			for(j=0;j<numSamples;j++)
			{
				op[0] = sampleFrom32Bit(u[j]);
				op[1] = sampleFrom32Bit(v[j]);
				op += stride;
			}
		}
		else
		{
			for(j=0,k=0;j<numSamples;j++,k+=2)
			{
				l = (u[j] << shift) | static_cast<tuint32>(shiftUV[k]);
				r = (v[j] << shift) | static_cast<tuint32>(shiftUV[k+1]);
				op[0] = sampleFrom32Bit(l);
				op[1] = sampleFrom32Bit(r);
				op += stride;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::copyPredictorTo16(tint32 *in,sample_t *out,tint32 stride,tint32 numSamples)
{
	tint32 j;
	sample_t *op = out;
	
	for(j=0;j<numSamples;j++)
	{
		tint32 val = in[j];
		*op = sampleFrom16Bit(val);
		op += stride;
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::copyPredictorTo24(tint32 *in,sample_t *out,tint32 stride,tint32 numSamples)
{
	tint32 j;
	sample_t *op = out;
	
	for(j=0;j<numSamples;j++)
	{
		tint32 val = in[j];
		*op = sampleFrom24Bit(val);
		op += stride;
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::copyPredictorTo24Shift(tint32 *in,tuint16 *shift,sample_t *out,tuint32 stride,tint32 numSamples,tint32 bytesShifted)
{
	tint32 j;
	tint32 shiftVal = bytesShifted << 3;
	sample_t *op = out;
	
	for(j=0;j<numSamples;j++)
	{
		tint32 val = in[j];
		val = (val << shiftVal) | static_cast<tuint32>(shift[j]);
		*op = sampleFrom24Bit(val);
		op += stride;
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::copyPredictorTo20(tint32 *in,sample_t *out,tuint32 stride,tint numSamples)
{
	tint32 j;
	sample_t *op = out;
	
	for(j=0;j<numSamples;j++)
	{
		tint32 val = in[j] << 4;
		*op = sampleFrom24Bit(val);
		op += stride;
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::copyPredictorTo32(tint32 *in,sample_t *out,tuint32 stride,tint32 numSamples)
{
	tint32 i,j;
	
	for(i=0,j=0;i<numSamples;i++,j+=stride)
	{
		out[j] = sampleFrom32Bit(in[j]);
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::copyPredictorTo32Shift(tint32 *in,tuint16 *shift,sample_t *out,tuint32 stride,tint32 numSamples,tint32 bytesShifted)
{
	tint32 j,val;
	tuint32 shiftVal = bytesShifted << 3;
	sample_t *op = out;
	
	for(j=0;j<numSamples;j++)
	{
		val = (in[j] << shiftVal) | static_cast<tuint32>(shift[j]);
		*op = sampleFrom32Bit(val);
		op += stride;
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::clip(sample_t *out,tint32 numSamples,tuint32 stride)
{
	tint32 j;
	sample_t val;
	sample_t *op = out;
	
	for(j=0;j<numSamples;j++)
	{
		val = *op;
		
#if defined(SINGLE_FLOAT_SAMPLE)
		if(val<-1.0f)
		{
			val = -1.0f;
		}
		else if(val>1.0f)
		{
			val = 1.0f;
		}
#else
		if(val<-1.0)
		{
			val = -1.0;
		}
		else if(val>1.0)
		{
			val = 1.0;
		}
#endif
		*op = val;
		op += stride;
	}
}

//-------------------------------------------------------------------------------------------

void ALACMatrix::clipLR(sample_t *out,tint32 numSamples,tuint32 stride)
{
	tint32 j;
	sample_t valL,valR;
	sample_t *op = out;
	
	for(j=0;j<numSamples;j++)
	{
#if defined(SINGLE_FLOAT_SAMPLE)		
		valL = op[0];
		if(valL<-1.0f)
		{
			valL = -1.0f;
		}
		else if(valL>1.0f)
		{
			valL = 1.0f;
		}
		
		valR = op[1];
		if(valR<-1.0f)
		{
			valR = -1.0f;
		}
		else if(valR>1.0f)
		{
			valR = 1.0f;
		}
#else
		valL = op[0];
		if(valL<-1.0)
		{
			valL = -1.0;
		}
		else if(valL>1.0)
		{
			valL = 1.0;
		}
		
		valR = op[1];
		if(valR<-1.0)
		{
			valR = -1.0;
		}
		else if(valR>1.0)
		{
			valR = 1.0;
		}
#endif
		
		op[0] = valL;
		op[1] = valR;
		op += stride;
	}
}

//-------------------------------------------------------------------------------------------
} // namespace redomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------

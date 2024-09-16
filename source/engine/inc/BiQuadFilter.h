//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_ENGINE_BQUADFILTER_H
#define __OMEGA_ENGINE_BQUADFILTER_H
//-------------------------------------------------------------------------------------------
/*
BiQuad IIR filter implementation based on:
https://www.w3.org/TR/audio-eq-cookbook/
https://webaudio.github.io/Audio-EQ-Cookbook/Audio-EQ-Cookbook.txt
https://octovoid.com/2017/11/04/coding-a-parametric-equalizer-for-audio-applications/
https://octovoid.com/wp-content/uploads/2017/11/orfanidis.pdf
https://github.com/dimtass/dsp-c-filters.git
https://github.com/dimtass/DSP-Cpp-filters.git
*/
//-------------------------------------------------------------------------------------------

#include "engine/inc/AData.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------

class BiQuadFilter
{
	public:
		typedef enum
		{
			e_AllPass_FirstOrder = 1,
			e_UnknownFilter = 0
		} Type;

	public:
		BiQuadFilter(const BiQuadFilter& rhs);
		
		const BiQuadFilter& operator = (const BiQuadFilter& rhs);
		
		sample_t process(const sample_t x0);
		
		Type type() const;
		sample_t a0() const;
		sample_t a1() const;
		sample_t a2() const;
        sample_t b1() const;
        sample_t b2() const;
				
		static BiQuadFilter allPassFirstOrder(sample_t fC, sample_t fS);
		
	private:
		
		Type m_type;
		
		sample_t m_a0;
		sample_t m_a1;
		sample_t m_a2;
		sample_t m_b1;
		sample_t m_b2;
		
		sample_t m_xMinus1;
		sample_t m_xMinus2;
		sample_t m_yMinus1;
		sample_t m_yMinus2;
		
		BiQuadFilter();
		
		void copy(const BiQuadFilter& rhs);
		
		void coeffAllPassFirstOrder(sample_t fC, sample_t fS);
};

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------


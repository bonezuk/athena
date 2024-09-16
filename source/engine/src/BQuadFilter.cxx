#include "engine/inc/BiQuadFilter.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------

BiQuadFilter::BiQuadFilter() : m_type(BiQuadFilter::e_UnknownFilter),
	m_a0(0.0),
	m_a1(0.0),
	m_a2(0.0),
	m_b1(0.0),
	m_b2(0.0),
	m_xMinus1(0.0),
	m_xMinus2(0.0),
	m_yMinus1(0.0),
	m_yMinus2(0.0)
{}

//-------------------------------------------------------------------------------------------

BiQuadFilter::BiQuadFilter(const BiQuadFilter& rhs)
{
	copy(rhs);
}

//-------------------------------------------------------------------------------------------

void BiQuadFilter::copy(const BiQuadFilter& rhs)
{
	m_type = rhs.m_type;		
	m_a0 = rhs.m_a0;
	m_a1 = rhs.m_a1;
	m_a2 = rhs.m_a2;
	m_b1 = rhs.m_b1;
	m_b2 = rhs.m_b2;
	m_xMinus1 = rhs.m_xMinus1;
	m_xMinus2 = rhs.m_xMinus2;
	m_yMinus1 = rhs.m_yMinus1;
	m_yMinus2 = rhs.m_yMinus2;
}

//-------------------------------------------------------------------------------------------

const BiQuadFilter& BiQuadFilter::operator = (const BiQuadFilter& rhs)
{
	if(this != &rhs)
	{
		copy(rhs);
	}
	return *this;
}

//-------------------------------------------------------------------------------------------

BiQuadFilter::Type BiQuadFilter::type() const
{
	return m_type;
}

//-------------------------------------------------------------------------------------------

sample_t BiQuadFilter::a0() const
{
	return m_a0;
}

//-------------------------------------------------------------------------------------------

sample_t BiQuadFilter::a1() const
{
	return m_a1;
}

//-------------------------------------------------------------------------------------------

sample_t BiQuadFilter::a2() const
{
	return m_a2;
}

//-------------------------------------------------------------------------------------------

sample_t BiQuadFilter::b1() const
{
    return m_b1;
}

//-------------------------------------------------------------------------------------------

sample_t BiQuadFilter::b2() const
{
    return m_b2;
}

//-------------------------------------------------------------------------------------------

sample_t BiQuadFilter::process(const sample_t x0)
{
	sample_t y0 = (m_a0 * x0) + (m_a1 * m_xMinus1) + (m_a2 * m_xMinus2) - (m_b1 * m_yMinus1) - (m_b2 * m_yMinus2);
	m_xMinus2 = m_xMinus1;
	m_xMinus1 = x0;
	m_yMinus2 = m_yMinus1;
	m_yMinus1 = y0;
	return y0;
}

//-------------------------------------------------------------------------------------------
/* First order all-pass filter
	fC = corner frequency
	fS = sample frequency */
//-------------------------------------------------------------------------------------------

BiQuadFilter BiQuadFilter::allPassFirstOrder(sample_t fC, sample_t fS)
{
	BiQuadFilter f;
	f.coeffAllPassFirstOrder(fC, fS);
	return f;
}

//-------------------------------------------------------------------------------------------
    
void BiQuadFilter::coeffAllPassFirstOrder(sample_t fC, sample_t fS)
{
    sample_t a = (::tan((c_PI_D * fC) / fS) - 1.0) / (::tan((c_PI_D * fC) / fS) + 1.0);
    m_type = e_AllPass_FirstOrder;
	m_a0 = a;
	m_a1 = 1.0;
	m_a2 = 0.0;
	m_b1 = a;
	m_b2 = 0.0;
}

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------

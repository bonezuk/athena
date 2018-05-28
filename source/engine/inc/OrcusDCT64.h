//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_ORCUSDCT_H
#define __ORCUS_ENGINE_ORCUSDCT_H
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <memory.h>

#include "common/inc/Allocation.h"
#include "common/inc/BOTree.h"
#include "engine/inc/EngineDLL.h"

#include <QMap>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
//-------------------------------------------------------------------------------------------

class ENGINE_EXPORT OrcusDCT64
{
	public:
		OrcusDCT64(int N);
		virtual ~OrcusDCT64();
		
		static OrcusDCT64 *get(tint N);
		static void stop();
	
		tfloat64 *input();
		const tfloat64 *input() const;
		
		tfloat64 *output();
		const tfloat64 *output() const;
	
		tfloat64 *TypeII(tfloat64 *x);
		tfloat64 *TypeIII(tfloat64 *x);
		tfloat64 *TypeIV(tfloat64 *x);
		
		tfloat64 *MDCT(tfloat64 *x);
		tfloat64 *InverseMDCT(tfloat64 *x);

		void WMDCT(tfloat64 *x,tfloat64 *X,tint offset);
		void WInverseMDCT(tfloat64 *x,tfloat64 *X,tint offset);

		void VSInverseMDCT(tfloat64 *x,tfloat64 *X);
		
	protected:
	
		static QMap<tint,OrcusDCT64 *> m_DCTCollection;
	
		static tint m_DCTCounter;
		static common::Allocation m_DCTAllocation;
		
		common::Allocation m_alloc;
		
		tint m_N;
		tfloat64 *m_x;
		tfloat64 *m_X;
		tfloat64 *m_Y;
		tint m_offsetY;
		tfloat64 **m_D4FactorArray;
		
		tfloat64 m_halfN;
		
		bool isMod2() const;
		bool isMod2(int N) const;
		
		int mod2() const;
		int mod2(int N) const;
		
		void init();
		void free();
		
		tfloat64 dctD4Factor(int k,int N) const;
		
		void Type2(tfloat64 *x,tfloat64 *X,int N);
		void Type2(tfloat64 *x,tfloat64 *X,int N,int lN);
		void Type3(tfloat64 *x,tfloat64 *X,int N);
		void Type3(tfloat64 *x,tfloat64 *X,int N,int lN);
		void Type4(tfloat64 *x,tfloat64 *X,int N);
		void Type4(tfloat64 *x,tfloat64 *X,int N,int lN);

		void T2L16(tfloat64 *x,tfloat64 *X);
		void T3L16(tfloat64 *x,tfloat64 *X);
		void T4L16(tfloat64 *x,tfloat64 *X);
};

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
/*
 * Parks-McClellan algorithm for FIR filter design
 *
 * https://en.wikipedia.org/wiki/Parks–McClellan_filter_design_algorithm
 * https://github.com/janovetz/remez-exchange/tree/master
 *
 */
//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_ENGINE_REMEZFIR_H
#define __OMEGA_ENGINE_REMEZFIR_H
//-------------------------------------------------------------------------------------------

#include "engine/inc/AData.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine 
{
//-------------------------------------------------------------------------------------------

class ENGINE_EXPORT RemezFIR
{
	public:
		RemezFIR();

		void remez(double h[], int *numtaps, int *numband, const double bands[], 
		           const double des[], const double weight[], int *type, int *griddensity);

	private:
		
		void createDenseGrid(int r, int numtaps, int numband, const double bands[],
		                     const double des[], const double weight[], int gridsize,
		                     double Grid[], double D[], double W[],
		                     int symmetry, int griddensity);
		
		void initialGuess(int r, int Ext[], int gridsize);

		void calcParms(int r, int Ext[], double Grid[], double D[], double W[],
		               double ad[], double x[], double y[]);

		double computeA(double freq, int r, double ad[], double x[], double y[]);

		void calcError(int r, double ad[], double x[], double y[],
		               int gridsize, double Grid[],
		               double D[], double W[], double E[]);

		int search(int r, int Ext[], int gridsize, double E[]);
		
		void freqSample(int N, double A[], double h[], int symm);
		
		int isDone(int r, int Ext[], double E[]);
};

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

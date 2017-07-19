/**
 * \class Envelope
 *
 *
 * \brief Simple low-pass filter.
 *
 * \author $Author: Corvin Jaedicke & Alberto Monciero $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2017/29/04 21:39:54 $
 *
 * Contact: c.jaedicke@math.tu-berlin.de
 *
 */

#pragma once

#define _USE_MATH_DEFINES

#include <cmath>

class Filter
{
public:
	// Default LPF filter
	Filter(void) :
	m_dAlpha(1.0),
	m_dBeta(0.0),
	m_dZ1(0.0),
	m_dFc(10000.0),
	// default
	m_uFilterType(LPF1),
	// sample rate
	m_dSampleRate(48000)
	{
		reset();
	};

	~Filter(void);
	double m_dFc;///< cut-off frequency
	// Trapezoidal Integrator Components
	double m_dAlpha; ///< Feed-Forward coefficient
	double m_dBeta;///< Moog-Ladder-Filter coefficient
	/// Get the feedback output.
	double getFeedbackOutput(){return m_dBeta * m_dZ1;}
	/// Reset memory
	void reset(){m_dZ1 = 0;}
	/// Apply the filter to a sample
	void doFilter(float * xn);
	enum{LPF1,HPF1,LPF2,HPF2,BPF2,BSF2,LPF4,HPF4,BPF4};
	unsigned int m_uFilterType;	///< Currently selected filter type
	unsigned int m_dSampleRate;///< sample rate

protected:
	double m_dZ1; ///< z-1 memory
};


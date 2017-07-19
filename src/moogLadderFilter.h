/**
 * \class MoogLadderFilter
 *
 *
 * \brief Applies a 4-stage low-pass filter to a sample.
 *
 * This class applies a 4-stage low-pass filter to a sample.
 * The filter is inspired by Moog Ladder Filter with Oberheim Xpander variations as described in:
 * Designing Software Synthesizer Plug-Ins in C++, Page 404, 7.6 Moog Ladder Filter Model
 *
 * \author $Author: Corvin Jaedicke & Alberto Monciero $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2017/29/04 19:14:45 $
 *
 * Contact: c.jaedicke@math.tu-berlin.de
 *
 */

#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include "filter.h"

using std::cout;
using std::endl;

class MoogLadderFilter
{
public:
	/// Filter with default SampleRate 48kHz and cut-off at 10kHz
	MoogLadderFilter() : 
	m_dK(1.0),
	m_dAlpha_0(1.0),
	m_dGamma(0.0),
	m_dA(0.0),
	m_dB(0.0),
	m_dC(0.0),
	m_dD(0.0),
	m_dE(0.0),
	// cut-off
	m_dFc(10000.0),
	m_dSampleRate(48000),
	// set default filter type
	m_uFilterType(LPF4) {
	// set all as LPF types
	// this shouldn't be changed!
	m_LPF1.m_uFilterType = LPF1;
	m_LPF2.m_uFilterType = LPF1;
	m_LPF3.m_uFilterType = LPF1;
	m_LPF4.m_uFilterType = LPF1;
	// update filter coefficients
	setFilter(LPF4);
	// set T
	calculateTimePerSample();
	update();
	// flush everything
	reset();
	};

	~MoogLadderFilter();

	// Ladder-Filter objects
	Filter m_LPF1; ///< First LPF
	Filter m_LPF2; ///< Second LPF
	Filter m_LPF3; ///< Third LPF
	Filter m_LPF4; ///< Fourth LPF

	/// Reset all four LPF
	void reset();

	/** 
     * \brief Set the resonance off the filter
     * \param K Factor with values between 1..4
     */
	void setResonance(float K);
	/// Re-calculates alpha value. Only needed after setting resonance.
	void calculateAlpha_0();
	// Set Filter type (e.g. LPF, BPF, HPF)
	void setFilter(unsigned int type);

	float m_dK;		///< K, set with Q
	float m_dGamma;	///< see block diagram
	float m_dAlpha_0;	///< see block diagram

	// Oberheim Xpander variations
	float m_dA; ///< Oberheim Xpander A-coefficient 
	float m_dB; ///< Oberheim Xpander B-coefficient 
	float m_dC; ///< Oberheim Xpander C-coefficient 
	float m_dD; ///< Oberheim Xpander D-coefficient 
	float m_dE; ///< Oberheim Xpander E-coefficient 

	float m_dFc; ///< Cut-off frequency
	
	unsigned int m_dSampleRate; ///< sample rate

	enum{LPF1,HPF1,LPF2,HPF2,BPF2,BSF2,LPF4,HPF4,BPF4}; ///< Different filter types

	unsigned int m_uFilterType = LPF4;	///< Selected filter type

	const float TWO_PI = 2.0*M_PI;

	/** 
     * \brief Set new Cut-off frequency. This function calls update().
     * \param Fc Cut-off frequency. Has to be <= 10kHz
     */
	void setCutOff(float Fc);
	/** 
     * \brief Updates all parameters used by the filter. This is an very expensive call!
     */
	void update();
	/** 
     * \brief Applies filter to given sample.
     * \param xn Sample
     * \return Filtered sample
     */
	float doFilter(float xn);
	/** 
     * \brief Applies filter to given sample. Uses call-by-ref
     * \param xn Pointer to sample
     */
	void doFilter(float * xn);

private:
	float T; ///< 1/sampleRate
	/** 
     * \brief Calculates the time step per sample.
     * \note Should be re-calculated after changing sampleRate
     */
	void calculateTimePerSample(){T = 1.0 / (float) m_dSampleRate;};

	float dLP1; ///< filter value one
	float dLP2; ///< filter value two
	float dLP3; ///< filter value three
	float dLP4; ///< filter value four

};


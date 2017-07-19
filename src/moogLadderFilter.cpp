#include "moogLadderFilter.h"


MoogLadderFilter::~MoogLadderFilter()
{
}

void MoogLadderFilter::calculateAlpha_0() {
	m_dAlpha_0 = 1.0/(1.0 + m_dK * m_dGamma);
}

void MoogLadderFilter::setCutOff(float Fc) {
	m_dFc = Fc;
	update();
}

void MoogLadderFilter::reset()
{
	// flush everything
	m_LPF1.reset();
	m_LPF2.reset();
	m_LPF3.reset();
	m_LPF4.reset();
}


void MoogLadderFilter::setResonance(float K)
{
	m_dK = K;
	// only alpha_0 needs re-calculation, no need to call update!
	calculateAlpha_0();
}

void MoogLadderFilter::update()
	{
		float g = tan(M_PI * m_dFc * T);

		// feed-forward coeff
		// same for LPF, HPF
		float G = g / (1.0 + g);

		// set alphas
		m_LPF1.m_dAlpha = G;
		m_LPF2.m_dAlpha = G;
		m_LPF3.m_dAlpha = G;
		m_LPF4.m_dAlpha = G;

		// set betas
		m_LPF4.m_dBeta = 1.0/(1.0 + g);
		m_LPF3.m_dBeta = G * m_LPF4.m_dBeta;
		m_LPF2.m_dBeta = G * m_LPF3.m_dBeta;
		m_LPF1.m_dBeta = G * m_LPF2.m_dBeta;

		m_dGamma = G*G*G*G; // G^4

		m_dAlpha_0 = 1.0 / (1.0 + m_dK * m_dGamma);
	}

	float MoogLadderFilter::doFilter(float xn)
	{
		float dSigma = m_LPF1.getFeedbackOutput() +
		 				 m_LPF2.getFeedbackOutput() +
		 				 m_LPF3.getFeedbackOutput() +
		 				 m_LPF4.getFeedbackOutput();

		// passband gain compensation
		// use factor on m_dK for gain compensation or let user choose?
		xn *= 1.0 + m_dK;

		// calculate input to first filter
		// float dU = xn * m_dAlpha_0;
		float dU = (xn - m_dK * dSigma) * m_dAlpha_0;

		// saturation compensation
		// use factor on dU for saturation compensation or let user choose?
		dU = tanh(dU);

		// cascade of 4 filters
		dLP1 = dU;
		m_LPF1.doFilter(&dLP1);
		dLP2 = dLP1;
		m_LPF2.doFilter(&dLP2);
		dLP3 = dLP2;
		m_LPF3.doFilter(&dLP3);
		dLP4 = dLP3;
		m_LPF4.doFilter(&dLP4);

		// Oberheim variations
		return m_dA*dU + m_dB*dLP1 + m_dC*dLP2 + m_dD*dLP3 +  m_dE*dLP4;
	}

	void MoogLadderFilter::doFilter(float * xn)
	{
		float dSigma = m_LPF1.getFeedbackOutput() +
		 				 m_LPF2.getFeedbackOutput() +
		 				 m_LPF3.getFeedbackOutput() +
		 				 m_LPF4.getFeedbackOutput();

		// passband gain compensation
		// use factor on m_dK for gain compensation or let user choose?
		*xn *= 1.0 + m_dK;

		// calculate input to first filter
		*xn = (*xn - m_dK * dSigma) * m_dAlpha_0;

		// saturation compensation
		// use factor on dU for saturation compensation or let user choose?
		*xn = tanh(*xn);
		
		// cascade of 4 filters
		dLP1 = *xn;
		m_LPF1.doFilter(&dLP1);
		dLP2 = dLP1;
		m_LPF2.doFilter(&dLP2);
		dLP3 = dLP2;
		m_LPF3.doFilter(&dLP3);
		dLP4 = dLP3;
		m_LPF4.doFilter(&dLP4);

		// Oberheim variations
		*xn = m_dA * *xn + m_dB*dLP1 + m_dC*dLP2 + m_dD*dLP3 +  m_dE*dLP4;
	}

	void MoogLadderFilter::setFilter(unsigned int type) {
		m_uFilterType = type;
		// Oberheim variation
		switch(m_uFilterType)
		{
			case LPF4:
				m_dA = 0.0;
				m_dB = 0.0;
				m_dC = 0.0;
				m_dD = 0.0;
				m_dE = 1.0;
				break;

			case LPF2:
				m_dA = 0.0;
				m_dB = 0.0;
				m_dC = 1.0;
				m_dD = 0.0;
				m_dE = 0.0;
				break;

			case BPF4:
				m_dA = 0.0;
				m_dB = 0.0;
				m_dC = 4.0;
				m_dD = -8.0;
				m_dE = 4.0;
				break;

			case BPF2:
				m_dA = 0.0;
				m_dB = 2.0;
				m_dC = -2.0;
				m_dD = 0.0;
				m_dE = 0.0;
				break;

			case HPF4:
				m_dA = 1.0;
				m_dB = -4.0;
				m_dC = 6.0;
				m_dD = -4.0;
				m_dE = 1.0;
				break;

			case HPF2:
				m_dA = 1.0;
				m_dB = -2.0;
				m_dC = 1.0;
				m_dD = 0.0;
				m_dE = 0.0;
				break;

			default: // LPF4
				m_dA = 0.0;
				m_dB = 0.0;
				m_dC = 0.0;
				m_dD = 0.0;
				m_dE = 1.0;
				break;
		}
	}

#include "filter.h"


Filter::~Filter()
{
}

void Filter::doFilter(float * xn)
{
	// calculate v(n)
	// change to double vn = (m_da0 * xn - m_dZ1) * m_dAlpha; for added input gain m_da0
	float vn = (*xn - m_dZ1) * m_dAlpha;
	// calculate LP output
	*xn = vn + m_dZ1;
	// memory
	m_dZ1 = vn + *xn;
}



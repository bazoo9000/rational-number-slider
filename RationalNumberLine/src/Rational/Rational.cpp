#include "Rational.h"
#include <iostream>
#include <math.h>

Rational::Rational(int num, int denom)
{
	if (denom == 0)
	{
		std::cout << "ERROR CREATING RATIONAL NUMBER! denominator is 0!";
		return;
	}

	m_Numerator = num;
	m_Denominator = denom;
}

Rational::Rational(const Rational& rational)
{
	if (rational.m_Denominator == 0)
	{
		std::cout << "ERROR COPYING RATIONAL NUMBER! denominator of the copy is 0!";
		return;
	}

	m_Numerator = rational.m_Numerator;
	m_Denominator = rational.m_Denominator;
}

Rational::~Rational()
{
	m_Numerator = 0;
	m_Denominator = 0;
}

float Rational::ToFloat()
{
	return ((float)m_Numerator / (float)m_Denominator);
}

Rational Rational::ToRational(float f)
{
	int intPart = floor(f);
	float fracPart = (f - intPart) * pow(10, 7);

	int num = fracPart;
	int denom = pow(10, 7);

	while (num % 10 == 0 && num % 10 == denom % 10)
	{
		num /= 10;
		denom /= 10;
	}

	Rational q(num + (denom * intPart), denom);
	return q;
}

Rational Rational::Amplify(int amount)
{
	m_Numerator   *= amount;
	m_Denominator *= amount;

	return *this;
}

Rational Rational::Simplify()
{
	int& num = m_Numerator;
	int& denom = m_Denominator;

	int gcd = GCD(num, denom);
	while (gcd > 1)
	{
		num = num / gcd;
		denom = denom / gcd;

		gcd = GCD(num, denom);
	}

	return *this;
}

Rational Rational::Add(const Rational& rational) const
{
	Rational p = *this;
	Rational q = rational;

	if (p.m_Denominator != q.m_Denominator)
	{
		p.Amplify(q.m_Denominator);
		q.Amplify(p.m_Denominator);
	}

	return Rational(p.m_Numerator + q.m_Numerator, q.m_Denominator);
}

Rational Rational::AddInt(const int& i) const
{
	Rational p(m_Numerator + i * m_Denominator, m_Denominator);
	return p;
}

Rational Rational::AddFloat(const float& f) const
{
	Rational p = Rational::ToRational(f);
	return Add(p);
}

Rational Rational::Multiply(const Rational& q) const
{
	Rational p(m_Numerator * q.m_Numerator, m_Denominator * q.m_Denominator);
	return p;
}

Rational Rational::MultiplyInt(const int& i) const
{
	Rational p(m_Numerator * i, m_Denominator);
	return p;
}

Rational Rational::MultiplyFloat(const float& f) const
{
	float fvalue = m_Numerator * f / m_Denominator;

	Rational p = Rational::ToRational(fvalue);
	return p;
}

std::string Rational::ToString()
{
	//std::string s = std::to_string(m_Numerator) + "/" + std::to_string(m_Denominator);
	if (m_Numerator % m_Denominator == 0)
	{
		return std::to_string(m_Numerator / m_Denominator);
	}
	return std::to_string(m_Numerator) + "/" + std::to_string(m_Denominator);
}

std::ostream& operator<<(std::ostream& os, const Rational& q)
{
	Rational p = q;
	os << p.ToString();
	return os;
}

int GCD(int a, int b)
{
	if (a == 0 || b == 0)
	{
		return 0;
	}

	a = abs(a);
	b = abs(b);

	while (a != b)
	{
		if (a > b)
		{
			a = a - b;
		}
		else
		{
			b = b - a;
		}
	}

	return a;
}
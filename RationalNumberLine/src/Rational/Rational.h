#pragma once

#include <string>

class Rational 
{
public:
	Rational(int num, int denom);
	Rational(const Rational& rational);
	~Rational();

public:
	float ToFloat();
	static Rational ToRational(float f);
	Rational Amplify(int amount);
	Rational Simplify();

public:
	Rational Add(const Rational& rational) const;
	Rational operator+(const Rational& rational) const { return Add(rational); }
	Rational AddInt(const int& i) const;
	Rational operator+(const int& i) const { return AddInt(i); }
	Rational AddFloat(const float& f) const;
	Rational operator+(const float& f) const { return AddFloat(f); }

	Rational Multiply(const Rational& rational) const;
	Rational operator*(const Rational& rational) const { return Multiply(rational); }
	Rational MultiplyInt(const int& i) const;
	Rational operator*(const int& i) const { return MultiplyInt(i); }
	Rational MultiplyFloat(const float& f) const;
	Rational operator*(const float& f)	const { return MultiplyFloat(f); }

	std::string ToString();
	friend std::ostream& operator<<(std::ostream& os, const Rational& rational);

	int GetNumerator() { return m_Numerator; }
	int GetDenominator() { return m_Denominator; }

private:
	friend int GCD(int a, int b);

private:
	int m_Numerator;
	int m_Denominator;
};

int GCD(int, int); // Greatest Common Devisor
std::ostream& operator<<(std::ostream&, const Rational&);
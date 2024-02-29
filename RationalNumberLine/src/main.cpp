#include <iostream>
#include <chrono>

#include "Rational/Rational.h"

//using namespace std; is pussies

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	std::chrono::duration<float> duration;

	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1000.0f;
		std::cout << "It took " << ms << " ms to complete.\n";
	}
};

int main()
{
	Timer* t = new Timer();

	Rational q = Rational::ToRational(1.125f);
	Rational p(2, 1);
	
	std::cout << q.Simplify() << " * " << p << " = " << (q * p).Simplify() << "\n";
	std::cout << q.Simplify() << " * " << 2 << " = " << (q * 2) << "\n";
	std::cout << q.Simplify() << " * " << 1.5f << " = " << (q * 1.5f) << "\n";
	std::cout << "\n";
	std::cout << q.Simplify() << " + " << p << " = " << (q + p).Simplify() << "\n";
	std::cout << q.Simplify() << " + " << 2 << " = " << (q + 2).Simplify() << "\n";
	std::cout << q.Simplify() << " + " << 1.5f << " = " << (q + 1.5f).Simplify() << "\n";
	std::cout << "\n";

	delete t;
	
	return 0;
}
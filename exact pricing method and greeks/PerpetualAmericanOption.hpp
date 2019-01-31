#ifndef PerpetualAmericanOption_HPP
#define PerpetualAmericanOption_HPP
#include <vector>

using namespace std;



class PerpetualAmericanOption
{
private:

	double K;       // Strike price
	double sig;     // Volatility
	double r;        // Interest rate
	double S;       // Underlying price
	double b;       // Cost of carry

public:
	//constructors
	PerpetualAmericanOption();
	PerpetualAmericanOption(double new_K, double new_sig, double new_r, double new_S, double new_b);
	~PerpetualAmericanOption();

	//option pricing
	double CallPrice(double input_S) const;
	double PutPrice(double input_S) const;

	//compute option prices for a monotonically increasing range of underlying values of S
	void Pricing_By_Stock();

	//compute option prices with different r,K,sig
	void Pricing_By_Parameter();

};
#endif

// global function for mesh array
vector<double> Mesh(double begin, double end, double size);

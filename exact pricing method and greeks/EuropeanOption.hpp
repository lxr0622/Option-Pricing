#ifndef EuropeanOption_HPP
#define EuropeanOption_HPP
#include <vector>

using namespace std;



class EuropeanOption
{
private:

	double T;       // Expiry date
	double K;       // Strike price
	double sig;     // Volatility
	double r;        // Interest rate
	double S;       // Underlying price
	double b;       // Cost of carry

public:
	//constructors
	EuropeanOption();
	EuropeanOption(double new_T,double new_K, double new_sig, double new_r,double new_S,double new_b);
	~EuropeanOption();

	//option pricing
	double CallPrice(double input_S) const;
	double PutPrice(double input_S) const;

	//check if a set of put/call prices satisfy parity
	bool ParityCheck() const;

	//calculate the option price by put-call parity
	void ParityCalculation() const;

	//compute option prices for a monotonically increasing range of underlying values of S
	void Pricing_By_Stock();

	//compute option prices with different T,K,sig
	void Pricing_By_Parameter();

	//delta
	double CallDelta() const;
	double PutDelta() const;

	//gamma
	double CallGamma() const;
	double PutGamma() const;

	//compute Delta for a monotonically increasing range of underlying values of S
	void Delta_By_Stock();

	//compute Delta with different T,K,sig
	void Delta_By_Parameter();

	//compute Gamma with different T,K,sig
	void Gamma_By_Parameter();

	//appoximate Delta
	void Approximate_Delta(double h);

	//appoximate Gamma
	void Approximate_Gamma(double h);
};
#endif

// global function for mesh array
vector<double> mesharray(double begin, double end, double size);


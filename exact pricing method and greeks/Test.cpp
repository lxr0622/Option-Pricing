#include "EuropeanOption.hpp"
#include "PerpetualAmericanOption.hpp"
#include <cmath>
#include <iostream>
#include <boost/math/distributions/normal.hpp>

int main()
{
	//Group A£º
	//Set of test values (T,K,sig,r,S,b)
	EuropeanOption Batch1(1.5, 120, 0.4, 0.04, 100,0.0);
	EuropeanOption Batch2(1, 100, 0.2, 0.0, 100,0.0);
	EuropeanOption Batch3(1, 10, 0.5, 0.12, 5,0.12);
	EuropeanOption Batch4(30, 100, 0.3, 0.08, 100,0.08);

	// A1(a) calculate and output the option price by BS formula
	cout << "A1(a) EuropeanOption:" << endl;
	cout << "Batch 1: " << "C= " << Batch1.CallPrice(100) << "," << "P= " << Batch1.PutPrice(100) << endl;
	cout << "Batch 2: " << "C= " << Batch2.CallPrice(100) << "," << "P= " << Batch2.PutPrice(100) << endl;
	cout << "Batch 3: " << "C= " << Batch3.CallPrice(5) << "," << "P= " << Batch3.PutPrice(5) << endl;
	cout << "Batch 4: " << "C= " << Batch4.CallPrice(100) << "," << "P= " << Batch4.PutPrice(100) << endl;

	// A1(b)  check the put-call parity
	cout << "A1(b):" << endl;
	cout << "check the put-call parity for Batch 1 (1:True, 0:False)" << endl;
	cout << Batch1.ParityCheck() << endl;
	
	// A1(b)  calculate the option price by put-call parity
	cout << "calculate the option price by put-call parity for Batch 1" << endl;
	Batch1.ParityCalculation();

	// A1(c)  compute option prices with different S
	cout << "A1(c) Pricing_By_Stock:" << endl;
	Batch1.Pricing_By_Stock();

	// A1(d)  compute option prices with different T,K,sig
	cout << "A1(d) Pricing_By_Parameter:" << endl;
	Batch1.Pricing_By_Parameter();





	// A2(a)  compute delta for put and call option
	cout << "A2(a):" << endl;
	//(T,K,sig,r,S,b)
	EuropeanOption Batch5(0.5, 100, 0.36, 0.1, 105,0);
	cout << "Batch 1: " << "C_Delta= " << Batch1.CallDelta() << "," << "P_Delta= " << Batch1.PutDelta() << endl;
	cout << "Batch 1: " << "C_Gamma= " << Batch1.CallGamma() << "," << "P_Gamma= " << Batch1.PutGamma() << endl;
	// A2(b)  compute delta with different S
	cout << "A2(b) Delta_By_Stock: " << endl;
	Batch1.Delta_By_Stock();

	// A2(c)  compute delta and gamma with different T,K,sig
	cout << "A2(c) Delta/Gamma_By_Parameter: " << endl;
	Batch1.Delta_By_Parameter();
	Batch1.Gamma_By_Parameter();

	// A2(d)  using divided differences to approximatw Delta and Gamma
	cout << "A2(d) Approximate Delta/Gamma: " << endl;
	Batch1.Approximate_Delta(0.1);
	Batch1.Approximate_Gamma(0.1);







	//Gourp B:
	//  B(b)  Test the data with K = 100, sig = 0.1, r = 0.1, S = 110, b = 0.02
	cout << "B(b) PerpetualAmericanOption: " << endl;
	PerpetualAmericanOption Batch6(100, 0.1, 0.1, 110, 0.02);
	cout << "Batch 6: " << "C= " << Batch6.CallPrice(110) << "," << "P= " << Batch6.PutPrice(110) << endl;

	//  B(c) compute option prices with different S
	cout << "B(c) Pricing_By_Stock: " << endl;
	Batch6.Pricing_By_Stock();

	//  B(d) compute option prices with different r,K,sig
	cout << "B(d) Pricing_By_Parameter: " << endl;
	Batch6.Pricing_By_Parameter();

	return 0;
}
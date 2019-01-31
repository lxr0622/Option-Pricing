#include "PerpetualAmericanOption.hpp"
#include <cmath>
#include <iostream>
#include <vector>

//default constructor
PerpetualAmericanOption::PerpetualAmericanOption()
{
	//default value
	K = 65;
	sig = 0.3;
	r = 0.08;
	S = 60;
	b = 0.08;
}

//paramter consturctor
PerpetualAmericanOption::PerpetualAmericanOption(double new_K, double new_sig, double new_r, double new_S, double new_b)
{
	K = new_K;
	sig = new_sig;
	r = new_r;
	S = new_S;
	b = new_b;
}

//destructor
PerpetualAmericanOption::~PerpetualAmericanOption()
{

}


//call option pricing
double PerpetualAmericanOption::CallPrice(double input_S) const
{

	double sig2 = sig*sig;
	double fac = b / sig2 - 0.5; fac *= fac;
	double y1 = 0.5 - b / sig2 + sqrt(fac + 2.0*r / sig2);

	if (1.0 == y1)
		return input_S;

	double fac2 = ((y1 - 1.0)*input_S) / (y1 * K);
	double c = K * pow(fac2, y1) / (y1 - 1.0);

	return c;

}

//put option pricing
double PerpetualAmericanOption::PutPrice(double input_S) const
{
	double sig2 = sig*sig;
	double fac = b / sig2 - 0.5; fac *= fac;
	double y2 = 0.5 - b / sig2 - sqrt(fac + 2.0*r / sig2);

	if (0.0 == y2)
		return input_S;

	double fac2 = ((y2 - 1.0)*input_S) / (y2 * K);
	double p = K * pow(fac2, y2) / (1.0 - y2);

	return p;
}

//compute option prices for a monotonically increasing range of underlying values of S
void PerpetualAmericanOption::Pricing_By_Stock()
{
	vector<double>::const_iterator i;//declare the iterator
	vector<double> Stock = Mesh(10, 100, 5);//Set a array of stock prices that from 10 to 100, interval=5.
	vector<double> CallPrice_S;//declare the vector that store call option price
	vector<double> PutPrice_S;//declare the vector that store put option price

	//assign option price to vectors
	for (i = Stock.begin(); i != Stock.end(); i++)
	{
		S = *i;//change the stock price for each loop
		CallPrice_S.push_back(CallPrice(S));
		PutPrice_S.push_back(PutPrice(S));
	}

	//explanation
	cout << "variation of S " << endl;

	//output the option price
	for (unsigned int j = 0; j < CallPrice_S.size(); j++)
	{
		cout << "C= " << CallPrice_S[j] << "," << "P= " << PutPrice_S[j] << endl;
	}

}

//compute option prices with different r,K,sig
void PerpetualAmericanOption::Pricing_By_Parameter()
{
	vector<double>::const_iterator i;//declare the iterator

	vector<vector<double>> OptionParameter;//declare the matrix for option parameters
	OptionParameter.push_back(Mesh(0.01, 0.1, 0.01));//assign the value of r to matrix (from 0.01 to 0.1, interval=0.01)
	OptionParameter.push_back(Mesh(10, 100, 10));//assign the value of K to matrix (from 10 to 100, interval=10)
	OptionParameter.push_back(Mesh(0.1, 1, 0.1));//assign the value of sigma to matrix (from 0.1 to 1, interval=0.1)
	OptionParameter.push_back(Mesh(0.01, 0.1, 0.01));//assign the value of b to matrix (from 0.01 to 0.1, interval=0.01)

	vector<vector<double>> CallPrice_Parameter(4, vector<double>());//declare the matrix that store call option price
	vector<vector<double>> PutPrice_Parameter(4, vector<double>());//declare the matrix that store put option price

	//three total loops for three parameters
	for (int j = 0; j <= 3; j++)
	{
		//assign option price to matrix
		for (i = OptionParameter[j].begin(); i != OptionParameter[j].end(); i++)
		{

			if (j == 0)
				r = *i;//first loop, change r
			else if (j == 1)
				K = *i;//second loop, change K
			else if (j == 2)
				sig = *i;//3rd loop, change sigma
			else
				b = *i;//4th loop, change b
			CallPrice_Parameter[j].push_back(CallPrice(S));
			PutPrice_Parameter[j].push_back(PutPrice(S));
		}

		//explanation
		if (j == 0)
			cout << "variation of r" << endl;
		else if (j == 1)
			cout << "variation of K" << endl;
		else if (j == 2)
			cout << "variation of sigma" << endl;
		else
			cout << "varation of b" << endl;

		//output the option price
		for (unsigned int k = 0; k < CallPrice_Parameter[j].size(); k++)
		{
			cout << "C= " << CallPrice_Parameter[j][k] << "," << "P= " << PutPrice_Parameter[j][k] << endl;
		}
	}

}


// global function for mesh array 
vector<double> Mesh(double begin, double end, double size)
{
	vector<double> arr;//declare the array that store stock price
	double var = begin;//variable for each loop

	for (int i = 0; i <= (end - begin) / size; i++)
	{
		arr.push_back(var);
		var += size;
	}
	return arr;
}
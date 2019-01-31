#include "EuropeanOption.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <boost/math/distributions/normal.hpp>



//default constructor
EuropeanOption::EuropeanOption()
{ 
	//default value
	T = 0.5;
	K = 100;
	sig = 0.36;
	r = 0.1;
	S = 105;
	b = 0;
	
}

//paramter consturctor
EuropeanOption::EuropeanOption(double new_T, double new_K, double new_sig, double new_r, double new_S,double new_b)
{
	T = new_T;
	K = new_K;
	sig = new_sig;
	r = new_r;
	S = new_S;
	b = new_b;
}

//destructor
EuropeanOption::~EuropeanOption()
{

}

//call option pricing
double EuropeanOption::CallPrice(double input_S) const
{
	//create a normal distribution
	using namespace boost::math;
	normal_distribution<> N;
	
	//BS formula for call option
	double tmp = sig * sqrt(T);
	double d1 = (log(input_S / K) + (b + (sig*sig)*0.5) * T) / tmp;
	double d2 = d1 - tmp;
	return (input_S * exp((b - r)*T) * cdf(N,d1)) - (K * exp(-r * T)* cdf(N,d2));

}

//put option pricing
double EuropeanOption::PutPrice(double input_S) const
{
	//create a normal distribution
	using namespace boost::math;
	normal_distribution<> N;

	//BS formula for call option
	double tmp = sig * sqrt(T);
	double d1 = (log(input_S / K) + (b + (sig*sig)*0.5) * T) / tmp;
	double d2 = d1 - tmp;
	return (K * exp(-r * T)* cdf(N, -d2)) - (input_S * exp((b - r)*T) * cdf(N, -d1));
}

//put-call parity check
bool EuropeanOption::ParityCheck() const
{
	return (CallPrice(S)+K*exp(-r*T)) == (PutPrice(S)+S);
}


//calculate the option price by put-call parity
void EuropeanOption::ParityCalculation() const
{
	cout << "C= " << CallPrice(S) << "," << "P= " << (CallPrice(S) + K*exp(-r*T)-S) << endl;
}


//compute option prices for a monotonically increasing range of underlying values of S
void EuropeanOption::Pricing_By_Stock() 
{
	vector<double>::const_iterator i;//declare the iterator
	vector<double> Stock=mesharray(10,100,5);//Set a array of stock prices that from 10 to 100, interval=5.
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
	cout << "variation of S" << endl;

	//output the option price
	for (unsigned int j = 0; j < CallPrice_S.size(); j++)
	{
		cout << "C= " << CallPrice_S[j] << "," << "P= " << PutPrice_S[j] << endl;
	}
	
}

//compute option prices with different T,K,sig,r
void EuropeanOption::Pricing_By_Parameter()
{
	vector<double>::const_iterator i;//declare the iterator

	vector<vector<double>> OptionParameter;//declare the matrix for option parameters
	OptionParameter.push_back(mesharray(0.1, 1, 0.1));//assign the value of T to matrix (from 0.1 to 1, interval=0.1)
	OptionParameter.push_back(mesharray(10, 100,10));//assign the value of K to matrix (from 10 to 100, interval=10)
	OptionParameter.push_back(mesharray(0.1, 1, 0.1));//assign the value of sigma to matrix (from 0.1 to 1, interval=0.1)
	OptionParameter.push_back(mesharray(0.01, 0.1, 0.01));//assign the value of r to matrix (from 0.01 to 0.1, interval=0.01)
	OptionParameter.push_back(mesharray(0.01, 0.1, 0.01));//assign the value of b to matrix (from 0.01 to 0.1, interval=0.01)

	vector<vector<double>> CallPrice_Parameter(5,vector<double>());//declare the matrix that store call option price
	vector<vector<double>> PutPrice_Parameter(5, vector<double>());//declare the matrix that store put option price

	//three total loops for three parameters
	for (int j=0;j<=4;j++)
	{ 
		//assign option price to matrix
		for (i = OptionParameter[j].begin(); i != OptionParameter[j].end(); i++)
		{
			
			if (j == 0)
				T = *i;//first loop, change T
			else if (j == 1)
				K = *i;//second loop, change K
			else if (j == 2)
				sig = *i;//3rd loop, change sigma
			else if (j == 3)
				r = *i;//4th loop, change r
			else
				b = *i;//5th loop, change b

			CallPrice_Parameter[j].push_back(CallPrice(S));
			PutPrice_Parameter[j].push_back(PutPrice(S));
		}
		
		//explanation
		if (j == 0)
			cout << "variation of T" << endl;
		else if (j == 1)
			cout << "variation of K" << endl;
		else if (j == 2)
			cout << "variation of sigma" << endl;
		else if (j == 3)
			cout << "varaition of r" << endl;
		else
			cout << "variation of b" << endl;
		//output the option price
		for (unsigned int k = 0; k < CallPrice_Parameter[j].size(); k++)
		{
			cout << "C= " << CallPrice_Parameter[j][k] << "," << "P= " << PutPrice_Parameter[j][k] << endl;
		}
	}

}

//Greeks (Delta and Gamma) for put and call option
double EuropeanOption::CallDelta() const
{
	//create a normal distribution
	using namespace boost::math;
	normal_distribution<> N;
	
	double tmp =sig*sqrt(T);
	double d1 = (log(S / K) + (b + (sig*sig)*0.5) * T) / tmp;
	return exp((b - r)*T) * cdf(N,d1);
}

double EuropeanOption::PutDelta() const
{
	//create a normal distribution
	using namespace boost::math;
	normal_distribution<> N;
	
	double tmp = sig * sqrt(T);
	double d1 = (log(S / K) + (b + (sig*sig)*0.5) * T) / tmp;
	return exp((b- r)*T) * (cdf(N,d1) - 1.0);
}

double EuropeanOption::CallGamma() const
{
	//create a normal distribution
	using namespace boost::math;
	normal_distribution<> N;
	
	double tmp = sig * sqrt(T);
	double d1 = (log(S/ K) + (b + (sig*sig)*0.5) * T) / tmp;
	double d2 = d1 - tmp;
	return (cdf(N,d1) * exp((b - r)*T)) / (S * tmp);
}

double EuropeanOption::PutGamma() const
{
	return CallGamma();
}

//compute Delta for a monotonically increasing range of underlying values of S
void EuropeanOption::Delta_By_Stock()
{
	vector<double>::const_iterator i;//declare the iterator
	vector<double> Stock = mesharray(10, 105, 5);//Set a array of stock prices that from 10 to 105, interval=5.
	vector<double> CallDelta_S;//declare the vector that store call delta
	vector<double> PutDelta_S;//declare the vector that store put delta

	//assign Delta to vectors
	for (i = Stock.begin(); i != Stock.end(); i++)
	{
		S = *i;//change the stock price for each loop
		CallDelta_S.push_back(CallDelta());
		PutDelta_S.push_back(PutDelta());
	}

	//explanation
	cout << "variation of S" << endl;

	//output the delta
	for (unsigned int j = 0; j < CallDelta_S.size(); j++)
	{
		cout << "C_Delta= " << CallDelta_S[j] << "," << "P_Delta= " << PutDelta_S[j] << endl;
	}

}

//compute Delta with different T,K,sig,r
void EuropeanOption::Delta_By_Parameter()
{
	vector<double>::const_iterator i;//declare the iterator

	vector<vector<double>> OptionParameter;//declare the matrix for option parameters
	OptionParameter.push_back(mesharray(0.1, 1, 0.1));//assign the value of T to matrix (from 0.1 to 1, interval=0.1)
	OptionParameter.push_back(mesharray(10, 100, 10));//assign the value of K to matrix (from 10 to 100, interval=10)
	OptionParameter.push_back(mesharray(0.1, 1, 0.1));//assign the value of sigma to matrix (from 0.1 to 1, interval=0.1)
	OptionParameter.push_back(mesharray(0.01, 0.1, 0.01));//assign the value of r to matrix (from 0.01 to 0.1, interval=0.01)
	OptionParameter.push_back(mesharray(0.01, 0.1, 0.01));//assign the value of b to matrix (from 0.01 to 0.1, interval=0.01)


	vector<vector<double>> CallDelta_Parameter(5, vector<double>());//declare the matrix that store call delta
	vector<vector<double>> PutDelta_Parameter(5, vector<double>());//declare the matrix that store put delta

	//three total loops for three parameters
	for (int j = 0; j <= 4; j++)
	{
		//assign delta to matrix
		for (i = OptionParameter[j].begin(); i != OptionParameter[j].end(); i++)
		{

			if (j == 0)
				T = *i;//first loop, change T
			else if (j == 1)
				K = *i;//second loop, change K
			else if (j == 2)
				sig = *i;//3rd loop, change sigma
			else if (j == 3)
				r = *i;//4th loop, change r
			else
				b = *i;//5th loop, change b


			CallDelta_Parameter[j].push_back(CallDelta());
			PutDelta_Parameter[j].push_back(PutDelta());
		}

		//explanation
		if (j == 0)
			cout << "variation of T" << endl;
		else if (j == 1)
			cout << "variation of K" << endl;
		else if (j == 2)
			cout << "variation of sigma" << endl;
		else if (j == 3)
			cout << "varaition of r" << endl;
		else
			cout << "variation of b" << endl;


		//output the delta
		for (unsigned int k = 0; k < CallDelta_Parameter[j].size(); k++)
		{
			cout << "C_Delta= " << CallDelta_Parameter[j][k] << "," << "P_Delta= " << PutDelta_Parameter[j][k] << endl;
		}
	}

}

//compute Gamma with different T,K,sig
void EuropeanOption::Gamma_By_Parameter()
{
	vector<double>::const_iterator i;//declare the iterator

	vector<vector<double>> OptionParameter;//declare the matrix for option parameters
	OptionParameter.push_back(mesharray(0.1, 1, 0.1));//assign the value of T to matrix (from 0.1 to 1, interval=0.1)
	OptionParameter.push_back(mesharray(10, 100, 10));//assign the value of K to matrix (from 10 to 100, interval=10)
	OptionParameter.push_back(mesharray(0.1, 1, 0.1));//assign the value of sigma to matrix (from 0.1 to 1, interval=0.1)
	OptionParameter.push_back(mesharray(0.01, 0.1, 0.01));//assign the value of r to matrix (from 0.01 to 0.1, interval=0.01)
	OptionParameter.push_back(mesharray(0.01, 0.1, 0.01));//assign the value of b to matrix (from 0.01 to 0.1, interval=0.01)

	vector<vector<double>> CallGamma_Parameter(5, vector<double>());//declare the matrix that store call gamma
	vector<vector<double>> PutGamma_Parameter(5, vector<double>());//declare the matrix that store put gamma

	//three total loops for three parameters
	for (int j = 0; j <=4; j++)
	{
		//assign gamma to matrix
		for (i = OptionParameter[j].begin(); i != OptionParameter[j].end(); i++)
		{


			if (j == 0)
				T = *i;//first loop, change T
			else if (j == 1)
				K = *i;//second loop, change K
			else if (j == 2)
				sig = *i;//3rd loop, change sigma
			else if (j == 3)
				r = *i;//4th loop, change r
			else
				b = *i;//5th loop, change b


			CallGamma_Parameter[j].push_back(CallGamma());
			PutGamma_Parameter[j].push_back(PutGamma());
		}

		//explanation
		if (j == 0)
			cout << "variation of T" << endl;
		else if (j == 1)
			cout << "variation of K" << endl;
		else if (j == 2)
			cout << "variation of sigma" << endl;
		else if (j == 3)
			cout << "varaition of r" << endl;
		else
			cout << "variation of b" << endl;

		//output the delta
		for (unsigned int k = 0; k < CallGamma_Parameter[j].size(); k++)
		{
			cout << "C_Gamma= " << CallGamma_Parameter[j][k] << "," << "P_Gamma= " << PutGamma_Parameter[j][k] << endl;
		}
	}

}

//Approximate Delta 
void EuropeanOption::Approximate_Delta(double h)
{
	vector<double>::const_iterator i;//declare the iterator
	vector<double> Stock = mesharray(10, 105, 5);//Set a array of stock prices that from 10 to 105, interval=5.
	vector<double> CallDelta_S;//declare the vector that store call delta
	vector<double> PutDelta_S;//declare the vector that store put delta

	//assign Delta to vectors
	for (i = Stock.begin(); i != Stock.end(); i++)
	{
		S = *i;//change the stock price for each loop
		
		//using divided differences 
		double C_Delta = (CallPrice((S+ h)) - CallPrice((S - h))) / (2 * h);
		double P_Delta = (PutPrice((S + h)) - PutPrice((S - h))) / (2 * h);
		
		CallDelta_S.push_back(C_Delta);
		PutDelta_S.push_back(P_Delta);
	}

	//explanation
	cout << "variation of S and using divided differences approximation" <<"and h= "<<h<< endl;

	//output the delta
	for (unsigned int j = 0; j < CallDelta_S.size(); j++)
	{
		cout << "C_Delta= " << CallDelta_S[j] << "," << "P_Delta= " << PutDelta_S[j] << endl;
	}

}

//Approximate Gamma
void EuropeanOption::Approximate_Gamma(double h)
{
	vector<double>::const_iterator i;//declare the iterator
	vector<double> Stock = mesharray(10, 105, 5);//Set a array of stock prices that from 10 to 105, interval=5.
	vector<double> CallGamma_S;//declare the vector that store call gamma
	vector<double> PutGamma_S;//declare the vector that store put gamma

	//assign Delta to vectors
	for (i = Stock.begin(); i != Stock.end(); i++)
	{
		S = *i;//change the stock price for each loop

		//using divided differences 
		double C_Gamma = (CallPrice((S + h)) + CallPrice((S - h))-2*CallPrice(S)) / (h * h);
		double P_Gamma = C_Gamma;

		CallGamma_S.push_back(C_Gamma);
		PutGamma_S.push_back(P_Gamma);
	}

	//explanation
	cout << "variation of S and using divided differences approximation" << "and h= " << h << endl;

	//output the delta
	for (unsigned int j = 0; j < CallGamma_S.size(); j++)
	{
		cout << "C_Gamma= " << CallGamma_S[j] << "," << "P_Gamma= " << PutGamma_S[j] << endl;
	}

}

// global function for mesh array 
vector<double> mesharray(double begin, double end, double size)
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
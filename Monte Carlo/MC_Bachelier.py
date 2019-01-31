import numpy as np
import matplotlib.pyplot as plt  
import statsmodels.api as sm

class Bachelier():
    def __init__(self,r = 0,sigma = 10,S0 = 100,K = 100,T = 1,n_steps = 100,N = 10000):
        self.r = r
        self.sigma = sigma
        self.S0 = S0
        self.K = K
        self.T = T
        self.n_steps = n_steps
        self.N = N
    
    # Monte Carlo for all of simulated paths    
    def mc_simu(self,S0):
        dt = self.T/self.n_steps
        all_simu = []
        for i in range(self.N):
            simu = [S0]
            for j in range(self.n_steps):
                dW = np.random.normal(0,np.sqrt(dt))
                simu.append(self.sigma*dW + simu[j])
            all_simu.append(simu)
        return np.array(all_simu)
    
    # lookback put option price
    def lookback_put(self,S0):
        min_S = np.min(self.mc_simu(S0),axis = 1)
        payoff = [np.maximum(self.K - S, 0) for S in min_S]
        P = 1 / self.N * np.sum(payoff) * np.exp(-self.r * self.T)
        return P
    
    # delta by FDM
    def delta(self,eps):
        return (self.lookback_put(self.S0 + eps) - self.lookback_put(self.S0 - eps))/(2 * eps)
        
        
## (b) ##
bachelier_model = Bachelier() 
end_simu = bachelier_model.mc_simu(100)[:,-1]    
plt.figure()
plt.hist(end_simu)
plt.xlabel("ending value of simulation")
plt.title("Q2(b):Histogram of ending value for simulations")
print("p value of Kolmogorov-Smirnov test for ending value of simulations: ",sm.stats.diagnostic.kstest_normal(end_simu )[1])

## (c) ##
put = bachelier_model.lookback_put(100)
print("price of a Lookback put option:",put)

## (d) ##
eps_list = [0.01,0.1,0.3,0.5,1,3,5,10]
delta_list = []
for eps in eps_list:
    delta_list.append(bachelier_model.delta(eps))

plt.figure()
plt.plot(eps_list, delta_list)
plt.xlabel("eps")
plt.ylabel("delta")
plt.title("delta vs eps")    
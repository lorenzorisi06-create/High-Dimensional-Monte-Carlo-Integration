# High-Dimensional-Monte-Carlo-Integration
A comparative study between Monte Carlo methods and deterministic quadrature. This project explores the "Curse of Dimensionality", demonstrating through error analysis and visualization how stochastic sampling achieves a convergence rate independent of spatial dimension d, overcoming the efficiency breakdown of grid-based methods.
# Monte Carlo PI Estimation

A C-based implementation of the Monte Carlo method to estimate the value of $\pi$ through stochastic sampling. This project serves as an introductory exploration of numerical integration and the convergence properties of Monte Carlo algorithms.

## Theoretical Background

The estimation is based on the ratio between the area of a unit circle and its circumscribed square. By generating $N$ uniform random points $(x, y)$ in the domain $[-1, 1] \times [-1, 1]$, the probability of a point falling inside the circle ($x^2 + y^2 \leq 1$) is:

$$P(\text{inside}) = \frac{\text{Area of Circle}}{\text{Area of Square}} = \frac{\pi \cdot r^2}{(2r)^2} = \frac{\pi}{4}$$

Therefore, we can estimate $\pi$ as:
$$\pi \approx 4 \cdot \frac{\text{hits}}{N}$$

### Convergence and Error
Unlike deterministic quadrature (grid-based integration), the Monte Carlo method has a convergence rate of $O(1/\sqrt{N})$, which is independent of the number of dimensions $d$. This makes it the superior choice for high-dimensional problems, overcoming the **Curse of Dimensionality**.



## Features
- **Real-time Estimation**: Computes $\pi$ and absolute error for every iteration.
- **Data Persistence**: Saves convergence data for post-processing.
- **Visual Analysis**: 
    - Log-log plots comparing empirical error with the theoretical $1/\sqrt{N}$ trend.
    - Stability plots for the $\pi$ estimate.
    - Real-time sampling animation via Gnuplot.

## Requirements
- `gcc` (C compiler)
- `gnuplot` (for visualization)

## How to Run
1. Clone the repository:
   ```bash
   git clone [https://github.com/tuo-username/nome-repo.git](https://github.com/tuo-username/nome-repo.git)
   cd nome-repo

2. Compile the source code:
  gcc montecarlo_pi.c -o mc_pi -lm

3. Run the simulation:
   ./mc_pi -lm

## Mathematical Proof

The validity of the Monte Carlo method is rooted in two fundamental pillars of probability theory:

### 1. The Law of Large Numbers (LLN)
Let $X_1, X_2, \dots, X_N$ be independent and identically distributed (i.i.d.) random variables. In our case, $X_i$ is an indicator variable:
$$X_i = 
\begin{cases} 
1 & \text{if point } p_i \in \text{Circle} \\
0 & \text{otherwise}
\end{cases}$$
The Strong Law of Large Numbers states that the sample mean $\bar{X}_N$ converges almost surely to the expected value $E[X]$ as $N \to \infty$:
$$\bar{X}_N = \frac{1}{N} \sum_{i=1}^{N} X_i \xrightarrow{a.s.} E[X] = P(\text{inside})$$
Since $P(\text{inside}) = \pi/4$, then $4 \cdot \bar{X}_N \to \pi$.

### 2. Central Limit Theorem (CLT) and Error Estimation
The CLT provides the distribution of the error. For large $N$, the distribution of the estimate follows a Normal distribution:
$$\frac{\bar{X}_N - \mu}{\sigma / \sqrt{N}} \xrightarrow{d} \mathcal{N}(0,1)$$
This implies that the standard deviation of our estimate (the error) scales with:
$$\text{Error} \approx \frac{\sigma}{\sqrt{N}}$$
This confirms that to gain one extra digit of precision (reduce error by 10), we need to increase the number of samples $N$ by a factor of 100.

---

## Future Improvements
- [ ] Implement **Importance Sampling** to reduce variance.
- [ ] Extend the integration logic to $d$-dimensional hyperspheres.
- [ ] Apply **Markov Chain Monte Carlo (MCMC)** algorithms for sampling from non-uniform distributions.


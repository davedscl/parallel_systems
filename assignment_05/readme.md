# Assignment 5, due November 30th 2020

Buchauer, De Sclavis, Handl


The goal of this assignment is to implement an n-body-type (lagrangian) simulation and consider its optimization potential.

## Exercise 1

This exercise consists in implementing a sequential 2D n-body simulation.

### Description

N-body simulations form a large class of scientific applications, as they are used in research ranging from astrophysics to molecular dynamics. At their core, they model and simulate the interaction of moving particles in physical space. For this assignment, the specific n-body setting relates to astrophysics, where the mutual graviational effect of stars is investigated. Each particle has several properties which include at least
- position,
- velocity, and
- mass.

For each timestep (you can assume e.g. `dt = 0.05`), particles must be moved by first computing the force excerted on them according to the [Newtonian equation for gravity](https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation) - since we are working in two-dimensional space, are working with the altered equation `force = G * (mass_1 * mass_2) / radius`, where `G` is the gravitational constant (and can be assumed as `G = 1` for simplicity). Second, using the computed force on a particle, its position and velocity can be updated via `velocity = velocity + force / mass` and `position = position + velocity * dt`.

### Tasks

- Provide a sequential implementation of the n-body simulation in 2D. Hints on how to proceed (not mandatory to follow):
	1. generate particles randomly, e.g. uniformly distributed
	2. provide a function for computing forces and moving particles
	3. move particles in a time loop for a given number of steps
- Measure the execution time for various problem sizes. What can you observe?
- Add your best sequential wall time for 10000 particles and 1000 time steps into the comparison spreadsheet linked on Discord.

## Exercise 2

This exercise consists in investigating and planning optimization and parallelization techniques for Exercise 1.

### Tasks

- Study the nature of the problem in Exercise 1, focusing on its characteristics with regard to optimization and parallelization.
The n-body problem is about simulating the movement of N particles in space induced by the gravitational forces between them. In theory
all N particles of the whole space have to be taken into account. This fact leads to the necessity to compute pairwise all forces to get
the most accurate result. However, in practice particles far away influence each other only so little that they can be ignored. As a result the number
of pairwise computations can be reduced. Relating to the parallelization of this problem it is, again, for the theoretical case where all interact with each other required that after each timestep all particles have to be exchanged. But only ranks that serve spatially close elements have to synchronize their data. 

- What optimization methods can you come up with in order to improve the performance of Exercise 1?
We had several ideas. In the first place we have tried to reduce the number of expensive operations like pow() and so on by rewriting the operations.
Then we focues on a minimal data traffic by passing pointers instead of whole structs. Later we have tried to reduce the number of the expensive computations of the force by compute it only for one pair, store the result and reuse the result inversed for the other case. So when force(A, B) = 10 we can use force(B, A) = -10. In our implementation it does not improved the performance, however.
Another idea is to ignore particles that are far away. So depending on how accurate the result should be, we can leave out particles that have a bigger distance and have less mass than a certain theshold.
- What parallelization strategies would you consider for Exercise 1 and why?
The most simple way to parallelize the problem would be to split the space into slices or squares and exchange all particles. This is, however,
rather impractical. Since the effect on pairs of particles depends on the distance that is the same if the mass is equal when the radius is the same, the importance of particles decreases in circle. So squares are the most suiteable form for splitting the domain. In addition it is also only necessary to communicate with near neighbors, because further distanced ranks are not of interests since their effect can be ignored. The density of each square can vary throughout the simulation, hence, a dynamic load balancing is probably the most suitable one.   


## General Notes

All the material required by the tasks above (e.g. code, figures, etc...) must be part of the solution that is handed in. Your experiments should be reproducible and comparable to your own measurements using the solution materials that you hand in. For source code, please provide a makefile or other, intuitive means of compiling with the required flags and settings.

**Every** member of your group must be able to explain the given problem, your solution, and possible findings. You may also need to answer detailed questions about any of these aspects.

**Please run any benchmarks or heavy CPU loads only on the compute nodes, not on the login node.**
If you want to do some interactive experimentation, use an *interactive job* as outlined in the tutorial. Make sure to stop any interactive jobs once you are done.
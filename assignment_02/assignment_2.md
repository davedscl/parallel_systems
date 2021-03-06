# Assignment 2, due November 9th 2020
Buchauer Manuel, De Sclavis Davide, Handl Philip 


## Exercise 1

This exercise consists in writing a parallel application to speed up the computation of π.

### Description

There are many ways of approximating π, one being a well-known Monte Carlo method: The ratio of the areas of a square and its incircle is π/4. Since the exact area of a circle cannot be computed (we don't know the value of π yet), one can instead sample random points, check their distance from the center and compute the ratio of points inside the circle to all sampled points.

<img src="https://upload.wikimedia.org/wikipedia/commons/2/20/MonteCarloIntegrationCircle.svg" width="40%">

### Tasks

####  1) Write a sequential application `pi_seq` in C or C++ that computes π for a given number of samples (command line argument). Test your application for various, large sample sizes to verify the correctness of your implementation.
We know that area of the square is 1 and the area of the circle is π/4.
Now for a very large number of randomly created generated points we have:


<img src="./pictures/formula1.png"> 

and therefore we get:


<img src="./pictures/formula2.png"> 


| Samples                   | π (1st run)    | π (2nd run)    | π (3rd run)    | π (4th run)    | π (5th run)    | π (avg)    |      
|---------------------------|----------------|----------------|----------------|----------------|----------------|------------|
|100                        |3.16            |3.2             |3.12            |3.16            |3.24            |3.176       |
|1000                       |3.208           |3.148           |3.148           |3.136           |3.068           |3.1416      |
|10000                      |3.15            |3.1208          |3.1168          |3.1168          |3.1416          |3.1292      |
|100000                     |3.13928         |3.14852         |3.14852         |3.13764         |3.13404         |3.1416      |
|1000000                    |3.141036        |3.139284        |3.139284        |3.143072        |3.143916        |3,1413184   |
|10000000                   |3.141859        |3.142368        |3.141154        |3.141900        |3.141432        |3,1417426   |
|100000000                  |3.141764        |3.141686        |3.141353        |3.141532        |3.141575        |3,141582    |
|200000000                  |3.141511        |3.141649        |3.141418        |3.141551        |3.141510        |3,1415278   |
|400000000                  |3.141519        |3.141510        |3.141641        |3.141601        |3.141594        |3,141573    |
|800000000                  |3.141500        |3.141670        |3.141576        |3.141597        |3.141631        |3,1415948   |

The values computed by the program is approximating π and therefore we expect our algorithm to work properly.

We have in addition also measured the execution time of the sequential program and got following average execution times:

| Samples                   | average execution times  | execution time (using script that swans only one process)|
|---------------------------|--------------------------|----------------------------------------------------------|
|100                        |too less accuracy         |too less accuracy                                         |
|1000                       |too less accuracy         |too less accuracy                                         |
|10000                      |too less accuracy         |too less accuracy                                         |
|100000                     |too less accuracy         |too less accuracy                                         |
|1000000                    |0.04                      |0.16                                                      |
|10000000                   |0.41                      |1.14                                                      |
|100000000                  |4.23                      |10.910                                                    |
|200000000                  |10.87                     |21.76                                                     |
|400000000                  |17.71                     |43.55                                                     |
|800000000                  |34.37                     |86.89                                                     |

As we can obtain from this table the time complexity is as expected O(n).

#### 2) Consider a parallelization strategy using MPI. Which communication pattern(s) would you choose and why?

For this task it is necessary to create as many random points as possible to rise the accuracy. Since it does not matter
in which order they were generated and they do not depend on each other the number of samples N can simply be divided into the number of 
available ranks R and each rank simply generates N/R points and check whether they are in- or outliers. The resulting subresults = number of inliers are reduced by summing them up by one root node. So as a communication pattern "reduction to one" is used because we only need one result. This means, not all 
nodes require all subresults. "Gathering to one" would be possible but is a worse strategy because summing the intermediate results up can be made directly and not by the root node programmatically on our own.

#### 3) Implement your chosen parallelization strategy as a second application `pi_mpi`. Run it with varying numbers of ranks and sample sizes and verify its correctness by comparing the output to `pi_seq`.

Before executing the Makefile load openmpi with: `module load openmpi/3.1.1`

| Samples                   | 2 ranks(avg of 3 runs)     | 4 ranks (avg of 3 runs)   |      
|---------------------------|----------------------------|---------------------------|
|100                        |3.17333                     |3.213333                   |
|1000                       |3.10666                     |3.13733                    | 
|10000                      |3.1358                      |3.1342                     |
|100000                     |3.14184                     |3.14036                    |
|1000000                    |3.140050667                 |3.142745333                |
|10000000                   |3.141403333                 |3,14145833333              |
|100000000                  |3.141581                    |3.141630333                |
|200000000                  |3.141523                    |3.141597333                |
|400000000                  |3.141575667                 |3.141457667                |
|800000000                  |3.141617                    |3.141548667                |

| Samples                   | average execution times (2 ranks) - avg of 3 runs   | average execution times (4 ranks) - avg of 3 runs  |
|---------------------------|-----------------------------------------------------|----------------------------------------------------|
|100                        |0.16                                                 |0.1033                                              |
|1000                       |0.09666                                              |0.13333                                             |
|10000                      |0.09                                                 |0.075                                               |
|100000                     |0.08                                                 |0.085                                               |
|1000000                    |0.13                                                 |0.11                                                |
|10000000                   |0.62                                                 |0.376666667                                         |
|100000000                  |5.6                                                  |2.81                                                |
|200000000                  |11.9                                                 |5.576666667                                         |
|400000000                  |22.0                                                 |11.04                                               |
|800000000                  |43.9                                                 |22.04                                               |

#### 4) Discuss the effects and implications of your parallelization.
The data is not depending on each other. Hence, each rank can work independently and no synchronization is necessary. Since each rank just have to
determine random numbers and compute the sum of its subpart, the task can be distributed well over several ranks without producing a huge overhead. 
Therefore we expected a clear performance gain. We have also tried to measure the execution time, however, the numbers of the table for the parallel programm
should not be considered too seriously because we struggeled to measure the parallel execution time such that it measures the correct part.

In the tables above it can be seen that the sequential approach is more accurate than the parallel one if the sample sizes are the same. In our opinion this is an effect of the random number generation which uses the current time as seed. Therefore some random numbers are more likely the same for different ranks and the results are not that good due to duplicates. Duplicates in this algorithms have no additional information gain and can be seen as reducing the number of iterations. However, for big sample sizes the sequential program is no option because of the time it takes.


## Exercise 2

This exercise consists in parallelizing an application simulating the propagation of heat.

### Description

A large class of scientific applications are so-called stencil applications. These simulate time-dependent physical processes such as the propagation of heat or pressure in a given medium. The core of the simulation operates on a grid and updates each cell with information from its neighbor cells.

### Tasks

#### 1) A sequential implementation of a 1-D heat stencil is available in [heat_stencil_1D_seq.c](heat_stencil_1D/heat_stencil_1D_seq.c). Read the code and make sure you understand what happens. See the Wikipedia article on [Stencil Codes](https://en.wikipedia.org/wiki/Stencil_code) for more information.
The sequential code is working in the following manner:

In the first step the model is initialized by setting the number of cells N and the number of iterations (timesteps) T. There is in addition a vector A that holds all the current cell temperatures and is initialized by 0°C = 273K. 

Then a heatsource is added with 60°C (333K) at position N/4 which means at the left quarter of our space.

Now, when the system is initialized the iterations can start. For each timestep for each cell the new temperature is calculated by adding 10% of the difference to its left and its right neighbour. The values are temporarily stored in a second array B. After finishing all the temperature updates the new vector B replaces the values of A.

This is repeated as long as the timesteps limit T is not reached. At the end a check is done if all cell values are between 273K - 333K. 


#### 2) Consider a parallelization strategy using MPI. Which communication pattern(s) would you choose and why? Are there additional changes required in the code beyond calling MPI functions? If so, elaborate!

In comparison to the previous exercise we have dependent data here. So the only thing we found that is parallelizable is the computation of 
the new vector B.

At the beginning of each timestep iteration computation the current A vector, containing the latest states of the cells, is "broadcasted" to all 
nodes. Each node is then responsible for computing the heat stencil for the i-th subpart of A of size number_of_cells/number_of_ranks.

After computing the subresults the root node is "gathering" all subresults and updates its A vector for the next iteration.


In the code several changes were required.
Lines 42-46:
Here, all organizatorial stuff for MPI is set, such that the programm can be really executed in parallel

Lines 49-59:
For the parallel programm we need broadcasting and gathering. Since the numbers of ranks does not
necessarily be a multiple of the number of cells the assigned subparts can differ. For this we need one array
"receive_counts" that contains the number of assigned cells for each rank and an array "displs" that contains
the displacement indices where gathering should place them. 

Lines 62-63:
min_index and max_index are defining the interval of the assigned cells from array A to the corresponding rank.

Line 78:
Here, A is broadcasted by the root rank such that all have the latest state of A for the next iteration.

Lines 107-109:
The root rank has to gather the subresults and merge them again for the next iteration.

Lines 111-117 and 122-145:
Only one rank (the root rank) should print the results, otherwise an output chaos would occur

Line 149:
Finalization of the parallel execution setup 

#### 3) Implement your chosen parallelization strategy as a second application `heat_stencil_1D_mpi`. Run it with varying numbers of ranks and problem sizes and verify its correctness by comparing the output to `heat_stencil_1D_seq`.
100: 
SEQ: X                                                                                                                        X
MPI: X                                                                                                                        X

500: 
SEQ: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@X
MPI: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@X

1000: 
SEQ: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%#####XXXXX^^^^^^*******++++++++++=========X
MPI: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%#####XXXXX^^^^^^*******++++++++++=========X

1235:
SEQ: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%####XXXX^^^^^*****++++++========::::::::::::::::X
MPI: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%####XXXX^^^^^*****++++++========::::::::::::::::X

1500: 
SEQ: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%###XXXX^^^^*****+++++======::::::::----------------X
MPI: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%###XXXX^^^^*****+++++======::::::::----------------X

2000:
SEQ: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%###XXX^^^****+++++=====::::::--------...............    X
MPI: X@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%###XXX^^^****+++++=====::::::--------...............    X

10000:
SEQ: X:::====++**^^X##%@@@@@@@@@@@@@@@@@@@@@@@@@@%##X^^*++==:::---.....                                                       X
MPI: X:::====++**^^X##%@@@@@@@@@@@@@@@@@@@@@@@@@@%##X^^*++==:::---.....                                                       X

#### 4) Discuss the effects and implications of your parallelization.
In this case the parallelization was not as simple as in task 1, where we had to deal with independent data. For this task each iteration is
dependent of the previous one and the edges of the intervals of the neighbour ranks. So the splitting of the data and merging has to be organized in 
a more complex way.

One problem here is that the next iteration can only start, if the neighbour ranks have also finished the current iteration and after each iteration 
all faster ranks have to wait for the slower ones. 

We have not precisely measured the execution time but while running the parallel programm was significantly slower than the sequential one. A possible reason can be
that the overhead of sending, receiving,merging and waiting for other ranks after each iteration is more costly than the computation of one iteration. 

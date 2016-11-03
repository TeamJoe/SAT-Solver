# SAT-Solver
A SAT solver I created in 2012, significantly faster than brute force, still no P=NP

This was created to run on Windows, and would need to be rewritten to run on other OSs. 

It ran it a depth limit, and used a basic AI cut paths early.
In most of my tests of 100 variables, it would find the solution or that the solution didn't exist in about 10 seconds. 
I had added a bunch of different functions it would go through, in order to determine which was faster.
I also added many #define to quickly set how you wanted the program to run.
This program was in no way optimized for parralization, as I only use multi-threading to test multiple methods at once.

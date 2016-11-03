# SAT-Solver
A SAT solver I created in 2012, significantly faster than brute force, still no P=NP

This was created to run on Windows, and would need to be rewritten to run on other OSs. 

It ran it a depth limit, and used a basic AI cut paths early.
In most of my tests of 100 variables, it would find the solution or that the solution didn't exist in about 10 seconds. 
I had added a bunch of different functions it would go through, in order to determine which was faster.
I also added many #define to quickly set how you wanted the program to run.
This program was in no way optimized for parralization, as I only use multi-threading to test multiple methods at once.
This program also has the option to find all solutions, rather than just a single solution, which in most cases completes in about the same amount of time.


Output file is listed in the following:

Count_in_the_File, Program_Result(N) (Program_Variable_Tries(N))..., Program_Result, Variable_Count, Clause_Count, File_Location, (Program_Result)Number_of_Results_Found, Variables_In_Solution(N)(Solution(N))...

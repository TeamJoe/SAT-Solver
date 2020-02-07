# SAT-Solver
A SAT solver I created in 2011, made changes in 2012 and 2014, significantly faster than brute force, still no P=NP

This was created to run on Windows using Visual Studios Express 2012, and would need to be rewritten to run on other OSs. 

It runs with depth limits and can be adjusted in Constants.h file.
It uses a basic AI cut paths early.
In most of my tests of 100 variables, it would find the solution or that the solution didn't exist in less than 1 second. 
I had added a bunch of different functions it would go through, in order to determine which was faster.
I also added many #define to quickly set how you wanted the program to run.
This program was in no way optimized for parralization, as I only use multi-threading to test multiple methods at once.
This program also has the option to find all solutions, rather than just a single solution, which in most cases completes in about the same amount of time.


Output file is listed in the following:

Count_in_the_File, Program_Result(N) (Program_Variable_Tries(N)) (Program_Solutions_Found(N)), Program_Result, Variable_Count, Clause_Count, File_Location, Program_Result, Number_of_Results_Found, Compile_Time (Time to read the file), Analyze_Time (PreSolutioning time), Solve_Time (Time it actually spent solving), Clean_Time (Time is spent cleaning up) Total_Time (The actually time it took to run from start to finish)

You can get more SAT files in CNF format here: http://www.satcompetition.org/


-----------------
Some fun math stuff (Number of unique clauses):

2-SAT \
ab \
1 \
ab | ac bc \
1 2 \
ab | ac bc | ad bd cd \
1 2 3 \
Max Clauses -> 2^2 * ∑(1,v)x = 2^2/2! * v(v+1) \
2^2 is added for the differnet possible binary combinations

3-SAT \
abc \
1 \
abc | abd acd | bcd \
1 2 1 \
abc | abd acd | abe ace ade | bcd | bce bde | cde \
1 2 3 1 2 1 \
Max Clauses -> 2^3 * ∑(1,v)∑(1,x)y = 2^3/3! * v(v+1)(v+2) \
2^3 is added for the differnet possible binary combinations

4-SAT \
abcd \
1 \
abcd | abce abde acde | bcde \
1 3 1 \
abcd | abce abde acde | abcf abdf acdf abef acef adef | bcde | bcdf bdef bcef | cdef \
1 3 6 1 3 1 \
Max Clauses -> 2^4 * ∑(1,v)∑(1,x)∑(1,y)z = 2^4/4! * v(v+1)(v+2)(v+3) \
2^4 is added for the differnet possible binary combinations

N-SAT \
Max Clauses -> 2^N/N! ∏(1,N)(v+N-x) = 2^N/N! * v(v + N -1)!/v! =  2^N/N! * (v + N -1)!/(v-1)!

SAT \
(Max caluse size has to be <= v), So \
Max Clauses -> ∑(1,v) 2^x/x! * (v + x -1)!/(v-1)!
-----------------

Ideas:
* Should regularly prune clauses to remove duplicates during solve

--Sifter--
* While evaluating should check for certain graph conditions
** Check if identical clauses exist, and remove those clauses.
** Do graph spiting where possible, and evaluate separately.
*** If a child graph returns false, we know the parent graph is also false
*** Choose the graph that is easiest to solve? Or maybe the one most likely to come back false?
**** Could graph spiting be optimised with caching? 

--Statistics Sorter--
* Can I improve the statistical function?
** Is the score scaling correctly?
** Could scoring be optimized in a different spot in the decision process?
** Is the statistic decider actually better?
* Scoring seems good at finding single solutions, but terrible at evaluating an entire tree
** Should I reorder the sort function?
** Should I add variance to allow the sorter to continue downwards?
*** Two doubles are rarely equal
*** If they are close, it should use other functions anyways?
**** Within 1%, 5%, 10%?

--Connection Sorter--
* Evalute the most connected variable (Highest sibling count)
* Go to sibling of last variable with most connections?
* Go to sibling of last variable with most connections to last variable?

--Split Graph--
* Split graph when graph has disconnections
** This can be done using the SIBLING_CALCULATIONS, and then check if there are variables without specific siblings and their siblings as don't have those siblings.
* Could look for maximum splits
** Function to check for variables with most siblings
** Function to check for variable with most `unique` (in-laws) siblings. Siblings your siblings don't also have
* Can speed up SATState::getSplit() by using maps and adding current sibling map to VariableState

--Caching--
* What is the same cached value?
** Same clause remaining and same variables remaining
** Same clause remaining and a subset of variables remaining
** Superset clause remaining and same variables remaining
** Superset clause remaining and subset of variables remaining
* Store Least clauses with most variables, and look for that cache element?
** How would I easily look that up?
* Is there a way to cache structure without caching values?
** Don't use variable names, should name variables by some more deterministic, like usage, sibling counts, etc
* Is there a way to cache where all the literals are opposite?
** This would have the same solution, just opposite values.
* Cache trees that took more variable attempts then desired. (Difficult branches)
* If part of the graph matches a cache value
** If the cache value has no solutions, we know that the graph has no matches.
*** What are some common, easy to recongize graphs without solutions?
** If the cache value has solutions, we know that the graph could potentially have a solution.



Should include in output
* How many variables were decided on a tie
* How many decisions were decided on a tie
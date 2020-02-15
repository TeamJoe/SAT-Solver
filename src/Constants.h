#ifndef CONSTANTS_H
#define CONSTANTS_H

//#define DEPTH_SAT
#define SPLIT_SAT
//#define BOTTOM_ANALYZE_SAT

#define SIBLING_CALCULATIONS 
#define STATISTICS_STEPS 5

#define PARALLEL
#define LET_ALL_SOLVERS_FINISH
//#define FIND_ALL_SOLUTIONS

#ifdef DEPTH_SAT
	#define ANALYZER analyzeDepthSat
	#define VARIABLE_CREATOR createDepthSatVariable
	#define SOLVER solveDepthSat
	#ifdef _DEBUG
		//#define OUTPUT_INTERMEDIATE_SOLUTION
		#define MAX_DEPTH_LIMIT 300
		#define SET_SIZE 1
	#else
		#define MAX_DEPTH_LIMIT -1
		//#define ORGINAL_SET 8
		//#define SECONDARY_SET 16
		//#define THIRD_SET 8
		#ifdef STATISTICS_STEPS
			#define FOURTH_SET 34
		#else
			#define FOURTH_SET 12
		#endif
		#ifdef STATISTICS_STEPS
			#define SET_SIZE 34
		#else
			#define SET_SIZE 12
		#endif
	#endif
#else
	#ifdef SPLIT_SAT
		#define ANALYZER analyzeSplitSat
		#define VARIABLE_CREATOR createSplitSatVariable
		#define SOLVER solveSplitSat
		#ifdef _DEBUG
			//#define OUTPUT_INTERMEDIATE_SOLUTION
			#define MAX_DEPTH_LIMIT 300
			#define SET_SIZE 1
		#else
			#define MAX_DEPTH_LIMIT -1
			//#define ORGINAL_SET 8
			//#define SECONDARY_SET 16
			//#define THIRD_SET 8
			#ifdef STATISTICS_STEPS
				#define FOURTH_SET 34
			#else
				#define FOURTH_SET 12
			#endif
			#ifdef STATISTICS_STEPS
				#define SET_SIZE 34
			#else
				#define SET_SIZE 12
			#endif
		#endif
	#else
		#define MAX_DEPTH_LIMIT 0
	#endif
#endif

#ifdef BOTTOM_ANALYZE_SAT
	#define ANALYZER analyzeBottomSat
	#define VARIABLE_CREATOR createBottomSatVariable
	#define SOLVER solveBottomSat
	#define SET_SIZE 1
	#ifdef _DEBUG
		//#define OUTPUT_INTERMEDIATE_SOLUTION
	#endif
#endif

enum class SolverState { UNKNOWN_ERROR = -3, TERMINATE_EARLY = -2, MAX_DEPTH_REACHED = -1, NO_SOLUTION_FOUND = 0, SOLUTION_FOUND = 1 };
enum class VariableSolutions { VARIABLE_NO_SOLUTION = 0x7FFFFFFF, MUST_NEGATIVE = -2, VARIABLE_NEGATIVE = -1, VARIABLE_UNKNOWN = 0, VARIABLE_POSITIVE = 1, MUST_POSITIVE = 2 };
enum class SolvedStates { COMPLETED_UNKNOWN = 0x7FFFFFFF, COMPLETED_NO_SOLUTION = -2, NOT_COMPLETED_NO_SOLUTION = -1, NOT_COMPLETED = 0, NOT_COMPLETED_SOLUTION = 1, COMPLETED_SOLUTION = 2 };

#endif
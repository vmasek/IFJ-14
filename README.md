# README #


Makefile usage:

	For testing your modules create source file containing main function.
	If is your source file placed in project directory, testing program
	should be compiled wihout problems.
	Compile program by calling `make` in project directory.

Debugging:

	For enable debugging macros compile program with -DDebug switch
	When using makefile try using this command. It has useful aditional gcc switches and Debug enabled.
	"make -B FLAGS='-DDEBUG -g -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wunreachable-code -Winit-self -Werror-implicit-function-declaration'"

Tests:

	ADVICE:	
	When using test make sure that your normal main(), not the one in tests.c is located in main.c file, not just under your current testing implementation.
	Otherwise it cannot be filtred out and tests will yell.

	Two types of testing were implemented. Direct and by Script.

	Running tests directly:

		`./test_ifj`		will run all of the implemented tests.
		`./test_ifj [args]`	will run all test mentonied in [args].
		
		example:
			`./test_ifj stack` 		will run test for stack
			`./test_ifj kmp stack` 	will run test for kmp and stack

	Using test script:

		All tests are made automatic. 
		Output of tests is coloured to easily spot the error.
		Run `./test` in Bash
		
		Advanced testing:
		
			Use script arguments to specify test:
			
			mem	- test memory allocation/deallocation (valgrind and advanced valgrind)
			debug	- tests will be runned with debugging macros turned on

		Adding tests to script:
			If you want to add new test that you just implemented:
				1.   Open script.
				2.   Add keyword to array.
				2.1. If needed add keyword to array that will redirect test output to file.
				3.   Use.



1337 IFJ projectis
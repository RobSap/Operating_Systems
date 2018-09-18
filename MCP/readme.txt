/////////// Purpose /////////////

This project is the MCP for operating Systems 415 at the University of Oregon.
The purpose of MCP is to run multiple programs at once and schedule which program runs when.


////////// How to run the program /////////

I have a default setup to readin example.txt. Its set up to run using the make file I provided. 

Right now this is what the makefile looks like and how it works:

build:
	gcc MCP.c -g -o MCP -lprocps
run: 
	./MCP example.txt

valgrind:
	valgrind ./MCP example.txt

valgrind2:
	valgrind --leak-check=full ./MCP example.txt


To run this program do the following:
make
make run


To test with runvalgrind:
make valgrind

Example.txt explained
error fish  	-- this should not run at all and tests thats
./batmam	-- this should print in a loop with a 1 second wait between loops
./batman2	-- does the same thing as batman
ls -a		--prints ls stuff
ls -l		--
ls		--
./runhard	-- this is designed to test the cpu (utime and stime)


 

/////////// Purpose /////////////

This project is ATP for Operating Systems 415 at the University of Oregon.
The purpose of ATP is to have ATP act as a server and have multiple publishers and multiple subsribers reading and writing at the same time. They goal is to ensure the server ATP is thread safe, and that mutliple Subscribers and Publishers can run.


///////// Background Information ///////

The assignment was borken up into 6 parts. This sections explains how to check each part 1-6 works:

//For all these 1 is yes 0 is no (1 to see results and 0 for no)
int part2 = 1; //Part 1 and part 2 combined (threading) 
int part3 = 1; // Check out the enqueue and dequeue
int part4 = 1; // See the archive works, see it switches, see whats left in buffer (shows what was saved before deleting)
int part5 = 1; // Prints subscriber read articles and publisher publisher articles

////////// How to run the program /////////

to build the program:
make

to run the program:
make run

to see valgrinds output: (turn off print statements first, its easyer)
make valgrind

To clear saved to disk files:
make clean


Running different m n t values:
Each example below is working versions to fill the requirements of part 6. Due to each setion having certain requirements per section and no set required values, there are limits to prove they work. Some values need to be changed as sub/pub/#topics change. Below I show different options to complete this. Option 1 is the method shown in the example, while option 2 shows another option. But first, Ill explain what each variable controls.

Explenation of each variable, and what it controls
subSize 25		//Number of subscribers
pubSize 10		//Number of Publishers
numTopics 5   		//Number of topics total
topics_pub 5 		//Number of articles published by each publisher 
MAXENTRIES 9	//Max entries in queue, before q needs dequeue before new enqueue
num_topics_subcribed_to 2  //Number of articles each sub reads per topic
articlesread 2 	 // Dequeues the artcile once it was read this amount of times (means old)
		// this can be set to match ...so all 25 subscribers read 1 article before
		// deleting it (requires MAXENTRIES to be +1 over articles read)
IOBUFFERSIZE 4 		//Size of EACH TOPIC ARCHIVE q BEFORE being sent to disk
ENTRYSIZE 256 		//worth of bytes
int timeTillOld = 100; 	// This sets in nano seconds what is considered old to skip/archive



//Best Working Example and default to see all features and parts working.
subSize 25  
pubSize 10
numTopics 5
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 2  
articlesread 2  
IOBUFFERSIZE 4
ENTRYSIZE 256 

//1 pub 1 sub 1 shared topic
//Part 6 testing. A few of these require other values to meet the conditions. The other values represents various limits thoughout the program that was/were abritraily picked. 

//Works fine
subSize 1	
pubSize 1
numTopics 1
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 2  
articlesread 2  
IOBUFFERSIZE 4
ENTRYSIZE 256 

//n pub / 1 sub / 1 shared topic
//Topic Pub must be set to size 1 - this is because (MAXENTRIES < PubSize * topics_pub ) because only 1 subsriber will view article to dequeue them from the queue (as it fills and publishers have to wait for room due to maxentries constraint), and it runs to fast for the system to determine their old to delete. So increase max entries ( PubSize * topics_pub)  or decrease topic pub to 1.
subSize  1	
 pubSize 9  
numTopics 1
topics_pub 1 - Option 1 - this should be set to 1  
MAXENTRIES 9 - Option 2 - this needs to be set to (MAXENTRIES >= PubSize * topics_pub)
num_topics_subcribed_to 2  
articlesread 2  
IOBUFFERSIZE 4
ENTRYSIZE 256 

// 1 pub / m sub / 1 shared topic
// In this case beause articles have limits to how many are read before deleting, we need to increase the articles read feature before deleting, otherwise we delete articles before other subscribers can read them
subsize 10 
pubSize 1
numTopics 1
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 2   -- Option 2 - decrease this to 1
articlesread 4 - Option 1 - Increase this value to 4 (or higher, but remember maxentries+1>articlesread)
IOBUFFERSIZE 4
ENTRYSIZE 256 

//n pub / m sub / 1 shared topics
 // This one publishes 1 topic by 10 publishers and 5 articles each (50 articles). If each of the 25 subscribers reads 2 articles per topic, thats 50 articles total. option 1:   articlesread needs to be at least 50  (subsize * num_topics_subscribe_to) 
subsize 25
pubSize 10  
numTopics 1
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 1 - option 1 set to 1  
articlesread 25 -option 1 set to 25
IOBUFFERSIZE 4
ENTRYSIZE 256 

//1 pub 1 sub 1 shared topic
//Part 6 testing. A few of these require other values to meet the conditions. The other values represents various limits thoughout the program that was/were abritraily picked. 

//Works fine
subSize 1	
pubSize 1
numTopics 2
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 2  
articlesread 2  
IOBUFFERSIZE 4
ENTRYSIZE 256 


n pub / 1 sub / 2 shared topic
//  MAXENTRIES >50 because there are no dequeues from subscribers (just the 1) to make room in the topic queue for more enties.. (default is 10 publishers making 2 topics with 50 articles each
subSize 1
pubSize 10 
numTopics 2
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 2  
articlesread 2  
IOBUFFERSIZE 4
ENTRYSIZE 256 

1 pub / m sub / 2 shared topic
//Due to subscribers reading with one publisher, subs have nothing else to read (with dequeue  removing after being read (articlesread) and thus removing them. So they can run out of articles. Option 1:  If you incrase articlesread to 10 then subsize can go to 10. Or if you lower num_topics_subcribed_to to 1 then subsize can also be 16. If you do both, subsize can esily be 25.
subSize 8 
pubSize 1
numTopics 2
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 2  
articlesread 4 - Option 1 increase to 4
IOBUFFERSIZE 4
ENTRYSIZE 256 

n pub / m sub / t shared topics
//Again my Best Working Example and default , this works best for testing all parts
subSize 25  
pubSize 10
numTopics 5
topics_pub 5 
MAXENTRIES 9	
num_topics_subcribed_to 2  
articlesread 2  
IOBUFFERSIZE 4
ENTRYSIZE 256 






#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

//For part 6 you can change any of these around
#define subSize 25	//Number of subscribers / max 25
#define pubSize 10	//Publishers size / max 9 with maxentries at 9
#define numTopics 5   //Number of topics total

#define topics_pub 5 //Number of articles published by each publisher 

//For part 3
#define MAXENTRIES 9	//Max entries in queue
#define num_topics_subcribed_to 2  //Number of articles each sub reads per topic
#define articlesread 2  // Dequeues the artcile once it was read this amount of times(means old)
		// this can be set to match so all 25 subscribers read 1 article before
		// deleting it (requires max entries to be +1 over articles read)
//Part4
#define IOBUFFERSIZE 4 //Size of EACH TOPIC ARCHIVE BEFORE beding sent to disk
int timeTillOld = 100; // This sets in nano seconds what is considered old to skip/archive

//Part 5
#define ENTRYSIZE 256 //worth of bytes


//Turn on print statements to check sections
//For all these 1 is yes 0 is no
int part2 = 1; //Part 1 and part 2 combined (threading)  - 1 to see results 0 to hide
int part3 = 1; // Check out the enqueue and dequeue
int part4 = 1; // See the archive works, see it switches
int part5 = 1; // Prints subscriber read and publisher put
int archiveAtEnd = 0; // This tells it to send anything left in the buffers to disk (shows part 4 works by only sending once a topic buffer is full)

//Number of topics
const char *myTopics[] = {"computers", "xbox","school", "football", "soccer"};

pthread_t bufferDump2;
pthread_t bufferDump;

//****************************************************************************************
// ******************************** Global Things called  ********************************
//
//
//
//****************************************************************************************



//Connections record struct
struct saveData
	{
	char pid[32]; 		//Publisher pid who wrote about topic
	char articles[256];	// whatever topics the publisher does articles on 
				// whatever topics the subscriber wants to read
	char pipesUsed[4];	//pipes used (index)
	};

//Create a struct for each type of connection record
struct saveData dataPub[pubSize];
struct saveData dataSub[subSize];

//Set countersfor arrays later on
int countPub = 0;
int countSub = 0;

//Other Notes
//10 publishers
//subSize subscribers
// 35 structs

//Set up pid array
	//int size = 1024;
	pid_t pid_server;
	pid_t pid_publisher[pubSize];
	pid_t pid_subscriber[subSize];

//Set up pipes
	int pipe_publisher_to_server[pubSize][2];
	int pipe_server_to_publisher[pubSize][2];
	int pipe_subscriber_to_server[subSize][2];
	int pipe_server_to_subscriber[subSize][2];
	
//****************************************************************************************
// ******************************** Global Queue  ********************************
//
//
//
//****************************************************************************************
struct queue *q[numTopics];//num Topic queues
struct buff *buffer[numTopics][IOBUFFERSIZE]; //two archives
struct buff *buffer2[numTopics][IOBUFFERSIZE]; //two archives

pthread_mutex_t mutex[numTopics];

int buffcount[numTopics];
int buffcount2[numTopics];
int increment[numTopics];

struct buff
{
	char data[ENTRYSIZE];
	char ID[64];
	int timestamp;
	int topicID;
};


//****************************************************************************************
// ******************************** Thead write writes files to disk *********************
//
//
//
//****************************************************************************************

//void *writeFunction(void *input)
void writeFunction(int input)
{

int x = (int)input;
	

//x = 0;
int i=0;
char bud[128];
//pthread_mutex_lock(&mutex[x]);
snprintf(bud, sizeof(char) * 32, "file_%s", myTopics[buffer[x][0]->topicID]);
increment[x]++;


FILE *f = fopen(bud, "a");

for(i=0; i < IOBUFFERSIZE; i++)
{

	
	fprintf(f, "%s\n", buffer[x][i]->ID);
	strcpy(buffer[x][i]->ID, "" );
	fprintf(f, "Topic: %s\n", myTopics[buffer[x][i]->topicID]);
	buffer[x][i]->topicID=0;
	fprintf(f, " %s\n", buffer[x][i]->data);
	strcpy(buffer[x][i]->data, "");
	fprintf(f, "Time Created: %d\n\n", buffer[x][i]->timestamp);
	buffer[x][i]->timestamp = 0;
	

}
//pthread_mutex_lock(&mutex[x]);
fclose(f);


//return 0;
}


//Saves the article information
struct node
{
	struct node *next;
	int timestamp;
	char data[ENTRYSIZE];
	char ID[64];		//Pub ID

	char ID2[64][10]; //ID of people who read article
	int topicID;	 //Which topic this node is
	int count;		//Counter
	pthread_mutex_t lock;
};

struct queue
{
	struct node *head;
	struct node *tail;
	int entriesCount;
	pthread_mutex_t lock;
};


void makeq(struct queue *q)
{
	q->tail=NULL;
	q->head=NULL;
}

struct queue *enqueue(struct queue *q, char article[], char pid[], int topicInfo)
{
	pthread_mutex_lock(&q->lock);	
	q->entriesCount++;
  	  struct node *temp;
   	 temp=(struct node *)malloc(sizeof(struct node));
	strcpy(temp->data,article);
	
	
	temp->topicID=topicInfo;
//printf("\n********Here is the article2 %s", temp->data);

    	strcpy(temp->ID, pid );
	struct timeval tv;
	gettimeofday(&tv, NULL);
   	temp->timestamp =tv.tv_usec;
    
    if(q->head==NULL)
    {
	
        q->head=temp;
        q->tail=temp;
        q->head->next=q->tail->next=NULL;
    }
    else
    {	
        q->tail->next=temp;
        q->tail=temp;
        q->tail->next=NULL;
    }
pthread_mutex_unlock(&q->lock);

    return q;

}

struct queue *printDelete(struct queue *q)
{
    struct node *ptr;
    ptr=q->head;
    if(q->head==NULL)
      {  printf("\n Print Delete, Queue Empty\n");
	}
    else
    {
	//Print links till i find last one
        if(ptr!=q->tail)
        {
int i;
		for(i=0; i <articlesread;i++)
		{
			if(part3==1)
			{printf("\nSub %s Read topic: %s  %s article: %s", ptr->ID2[i], myTopics[ptr->topicID],ptr->ID , ptr->data );}
		}
	


		if(part3==1) //Article: %d*
		printf("\nDeleting: (%d reads) %s Topic: %s Article: %s Timestamp: %d",articlesread,ptr->ID, myTopics[ptr->topicID] , ptr->data, ptr->timestamp);
		
		ptr=ptr->next;
        }
       if(part3==1) 
	printf("\n");
    }
    return q;
}


struct queue *dequeue(struct queue *q, char pid[])
{
sleep(1);
pthread_mutex_lock(&q->lock);

struct node *ptr;
ptr=q->head;	

struct timeval tv2;
gettimeofday(&tv2, NULL);


int i;
		for(i=0; i <ptr->count+1; i++)
		{	
			if(strcmp(pid , ptr->ID2[i])!=0)
			{
			
			}
			else
			{
				
				//printf("\n Print i %d Equal the same %s and %s",i, pid, ptr->ID2[i]);
				strcpy(ptr->ID2[ptr->count], "0" );
				//Do something here to not use this one....
				ptr=q->head->next;
			}

		}

strcpy(ptr->ID2[ptr->count], pid );
ptr->count++;
	if((ptr->count == articlesread) ||(ptr->timestamp-(int)tv2.tv_usec) > timeTillOld)
	{
		if(q->head==NULL)
		{
			printf("\nQueue Empty\n");
		}
		else
		{
  	 	
			if(buffcount[ptr->topicID]<IOBUFFERSIZE && increment[ptr->topicID]==0)
			{	

				//printf("\n******************************Topic ID %d  Buffer Count%d Topic ID %d  ", ptr->topicID, buffcount[ptr->topicID], ptr->topicID);
		buffer[ptr->topicID][buffcount[ptr->topicID]]=(struct buff *)malloc(sizeof(struct buff));
		strcpy(buffer[ptr->topicID][buffcount[ptr->topicID]]->data , ptr->data);
		buffer[ptr->topicID][buffcount[ptr->topicID]]->topicID=ptr->topicID;
		buffer[ptr->topicID][buffcount[ptr->topicID]]->timestamp=ptr->timestamp;
		strcpy(buffer[ptr->topicID][buffcount[ptr->topicID]]->ID, ptr->ID );
	
				buffcount[ptr->topicID]++;
				
				if(buffcount[ptr->topicID]==IOBUFFERSIZE)
				{

					writeFunction(ptr->topicID);
					increment[ptr->topicID] = 1;
					buffcount[ptr->topicID]=0;
				}	
			
			}
		
			else if(buffcount2[ptr->topicID]<IOBUFFERSIZE||increment[ptr->topicID]==1)
			{

			
			buffer2[ptr->topicID][buffcount2[ptr->topicID]]=(struct buff *)malloc(sizeof(struct buff));
		
			
			strcpy(buffer2[ptr->topicID][buffcount2[ptr->topicID]]->data , ptr->data );
			buffer2[ptr->topicID][buffcount2[ptr->topicID]]->topicID=ptr->topicID;
			buffer2[ptr->topicID][buffcount2[ptr->topicID]]->timestamp=ptr->timestamp;
			strcpy(buffer2[ptr->topicID][buffcount2[ptr->topicID]]->ID, ptr->ID );
			
			buffcount2[ptr->topicID]++;


				if(buffcount2[ptr->topicID]==IOBUFFERSIZE)
					{
						writeFunction(ptr->topicID);
						increment[ptr->topicID] =0;
						buffcount2[ptr->topicID]=0;
					}
   		 	}

		//pthread_mutex_lock(&q->lock);
			if(part3==1)
			{
			printf("\nI had %d subscribers read, now showing who read this topics article: " , articlesread );
			printDelete(q);
			}
			q->head=q->head->next;
		//pthread_mutex_unlock(&q->lock);
			free(ptr);
		
		}
	q->entriesCount--;
	pthread_mutex_unlock(&q->lock);
	return q;
	}	
	else
	{
		//printf("\nI dont have count=articlesread,  count = %d", ptr->count );
	}
pthread_mutex_unlock(&q->lock);
return q;

}

struct queue *print(struct queue *q)
{
    struct node *ptr;
    ptr=q->head;
    if(q->head==NULL)
        printf("\nQueue Empty");
    else
    {
	//Print links till i find last one
        while(ptr!=q->tail)
        {

		if(part3==1)
		{
		printf("\nQueue:%s Topic: %s Article: %s  Timestamp:%d",ptr->ID, myTopics[ptr->topicID] , ptr->data, ptr->timestamp);
		}
		
		ptr=ptr->next;
		//free(ptr);
        }
	//Print the last link
	if(part3==1)
	{
	printf("\nQueue:%s Topic: %s Article: %s  Timestamp:%d",ptr->ID, myTopics[ptr->topicID] ,ptr->data, ptr->timestamp);
	}
	ptr=ptr->next;
		//free(ptr);
        printf("\n");
    	}
    return q;
}


//****************************************************************************************
// ******************************** Thead pub  called from server ********************************
// pub thread thread pub
//
//
//****************************************************************************************

void *pubFunction(void *sPub)
{
			int x = (int)sPub;
			//printf("\nhere is ptr %d" , x);
// Get Pid from pub
			//Close the other end of the pipe
			close(pipe_publisher_to_server[x][1]);
			char buff[256];
			read(pipe_publisher_to_server[x][0], buff , 256 );
			sprintf(dataPub[x].pid, "Pub %s", buff); 
			sprintf(dataPub[x].pipesUsed, "%d", x);

	//Pass Back got info
			//Close other end of pipe
			close(pipe_server_to_publisher[x][0]);
			//Message to pipe
			write(pipe_server_to_publisher[x][1],"accept", sizeof("accept" ));

	// Get topics from sub
			//Close the other end of the pipe
			char buff10[256]; int b; char topics3[100]="";
			char buff67[256];
			//For topics_published
			for (b=0; b<topics_pub; b++)
			{ 
				close(pipe_publisher_to_server[x][1]);
				read(pipe_publisher_to_server[x][0], buff10 , 256 );
				strcat(topics3, buff10 );
				strcat(topics3, " " );
				sprintf(dataPub[x].articles, "%s", topics3);
				

/*******************************************************************/

	//Pass Back got info
				//Close other end of pipe
				close(pipe_server_to_publisher[x][0]);
				//Message to pipe
				write(pipe_server_to_publisher[x][1],"accept", sizeof("accept" ));

				close(pipe_publisher_to_server[x][1]);
				read(pipe_publisher_to_server[x][0], buff67 , 256 );

//printf("\n********** Server got topic %s",buff67);
/*******************************************************************/			
// Create Topics - add them to the enqueue
			 
				while (q[b%numTopics]->entriesCount == MAXENTRIES)
				{//wait for entrie count not to be max
			
				}

				//q[b] specifies that topic
				// b is the random message sent about the topic
				// dataPub[x].pid is the pid for this one
				// x is the publisher we are on ... 1-10
				enqueue(q[b%numTopics],buff67,dataPub[x].pid , b);
					
				
/*******************************************************************/

	//Pass Back got info
				//Close other end of pipe
				close(pipe_server_to_publisher[x][0]);
				//Message to pipe
				write(pipe_server_to_publisher[x][1],"successfull", sizeof("successfull" ));

			}

	// Get end from pub
				//Close the other end of the pipe
				close(pipe_publisher_to_server[x][1]);
				char buff3[256];
				read(pipe_publisher_to_server[x][0], buff3 , 256 );
	//Pass Back got info
				//Close other end of pipe then do message
				close(pipe_server_to_publisher[x][0]);

				write(pipe_server_to_publisher[x][1],"accept", sizeof("accept" ));
	// Get Terminate
				//Close the other end of the pipe
				close(pipe_publisher_to_server[x][1]);
				char buff4[256];
				read(pipe_publisher_to_server[x][0], buff4 , 256 );;
	//Pass Back Terminate
			//Close other end of pipe and message to pipe
			close(pipe_server_to_publisher[x][0]);
			write(pipe_server_to_publisher[x][1],"terminate", sizeof("terminate" ));
	//Close all ends of pipe
			close(pipe_publisher_to_server[x][0]);
			close(pipe_server_to_publisher[x][1]);
	//Print Data	
//********************************
//********************************
 if(part2==1)
	printf("\nTerminated: %s \n", dataPub[x].pid);
//********************************


return NULL;
}


//****************************************************************************************
// ******************************** Thead Sub called from server ********************************
// sub thread sub
//
//
//****************************************************************************************
void *subFunction(void *sSub)
{

int x = (int)sSub;

// Get pid from sub 
			//Close the other end of the pipe
			close(pipe_subscriber_to_server[x][1]);
			char buff5[256];
			read(pipe_subscriber_to_server[x][0], buff5 , 256 );
			sprintf(dataSub[x].pid, "%s", buff5);

			sprintf(dataSub[x].pipesUsed, "%d", x);	   
	//Pass Back got info
			//Close other end of pipe and message to pipe
			close(pipe_server_to_subscriber[x][0]);
			write(pipe_server_to_subscriber[x][1],"accept", sizeof("accept" ));
	// Get topics from sub
			char buff6[256]; int a; char articles2[100]="";
			

			for (a=0; a<num_topics_subcribed_to; a++)
			{
				//This takes in the topics
				close(pipe_subscriber_to_server[x][1]);
				read(pipe_subscriber_to_server[x][0], buff6 , 256 );
		
				strcat(articles2, buff6 );
				strcat(articles2, " " );
				//printf("\n******************************* articles2 %s", articles2);
				sprintf(dataSub[x].articles, "%s", articles2);

//printf("\n************************************** %s ", myTopics[(x+a)%numTopics]); fflush(stderr);

struct node *temp;
				temp=q[(x+a)%numTopics]->head;
				if(temp->data!=NULL)
				{close(pipe_server_to_subscriber[x][0]);
				//Message to pipe	
				write(pipe_server_to_subscriber[x][1],temp->data, sizeof(temp->data));			}
				dequeue(q[(x+a)%numTopics], buff5);
				
				 	
sleep(1);
				


sleep(.5);
	
	//Pass Back got info
			//Close other end of pipe
				close(pipe_server_to_subscriber[x][0]);
				//Message to pipe	
				write(pipe_server_to_subscriber[x][1],"successfull", sizeof("successfull" ));
	
			} //end for loop
		
	// Get end from sub
			//Close the other end of the pipe
			close(pipe_subscriber_to_server[x][1]);
			char buff7[256];
			read(pipe_subscriber_to_server[x][0], buff7 , 256 );
	//Pass Back got info
			//Close other end of pipe
			close(pipe_server_to_subscriber[x][0]);
			write(pipe_server_to_subscriber[x][1],"accept", sizeof("accept" ));

	// Get Terminate
			//Close the other end of the pipe
			close(pipe_subscriber_to_server[x][1]);
			char buff8[256];
			read(pipe_subscriber_to_server[x][0], buff8 , 256 );
	//Pass Back Terminate
			//Close other end of pipe and mesage to pipe
			close(pipe_server_to_subscriber[x][0]);
			write(pipe_server_to_subscriber[x][1],"terminate", sizeof("terminate" ));

	//Then Close it	
			close(pipe_subscriber_to_server[x][0]);
			close(pipe_server_to_subscriber[x][1]);
	//Print Data	
//********************************
//********************************
if(part2==1)
	printf("\nTerminated Sub: %s \n", dataSub[x].pid);
//********************************

return NULL;
}

//****************************************************************************************
// ******************************** MAIN  ********************************
//
//
//
//****************************************************************************************
int main (int argc, char **argv)
{

int i;

	for( i = 0; i<numTopics; i++)
	{
		 buffcount[i]=0;
 		buffcount2[i]=0;
		increment[i]= 0;
	}
//Make the pipes, check for errors

	for( i = 0; i<pubSize; i++)
	{
		int a, b;
		b=pipe(pipe_publisher_to_server[i]);
		a=pipe(pipe_server_to_publisher[i]);
		if((a == -1)||(b==-1))
			perror("Error");
	}
	for( i =0; i <subSize; i++)
	{
		int a, b;
		b=pipe(pipe_subscriber_to_server[i]);
		a= pipe(pipe_server_to_subscriber[i]);
		if((a == -1)||(b==-1))
			perror("Error");
	}

//Make the Queue
int m;
for(m = 0; m < numTopics; m++)
{	
	q[m] = (struct queue *)malloc(sizeof(struct queue));
	makeq(q[m]);
}


/*******************************************************************************
********************************************************************************
				Fork ATP (SERVER)
The ATP server creates a pipe with each publisher and each subscriber process.
It then listens for information on each pipe.
********************************************************************************
********************************************************************************/


	pid_server = fork();


	//If Fork Failed
	if (pid_server < 0)
	{
		//Handle the error appropriatley
		perror("\n fork error");
		exit(0);
	}

	//Fork worked, in child process
	else if (pid_server == 0)
	 {
		pthread_t subThread[subSize];
		pthread_t pubThread[pubSize];
int x,y;
		for(x = 0; x <pubSize; x++)
		{		
			if(pthread_create(&pubThread[x],NULL,pubFunction,(void*)x)!=0)
			{
				fprintf(stderr, "error creating thread\n");
				exit(1);
			}
		}
		
		
		for(x = 0; x <subSize; x++)
		{ 
			if(pthread_create(&subThread[x],NULL,subFunction,(void*)x /*&x*/)!=0)
			{
				fprintf(stderr, "error creating thread\n");
				exit(1);
			}
		}
x=0;
			
		

		for(y=0; y<pubSize; y++)
		{	
			if(pthread_join(pubThread[y],NULL)!=0)
			{
				fprintf(stderr,"Error joining PubThread\n");
				exit(1);	
			}
			if(part2==1)
			printf("\nPub ID: %s\nPipes Used: %s \nTopics wrote : %s\n" , dataPub[y].pid, dataPub[y].pipesUsed, dataPub[y].articles);
		
		}

		for(y=0; y<subSize; y++)
		{	
			if(pthread_join(subThread[y],NULL)!=0)
			{
				fprintf(stderr,"Error joining SubThread\n");
				exit(1);	
			}
			if(part2==1)
			printf("\n Sub ID: %s\nPipes Used: %s \nTopics read: %s\n" , dataSub[y].pid, dataSub[y].pipesUsed, dataSub[y].articles);

		
		}//End of For loop

//See what is in the queue q[b], print queue
// *********************************************************************************
//				Show Unread articles per topic
//
///
// **********************************************************************************
	int b;
		for (b=0; b<numTopics; b++)
		{
			if(part3==1){
			printf("\nShow topic and arcticles the topic has left");
			print(q[b]);}
		}


// *********************************************************************************
//				Print Archive buffers
//
//
// **********************************************************************************
		

//Option to write anything in arhvie to disk before exiting
if(archiveAtEnd==1)
{
printf("\n Sending remaining items in both archives to disk, arhives should show empty \n\n");
	for(i=0; i<numTopics; i++)
	{
		if(buffcount[i]>0)
		{
		writeFunction(i);
		buffcount[i]=0;
		}
	}
	for(i=0; i<numTopics; i++)
	{
		if(buffcount2[i]>0)
		{
		writeFunction(i);
		buffcount2[i]=0;
		}
	}
}

//Show arhive buffers 
		if(part4==1){
		printf("\n%d = total number of articles archived " , (num_topics_subcribed_to*subSize)/articlesread);
		printf("\n List of articles in archive buffers  (not send to disk)");
		}
		
int a;
	for(a=0; a < numTopics; a++)
		{	if(part4==1)
				printf("\nLeft in Topic Archive1 at exit: %s ", myTopics[a]);
			for (b=0; b<buffcount[a]; b++)
			{
			if(part4==1){
			printf("\n Archive1 %s article %s timestamp %d Topic %s " ,  buffer[a][b]->ID,  buffer[a][b]->data, buffer[a][b]->timestamp,  myTopics[buffer[a][b]->topicID]);
		}

			}
		if(part4==1)
		printf("\n ");
			
		}

	for(a=0; a < numTopics; a++)
		{	if(part4==1)
			printf("\nLeft in Topic Archive2 at exit: %s ", myTopics[a]);
			
			for (b=0; b<buffcount2[a]; b++)
			{
			if(part4==1)
			printf("\nArchive2 %s article %s timestamp %d Topic %s " ,  buffer2[a][b]->ID,  buffer2[a][b]->data, buffer2[a][b]->timestamp,  myTopics[buffer2[a][b]->topicID]);
		
			}
		if(part4==1)
		printf("\n ");
		
}

			printf("\n");

for(a=0; a < numTopics; a++)
{
	for(b=0; b <IOBUFFERSIZE; b++)
	{
		free(buffer[a][b]);
		free(buffer2[a][b]);
	}
}
			
		exit(0);
	 }//end else if statement (child process)
	//Parent process
	else 
	{
//Do nothing with parent
	}

/*******************************************************************************
********************************************************************************
				Fork publishers
When each publisher Pi starts up and has established a pipe with the ATP server, it sends the following message:
"pub pubid connect"
"pub pubid topic k" 1 <= k <= t for each topic
"end"

after each string waits  for response from atp server before sending next string
(then it accepts or fails (then terminates) )
********************************************************************************
********************************************************************************/

for(countPub=0; countPub < pubSize; countPub++)
{		
	pid_publisher[countPub] = fork();

//		If Fork Failed
		if (pid_publisher[countPub] < 0)
		{
			//Handle the error appropriatley
			//Need some type of wait here
			perror("\n fork error");
			exit(0);
		}

		//Fork worked, in child process
		else if (pid_publisher[countPub] == 0)
		 {

		//Send Info
			/*Close other end of pipe*/
			close(pipe_publisher_to_server[countPub][0]);
			char pid[10] = "";
			sprintf(pid, "%d", getpid());
			write(pipe_publisher_to_server[countPub][1], pid , sizeof(pid));

		//Confirm other side got it
			//Close the other end of the pipe
			close(pipe_server_to_publisher[countPub][1]);
			char buff[256];
			read(pipe_server_to_publisher[countPub][0], buff , 256 );
		
			while (strcmp(buff, "accept") !=0)
			{		
			 	if (strcmp(buff, "reject") ==0)
				{
					perror("\nrejected read again");
					close(pipe_server_to_publisher[countPub][1]);
					read(pipe_server_to_publisher[countPub][0], buff , 256 );
				}
	
			}
	
		//Send k Topics based on topics array
			int a;
			char stuff[128] = "";
			char stuff67[128] = "";
			char buff2[256];
			char buff52[256];
			for (a=0; a<topics_pub; a++)			
			{
				//Close other end of pipe
				close(pipe_publisher_to_server[countPub][0]);	
				sprintf(stuff, "%s", myTopics[a%numTopics]);
				write(pipe_publisher_to_server[countPub][1],stuff, sizeof(stuff));
				
		//Confirm other side got k topics
				//Close the other end of the pipe
				close(pipe_server_to_publisher[countPub][1]);
			
				read(pipe_server_to_publisher[countPub][0], buff2 , 256 );
				while (strcmp(buff2, "accept") !=0)
				{		
			 		if (strcmp(buff2, "reject") ==0)
					{
						perror("\nrejected read again");
						close(pipe_server_to_publisher[countPub][1]);
						read(pipe_server_to_publisher[countPub][0], buff2 , 256 );
					}
	
				}

//Close other end of pipe
				close(pipe_publisher_to_server[countPub][0]);	
				sprintf(stuff67, "%s %d","here is the article ", a+countPub);
				
				if(part5==1)
				{
				printf("\nPublisher %d sending topic %s article: here is the article %d \n" , getpid(), myTopics[a%numTopics], a);			}
				write(pipe_publisher_to_server[countPub][1],stuff67, sizeof(stuff));
				
		//Confirm other side got k topics
				//Close the other end of the pipe
				close(pipe_server_to_publisher[countPub][1]);
			
				read(pipe_server_to_publisher[countPub][0], buff52 , 256 );
				while (strcmp(buff52, "successfull") !=0)
				{		
			 		if (strcmp(buff52, "retry") ==0)
					{
						perror("\nretry");
						close(pipe_server_to_publisher[countPub][1]);
						read(pipe_server_to_publisher[countPub][0], buff2 , 256 );
					}
	
				}



			} //Close the loop


		//Send End
			/*Close other end of pipe*/
			close(pipe_publisher_to_server[countPub][0]);
			write(pipe_publisher_to_server[countPub][1],"end", sizeof("end" ));
			

		//Confirm server got End
			//Close the other end of the pipe
			close(pipe_server_to_publisher[countPub][1]);
			read(pipe_server_to_publisher[countPub][0], buff , 256 );
			while (strcmp(buff, "accept") !=0)
			{		
			 	if (strcmp(buff, "reject") ==0)
				{
					perror("\nrejected read again");
					close(pipe_server_to_publisher[countPub][1]);
					read(pipe_server_to_publisher[countPub][0], buff , 256 );
				}
	
			}
		// Simulate work
		sleep(1);
		//Send Terminate
			//Close other end of pipe
			close(pipe_publisher_to_server[countPub][0]);
			write(pipe_publisher_to_server[countPub][1],"terminate", sizeof("terminate"));


		//Confirm Terminate
			//Close the other end of the pipe
			close(pipe_server_to_publisher[countPub][1]);
			//char buff2[128];
			read(pipe_server_to_publisher[countPub][0], buff , 256 );
			while (strcmp(buff, "terminate") !=0)
			{		
			 	if (strcmp(buff, "reject") ==0)
				{
					perror("\nrejected read again");
					close(pipe_server_to_publisher[countPub][1]);
					read(pipe_server_to_publisher[countPub][0], buff , 256 );
				}
	
			}
			
			close(pipe_server_to_publisher[countPub][0]);
			close(pipe_publisher_to_server[countPub][1]);	
			exit(0);
		 }
	
		//Parent process
		else // (pid[] >0)
		{
			//In main
			
		}
}
/*******************************************************************************
********************************************************************************
				Fork Subscribers
Fr each subscriber Si send the messages to ATP server
"Sub subid connect"
"sub subbid topic k" 1 <= k <= t for each topic
"end"
********************************************************************************
********************************************************************************/

for(countSub=0; countSub < subSize;countSub++)
{
		
		pid_subscriber[countSub] = fork();

//		If Fork Failed
		if (pid_subscriber[countSub] < 0)
		{
			//Handle the error appropriatley
			//Need some type of wait here
			perror("\n fork error");
			exit(0);
		}

		//Fork worked, in child process
		else if (pid_subscriber[countSub] == 0)
		 {

		//Send Info
			//Close other end of pipe and send message to pipe
			close(pipe_subscriber_to_server[countSub][0]);
			char pid[10]= "";
			sprintf(pid, "%d", getpid());

			write(pipe_subscriber_to_server[countSub][1],pid, sizeof(pid ));

		//Confirm other side got it
			//Close the other end of the pipe
			close(pipe_server_to_subscriber[countSub][1]);
			char buff[256];
			read(pipe_server_to_subscriber[countSub][0], buff , 256 );
			while (strcmp(buff, "accept") !=0)
			{		
			 	if (strcmp(buff, "reject") ==0)
				{
					perror("\nrejected read again");
					close(pipe_server_to_subscriber[countSub][1]);
					read(pipe_server_to_subscriber[countSub][0], buff , 256 );
				}
	
			}

		//Send topics Im interested in.......
			int a;
			char stuff[128] = "";
			char buff2[256];
			char buff223[256];
			for (a=0; a<num_topics_subcribed_to; a++)			
			{
				//Close other end of pipe
				close(pipe_subscriber_to_server[countSub][0]);
				sprintf(stuff, "%s", myTopics[(countSub+a)%numTopics]);
			
			//Confirm the right topics come through
				//printf("\n******************************* Server sent topic %s and confirm %s",stuff, myTopics[(x+a)%5]);  
				//fflush(stdout);
				
				write(pipe_subscriber_to_server[countSub][1],stuff, sizeof(stuff));

				
				close(pipe_server_to_subscriber[countSub][1]);
				read(pipe_server_to_subscriber[countSub][0], buff2 , 256 );
				if (strcmp(buff2, "successfull") !=0)
				{
if(part5==1){
printf("\nSubscriber: %s read topic: %s article: %s \n" , pid, myTopics[(countSub+a)%numTopics] , buff2); 
}
			
		//Confirm other side got it 2
				//Close the other end of the pipe
				close(pipe_server_to_subscriber[countSub][1]);
				read(pipe_server_to_subscriber[countSub][0], buff223 , 256 );
				while (strcmp(buff223, "successfull") !=0)
				{		
			 		if (strcmp(buff223, "retry") ==0)
					{
						perror("\nrejected read again");
						close(pipe_server_to_subscriber[countSub][1]);
						read(pipe_server_to_subscriber[countSub][0], buff2 , 256 );
					}
	
				}
				}
			} //Close the loop

		//Send Info 3
			/*Close other end of pipe*/
			close(pipe_subscriber_to_server[countSub][0]);
			write(pipe_subscriber_to_server[countSub][1],"end", sizeof("end" ));
			

		//Confirm got it 3
			//Close the other end of the pipe
			close(pipe_server_to_subscriber[countSub][1]);
			//char buff3[256];
			read(pipe_server_to_subscriber[countSub][0], buff , 256 );
			
			while (strcmp(buff, "accept") !=0)
			{		
			 	if (strcmp(buff, "reject") ==0)
				{
					perror("\nrejected read again");
					close(pipe_server_to_subscriber[countSub][1]);
					read(pipe_server_to_subscriber[countSub][0], buff , 256 );
				}
	
			}

		//Send Terminate
			// Simulate work
			sleep(1);
			//Close other end of pipe
			close(pipe_subscriber_to_server[countSub][0]);
			write(pipe_subscriber_to_server[countSub][1],"terminate", sizeof("terminate" ));
			

		//Confirm got terminate
			//Close the other end of the pipe
			close(pipe_server_to_subscriber[countSub][1]);
			//char buff4[256];
			read(pipe_server_to_subscriber[countSub][0], buff , 256 );

			while (strcmp(buff, "terminate") !=0)
			{		
			 	if (strcmp(buff, "reject") ==0)
				{
					perror("\nrejected read again");
					close(pipe_server_to_subscriber[countSub][1]);
					read(pipe_server_to_subscriber[countSub][0], buff , 256 );
				}
	
			}
		//Then Close it	


		close(pipe_server_to_subscriber[countSub][0]);
		close(pipe_subscriber_to_server[countSub][1]);

			exit(0);
	
		 }
	
		//Parent process
		else // (pid[] >0)
		{
			//In main
			
		}

}
//Wait for all pid id's to finish before main MPC terminates
//This should not be needed, but just in case.
int j;
	

	for(j=0; j < pubSize; j++)
	{
		int wait;
	wait=waitpid(pid_publisher[j],0,0);
		if(wait==0)
			sleep(2);
	}

for(j=0; j < subSize; j++)
	{
		int wait;
	wait=waitpid(pid_subscriber[j],0,0);
		if(wait==0)
			sleep(2);
	}


while(waitpid(pid_server,0,0)==0)
	sleep(1);



for(m = 0; m < numTopics; m++)
{
   	free(q[m]);
}
    

return 0;
}


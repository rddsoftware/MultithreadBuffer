/**
 * Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Rusty Dillard
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */

#include "buffer.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// global buffer object
Buffer buffer;

// Producer thread function
void *producer(void *param)
{
  // Each producer insert its own ID into the buffer
  // For example, thread 1 will insert 1, thread 2 will insert 2, and so on.
  buffer_item item = *((int *)param);

  while (true)
  {
    // Sleep for a random period of time
    usleep(rand() % 1000000);

    //  If the buffer is not full, insert the item.
    if (buffer.insert_item(item))
    {
      cout << "Producer " << item << ": Inserted item " << item << endl;
      buffer.print_buffer();
    }
    
    //  If the buffer is full, print the following message.
    else
    {
      cout << "Producer error condition" << endl; // shouldn't come here
    }
  }
}

// Consumer thread function
void *consumer(void *param)
{
  // Each consumer removes an item from the buffer
  buffer_item item;

  while (true)
  {
    // Sleep for a random period of time
    usleep(rand() % 1000000);

    //  If the buffer is not empty, remove the item.
    if (buffer.remove_item(&item))
    {
      cout << "Consumer " << item << ": Removed item " << item << endl;
      buffer.print_buffer();
    }

    //  If the buffer is empty, print the following message.
    else
    {
      cout << "Consumer error condition" << endl; // shouldn't come here
    }
  }
}


//  Main function to drive the 
int main(int argc, char *argv[])
{
  //  Exit the program if the number of arguments passed into the main function is not 4.
  if (argc != 4)
  {
    cerr << "Usage: " << argv[0] << " <sleep_time> <num_producers> <num_consumers>" << endl;
    exit(1);
  }

  //  Grab the arguments passed into the main function at runtime.
  int sleep_time = stoi(argv[1]);
  int num_producers = stoi(argv[2]);
  int num_consumers = stoi(argv[3]);

  //  Vectors of threads to store the threads of the consumers and producers.
  vector<pthread_t> producer_threads(num_producers);
  vector<pthread_t> consumer_threads(num_consumers);

  //  Create the number of threads equal to the num_producers passed into the main function and store them in the producer_threads vector.
  for (int i = 1; i <= num_producers; ++i)
  {
    //  Create a new integer pointer to store the id of the producer thread.
    int *id = new int(i);
    pthread_create(&producer_threads[i - 1], NULL, producer, id);
  }

  //  Create the number of threads equal to the num_consumers passed into the main function and store them in the consumer_threads vector.
  for (int i = 1; i <= num_consumers; ++i)
  {
    //  Create a new integer pointer to store the id of the consumer thread.
    pthread_create(&consumer_threads[i - 1], NULL, consumer, NULL);
  }

  //  Hold execution for the prescribed number of seconds.
  sleep(sleep_time);

  //  Run the producer threads.
  for (int i = 0; i < num_producers; ++i)
  {
    pthread_cancel(producer_threads[i]);
    pthread_join(producer_threads[i], NULL);
  }

  //  Run the consumer threads.
  for (int i = 0; i < num_consumers; ++i)
  {
    pthread_cancel(consumer_threads[i]);
    pthread_join(consumer_threads[i], NULL);
  }

  exit(0);
}

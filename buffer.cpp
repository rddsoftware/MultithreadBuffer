/**
 * Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Rusty Dillard
 * @brief Implementation file for the buffer class
 * @version 0.1
 */
 
#include "buffer.h"
#include <iostream>

using namespace std;

//  Set the semaphores for the buffer operations.
Buffer::Buffer(int size) : size(size) {
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, size);
}

//  Destroy the semaphores.
Buffer::~Buffer() {
  sem_destroy(&full);
  sem_destroy(&empty);
}

//  This function waits for the signal that there is space to insert an item into the buffer.
bool Buffer::insert_item(buffer_item item) {
  //  Waits for the empty semaphore.
  sem_wait(&empty);
  //  Locks the mutex to allow for safe data manipulation and pushes the item into the buffer.
  mtx.lock();
  buffer.push_back(item);
  //  Unlock the mutex.
  mtx.unlock();
  //  Signals that there exists at least one item in the buffer.
  sem_post(&full);
  return true;
}

//  This function waits for the full signal (whether or not an item exists in the buffer) and removes it.
bool Buffer::remove_item(buffer_item *item) {
  //  Waits for the full semaphore.
  sem_wait(&full);
  //  Locks the mutex to allow for safe data manipulation and erases the item at the front of the buffer.
  mtx.lock();
  *item = buffer.front();
  buffer.erase(buffer.begin());
  //  Unlock the mutex.
  mtx.unlock();
  //  Signal that there is space in the buffer.
  sem_post(&empty);
  return true;
}

//  Return the maximum size allowed.
int Buffer::get_size() 
{ 
  return size; 
}

//  Return the number of items in the buffer.
int Buffer::get_count() 
{
  return buffer.size();
}

//  Return whether or not the buffer is empty.
bool Buffer::is_empty()
{ 
  return buffer.empty(); 
}

//  Return whether or not the buffer is full.
bool Buffer::is_full() 
{ 
  return buffer.size() == size;
}

//  Print the buffer with the prescribed format.
void Buffer::print_buffer() 
{
  cout << "Buffer: [";
  for (int i = 0; i < buffer.size(); ++i) 
  {
    cout << buffer[i] << (i < buffer.size() - 1 ? ", " : "");
  }
  cout << "]" << endl;
}

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include<iostream>
#include<mutex>
#include <array>
using namespace std;
#define N 5

mutex mtx;           // mutex for critical section
array<mutex,N> mtxS;

struct PrepareOk {
  int n;
  int na;
  int va;
  bool ok;
  bool read;
};
struct Prepare {
  int n;
  bool read;
  PrepareOk reply;
};

struct AcceptOk {
  int n;
  bool ok;
  bool read;
};
struct Accept {
  int n;
  int v_;
  bool read;
  AcceptOk reply;
};
struct State {
  int np;
  int na;
  int va;
  bool decided;
};
array<array<Prepare, N>,N> prepares; // msgs[TO][FROM]
array<array<Accept,N>,N> accepts; // msgs[TO][FROM]
array<bool,N> decided;
array<State, N> states;


void * accept(int a) {
  Accept*  msg;
  while (true) {
    mtx.lock();
    mtxS[a].lock();
    if (states[a].decided){
    mtxS[a].unlock();
      mtx.unlock();
      return NULL;
    }
     
        
    for (int i = 0; i < N; i++){
      msg = &accepts[a][i];
      if (!msg->read) {
        msg->read = true;
        msg->reply.read = false;
        if (msg->n >= states[a].np ){
          states[a].np = msg->n;
          states[a].na = msg->n;
          states[a].va = msg->v_;
          msg->reply.n = msg->n;
          msg->reply.ok = true; /// TODO Lock on this>?
        }else{
          msg->reply.ok = false;
        }
      }
    }
    mtxS[a].unlock();
    mtx.unlock();

  }
}

void * prepare(int a) {
  Prepare * msg;
  while (true) {
    mtx.lock();
    mtxS[a].lock();
    if (states[a].decided){
    mtxS[a].unlock();
      mtx.unlock();
      return NULL;}
    for (int i = 0; i < N; i++){

      msg = &prepares[a][i];
      if (!msg->read) {
        msg->read = true;
        msg->reply.read = false;
        if (msg->n >= states[a].np ){
          if (msg->n > states[a].np ){
            states[a].np = msg->n;
            msg->reply.n = msg->n;
            msg->reply.na = states[a].na;
            msg->reply.va = states[a].va;
            msg->reply.ok = true;
          }else{
            msg->reply.ok = false;
          }
        }
      }


    }
    mtxS[a].unlock();
    mtx.unlock();
  }
}

void *paxos(int a){
  thread *acc = new thread(accept, a);
  prepare(a);
  acc->join();
  return NULL;
}

void *paxosPropose(int a, int v){
  thread * px = new thread(paxos, a);
  int n = 0;

  while (true) {
    n++;

    mtx.lock();

    if (states[a].decided){
      mtx.unlock();
      return NULL;}

    for (int i = 0; i < N; i++){
      prepares[i][a].n = n;
      prepares[i][a].read = false;
    }
    mtx.unlock();


      
    auto accepted = 0;
    auto rejected = 0;
    while (accepted + rejected < N) {
      mtx.lock();
      if (states[a].decided){
        mtx.unlock();
        return NULL;}

      for (int i = 0; i < N; i++){
        if (! prepares[i][a].reply.read) {
          prepares[i][a].reply.read = true;
          if (prepares[i][a].reply.ok) {
            accepted += 1;
          }else {
            rejected += 1;
          }
        }
      }

      mtx.unlock();
    
   
    }

    if (accepted > (N/2)) { // TODO Fix majority!!
      mtx.lock();

      if (states[a].decided){
        mtx.unlock();
        return NULL;}
      auto v_ = v;
      auto na_ = states[a].na;
      for (int i = 0; i < N; i++){
        if (prepares[i][a].reply.na > na_){
          na_ = prepares[i][a].reply.na;
          v_ = prepares[i][a].reply.va;
        }
      }

      for (int i = 0; i < N; i++){
        accepts[i][a].n = n;
        accepts[i][a].v_ = v_;
        accepts[i][a].read = false;
      }

      mtx.unlock();
      auto accepted2 = 0;
      auto rejected2 = 0;
      while (accepted2 + rejected2 < N) {
        mtx.lock();
        if (states[a].decided) {
          mtx.unlock();
          return NULL;
        }
        for (int i = 0; i < N; i++){
          if (! accepts[i][a].reply.read) {
            accepts[i][a].reply.read = true;
            if (accepts[i][a].reply.ok) {
              accepted2 += 1;
            }else {
        
              rejected2 += 1;
            }
          }
        }

        mtx.unlock();
      }

      if (accepted2 > (N/2)) { // TODO Fix majority!!
        printf("WIN %i\n", v);
        mtx.lock();
        for (int i = 0; i < N; i++ )
          states[a].decided = true;

        mtx.unlock();
        return NULL;
      }
      
      
    }
  }
  px->join();
}
int main () {
  for (int i = 0; i < N; i ++){
    for (int j=0; j < N; j++){
      prepares[i][j].read = true;
      prepares[i][j].reply.read = true;
      accepts[i][j].read = true;
      accepts[i][j].reply.read = true;
    }
  }
  thread * t1 = new thread(paxos, 0);
  thread * t2 = new thread(paxos, 1);
  thread * t4 = new thread(paxos, 2);
  thread * t3 = new thread (paxosPropose, 3, 5);
  thread * t5 = new thread (paxosPropose, 4, 7);
  t1->join();
  printf("D\n");
  t2->join();
  t4->join();
  t3->join();
  t5->join();
  
}

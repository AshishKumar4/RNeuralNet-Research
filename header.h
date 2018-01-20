#pragma once

#include "vector"
#include "iostream"
#include "algorithm"
#include "math.h"

using namespace std;

#define W_CONST   1
#define S_CONST   1
#define CONST_E   2.71828184
#define P_DECAY_RATE    1

class Packets_t
{
public:
  float value;
  Packets_t* next;
  Packets_t* back;

  Packets_t(float v)
  {
    value = v;
  }
};

class Lists_t
{
  Packets_t* packets;
  Packets_t* start;
  Packets_t* end;
public:

  void InQueue(Packets_t* packet)
  {
    if(end == NULL)
    {
      start = end = packet;
      packet->back = packet->next = NULL;
      return;
    }
    end->next = packet;
    packet->back = end;
    packet->next = NULL;
    end = packet;
  }

  void InQueue(float val)
  {
    Packets_t* p = new Packets_t(val);
    InQueue(p);
  }

  float LastVal()
  {
    return start->value;
  }

  float DeQueue()
  {
    Packets_t* p = start;
    if(start->next)
    {
      start->next->back = NULL;
      start = start->next;
    }
    else
    {
      start = end = NULL;
    }
    float val = p->value;
    delete p;
    return val;
  }
};

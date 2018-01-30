#pragma once

#include "vector"
#include "iostream"
#include "algorithm"
#include "math.h"
#include "random"
#include "gnuplot-iostream.h"

Gnuplot gp, gp2;

using namespace std;

#define W_CONST   0.01
#define S_CONST   1
#define A_CONST   1
#define CONST_E   2.71828184
#define P_DECAY_RATE    0.001
#define P_TIME_TICKS    1


#define MAX_DENDRITES 16
#define MAX_AXIONS    16


float RewardGenerated = 0;

vector<float>  plotVecNN;

class Packets_t
{
public:
  float value;
  int   var;
  Packets_t* next;
  Packets_t* back;

  Packets_t(float v)
  {
    value = v;
  }

  Packets_t(float v, int timer)
  {
    value = v;
    var = timer;
  }
};

class Lists_t
{
  Packets_t* packets;
  Packets_t* start;
  Packets_t* end;
public:
  Lists_t()
  {
    start = end = packets = NULL;
  }

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

  void InQueue(float val, int timestamp)
  {
    Packets_t* p = new Packets_t(val, timestamp);
    InQueue(p);
  }

  void InQueue(float val)
  {
    Packets_t* p = new Packets_t(val);
    InQueue(p);
  }

  float LastVal()
  {
    if(!start)
    {
      return -1;
    }
    return start->value;
  }

  float DeQueue()
  {
    Packets_t* p = start;
    if(!start) return 0;
  //  cout<<"YES";
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

double phi = 12;

double _gamma = 3.7;

double Sigmoid(double in)
{
  return  1 / (1 + pow(CONST_E, -in));
}

double SigClamp(double in)
{
  return  (2*(1 / (1 + pow(CONST_E, -in)))) - 1;
}

double FuncNeural(double sum)
{
//  return tanhf(sum);
//  return ((sum/powf(1+(sum*sum), 0.5))+1)/2;
  return Sigmoid(sum);//
  return ((sum/(1+fabs(sum)))+1)/2;

//  return logf(1+powf(CONST_E, sum));  // Softplus Function
//  return erff(sum);
  return Sigmoid(sum);
  if(sum<0) return 0.00001*sum; // Liner-Rectifier
  return log(sum);
}

double FuncDerivative(double a)
{/*
  return 1/powf(fabs(a)+1, 2); //--> use in combination of sigmoid ->

  return 1/powf(2, fabs(a)+1); //--> use in combination of sigmoid -> 88%

  double b = FuncNeural(a);
  return b*(1 - b);
*/
  double b = tanh(a);
  return (1 - b*b);

  return Sigmoid(a)*(1-Sigmoid(a));

  if(a<0) return 0.00001; // Liner-Rectifier
  return 1/a;
}

double CostFunc(double a, double y)
{
  return y*log(a + 0.00001) + (1-y)*log(1.00001-a);
}

double CostFuncDerivative(double a, double y)
{
  return a-y;//(y/(a+1.00001)) - ((1-y)/(1.00001-a));
}


double Func(double x)
{
//  return Sigmoid(x);
  return tanh(x);//Sigmoid(x);
}

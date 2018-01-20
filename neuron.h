#pragma once

#include "header.h"

template <class In, class Out>
class Neurite_t // Axons or Dendrites
{
  float var;
  Lists_t activations;
  Lists_t timers;
  
public:
  In* Start;
  Out* End;

  float Myelin;
  float Weight;

  float Role; // From InSource to OutSource

  float softmax_tmp;

  Neurite_t();
  Neurite_t(float weight, float myelin);
  void WeightPostProcessor();

  void InQueue(float val);
  float DeQueue();
  bool TimerTicks();
};

class Soma_t
{
  float TPotential;

protected:

  float ActivationFunction()
  {
    ValSum = Sum;   // Function to be applied on the Summation
    if(ValSum < TPotential)
    {
      Fired = false;
      return 0;    // Rectified Linear Unit
    }
    Fired = true;
    return ValSum;
  }

public:
  float Sum;
  float ValSum;

  float Output;
  float OldOutput;

  vector<float> CurrentInputs;
  vector<float> FinalInputs;

  float LocalReward;

  bool Fired;

  float softmax_tmp;
  bool softmax_calc_back;

  Soma_t();
  Soma_t(float threshold);
  float Calc();    // Summation and Function Applier

  void PotentialDecrement(float val);
};

class Neuron_t : public Soma_t
{
public:
  int   inVar;
  int   outVar;
  vector<Neurite_t<Neuron_t, Neuron_t>*>  Dendrites;
  vector<Neurite_t<Neuron_t, Neuron_t>*>  Axions;

  Neuron_t();
  Neuron_t(float threshold);
  float ComputeLocalReward();
  float ComputeOutput();
};

Neurite_t<Neuron_t, Neuron_t>* Connect_Neurons(Neuron_t* start, Neuron_t* end);

Neurite_t<Neuron_t, Neuron_t>* Connect_Neurons(Neuron_t* start, Neuron_t* end, float weight, float myelin);

//void Neurite_WeightPostProcessor(Neurite_t<Neuron_t, Neuron_t>* neurite);  // Adjusts weights; Prev means neurite start

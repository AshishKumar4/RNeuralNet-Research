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
  float tmp_Out;

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
    ValSum = Sigmoid(ValSum);
    if(ValSum < TPotential)
    {
      Fired = false;
      Output = 0;
      return 0;    // Rectified Linear Unit
    }
    Fired = true;
    Output = ValSum;
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
  float LocalReward_tmp;

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
  int   type;
  int   inVar;
  int   outVar;
  int   Var2;
  int   id;
  int   Lock;
  vector<Neurite_t<Neuron_t, Neuron_t>*>  Dendrites;
  vector<Neurite_t<Neuron_t, Neuron_t>*>  Axions;

  Neuron_t();
  Neuron_t(float threshold);
  Neuron_t(float threshold, int Ntype);
  float ComputeLocalReward();
  float ComputeOutput();
};

int nnid = 0, RewardedNeurons = 0, Neurons = 0;

Neurite_t<Neuron_t, Neuron_t>* Connect_Neurons(Neuron_t* start, Neuron_t* end);

Neurite_t<Neuron_t, Neuron_t>* Connect_Neurons(Neuron_t* start, Neuron_t* end, float weight, float myelin);

Neuron_t* RewardFeederNeuron;
vector<Neuron_t*>  NeuralNet, input_layer, output_layer;
vector<Neurite_t<Neuron_t, Neuron_t>*> GreyMatter;

//void Neurite_WeightPostProcessor(Neurite_t<Neuron_t, Neuron_t>* neurite);  // Adjusts weights; Prev means neurite start

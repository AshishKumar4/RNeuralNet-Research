#include "header.h"
#include "neuron.h"

/**********************************//* NEURITE_T DEFINITIONS *//**********************************/
Neurite_t<Neuron_t, Neuron_t>* Connect_Neurons(Neuron_t* start, Neuron_t* end)
{
  Neurite_t<Neuron_t, Neuron_t>* conn = new Neurite_t<Neuron_t, Neuron_t>();
  conn->Start = start;
  conn->End = end;
  start->Axions.push_back(conn);
  end->Dendrites.push_back(conn);
  return conn;
}

Neurite_t<Neuron_t, Neuron_t>* Connect_Neurons(Neuron_t* start, Neuron_t* end, float weight, float myelin)
{
  Neurite_t<Neuron_t, Neuron_t>* conn = Connect_Neurons(start, end);
  conn->Weight = weight;
  conn->Myelin = myelin;
  return conn;
}

template<class In, class Out>
Neurite_t<In, Out>::Neurite_t()
{

}

template<class In, class Out>
Neurite_t<In, Out>::Neurite_t(float weight, float myelin)
{
  Weight = weight;
  Myelin = myelin;
}

template<class In, class Out>
void Neurite_t<In, Out>::WeightPostProcessor()  // Adjusts weights; Prev means neurite start
{
  if(!this->End->LocalReward)
  {
    this->End->ComputeLocalReward();
  }
  float lr = (W_CONST * this->End->LocalReward)/(1); // we need to compute the amount to add to weight
  if(!(this->End->softmax_tmp)) // If Softmax is not precomputed
  {
    for(int i = 0; i < this->End->Dendrites.size(); i++)
    {
      // Compute the softmax summation by summing e^(output of all prev neurons).
      this->End->softmax_tmp += pow(CONST_E, this->End->Dendrites[i]->Start->ComputeOutput());
    }
  }
  this->softmax_tmp = pow(CONST_E, this->Start->ComputeOutput());   // Compute e^(output of this specific prev neuron)
  this->softmax_tmp /= this->End->softmax_tmp;  // Get the softmax Ratio
  this->Start->inVar += 1;   // Tell the start neron that another output neuron has been updated
  this->End->outVar += 1;    // Tell the end neuron that another input neuron has been updated
  lr *= this->softmax_tmp; // Compute amount to add to weight
  this->softmax_tmp = lr;  // set this computed amount in this softmax temp variable
  this->Weight += lr;    // Add the amount to the weight
}

template<class In, class Out>
void Neurite_t<In, Out>::InQueue(float val)
{
  activations.InQueue(val);
  timers.InQueue(Myelin);
}

template<class In, class Out>
float Neurite_t<In, Out>::DeQueue()
{
  timers.DeQueue();
  return activations.DeQueue();
}

template<class In, class Out>
bool Neurite_t<In, Out>::TimerTicks()
{
  var += P_DECAY_RATE;
  if(timers.LastVal() == var)
  {
    return true;
  }
  return false;
}

/**********************************//* SOMA_T DEFINITIONS *//**********************************/
Soma_t::Soma_t()
{
  Sum = 0;
  Output = 0;
  OldOutput = 0;
  TPotential = 0;
}

Soma_t::Soma_t(float threshold)
{
  Soma_t();
  TPotential = threshold;
}

float Soma_t::Calc()    // Summation and Function Applier
{
  // Run a loop and sum inputs from all available dendrites and compute the output.
}

void Soma_t::PotentialDecrement(float val)
{
  ValSum -= val;
}

/**********************************//* NEURON_T DEFINITIONS *//**********************************/
Neuron_t::Neuron_t()
{

}

Neuron_t::Neuron_t(float threshold) : Soma_t(threshold)
{

}

float Neuron_t::ComputeOutput()
{
  return ActivationFunction(); // The Value of Sum would be constantly updated by global threads.
}

float Neuron_t::ComputeLocalReward() // Computes local reward for a given neuron by getting softmax results from all outgoing neurons
{
  for(int i = 0; i < this->Axions.size(); i++)
  {
    this->Axions[i]->WeightPostProcessor(); // Process the weights on outgoing neurons, which also computes softmax contributions.
    this->LocalReward += this->Axions[i]->softmax_tmp * S_CONST;    // add this ratio with some constant to localreward of prev neuron
  }
  return this->LocalReward;
}

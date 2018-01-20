#include "header.h"
#include "neuron.h"
#include "Processor.h"

void Global_ActivationTransporter()   // Decrement myelin conjugate and if 0, add value to end neuron's sum; for every neuron and neurite
{

}

void Global_Refresher(vector<Neuron_t*> nnet)   // Control the refreshmet of all time dependent values of all neurons and neurites
{
  for(int i = 0; i < nnet.size(); i++)
  {
    nnet[i]->PotentialDecrement(P_DECAY_RATE);
    nnet[i]->ComputeOutput();
  }
}

void Global_ForwardProcessor(vector<Neurite_t<Neuron_t, Neuron_t>*> dnet)
{
  for(int i = 0; i < dnet.size(); i++)
  {
    // First get the output value from start neuron
    if(dnet[i]->Start->Fired)
    {
      dnet[i]->InQueue(dnet[i]->Start->ValSum * dnet[i]->Weight);
      dnet[i]->Start->Fired = false;
      dnet[i]->Start->Sum = 0;
      dnet[i]->Start->ValSum = 0;
    }
    if(dnet[i]->TimerTicks())   // As the packets near the destination, transfer them from the neurite ends to the neurons
    {
      float val = dnet[i]->DeQueue();
      dnet[i]->End->CurrentInputs.push_back(val);
      dnet[i]->End->Sum += val;
      dnet[i]->End->ComputeOutput();
    }
  }
}

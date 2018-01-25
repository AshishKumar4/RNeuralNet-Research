#include "header.h"
#include "neuron.h"
#include "Processor.h"

void Global_Adjuster(vector<Neuron_t*> input_layer)   // Decrement myelin conjugate and if 0, add value to end neuron's sum; for every neuron and neurite
{
  printf("|");
/*  for(int i = 0; i < input_layer.size(); i++)
  {
    input_layer[i]->ComputeLocalReward();
  }*/
  RewardFeederNeuron->LocalReward = RewardGenerated;
  Global_RewardSpreader(RewardFeederNeuron, RewardGenerated);
}

void Global_RewardSpreader(Neuron_t* RewardFeederNeuron, float Reward)
{
  if(RewardFeederNeuron->Var2 >= RewardFeederNeuron->Dendrites.size())
  {
    if(!RewardFeederNeuron->LocalReward)
    {
      RewardFeederNeuron->LocalReward = RewardFeederNeuron->LocalReward_tmp;
      //RewardFeederNeuron->LocalReward_tmp = 0;
    }
    return;
  }

  printf("\n[%d]=>", RewardFeederNeuron->id);

  for(int i = 0; i < RewardFeederNeuron->Dendrites.size(); i++)
  {
    RewardFeederNeuron->Dendrites[i]->Start->LocalReward_tmp += Reward * (RewardFeederNeuron->Dendrites[i]->tmp_Out / RewardFeederNeuron->Output);//RewardFeederNeuron->Dendrites[i]->Weight;
    printf("(%d)", RewardFeederNeuron->Dendrites[i]->Start->id);
    RewardFeederNeuron->Var2 += 1;
  }
  for(int i = 0; i < RewardFeederNeuron->Dendrites.size(); i++)
  {
    Global_RewardSpreader(RewardFeederNeuron->Dendrites[i]->Start, RewardFeederNeuron->Dendrites[i]->Start->LocalReward_tmp);
  }
}

/*
void Global_RewardFeeder(int i)
{
  while(1)
  {
    if(RewardedNeurons)
    {
      NeuralNet[i]->LocalReward = 0;
      NeuralNet[i]->inVar = 0;
      printf(".");
      for(int j = 0; j < NeuralNet[i]->Axions.size(); j++)
      {
        while(NeuralNet[i]->Axions[j]->End->inVar < NeuralNet[i]->Axions[j]->End->Dendrites.size());
        NeuralNet[i]->LocalReward += NeuralNet[i]->Axions[j]->End->LocalReward * NeuralNet[i]->Axions[j]->Weight;
        NeuralNet[i]->inVar += 1;
      }
      --RewardedNeurons;
    }
  }
}
*/
void Global_Teacher()
{
  for(int i = 0; i < GreyMatter.size(); i++)
  {
    GreyMatter[i]->WeightPostProcessor();
  }
}

void Global_Renew()
{
  for(int i = 0; i < NeuralNet.size(); i++)
  {
    NeuralNet[i]->inVar = 0;
    NeuralNet[i]->outVar = 0;
    NeuralNet[i]->Var2 = 0;
    NeuralNet[i]->LocalReward = 0;
    NeuralNet[i]->softmax_tmp = 0;
  }
  RewardFeederNeuron->inVar = 0;
  RewardFeederNeuron->outVar = 0;
  RewardFeederNeuron->Var2 = 0;
  RewardFeederNeuron->LocalReward = 0;
  RewardFeederNeuron->softmax_tmp = 0;
  for(int i = 0; i < output_layer.size(); i++)
  {
    output_layer[i]->inVar = 0;
    output_layer[i]->outVar = 0;
    output_layer[i]->Var2 = 0;
    output_layer[i]->LocalReward = 0;
    output_layer[i]->softmax_tmp = 0;
  }
}

void Global_Refresher(vector<Neuron_t*> nnet)   // Control the refreshmet of all time dependent values of all neurons and neurites
{
//  printf(".");
  for(int i = 0; i < nnet.size(); i++)
  {
    nnet[i]->PotentialDecrement(P_DECAY_RATE);
    nnet[i]->ComputeOutput();
  }
}

void Global_ForwardProcessor(vector<Neurite_t<Neuron_t, Neuron_t>*> dnet)
{
//  printf("\n.");
  for(int i = 0; i < dnet.size(); i++)
  {
    //printf("%d-", i);
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
      dnet[i]->tmp_Out = val;
      dnet[i]->End->Sum += val;
      dnet[i]->End->ComputeOutput();
    }
  }
}

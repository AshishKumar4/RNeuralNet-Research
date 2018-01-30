#include "header.h"
#include "neuron.h"
#include "Processor.h"

void Global_Adjuster(vector<Neuron_t*> input_layer)   // Decrement myelin conjugate and if 0, add value to end neuron's sum; for every neuron and neurite
{
//  printf("|");
/*  for(int i = 0; i < input_layer.size(); i++)
  {
    input_layer[i]->ComputeLocalReward();
  }*/
  RewardFeederNeuron->LocalReward = RewardGenerated;
  RewardGenerated = 0;
  Global_RewardSpreader(RewardFeederNeuron, RewardFeederNeuron->LocalReward);
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

  //printf("\n[%d]=>", RewardFeederNeuron->id);
  RewardFeederNeuron->softmax_tmp = 0;
  for(int i = 0; i < RewardFeederNeuron->Dendrites.size(); i++)
  {
    RewardFeederNeuron->softmax_tmp += (expf(RewardFeederNeuron->Dendrites[i]->Start->OldOutput));//RewardFeederNeuron->Dendrites[i]->Weight;
    //RewardFeederNeuron->Dendrites[i]->Start->LocalReward_tmp += Reward * (expf(RewardFeederNeuron->Dendrites[i]->Start->OldOutput) / expf(RewardFeederNeuron->OldOutput));//RewardFeederNeuron->Dendrites[i]->Weight;

//    printf("(%d)", RewardFeederNeuron->Dendrites[i]->Start->id);
  }
  
  for(int i = 0; i < RewardFeederNeuron->Dendrites.size(); i++)
  {
    RewardFeederNeuron->Dendrites[i]->softmax_tmp = (expf(RewardFeederNeuron->Dendrites[i]->Start->OldOutput) / RewardFeederNeuron->softmax_tmp);
    RewardFeederNeuron->Dendrites[i]->Start->LocalReward_tmp += Reward * RewardFeederNeuron->Dendrites[i]->softmax_tmp;
   // printf("{%d->%d, %f}", RewardFeederNeuron->Dendrites[i]->Start->id, RewardFeederNeuron->id, RewardFeederNeuron->Dendrites[i]->softmax_tmp);
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
    GreyMatter[i]->tmp_Out = 0;
  }
}

void Global_Renew()
{
  for(int i = 0; i < NeuralNet.size(); i++)
  {
/*    if(NeuralNet[i]->Var2 < NeuralNet[i]->Dendrites.size())
      cout<<"{"<<NeuralNet[i]->Var2 - NeuralNet[i]->Dendrites.size()<<","<<i<<"}";*/
    NeuralNet[i]->inVar = 0;
  //  NeuralNet[i]->outVar = 0;
    NeuralNet[i]->Var2 = 0;
    NeuralNet[i]->LocalReward = 0;
    NeuralNet[i]->LocalReward_tmp = 0;
    NeuralNet[i]->softmax_tmp = 0;
  }
  RewardFeederNeuron->inVar = 0;
  //RewardFeederNeuron->outVar = 0;
  RewardFeederNeuron->Var2 = 0;
  RewardFeederNeuron->LocalReward = 0;
  RewardFeederNeuron->LocalReward_tmp = 0;
  RewardFeederNeuron->softmax_tmp = 0;
  for(int i = 0; i < output_layer.size(); i++)
  {
    output_layer[i]->inVar = 0;
   // output_layer[i]->outVar = 0;
    output_layer[i]->Var2 = 0;
    output_layer[i]->LocalReward = 0;
    output_layer[i]->LocalReward_tmp = 0;
    output_layer[i]->softmax_tmp = 0;
  }
}

void Global_Refresher(vector<Neuron_t*> nnet)   // Control the refreshmet of all time dependent values of all neurons and neurites
{
//  printf(".");
 // gp2 << "plot '-' \n";
  vector<float> vec;
  for(int i = 0; i < nnet.size(); i++)
  {
    nnet[i]->PotentialDecrement(P_DECAY_RATE);
    nnet[i]->ComputeOutput();
    /*if(nnet[i]->Fired)
      vec.push_back((nnet[i]->ValSum));
    else  
      vec.push_back(0);*/
  }
  //gp2.send1d(vec);
}

void Global_OutputGenerator(vector<Neuron_t*> nnet)
{
  //gp2 << "plot '-' \n";
  vector<float>  vec;
  for(int i = 0; i < nnet.size(); i++)
  {
    for(int j = 0; j < nnet[i]->Dendrites.size(); j++)
    {  
      if(nnet[i]->Dendrites[j]->TimerTicks())   // As the packets near the destination, transfer them from the neurite ends to the neurons
      {
        float val = nnet[i]->Dendrites[j]->DeQueue();
      //  printf("{(%f)%d->%d}", val, nnet[i]->Dendrites[j]->Start->id, nnet[i]->id);
        nnet[i]->CurrentInputs.push_back(val);
        nnet[i]->Dendrites[j]->tmp_Out = val;
        nnet[i]->Sum += val;
        nnet[i]->ComputeOutput();
      }
    }
    vec.push_back(nnet[i]->Output);
  }
  //gp2.send1d(vec);
}

void Global_ForwardProcessor(vector<Neurite_t<Neuron_t, Neuron_t>*> dnet)
{
  //printf("\n.");
  gp << "plot '-' \n";
  for(int i = 0; i < dnet.size(); i++)
  {
    //printf("%d-", i);
    // First get the output value from start neuron
    if(dnet[i]->Start->Fired)
    {
      plotVecNN[dnet[i]->Start->id - 1] = dnet[i]->Start->Output;
      //printf("{%d->%d(%f)}", dnet[i]->Start->id, dnet[i]->End->id, dnet[i]->Start->Output);
      dnet[i]->InQueue(dnet[i]->Start->Output * dnet[i]->Weight);
      ++dnet[i]->Start->outVar;
      if(dnet[i]->Start->outVar == dnet[i]->Start->Axions.size())
      {
        dnet[i]->Start->Fired = false;
        dnet[i]->Start->Sum = 0;
        dnet[i]->Start->ValSum = 0;
        dnet[i]->Start->OldOutput = dnet[i]->Start->Output;
        dnet[i]->Start->Output = 0;
        dnet[i]->Start->outVar = 0;
        dnet[i]->End->CurrentInputs.clear();
      }
    }
    if(dnet[i]->TimerTicks())   // As the packets near the destination, transfer them from the neurite ends to the neurons
    {
      float val = dnet[i]->DeQueue();
      //printf("{(%f)%d->%d}", val, dnet[i]->Start->id, dnet[i]->End->id);
      dnet[i]->End->CurrentInputs.push_back(val);
      dnet[i]->tmp_Out += val;
      dnet[i]->End->Sum += val;
      dnet[i]->End->ComputeOutput();
    }
  }
  gp.send1d(plotVecNN);
}

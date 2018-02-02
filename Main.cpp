#include "neuron.cpp"
#include "Processor.cpp"
#include "random"
#include "stdlib.h"
#include <thread>         // std::thread
#include <boost/tuple/tuple.hpp>

default_random_engine gen;

void NeuralNet_init()
{
  printf("\nSizeof Neuron_t: %x\nBuilding Neurons, Enter Power, Base, Multiple, : \n>", sizeof(Neuron_t));

  uint32_t power;
  cin>>power;
  uint32_t base;
  cin>>base;
  uint32_t multiple;
  cin>>multiple;

  if(!power) power = 4;
  if(!base) base = 15;
  if(!multiple) multiple = 1;

  Neurons = pow(base, power)*multiple - 1;
  Neuron_t* tmpn;
  Neurite_t<Neuron_t, Neuron_t>* tmpd;

  normal_distribution<double> dis(2, 0.5);

  gp2<<"plot '-' \n";
  vector<float> vec;

  for(uint32_t i = 0; i < Neurons; i++ )
  {
    tmpn = new Neuron_t(dis(gen));
    NeuralNet.push_back(tmpn);
    vec.push_back(tmpn->TPotential);
    //printf("\t%d", tmpn->id);
  }
  gp2.send1d(vec);

  uint32_t rand1, randnum;

  srand(dis(gen)*12567);

  printf("\nLinking Neural Networks, Building Random Connections...");

  vec.clear();
  gp<<"plot '-' \n";

  uint32_t tmp11;

  Neurite_t<Neuron_t, Neuron_t>* tdnn;
  for(uint32_t i = 0; i < Neurons - 1; i++ )
  {
    normal_distribution<double> dis2(0, 1/(powf(MAX_DENDRITES, 0.5)));
    for(uint32_t j = 0; j < MAX_AXIONS; j++)
    {
      tmp11 = 0;
      back:
      randnum = rand();
      rand1 = (randnum%(Neurons-1)); //Some random value between 0 to Neurons present.
      //printf("\n{%d}", rand1);
      // This way we get a Random Neuron
      if(rand1 == i) goto back;
      if(tmp11 == Neurons)  break;
      if(NeuralNet[rand1]->Dendrites.size() == MAX_DENDRITES)   //  Check if this random neuron has all slots full
      {
        ++tmp11;
        goto back;
      }
      for(int k = 0; k < NeuralNet[i]->Axions.size(); k++)
      {
        if(NeuralNet[i]->Axions[k]->End == NeuralNet[rand1])    // This Neuron has already been linked
        {
        //  printf("a");
          goto back;
        }
      }
      tdnn = Connect_Neurons(NeuralNet[i], NeuralNet[rand1], dis2(gen), rand() % 20);
      GreyMatter.push_back(tdnn);//*/
      vec.push_back(tdnn->Weight);
    }
  }
  gp.send1d(vec);

  printf("\nNeural Network setup complete. %d Neurons\nVerification: \n", Neurons);
  for(uint32_t i = 0; i < Neurons - 1; i++)
  {
    if(NeuralNet[i]->Axions.size() < MAX_AXIONS) printf("C%x=%x--", NeuralNet[i]->id, NeuralNet[i]->Axions.size());
    if(NeuralNet[i]->Dendrites.size() < MAX_DENDRITES)printf("P%x=%x--", NeuralNet[i]->id, NeuralNet[i]->Dendrites.size());
  }

  tmp11 = 0;

  cout<<"\nEnter number of Input Layer Neurons and Output Layer Neurons: \n>";
  int in, out;
  cin>>in>>out;
  vector<Neuron_t*> ttn;
  for(int i = 0; i < in; i++)
  {
    tmpn = new Neuron_t(0, 2);
    input_layer.push_back(tmpn);
    for(int j = 0; j < 5; j++)
    {
      tmp11 = 0;
      back2:
      rand1 = rand()%(Neurons-1);
      if(tmp11 == Neurons)  break;
      if(find(ttn.begin(), ttn.end(), NeuralNet[rand1]) == ttn.end())
      {
        //normal_distribution<double> dis2(0, 1/(pow(MAX_DENDRITES, 0.5)));
        GreyMatter.push_back(Connect_Neurons(tmpn, NeuralNet[rand1], 1, 1));
        ttn.push_back(NeuralNet[rand1]);
      }
      else
      {
        ++tmp11;
        goto back2;
      }
    }
  }
  cout<<"\nInput Layer produced...";

  tmp11 = 0;

  RewardFeederNeuron = new Neuron_t(0, 3);
  //For Output Layer
  /*
  for(int i = 0; i < out; i++)
  {
    tmpn = new Neuron_t(0, 1);
    output_layer.push_back(tmpn);
    tmpn->LocalReward = 0;
    GreyMatter.push_back(Connect_Neurons(tmpn, RewardFeederNeuron, 1, 4));
    //Connect_Neurons(tmpn, RewardFeederNeuron, 1, 4);
    for(int j = 0; j < 16; j++)
    {
      tmp11 = 0;
      back3:
      rand1 = rand()%(Neurons-1);
      if(tmp11 == Neurons)  break;
      if(find(ttn.begin(), ttn.end(), NeuralNet[rand1]) == ttn.end())
      {
        //normal_distribution<double> dis2(0, 1/(pow(MAX_AXIONS, 0.5)));
        GreyMatter.push_back(Connect_Neurons(NeuralNet[rand1], tmpn, 1, 4));
        //Connect_Neurons(NeuralNet[rand1], tmpn, 1, 4);
        ttn.push_back(NeuralNet[rand1]);
      }
      else
      {
        ++tmp11;
        goto back3;
      }
    }
  }
    /*/

    for(int i = 0; i < out; i++)
    {
      tmp11 = 0;
      plotVecON.push_back(0);
      back3:
      rand1 = rand()%(Neurons-1);
      tmpn = NeuralNet[rand1];
      
      if(tmp11 == Neurons)  break;
      if(find(ttn.begin(), ttn.end(), tmpn) == ttn.end())
      {
        output_layer.push_back(tmpn);
        GreyMatter.push_back(Connect_Neurons(tmpn, RewardFeederNeuron, 1, 4));
        //normal_distribution<double> dis2(0, 1/(pow(MAX_AXIONS, 0.5)));
        //GreyMatter.push_back(Connect_Neurons(input_layer[rand1], tmpn, 1, 1));
        ttn.push_back(tmpn);
        
      }
      else
      {
        ++tmp11;
        goto back3;
      }
    }//*/
/*
  for(int i = 0; i < out; i++)
  {
    NeuralNet.push_back(output_layer[i]);
  }*/
  cout<<"\nOutput Layer produced... Neural Network Completed!";
}

void InputThread()
{
  while(1)
  {
    for(int i = 0; i < input_layer.size(); i++)
    {
      float tmp;
      cout<<"\nInput number "<<i<<"> ";
      cin>>tmp;
      input_layer[i]->ValSum = tmp;
      input_layer[i]->Output = tmp;
      input_layer[i]->Fired = true;
    }
    cout<<"\nEnter Reward: ";
    cin>>RewardGenerated;
  }
}

void OutputThread()
{
  while(1)
  {
    //Global_OutputGenerator(output_layer);
  }
}

void ForwardPropogatorThread()
{
  while(1)
  {
    Global_ForwardProcessor(GreyMatter);
    Global_OutputGenerator(output_layer);
   // Global_Adjuster(input_layer);
  }
}

void LearnerThread()
{
  //while(1)
  {
    //printf("\n\nDOne!!");
  }
}

void TimeDecayThread()
{
  while(1)
  {
    Global_Refresher(NeuralNet);
  }
}

int main()
{
  NeuralNet_init();
  RewardGenerated = 0;
  for(int i = 0; i < input_layer.size(); i++)
  {
    float tmp;
    cout<<"\nInput number "<<i<<"> ";
    cin>>tmp;
    input_layer[i]->ValSum = tmp;
    input_layer[i]->Output = tmp;
    input_layer[i]->Fired = true;
  }
  thread forwardPropogator(ForwardPropogatorThread);
  thread timeDecayThread(TimeDecayThread);
  thread inputThread(InputThread);
  thread outputThread(OutputThread);

  while(1)
  {
    /*thread learnerThread(LearnerThread);
    learnerThread.join();
    thread teacherThread(Global_Teacher);
    teacherThread.join();
    thread renewThread(Global_Renew);
    renewThread.join();*/
    LearnerThread();
    Global_Teacher();
    Global_Renew();
    //printf("\nDone!");
  }
  forwardPropogator.join();
  return 0;
}

#pragma once
#include "neuron.h"

void Global_RewardSpreader(Neuron_t* RewardFeederNeuron, float Reward);
void Global_RewardFeeder(int i);
void Global_Teacher();
void Global_Renew();
void Global_Refresher(vector<Neuron_t*> nnet);
void Global_ForwardProcessor(vector<Neurite_t<Neuron_t, Neuron_t>*> dnet);

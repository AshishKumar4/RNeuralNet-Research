#include "header.h"

template <class In, class Out>
class Connection_t
{
  float var;
public:
  In InSource;
  Out OutSource;

  float weight;
  float mylin;

  float Role; // From InSource to OutSource
};

class Soma_t
{
  float TPotential;

public:
  vector<float> CurrentInputs;
  vector<float> FinalInputs;

  float Sum;

  float Output;
  float OldOutput;

  float LocalReward;

  bool fired;

  Soma_t()
  {

  }

  float Calc()    // Summation and Function Applier
  {

  }
};

class Neuron_t : public Soma_t
{
public:
  vector<Connection_t<Soma_t, Soma_t>>  InBounds;
  vector<Connection_t<Soma_t, Soma_t>>  OutBounds;

  Neuron_t()
  {

  }

};

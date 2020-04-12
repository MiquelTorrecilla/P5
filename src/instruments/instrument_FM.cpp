#include <iostream>
#include <math.h>
#include "instrument_FM.h"
#include "keyvalue.h"
#include "stdio.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentFM::InstrumentFM(const std::string &param)
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param"
    and configure your instrument.
    Take a Look at keyvalue.h
  */

  KeyValue kv(param);
if(!kv.to_float("I",I)) I = 0.5;
if(!kv.to_float("N1_N2",N)) N = 0.5;
if (!kv.to_float("F0_FM", fc)) fc = 0.5;
}

void InstrumentFM::command(long cmd, long note, long vel) {

  if (cmd == 9) {
    bActive = true;
    adsr.start();
    f0=440*(pow(2,((note-69.)/12.)));
    nota=f0/SamplingRate;
    fm=N*nota;
    I=I*f0;
    A = vel / 127.;
   }
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {  //Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentFM::synthesize() {

  if (!bActive)
    return x;

  if (!adsr.active()) {
    bActive = false;
    x.assign(x.size(), 0);
    return x;
  }

  for (unsigned int i=0; i<x.size(); ++i) {


    x[i]=A*sin(a+I*sin(b));

    a = a + 2*M_PI*nota;
    b = b + 2*M_PI*fm;

    while (a>M_PI)
      a-=2*M_PI;
    while (b>M_PI)
      b-=2*M_PI;
  }

  adsr(x); //apply envelope to x (and updates internal status of adsr)

  return x;
}

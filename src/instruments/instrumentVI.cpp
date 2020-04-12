#include <iostream>
#include <math.h>
#include "instrumentVI.h"
#include "keyvalue.h"

#include <stdlib.h>


using namespace upc;
using namespace std;

InstrumentVI::InstrumentVI(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" 
    and configure your instrument.
    Take a Look at keyvalue.h    
  */

  KeyValue kv(param);

  if (!kv.to_int("N", N)) N = 40; //default value
  float vibrato;
  if (!kv.to_float("VIBRATO", vibrato)) vibrato = 1.0; //default value
  if (!kv.to_float("FM", fm)) fm = 250; //default value
fprintf(stdout,"vibrato--->%f\n",kv.to_float("VIBRATO",vibrato));
fprintf(stdout,"fm--->%f\n",fm);
fprintf(stdout,"vibratof--->%f\n",vibrato);




  I_f = pow(2, vibrato / 12.);

  I_f = fm * (I_f - 1) / (I_f + 1);

}

void InstrumentVI::command(long cmd, long note, long vel) {

  if (cmd == 9) {
    bActive = true;
    adsr.start();
    index = 0;

	float f0 = 440 * pow(2, (note - 69.)/12.);
	alpha = 2.0F * M_PI * f0 / SamplingRate; /* fc = f0 * N */
	beta = 2.0F * M_PI * fm / SamplingRate;
//
	//I = 2 * M_PI * I_f / f0 / SamplingRate;
	I = I_f / f0;

	A =  vel / 127.;
	phaseM = 0;
	phaseC = 0;
  } else if (cmd == 8) {
    adsr.stop();
  }
    else if (cmd == 0) {  //Sound extinguished without waiting for release to end
    adsr.end();
  }
}

const vector<float> & InstrumentVI::synthesize() {

  if (!bActive)
    return x;

  if (!adsr.active()) {
    bActive = false;
    x.assign(x.size(), 0);
    return x;
  }

  for (unsigned int i=0; i<x.size(); ++i) {
	x[i] = A * sin(phaseC + I * sin(phaseM));

	phaseC += alpha;
	phaseM += beta;

	while(phaseC > 2*M_PI) phaseC = phaseC - M_PI;
	while(phaseM > 2*M_PI) phaseM = phaseM - M_PI;
  }

  adsr(x); //apply envelope to x (and updates internal status of adsr)

  return x;
}


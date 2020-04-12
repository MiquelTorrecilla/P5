#ifndef INSTRUMENT_VIB
#define INSTRUMENT_VIB

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentVI: public upc::Instrument {
    EnvelopeADSR adsr;
    unsigned int index;
	float A,alpha,beta,phaseC,phaseM,fm,I_f,I;
	int N;
  public:
    InstrumentVI(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif

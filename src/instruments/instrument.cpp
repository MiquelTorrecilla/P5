#include <iostream>
#include "instrument_dumb.h"
#include "seno.h"
#include "chello.h"
#include "keyvalue.h"
#include "instrument_FM.h"
#include "instrumentVI.h"
#include "envelope_adsr.h"
/*
  For each new instrument:
  - Add the header in this file
  - Add the call to the constructor in get_instrument() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Instrument * get_instrument(const string &name,
 			      const string &parameters) {
    Instrument * pInst = 0;
    //    cout << name << ": " << parameters << endl;
    if (name == "InstrumentDumb") {
      pInst = (Instrument *) new InstrumentDumb(parameters);
    }
    else if (name == "Instrumentseno"){
	pInst = (Instrument *) new Instrumentseno(parameters);
    }
    else if (name == "Instrumentchello"){
	pInst = (Instrument *) new Instrumentchello(parameters);
    }
    else if (name == "InstrumentFM"){
	pInst = (Instrument *) new InstrumentFM(parameters);
    }
    else if (name == "InstrumentVI"){
	pInst = (Instrument *) new InstrumentVI(parameters);
    }
    return pInst;
  }
}

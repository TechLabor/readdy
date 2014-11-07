//
// C++ Interface: start_state_srsim
//
// Description: 
//
//
// Author: Gerd Gruenert <gerdl@ipc758>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RUNMODIF_SRSIM_H
#define RUNMODIF_SRSIM_H

#include "pointers.h"
#include "atom_vec_srsim.h"
#include <SRSim/reactant_template.h>

namespace LAMMPS_NS {

/**
@author Gerd Gruenert
*/
class RunModifSRSim : protected Pointers
  {
  public:
    RunModifSRSim(class LAMMPS *lmp);
   ~RunModifSRSim();
  
    void command(int argc, char **argv);
    
   private:
    AtomVecSRSim *avec;
   
    void createAtomsByName (double xlo, double xhi, double ylo, double yhi, double zlo, double zhi, char* templ, int num2add);
    void createAtomsByMap  (double xlo, double xhi, double ylo, double yhi, double zlo, double zhi, char* fname, int num2add);
    void createAtoms       (double xlo, double xhi, double ylo, double yhi, double zlo, double zhi, SRSim_ns::ReactantTemplate* rt, int num2add);
    //void createDNATiles   (double xlo, double xhi, double xlo, double xhi, double xlo, double xhi, char* templ, int num2add);
    //void setSystemToRigid (char *onoff);
    };

}

#endif

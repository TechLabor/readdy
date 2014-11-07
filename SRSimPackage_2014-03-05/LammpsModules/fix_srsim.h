/* ----------------------------------------------------------------------
               fix_srsim.h
------------------------------------------------------------------------- */

#ifndef FIX_SRSIM_H
#define FIX_SRSIM_H

#include "fix.h"
#include "atom_vec_srsim.h"

#include <SRSim/defs.h>
#include <SRSim/rule_set.h>


namespace LAMMPS_NS {

/**
 *   executes Reactions and keeps the molecule's fitting-template-lists up to date.
 */
class FixSRSim : public Fix {
 friend class UpdateCallback;
 public:
   FixSRSim (class LAMMPS *, int, char **);
  ~FixSRSim ();
  
  int  setmask        ();
  void init           ();
  void final_integrate();
  
 private:
  class RanMars           *rng;
  class ComputeReapotAtom *computeReapot;
  char                   **computeCmd;
  
  SRSim_ns::RuleSet       *rset;
  AtomVecSRSim            *avec;
  
  int                      refractoryTime;
  
  // should be = 0 in the normal run-phase
  //int                      readyCountdown;
  
  
  
  /// The real simulation-modifying procedures:
  //void           addNewBond         (int i, int j_tag, int i_site, int j_site);
  //void           breakBond          (int i, int site);
  //void           addNewReactants    (int templID, double x, double y, double z);
  //vector<double>                  recDelLocalizeGraph(int idx, bool del, int distance);  
  
  //void updateInitial              ( );
  //void recUpdateTemplateData      (int startIdx, int distance);
  
};



}

#endif

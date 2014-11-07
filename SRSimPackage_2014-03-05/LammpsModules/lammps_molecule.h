/************************************************************************
  			lammps_molecule.h - Copyright gerdl

**************************************************************************/

#ifndef LAMMPS_MOLECULE_H
#define LAMMPS_MOLECULE_H
#include <string>
#include <SRSim/molecule.h>

#include "atom_vec_srsim.h"
#include "atom.h"

namespace LAMMPS_NS {

  // forward declarations:
  class AtomVecSRSim;
  
/**  This works on the data of a running Lammps-Simulation.
  *    uniqueID = avec->tag[i] 
  *
  *  as the number of LammpsMolecules can get very high, we'll use an array of the
  *  size of the Lammps - atom count to lookup already existent molecules.
  *     -> so don't forget to delete this static array with "staticDelete()" .
  *
  */
class LammpsMolecule : public SRSim_ns::Molecule
   {
   public:
   
    LammpsMolecule (int _idx);
   ~LammpsMolecule ();   // has to kill all the molecules it spawned!
   
    static void init (Atom* _atom, AtomVecSRSim *_avec);
    static void grow (int _nmax);
    static void staticDelete ();
    //static LammpsMolecule
   
    int        getType               ();
    int        numSites              ();
    int        getSiteType           (int sid);
    Molecule*  getMoleculeAtSite     (int sid);
    int        getModificationAtSite (int sid);
    int        getOtherSiteIDAtSite  (int sid);  // returns -1 if not connected.
    int        getUniqueID           ();
    
    static Atom                          *atom;
    static AtomVecSRSim                  *avec;
    static SRSim_ns::MoleculeTypeManager *mtm;
    static int                            nmax;
    static LammpsMolecule**               allMols;  // Array auf Pointer von LammpsMolecule-en
    static int                            refCnt;
    
    int                                   idx;
    
   private:
    stack<LammpsMolecule*>  spawnedMols;
    };
}

#endif //LAMMPS_MOLECULE_H


/* ----------------------------------------------------------------------
			atom_vec_srsim.h
------------------------------------------------------------------------- */

#ifndef ATOM_VEC_SRSIM_H
#define ATOM_VEC_SRSIM_H


#include <SRSim/defs.h>
#include <SRSim/rule_set.h>
#include <SRSim/sr_model.h>
#include <SRSim/reactant_template.h>

#include <list>
#include <set>

// #include "atom_vec_angle.h"
// #define SUPERCLASS	AtomVecAngle
#include "atom_vec_molecular.h"
#include "dump_rea.h"
#define SUPERCLASS	AtomVecMolecular

namespace LAMMPS_NS {

// we need an abstract definition first...
class AtomVecSRSim;
class DumpRea;


// general Callback
class SRSimCallback {
 public:
  virtual void run (int localIdx) = 0;
  };

// Callback for updating template data
class UpdateCallback : public SRSimCallback
  {
  public:
       UpdateCallback (AtomVecSRSim *_myAvec) : myAvec(_myAvec) {}
  void run (int localIdx);
  private:
  AtomVecSRSim *myAvec;
  };
  
// Callback for updating molecule-graph angles
class ReactivateAnglesCallback : public SRSimCallback
  {
  public:
       ReactivateAnglesCallback (AtomVecSRSim *_myAvec) : myAvec(_myAvec) {}
  void run (int localIdx);
  private:
  AtomVecSRSim *myAvec;
  };
  
  

class AtomVecSRSim : public SUPERCLASS/*AtomVecAngle*/ {
 public:                   
   AtomVecSRSim(class LAMMPS *, int, char **);
  ~AtomVecSRSim();
  
  void setRuleSys (SRSim_ns::SRModel *_srmodel);
  
  void grow(int);
  //void reset_special();
  void copy(int, int);
  int pack_comm(int, int *, double *, int, int *);
  void unpack_comm(int, int, double *);
  int pack_reverse(int, int, double *);
  void unpack_reverse(int, int *, double *);
  int pack_border(int, int *, double *, int, int *);
  int pack_border_one(int, double *);
  void unpack_border(int, int, double *);
  int unpack_border_one(int, double *);
  int pack_exchange(int, double *);
  int unpack_exchange(double *);
  int size_restart();
  int pack_restart(int, double *);
  int unpack_restart(double *);
  void create_atom(int, double *);
  void data_atom(double *, int, char **);
  int data_atom_hybrid(int, char **);
  double memory_usage();
  
  SRSim_ns::SRModel *srmodel;
  SRSim_ns::RuleSet *rset;
  DumpRea           *reaDumper;
  
  //int  *num_sites;           // Molecule-Type-Manager knows this...
#ifndef USE_TEMPL_AFFIL_MANAGER
  vector<vector<vector<int> > >   site_templs;
         vector<vector<int> >      all_templs; // any template fitting the molecule
                vector<int>     amount_templs; // Appearance of Templates...  Numbers of Molecules with this trait.
         //vector<set   <int> >   repres_templs; // a set of representatives for each template
#else 
  SRSim_ns::TemplAffiliationManager affiliations;
#endif
  
  int  **site_bond_id;      // either -1 or the atom->bond_atom - id. (since the site ids are not equal to the bond ids!)
  int  **site_other_site;   // other site id, if bound.
  int  **site_modified;     // either -1 or the modification id.
  int  **site_bound_tag;    // either -1 or the tag of the connected atom
                             
  int  *type2numSites;       // size: MaxType... so use small type-ids!
                             // rset->MoleculeTypeManager knows this info too, but we want it fast,
                             // so let's make a local copy in a standard array.
  
  stack<int> visitedStack;   // used when going recursively through the graph.
  int       *visited;        // same with this one...
                             // e.g. when updating templates or blacklisting molecules which already reacted.
  
  list<int> refractoryList;  // id's of refractory molecules
  int      *refractory;      // ntimestep the refractory time should be over.
                             // -1 if molecule is not in a refractory state...
                             
  //bool      rigid_system;    // 
  int       mol_Id_Cnt;      // each template added to the sim will be given a unique molecule id... 
                             //  (here, molecule is meant in the LAMMPS and not in the SRSim sense)
                             //  hence, rigid particle systems can be used later

  static string SRSimBnglName;
  static string SRSimMgeoName;
  static string SRSimTgeoName;
  static int    rndSeed;
  static bool   restartInfoAvail;
  
  bool templatesNeedRecalculation;  // e.g. after a restart no templates are set.
  void checkIfTemplateRecalculationNecessary();
  
                             
                             
  // ------------                             
                               
  typedef std::pair<int,int> SiteIDs;
                             
  vector< int >
       findAllFittingSites (int idx, int needType, int needModif, bool needFree);
#ifndef USE_TEMPL_AFFIL_MANAGER
  int  findFittingSite     (int idx, int needTemplate );
  bool fitsToTemplate      (int idx, int needTemplate);
#endif
  int  findSiteByTargetTag (int idx, int targetTag);
  int  findOtherEnd        (int idx, int site );
  
  void recVisit            (int idx, SRSimCallback *cback=NULL, int depth=0, int maxDepth=-1);
  void unVisitAll          ();
  
  // add / remove bonds, atoms, templates:
  void addNewBond             (int i, int j_tag, int i_site, int j_site);
  void breakBond              (int i, int site);
  void addTemplate2Sim        (SRSim_ns::ReactantTemplate *rt, double x, double y, double z);
  void removeAtomsByTag       (set<int>    atomTagsToRemove);
  void removeAtomsById        (vector<int> atomIdsToRemove);
  
  // refractory states:
  void refractMol          (int i, int until_ts);
  void unRefract           ();
  
  // recalculating data for angles and templates:
  void updateSubgraphTemplateData (int startIdx);
  void updateSingleTemplateData   (int idx);
  
  void reactivateSubgraphAngles   (int startIdx, int depth);
  void activateAngles             (int idx);
  
  // Globale Vars that should not be subject of deconstructions!

  };

/**
 * TODO:  werden schon alle Felder angelegt? wieder freigegeben?
 */

}

#endif



/************************************************************************
  			lammps_molecule.cpp - Copyright gerdl

**************************************************************************/

#include "lammps_molecule.h"

#include <stdlib.h>

using namespace LAMMPS_NS;

 
// static data definition:

Atom*                          LammpsMolecule::atom = NULL;
AtomVecSRSim*                  LammpsMolecule::avec = NULL;
SRSim_ns::MoleculeTypeManager *LammpsMolecule::mtm  = NULL;
int                            LammpsMolecule::nmax    = 0;
int                            LammpsMolecule::refCnt  = 0;
LammpsMolecule**               LammpsMolecule::allMols = NULL;
    
// member functions:

LammpsMolecule::LammpsMolecule( int _idx ) : idx(_idx)
  {
  //printf ("LM %p ==> Lammps mol %d \n",this, _idx);
  //printf ("We still have %d LammpsMolecules.  allMol[%d]=%p\n", refCnt, idx, allMols[idx] );
    
  assert( allMols[idx] == NULL );
  refCnt ++;
  
  //printf ("New LammpsMol %d -> total = %d\n", idx, refCnt);
  
  allMols[idx] = this;
  }

LammpsMolecule::~LammpsMolecule ()   // has to kill all the molecules it spawned!
  {
  while (! spawnedMols.empty() )
     {
     LammpsMolecule *m = spawnedMols.top();
     delete m;
     spawnedMols.pop();
     }
  
  refCnt--;
  allMols[idx] = NULL;
  
  //printf ("Del LammpsMol %d -> total = %d\n", idx, refCnt);
  }


/*static*/
void LammpsMolecule::init(Atom* _atom, AtomVecSRSim * _avec)
  {
  atom = _atom;
  avec = _avec;
  mtm  = avec->srmodel->mtm;  // get the type manager...
  }


/*static*/
void LammpsMolecule::grow(int _nmax)
  {
  int nOld =  nmax;
      nmax = _nmax;
  
  allMols = (LammpsMolecule**) realloc ( allMols, nmax*sizeof(LammpsMolecule*));
  for (int i=nOld ; i<nmax ; i++) allMols[i] = NULL;//new LammpsMolecule(i);
  }
  
/*static*/
void LammpsMolecule::staticDelete( )
  {
  printf("Destruction! [LammpsMolecule::staticDelete]\n");
  if (allMols != NULL) {free (allMols);}
  allMols = NULL;
  
  refCnt  = 0;
  atom    = NULL;
  avec    = NULL;
  mtm     = NULL;
  nmax    = 0;
  refCnt  = 0;
  allMols = NULL;
}


int LammpsMolecule::getType ()
  {
  // type id's begin with 1 in Lammps... so it's x-1
  // or maybe this is going to be a mess - so let's stay with x+0
  //printf ("Asked for type: %d.\n", atom->type[idx]);
  return atom->type[idx] ;//- 1;    
  }

int LammpsMolecule::numSites ()
  { // we'll ask the local copy of the molecule-type-manager... :
  int type = atom->type[idx];
  return avec->type2numSites[type];
  }
  
int LammpsMolecule::getSiteType (int sid)
  {
  int type = atom->type[idx];
  return mtm->getSiteType(type, sid);
  }
  
  
/// See definition of site_bound_to in AtomVecSRSim to understand what happens next:
//  int         **site_bound_to; // either -1 or the site-index of the bond
//                               // sites with a value < -99 are have their Lammps-bond defined on the 
//                               // other side (so (-sbt-100) is the tag of the other End of the bond. )
//                               //           _____________________________________
//                               // the bond is declared in the atom with the smaller tag!
SRSim_ns::Molecule* LammpsMolecule::getMoleculeAtSite (int sid)
  {
  //printf ("Asked for next mol at site %d.\n", sid);
  
  int siteBound = avec->site_bound_tag[idx][sid];
  if (siteBound == -1) return NULL;  // not bound.
  
  // idx of the next Molecule:
  int nextIdx = atom->map(siteBound);
  
  if (allMols[nextIdx] == NULL)
     {
     LammpsMolecule *nextM = new LammpsMolecule(nextIdx);
     spawnedMols.push (nextM);    // we want to delete the mols we created, later.
     //printf ("getMoleculeAtSite: nextIdx=%d\n",nextIdx);
     return nextM;
     }
  else
     return allMols[nextIdx];
  }
  
int LammpsMolecule::getModificationAtSite (int sid)
  {
  return avec->site_modified[idx][sid];
  }
  

int LammpsMolecule::getOtherSiteIDAtSite (int sid)  // returns -1 if not connected.
  {
  return avec->site_other_site[idx][sid];
  }


int LammpsMolecule::getUniqueID ()
  {
  return atom->tag[idx];
  }





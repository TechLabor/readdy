//#define NDEBUG
#include "defs.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "multi_mol_reactant_template.h"
#include "sr_error.h"
#include "templ_site.h"
#include "templ_molecule.h"


using namespace SRSim_ns;


bool MultiMolReactantTemplate::matchMolecule (Molecule *against)
  {
  for (int i=0 ; i<mols.size() ; i++)
      if ( matchSingleTM(against,mols[i]) ) return true;
  return false;
  }


/**
 *   In this variation of matchMolecules, we want the list of the other Molecules
 *     that also fit into this molecule's template returned.
 *   There is probably more than a single good solution though.
 *   Only one of them will be returned.
 *
 *   Beware: we assume that we know the molecules and the template _does_ match.
 *           If no match is found, we'll throw an assertion!
 */
vector<int> MultiMolReactantTemplate::returnMatchedMoleculeTags (Molecule *against)
  {
  
  assert (against!=NULL);
  
  // find matching
  bool ret;
  for (int i=0 ; i<mols.size() ; i++)
      {
      ret = recTryMatching      ( against, mols[i], /*recLayer*/1, /*i_startSite*/-1, /*j_startSite*/-1);
      if (ret) break;
      }
  assert( ret );
  
  // remember a vector of the matched molecules:
  vector<int> matched ( numMolecules() );
  for (int i=0 ; i<mols.size() ; i++)
      matched[i] = mols[i]->getRealization();
  
  unmarkTemplate();
  
  return matched;
  }




  
  
bool MultiMolReactantTemplate::equals (ReactantTemplate *other)
  {
  if (other->getRTType() != getRTType()) return false;
  if (dynamic_cast<MultiMolReactantTemplate*>(other) == NULL) return false;
  
  TemplMolecule *otherOne = other->getMolecule(0);  // use arbitrary molecule from the other template
  
  for (int i=0 ; i<mols.size() ; i++)               // and test against all our own ones...
      if ( matchSingleTM(otherOne,mols[i], true) )  // exact=true 
         return true;
  return false;
  }

  
ReactantTemplate * SRSim_ns::MultiMolReactantTemplate::clone( )
  {
  MultiMolReactantTemplate *mmrt = new MultiMolReactantTemplate(this);
  return mmrt;
  }

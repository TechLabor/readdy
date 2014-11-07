

//#define NDEBUG
#include "defs.h"
#include <assert.h>
#include <cstdio>

#include "names_manager.h"


using namespace SRSim_ns;


NamesManager::NamesManager (bool singleUser)
  {
  if (singleUser) myNumTypes = 1;
  else            myNumTypes = numTypes;
  names = new vector<string> [numTypes];
  }

NamesManager::~NamesManager ()
  {
  for (int i=0 ; i<myNumTypes ; i++) 
      names[i].clear();
  delete [] names;
  }
  

int NamesManager::getMaxID (NameTp tp)
  {
  return names[tp].size();
  }


/** returns the ID if name exists,
            otherwise creates the new name and returns its id;
*/
int NamesManager::getID (NameTp tp, string name)
  {
  //printf ("tp,myNumTypes = %d,%d\n", tp, myNumTypes);
  assert (tp < myNumTypes);
  
  // maybe this name already exists?
  for (int i=0 ; i<names[tp].size() ; i++)
      if (0 == names[tp][i].compare(name))
         return i;
      
  // no, does not exist -> create new one.
  //string x(name);
  names[tp].push_back(string(name));
  return (names[tp].size()-1) ;
  }


/** returns the ID if name exists,
            otherwise returns -1;
*/
int NamesManager::existsID (NameTp tp, string name)
  {
  assert (tp < myNumTypes);
  
  // maybe this name already exists?
  for (int i=0 ; i<names[tp].size() ; i++)
      if (0 == names[tp][i].compare(name))
         return i;
      
  // no, does not exist -> 
  return -1;
  }

string NamesManager::getName(NameTp tp, int id) 
  {
  assert (tp < myNumTypes);
  assert (tp >= 0);
  assert (id >= 0);
  //assert (id < names[tp].size());
  if (id >= names[tp].size())
     {
     printf ("Requested tp = %d, id = %d   ... names[tp].size() = %d \n", tp, id, names[tp].size());
     assert( id < names[tp].size() );
     }
  
  return names[tp][id];
  }

  
int NamesManager::getID (string name)
  {
  assert (myNumTypes==1);
  return getID (MoleculeSpeciesName, name);
  }

// returns -1 if name doesn't exist.
int NamesManager::existsID (string name)
  {
  assert (myNumTypes==1);
  return existsID (MoleculeSpeciesName, name);
  }

string NamesManager::getName(int id) 
  {
  assert (myNumTypes==1);
  return getName(MoleculeSpeciesName, id);
  }

 
void SRSim_ns::NamesManager::printAll( )
  {
  printf ("Printing all names that are known to the NamesManager: \n");
  //assert(false);
  for (int j=0 ; j<myNumTypes ; j++)
      {
      printf ("  Name-Type %d: \n", j);
      for (int i=0 ; i<names[j].size() ; i++)
          printf ("    N%d : %s \n",i,getName( (NameTp)j, i).c_str());
      }
  }
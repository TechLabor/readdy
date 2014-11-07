/************************************************************************
  			names_manager.h - Copyright gerdl

**************************************************************************/

#ifndef NAMES_MANAGER_H
#define NAMES_MANAGER_H
#include <string>
#include <vector>


namespace SRSim_ns {

using namespace std;

/** Manages Molecule, Site, etc... names. */
class NamesManager
   {
   public:
    NamesManager (bool singleUser = false);
   ~NamesManager ();
    
    enum NameTp {
    	MoleculeSpeciesName = 0,
    	SiteName            = 1,
    	ModificationName    = 2,
    	PatternName         = 3,
    	VariableName        = 4
    	};
    	
    int    getMaxID (NameTp tp);
    int    getID    (NameTp tp, string name);
    int    existsID (NameTp tp, string name);
    string getName  (NameTp tp, int id);
    
    // for outside users of only a single type...
    int    getID    (string name);  
    int    existsID (string name);  // returns -1 if it doesn't exist.
    string getName  (int id);
    
    void   printAll ();
    
   private:
    static const int numTypes = 5;
    
    int             myNumTypes;
    vector<string> *names;
    };

    
    
}    



#endif 


//#define NDEBUG
#ifndef DEFS_H
#define DEFS_H

#define SRSIM_UNDEFINED -1 

#define STAGE          1.01
#define SRSIM_VERSION  "2.08"

// maybe we can ignore some tedious assert-constructions...
// #define SRSIM_IGNORE_SLOW_ASSERTIONS

// we would like to try using the new class TemplRealization and TemplAffiliationManager:
//#define USE_TEMPL_AFFIL_MANAGER


#endif  // defs.h




/*
 Version History:
 
 2.0 : can do dihedral angles
       can export a value corresponding to the state of a site modification
       
 2.01b: can now implement exchange rules... changing the number of molecules per rules!
 
 2.02: some bug fixes
 
 2.03: fixed two bugs from 2.02:
        - bngl files with tabs around modifiers were parsed wrongly.
        - we didn't update the templates far enough in graphs - now set the update distance
          to maxReactiveTemplateSize() + 1
          
 2.04: - added a new dump_bonds module, so that we can plot the realized bonds in a network
       - killed a bug, SRSim crashes when running multiple runs - LammpsMolecule didn't tidy up correctly.
          
 2.05: - killed one more bug about dihedral angles (hopefully)
       - added support for dump bonds (BNGL_GRAPHS|BOND_SET) so that we 
         can output the graph of species residing in the reactor in 
         the form of a BNGL string.
       - examples/002 was changed - hopefully working again, now.
 
 2.06: - re-added support for writing/reading restart files!
 
 2.07: - support for dump_rea (writes reaction network from the actually executed reactions)

 2.08: - should hopefully compile now... renamed the UNDEFINED variable...
 
 */



/************************************************************************
  			templ_realization.h - Copyright gerdl
                         see file LICENSE in this distribution 
                        
      Set of all the realizing molecules in a reactor, that fit to the 
      Template tid...
      we need one of these TemplRealizations for every Template... mhm...

**************************************************************************/

#ifndef TEMPL_REALIZATION_H
#define TEMPL_REALIZATION_H

#include <vector>

namespace SRSim_ns {

   
class TemplAffiliationManager
  {
  public:
   //TemplAffiliationManager (RandomGenerator &_rand) : rand(_rand);     //
  
   bool belongsToTempl     (int tid, int mid);
   //bool belongsToTempl   (int tid, int mid, int site);
   int  sidRandFittingSite (int tid, int mid, double rand);
   int  midNthTemplReal    (int tid, int n);         // retrieves the moleculeID of this template realiz.
   int  numTemplAffils     (int tid);
  
   void addTemplToMol      (int tid, int mid);
   void addTemplToMol      (int tid, int mid, int site);
   
   void clearMolAffil      (int mol);
   void copyMol            (int from, int to);
   //void delMol             (int mid);
   //void addMol           (int n);
  
  private:
   
   class TemplRealization
     {
     public:
     std::vector<std::vector<int> > mol;           // x = molSite[i][j]:   x is either empty or the j'th realization in mol-i
     std::vector<int>               realizationM;  // realizationM[i]  is the molecule-id of the i-th realization
     std::vector<int>               realizationS;  // realizationS[i]  is the site-id     of the i-th realization, or -1 if it's not site-specific!
    
     /// k = mol[i];    or molSite[i][j]
     ///   -->  realization[k] == i
     void deleteRealizationByMoveFromBehind (int real2del);
     void copyMol                           (int from, int to);
     };
     
   std::vector<TemplRealization>  reals;   // one Realization class for each template:
   //std::vector<std::vector<int> >           molOverview; // molOverview[x] = y: 
   //void expandTemplateNumber();
   
  };



} // ends namespace

#endif //TEMPL_REALIZATION_H


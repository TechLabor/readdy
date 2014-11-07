
#include "defs.h"
#include <assert.h>

#include "start_state_definition.h"
#include "templ_site.h"
#include "templ_molecule.h"
#include "names_manager.h"
#include "bng_rule_builder.h"
#include "reactant_template.h"
//#include "multi_mol_reactant_template.h" 
#include "bound_reactant_template.h"
#include "sr_error.h"
#include "sr_model.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>



using namespace std;
using namespace SRSim_ns;

void have_fun(int argc, char **argv)
  {
  printf ("aheu\n");
  
  SRModel model(12345, argv[1], argv[2], argv[3], true);
  
  //printf ("b-heu\n");
                   
  assert( argc == 4 );                     
  //BNGRuleBuilder p(&model);
  //p.readFile(argv[1]); 
  
  //printf ("c-heu\n");
  
  //model.geo->readMGeoFile("spass.geo");
  
  //printf ("d-heu\n");
  
  /*ReactantTemplate *rt1 = p.parseBNGTemplate ("A(r,l!1).A(l,r!1)");
  //ReactantTemplate *rt2 = p.parseBNGTemplate ("A(r!1).A(l!1)");
  
  BoundReactantTemplate *brt = dynamic_cast<BoundReactantTemplate*>(model.ruleset->getRT(2));
  printf ("brt = %p\n", brt);
  printf ("T2 : matches site 0 =%d\n", brt->matchMolecule(rt1->getMolecule(0),0) );
  printf ("T2 : matches site 1 =%d\n", brt->matchMolecule(rt1->getMolecule(0),1) );
  rt1->getMolecule(0)->writeToDotFile("spass.dot");
  */
  vector<RuleTp*> &vrt = model.ruleset->getRules();
  for (int i=0 ; i<vrt.size() ; i++)
      cout << vrt[i]->toString() << "  at Rate= " << model.kinetics->getRate(i) << endl;
      
  printf ("\n\n\n");
  
  for (int i=0 ; i<model.ruleset->numTemplates() ; i++)
      {
      ReactantTemplate *rt = model.ruleset->getRT(i);
      
      cout << "T"<<i<<" : ";
      if      (rt->getRTType() == ReactantTemplate::MultiMolRT) cout<<"[MMRT]";
      else if (rt->getRTType() == ReactantTemplate::BoundRT   ) cout<<"[BRT] ";
      else if (rt->getRTType() == ReactantTemplate::SiteRT    ) cout<<"[SRT] ";
      else if (rt->getRTType() == ReactantTemplate::ModifRT   ) cout<<"[MRT] ";
      else assert(false);
      
      if      (rt->isUsableAs(ReactantTemplate::observableRT)) cout<<"<obs>";
      else if (rt->isUsableAs(ReactantTemplate::creatableRT )) cout<<"<cre>";
      else if (rt->isUsableAs(ReactantTemplate::reactingRT  )) cout<<"<rea>";
      else if (rt->isUsableAs(ReactantTemplate::senselessRT )) cout<<"<sls>";
      else                                                     cout<<"<???>";   // modification target template!
      
      cout <<" : "<<rt->getName()<<endl;
      }
  
  printf ("\n\n\n");
  

  //printf ("T11 and T22 are %s\n", 
       //(model.ruleset->getRT(11)->equals( model.ruleset->getRT(21) ) ? ("equal"):("distinct")) );
  









/*  for (int i=0 ; i<model.sstate->numTemplates() ; i++)
      {
      ReactantTemplate *rss = model.sstate->getRT(i);
      
      bool found = false;
      for (int j=0 ; j<model.ruleset->numTemplates() ; j++)
          {
          ReactantTemplate *rrs = model.ruleset->getRT(j);
          if ( rrs->equals(rss) ) 
             {
             printf ("  SSt-Templ-%d == RS-Templ-%d\n",i,j);
             found=true;
             }
          }
      if (!found) printf ("Nix gefindet!\n");
      }
      
  printf ("Filling system with species:\n");
  int numItems = model.sstate->numItems2Create();
  for (int i=0 ; i<numItems ; i++)
      {
      StartStateDefinition::Element e = model.sstate->getNextItem();
      
      int myType = -1;
      for (int j=0 ; j<model.sstate->numTemplates() ; j++)
          {
          ReactantTemplate *rss = model.sstate->getRT(j);
          if (rss->equals(e.rt)) {myType = j; break;}
          }
      
      printf ("x=%f,   y=%f,   z=%f,      Type=%d\n", e.x, e.y, e.z, myType);
      }*/
      
  /*for (int i=0 ; i<rs1->numTemplates() ; i++)
      {
      stringstream fname;
      fname<<"spass"<<i<<".dot";
      rs1->getRT(i)->writeTemplateToDotFile(&n2, fname.str().c_str());
      }*/
      
      
  return;
  }


  
  
int main (int argc, char **argv)
  {
  try { have_fun (argc, argv); }
  catch (SRException sre) {sre.report("sorry - no further info available...");}
  
  return 0;
  }

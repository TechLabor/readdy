//#define NDEBUG
#include "defs.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "rule_set.h"

#include "bound_reactant_template.h"
#include "site_reactant_template.h"
#include "modification_reactant_template.h"
#include "simple_mass_action_kinetics.h"

using namespace SRSim_ns;

RuleSet* RuleTp::rset;   // declaration of static element!

string RuleTp::toString()
  {
  stringstream sst;
  for (int i=0 ; i<in.size() ; i++)
      {
      //sst << in[i];
      sst << rset->getRT(in[i])->getName();
      if (i<in.size()-1) sst << " + ";
      }
  
  //if (reversible) sst << " <-> ";
  //else            sst <<  " -> ";
  sst <<  " -> ";
    
  for (int i=0 ; i<out.size() ; i++)
      {
      //sst << out[i];
      sst << rset->getRT(out[i])->getName();
      if (i<out.size()-1) sst << " + ";
      }
      
  if      (type == BindR)         sst << "  |BindR|";   // it's a bind rule...
  else if (type == BreakR)        sst << "  |BreakR|";   
  else if (type == ExchangeR)     sst << "  |ExchangeR|"; 
  else if (type == ModifyR)       sst << "  |ModifyR|"; 
  else if (type == BindIntramolR) sst << "  |BindIntramolR|";
  else assert(false);
  
  return sst.str();
  }

/* ---------------------------------------------------------------------- */
  
RuleSet::RuleSet( )
  {
  //kinetics
  RuleTp::rset  = this;   // init the static rset member of all RuleTp's !
  //TRMapping2nd  = NULL;
  //TRMappingSize = NULL;
  
  maxReactiveTemplSize = 0;
  }

/* ---------------------------------------------------------------------- */
  
RuleSet::~RuleSet ()
  {
  /*int dim = templates.size();
  for (int i=0 ; i<dim ; i++)
      {
      for (int j=0 ; j<dim ; j++)
          if (TRMapping2nd[i][j]!=NULL) delete[] TRMapping2nd[i][j];
      delete[] TRMapping2nd[i];
      delete[] TRMappingSize[i];
      }
  delete[] TRMapping2nd;
  delete[] TRMappingSize;*/
  
  for (int i=0 ; i<rules.size() ; i++)
      delete rules[i];
  //printf ("we've still got %d registered templates!\n", templates.size() );
  for (int i=0 ; i<templates.size() ; i++)
      delete templates[i];
  rules.clear();
  templates.clear();
  //delete kinetics;
  }

  
////////////  
// RuleSet:
////////////
  
  


string RuleSet::toString(  )
{
  stringstream out;
    
  for (int i=0 ; i<numRules() ; i++)
      {
      out << getRule(i)->toString() << endl;
      }
  
  return out.str();
}



  
/** 
 *  returns either the id of the stored or newly registered template...
 *     really compares the content-graph of the templates!
 */
int RuleSet::getTemplateID( ReactantTemplate * rt )
  {
  for (int i=0 ; i<templates.size() ; i++)
     if (templates[i]->equals(rt) ) return i;
     
  // so we've got to register & save this one...
  ReactantTemplate *rtnew = rt->clone();
  
  templates.push_back( rtnew );
  
  /*if (templates.size()-1 == 22)
     {
     printf ("!!!!!!!!!!!1 : %s   srt1-Type %d\n",rt->getName().c_str(), ((SiteReactantTemplate*)rt   )->getStartSite()->getType() );
     printf ("!!!!!!!!!!!1 : %s   srt1-Type %d\n",rt->getName().c_str(), ((SiteReactantTemplate*)rtnew)->getStartSite()->getType() );
     }*/
     
  
  return (templates.size()-1);
  }

/**
 *   Only searches for the pointer with the right id! 
 *      BEWARE: DOES NOT COMPARE THE TEMPLATE-GRAPHS!!!!
 */
int RuleSet::searchTemplateID( ReactantTemplate * rt )
  {
  for (int i=0 ; i<templates.size() ; i++)
     if ( templates[i] == rt ) return i;
  return -1;
  }

  
/*int SRSim_ns::RuleSet::registerTemplate( ReactantTemplate * rt )
  {
  templates.push_back( rt );
  
  //printf ("regging: %d\n", templates.size());
  
  // and register the molecules of this template:
  //for (int j=0 ; j < rt->numMolecules() ; j++)
  //    molecules.registerMolecule( rt->getMolecule(j) );
  //  now we're registering the molecules directly at the MoleculeTypeManager
                          
  return (templates.size()-1) ;
  }*/

/**
 *       Registers a new Rule in our ruleset...
 *       RuleSet saves a local copy of the template, so the caller can
 *               delete the ReactantTemplates after calling addNewRule...
 *    @rtype:   type of rule; Beware: Break Rules are not set explicitly, but
 *              determined from the in/out- reactant numbers!
 *    @returns: the rule-ids which have been registered.
 */
vector<int> SRSim_ns::RuleSet::addNewRule(vector<ReactantTemplate*> in, vector<ReactantTemplate*> out, RuleTp::RuleTpType rtype, bool reversible)
  {
  assert (in.size() <= 2 && out.size() <= 2);   // we are limited to binary rules by now...
  
  RuleTp *r = new RuleTp();
  
  r->type = rtype;
  if (rtype == RuleTp::BindR         && in.size()==1) r->type = RuleTp::BreakR;
  if (rtype == RuleTp::BindIntramolR && in.size()==1) r->type = RuleTp::BreakR;
  
  // registering templates...
  for (int p=0 ; p<2 ; p++)
      {
      vector<ReactantTemplate*> &vrt = (p==0)?(in):(out);
      for (int i=0 ; i<vrt.size() ; i++)
          {
          // registering is done automatically by getTemplateID
          // the caller of addNewRule has to take care of disposing
          // of this template!
          int id = getTemplateID(vrt[i]);
          
          // let's see if we have to activate the usability as reactingRT or creatableRT:
          if (p==0 || reversible)                                 templates[id]->addUse( ReactantTemplate::reactingRT  );
          if (r->type==RuleTp::ExchangeR && (p==1 || reversible)) templates[id]->addUse( ReactantTemplate::creatableRT );
          
          // update maxTemplSize:
          ReactantTemplate *rt = templates[id];
          if (rt->isUsableAs( ReactantTemplate::reactingRT ))
            if (maxReactiveTemplSize < rt->numMolecules() )
                maxReactiveTemplSize = rt->numMolecules();
          
          // adding the rule elements:
          if   (p==0) r->in .push_back(id);
          else        r->out.push_back(id);
          }
      }
    
  vector<int> ids;
  rules.push_back(r);    // and put it onto the rules-vector...
  ids  .push_back(rules.size()-1);
  //printf ("my new rule: %s\n", r->toString().c_str());
  
  // if it's a reversible one we'll add the same only swap in/out and bind/break
  if (reversible)
     {
     RuleTp *rr = new RuleTp();
     rr->in        = r->out;
     rr->out       = r->in;
     
     rr->type      = rtype;
     if (rtype    == RuleTp::BindR         && out.size()==1) rr->type = RuleTp::BreakR;
     if (rtype    == RuleTp::BindIntramolR && out.size()==1) rr->type = RuleTp::BreakR;
  
     rules.push_back( rr );
     ids  .push_back(rules.size()-1);
     
     //printf ("reverse rule: %s\n", rr->toString().c_str());
     }
     
  return ids;
  }

  
/**
 *      Calculates the indices of Templates which fit to Molecule m.
 *
 * @wholeMolTempls : array of vectors which will be filled with any fitting template.
 * @specificTempls : array of vectors which will be filled with site specific templates,
 *                      which are BoundReactantTemplates only at the moment!
 *                   SiteReactantTemplates are more easyly identifyable later, so
 *                      wo won't bother to add them in here momentarily!
 * @amountTempls   : the simulation amount of molecules with this trait.
 */
 
#ifndef USE_TEMPL_AFFIL_MANAGER
void SRSim_ns::RuleSet::fillFittingTemplates(Molecule *m, vector<int> &wholeMolTempls, vector<vector<int> > &specificTempls, vector<int> &amountTempls)
#else
void SRSim_ns::RuleSet::fillFittingTemplates(Molecule *m, int mid, TemplAffiliationManager &affi)
#endif
  {
  int numSites = m->numSites();
   
  // clear the Template-Vectors and delete the old amounts from the amountTempls vector.
  
  //printf ("SRSim_ns::RuleSet::fillFittingTemplates:   clearing... ");
#ifndef USE_TEMPL_AFFIL_MANAGER
  for (int i=0 ; i<wholeMolTempls.size() ; i++)
      {
      amountTempls[wholeMolTempls[i]] --;
      assert (amountTempls[wholeMolTempls[i]] >= 0);
      }
  wholeMolTempls.clear();
  for (int i=0 ; i<numSites ; i++)
      specificTempls[i].clear();
#else
  affi.clearMolAffil( mid );
#endif
  //printf ("done.\n");
  
  
  // find the actual fitting templates:
  for (int it=0 ; it<templates.size() ; it++)
      {
      //printf ("t%d\n",it);
      ReactantTemplate *t   = templates[it];
      if (! t->isUsableAs(ReactantTemplate::reactingRT)) continue;  // we don't need to find these!
      
      // it's a MMRT or a SRT...
      if (t->getRTType() == ReactantTemplate::MultiMolRT || 
          t->getRTType() == ReactantTemplate::SiteRT)   
         { 
         //printf ("  no brt.\n");
         if (t->matchMolecule(m)) 
            {
            //printf ("       Added Template %d.\n",it);
#ifndef USE_TEMPL_AFFIL_MANAGER
            wholeMolTempls.push_back( it ); 
            amountTempls[it] ++ ;    // we found a new template of type it.
#else
            affi.addTemplToMol( it, mid );
#endif
            }
         }
         
      // it's a BRT or a MRT...
      else if (t->getRTType() == ReactantTemplate::BoundRT || 
               t->getRTType() == ReactantTemplate::ModifRT )
         {   // ok, try a site-specific matching.            
         //printf ("     brt.\n");
         bool added = false;
         for (int is=0 ; is<numSites ; is++)
             if (t->matchMolecule(m, is))     // site-specific matching...
                {
                //printf ("    Site %d of type %d matched to template %d.\n",is, m->getSiteType(is), it);
#ifndef USE_TEMPL_AFFIL_MANAGER
                specificTempls[is].push_back( it );
                if (!added) 
                   { 
                   wholeMolTempls.push_back( it ); 
                   amountTempls[it] ++ ;    // we found a new template of type it.
                   added=true; 
                   }   // also push it onto the regular templates.
#else
                // here, we can ignore the 'bool added'
                affi.addTemplToMol( it, mid, is );
		//printf ("Added template %d to mol %d at site %d\n",it,mid,is);
		//printf ("  Asking for t %d, mol %d --> gives me site %d\n", it,mid, affi.sidRandFittingSite(it,mid,0) );
#endif
                }
         }
         
      // it's an unknown ReactantTemplate-Type
      else assert (false); 
      }
  
  /*printf ("  *** Templates of %d: ", m->getUniqueID());
  for (int i=0 ; i<wholeMolTempls.size() ; i++)
      printf ("%d ", wholeMolTempls[i]);
  printf ("\n");
  
  printf ("SRSim_ns::RuleSet::fillFittingTemplates:   filled.\n");*/
  }


/**
 *   Writes all the bimolecular reactions fitting to wmTemplsA & wmTemplsB into the vector reas.
 *
 *  
 */
#ifndef USE_TEMPL_AFFIL_MANAGER
int SRSim_ns::RuleSet::fillFittingRules( vector<int> & wmTemplsA, vector<int> & wmTemplsB, int* reas )
  {
  if (TRMapping2nd.size() == 0)     // someone should have initialized this poor bastard!
     buildTRMapping();
  int numA = wmTemplsA.size();
  int numB = wmTemplsB.size();
  
  // maybe there's old trash in the vector...
  //reas.clear();

  // run through all possibilities given by the templates.  
  int cnt=0;
  for (int a=0 ; a<numA ; a++)
      for (int b=0 ; b<numB ; b++)
          {
          vector<int> &trmap  = TRMapping2nd[wmTemplsA[a]][wmTemplsB[b]];
          int          numMap = trmap.size();
          
          //printf ("%d - %d --> %d ergs\n", wmTemplsA[a], wmTemplsB[b], numMap);
          
          for (int i=0 ; i<numMap ; i++)
              reas[cnt++] = trmap[i];
          //      reas.push_back( trmap[i] );          // we shouldn't get any reaction twice!
          }
          
  return cnt;
  }
#else
int SRSim_ns::RuleSet::fillFittingRules( TemplAffiliationManager &affi, int molA, int molB, int* reas )
  {
  if (TRMapping2nd.size() == 0)     // someone should have initialized this poor bastard!
     buildTRMapping();
  
  int nt = numTemplates();

  // run through all possibilities given by the templates.  
  int cnt=0;
  for (int a=0 ; a<nt ; a++)
      {
      if (! affi.belongsToTempl(a,molA) ) continue;
      for (int b=0 ; b<nt ; b++)
          {
          if (! affi.belongsToTempl(b,molB) ) continue;
          
          vector<int> &trmap  = TRMapping2nd[a][b];
          int          numMap = trmap.size();
          
          //printf ("%d - %d --> %d ergs\n", wmTemplsA[a], wmTemplsB[b], numMap);
          for (int i=0 ; i<numMap ; i++)
              reas[cnt++] = trmap[i];
          //      reas.push_back( trmap[i] );          // we shouldn't get any reaction twice!
          }
      }
          
  return cnt;
  }
#endif
  
  

/**
 *   Run Over every reaction and writes to the TRMapping-matrices.
 */
void SRSim_ns::RuleSet::buildTRMapping( )
  {
/*  RuleTp *r1 = rules[7];
  SiteReactantTemplate *srt1 = (SiteReactantTemplate *)(templates[r1->in[0]]);
  SiteReactantTemplate *srt2 = (SiteReactantTemplate *)templates[r1->in[1]];
  BoundReactantTemplate *srt3 = (BoundReactantTemplate *)templates[r1->out[0]];
  printf ("Rule7: srt1 - type = %d \n",srt1->getStartSite()->getType());
  printf ("Rule7: srt2 - type = %d   ... templ=%d\n",srt2->getStartSite()->getType(), r1->in[1]);
  printf ("Rule7: srt3 - type = %d \n",srt3->getStartSite()->getType());*/
  
  // first reserve mem for the map:
  int dim = templates.size();
  TRMapping2nd.resize (dim, vector<vector<int> >(dim));
  
  // run over all reactions ( or better rules ):
  for (int i=0 ; i<rules.size() ; i++)
      {
      RuleTp *r = rules[i];
      
      if (r->in.size() == 2)  // we're only interested in 2nd order reactions.
         {
         int x = r->in[0], y = r->in[1];
         TRMapping2nd[x][y].push_back(i);
         if (x!=y) TRMapping2nd[y][x].push_back(i);
         }
      }
  
  printf ("SRSim_ns::RuleSet::buildTRMapping: TR-Mapping built!\n");
  
  // plot the rule-matrix:
//   for (int i=0 ; i<dim ; i++)
//       {
//       printf ("       i=%2d: ",i);
//       for (int j=0 ; j<dim ; j++)
//           {
//           int rule1 = (TRMapping2nd[i][j].size() > 0)?(TRMapping2nd[i][j][0]):(-1);
//           printf (" %2d",rule1);
//           }
//       printf ("\n");
//       }
      
  
  }

  
/* ------------------------------------------------------------------ */
  
  


/*bool SRSim_ns::RuleSet::checkMax2Reactants ()
  {
  for (int i=0 ; i<rules.size() ; i++)
      {
      if (rules[i]->in .size() > 2) return false;
      if (rules[i]->out.size() > 2) return false;
      }
  return true;
  }*/


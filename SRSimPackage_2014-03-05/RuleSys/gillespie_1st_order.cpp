//
// C++ Implementation: gillespie_1st_order
//
// Description: 
//
//
// Author: Gerd Gruenert <gerdl@ipc758>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "defs.h"
#include "gillespie_1st_order.h"
#include "random_generator.h"
#include "kinetics_definition.h"

#include <assert.h>
#include <math.h>

using namespace SRSim_ns;



//void SRSim_ns::Gillespie1stOrder::init(int rnd_seed, vector< ReactantTemplate * > * __templates, vector< RuleTp * > * __rules )
void SRSim_ns::Gillespie1stOrder::init(SRModel *model)
  {
  //_templates= & rs->templates; _rules= & rs->rules; 
  _rs       = model->ruleset;
  _kinetics = model->kinetics;
  _rng      = model->random;
  }


#ifndef USE_TEMPL_AFFIL_MANAGER
double SRSim_ns::Gillespie1stOrder::timeToReaction( vector< int > & amountTempls )
#else
double SRSim_ns::Gillespie1stOrder::timeToReaction (TemplAffiliationManager &affi)
#endif
  
  {
  vector<RuleTp*>    &rules    =  _rs->rules;         // _rules seems to be NULL ... has init been used?
  KineticsDefinition &kinetics = *_kinetics;
  RandomGenerator    &rng      = *_rng;
  
  //printf ("Hello?  %p \n",_rules);
  //printf ("size of rules: %d\n",rules.size());
  //printf(" Gillespie:\n");
  
  // first let's calculate a0
  double a0 = 0;
  for (int i=0 ; i<rules.size() ; i++)
      {
      //printf(" Grompf!\n", rules[i]->toString() );
      //printf(" Gillespie: processing Rule %s\n", rules[i]->toString().c_str() );
      
      int inSize = rules[i]->in.size();
      if (inSize > 1) continue;          // only view zero- and first-order reactions

      if (inSize == 0)
         {
         //a0 += rules[i]->propensity;
         a0 += kinetics.getRate( i );
         }
      else /*if (inSize == 1)*/
         {
         int tid = rules[i]->in[0];
         //a0 += amountTempls[tid] * correctionTemplAmount(tid) * rules[i]->propensity;
#ifndef USE_TEMPL_AFFIL_MANAGER
         a0 += amountTempls[tid] * correctionTemplAmount(tid) * kinetics.getRate( i );
#else
         a0 += affi.numTemplAffils(tid) * correctionTemplAmount(tid) * kinetics.getRate( i );
#endif
         //printf("    rate = %f \n", kinetics.getRate(i) );
         //printf("    delta_a = %f \n", amountTempls[tid] * correctionTemplAmount(tid) * kinetics.getRate( i ) );
         }
      }
      
  //printf ("A0 = %f \n",a0);
      
  if (a0 == 0) return (1e200);   // something reallllllly big!
      
  // now from a0 calculate time until next reaction:
  double r1  = rng.uniform();
  double tau = 1.0 / a0 * log (1.0 / r1);
  
  return tau;
  }



#ifndef USE_TEMPL_AFFIL_MANAGER
int SRSim_ns::Gillespie1stOrder::typeOfReaction(vector<int> &amountTempls)
#else
int SRSim_ns::Gillespie1stOrder::typeOfReaction (TemplAffiliationManager &affi)
#endif
  {
  KineticsDefinition &kinetics = *_kinetics;
  vector<RuleTp*>    &rules    =  _rs->rules;
  RandomGenerator    &rng      = *_rng;
  vector< pair<double,int> >  propab;
  
  // first let's calculate a0
  double a0 = 0;
  propab.push_back( pair<double,int>(0.0, -1) );
  for (int i=0 ; i<rules.size() ; i++)
      {
      int inSize = rules[i]->in.size();
      if (inSize > 1) continue;          // only view zero- and first-order reactions

      if (inSize == 0)
         {
         //a0 += rules[i]->propensity;
         a0 += kinetics.getRate( i );
         }
      else /*if (inSize == 1)*/
         {
         int tid = rules[i]->in[0];
         // a0 += amountTempls[tid] * correctionTemplAmount(tid) * rules[i]->propensity;
#ifndef USE_TEMPL_AFFIL_MANAGER
         a0 += amountTempls[tid] * correctionTemplAmount(tid) * kinetics.getRate( i );
#else
         a0 += affi.numTemplAffils(tid) * correctionTemplAmount(tid) * kinetics.getRate( i );
#endif
         }
      propab.push_back( pair<double,int>(a0, i) );
      }
  
  assert (a0 > 0);   // if it's still 0 we shouldn't ask for a reaction...!
  
  double r2  = rng.uniform();
         r2 *= a0;
  
  // I'm too lazy for binary search now: so let's just go through the list
  // TODO: binary search.
  for (int i=1 ; i<propab.size() ; i++)
      if (r2 < propab[i].first  &&  r2 > propab[i-1].first)
         return (propab[i].second);

  // we shouldn't reach this place!!!!
  assert (false);
  return -1;  
  }



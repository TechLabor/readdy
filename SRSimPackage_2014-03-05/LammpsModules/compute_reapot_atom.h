/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifndef COMPUTE_REAPOT_ATOM_H
#define COMPUTE_REAPOT_ATOM_H

#include <SRSim/defs.h>
#include <assert.h>
#include <SRSim/gillespie_1st_order.h>
#include <SRSim/geometry_definition.h>

#include "compute.h"
#include "atom_vec_srsim.h"
#include <vector>
#include <map>

//blubKrach e454252%%&$%234 plode

using namespace SRSim_ns;
namespace LAMMPS_NS {

/**
 *    searches for fitting reactions.
 */
class ComputeReapotAtom : public Compute {
 public:
   ComputeReapotAtom(class LAMMPS *, int, char **);
  ~ComputeReapotAtom();
  void   init();
  void   init_list(int, class NeighList *);
  void   compute_peratom();
  int    pack_reverse_comm(int, int, double *);
  void   unpack_reverse_comm(int, int *, double *);
  double memory_usage();

  bool   ignoreBimolecularReactions;  // (we can gain a lot of speed here ;-)
  
  
  typedef std::pair<int,int> SiteIDs;
    
  class Event {
    public:
    inline  Event (int _i, int _j, int _rid                ) {i=_i; j=_j; reaID=_rid;              /*printf ("#### Juhuuuu!. %p !\n",this);*/}
    inline  Event (int _i, int _j, int _rid, SiteIDs _sites) {i=_i; j=_j; reaID=_rid; sites=_sites;/*printf ("#### Juhuuuu!. %p !\n",this);*/}
    int     i,j;
    int     reaID;
    SiteIDs sites;
    
    void maybe_flip_ij (AtomVecSRSim *avec, RuleTp *r);
    };
  std::vector<Event*> events;
 
 
 private:
  class RanMars      *rng;
  AtomVecSRSim       *avec;
  GeometryDefinition *geo;
  class NeighList    *list;
  //double             min1,min2,min3;
  
  // geometric reaction constraints:
  double                maxReactionDistSq;    // MaxBondDist+deviDist!
  //double              deviAngle;            // deviation from ideal Angle to allow bonding
  //double              deviDist;             // deviation from ideal Distance to allow bonding
  
  Gillespie1stOrder  gillespie;
  
  
  //vector<double>   propensities;
  //double           gillesspie_a0;
 
  double               getAng          (Coords& xa, Coords& xb);
  Coords               getAtomCoords   (int i);
  void                 testSiteGeo     (std::vector<SiteIDs> &sVec, int i, int j);
  std::vector<SiteIDs> getAllSitePairs (RuleTp *r, int i, int j=-666, double distSq=-666.0);
  
};

}

#endif

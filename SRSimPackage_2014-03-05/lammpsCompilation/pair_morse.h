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

#ifndef PAIR_MORSE_H
#define PAIR_MORSE_H

#include "pair.h"

namespace LAMMPS_NS {

class PairMorse : public Pair {
 public:
  PairMorse(class LAMMPS *);
  ~PairMorse();
  void compute(int, int);
  void settings(int, char **);
  void coeff(int, char **);
  double init_one(int, int);
  void write_restart(FILE *);
  void read_restart(FILE *);
  void write_restart_settings(FILE *);
  void read_restart_settings(FILE *);
  double single(int, int, int, int, double, double, double, double &);

 protected:
  double cut_global;
  double **cut;
  double **d0,**alpha,**r0;
  double **morse1;
  double **offset;

  void allocate();
};

}

#endif

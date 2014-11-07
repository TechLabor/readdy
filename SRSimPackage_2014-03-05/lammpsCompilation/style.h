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

#ifdef AngleInclude
#endif

#ifdef AngleClass
#endif

#ifdef AtomInclude
#include "atom_vec_atomic.h"
#include "atom_vec_charge.h"
#include "atom_vec_hybrid.h"
#endif

#ifdef AtomClass
AtomStyle(atomic,AtomVecAtomic)
AtomStyle(charge,AtomVecCharge)
AtomStyle(hybrid,AtomVecHybrid)
#endif

#ifdef BondInclude
#endif

#ifdef BondClass
#endif

#ifdef CommandInclude
#include "change_box.h"
#include "create_atoms.h"
#include "create_box.h"
#include "delete_atoms.h"
#include "delete_bonds.h"
#include "displace_atoms.h"
#include "displace_box.h"
#include "minimize.h"
#include "read_data.h"
#include "read_restart.h"
#include "replicate.h"
#include "run.h"
#include "set.h"
#include "shell.h"
#include "temper.h"
#include "velocity.h"
#include "write_restart.h"
#endif

#ifdef CommandClass
CommandStyle(change_box,ChangeBox)
CommandStyle(create_atoms,CreateAtoms)
CommandStyle(create_box,CreateBox)
CommandStyle(delete_atoms,DeleteAtoms)
CommandStyle(delete_bonds,DeleteBonds)
CommandStyle(displace_atoms,DisplaceAtoms)
CommandStyle(displace_box,DisplaceBox)
CommandStyle(minimize,Minimize)
CommandStyle(read_data,ReadData)
CommandStyle(read_restart,ReadRestart)
CommandStyle(replicate,Replicate)
CommandStyle(run,Run)
CommandStyle(set,Set)
CommandStyle(shell,Shell)
CommandStyle(temper,Temper)
CommandStyle(velocity,Velocity)
CommandStyle(write_restart,WriteRestart)
#endif

#ifdef ComputeInclude
#include "compute_angle_local.h"
#include "compute_bond_local.h"
#include "compute_centro_atom.h"
#include "compute_cna_atom.h"
#include "compute_com.h"
#include "compute_com_molecule.h"
#include "compute_coord_atom.h"
#include "compute_dihedral_local.h"
#include "compute_displace_atom.h"
#include "compute_group_group.h"
#include "compute_gyration.h"
#include "compute_gyration_molecule.h"
#include "compute_heat_flux.h"
#include "compute_improper_local.h"
#include "compute_ke.h"
#include "compute_ke_atom.h"
#include "compute_msd.h"
#include "compute_msd_molecule.h"
#include "compute_pair_local.h"
#include "compute_pe.h"
#include "compute_pe_atom.h"
#include "compute_pressure.h"
#include "compute_rdf.h"
#include "compute_reduce.h"
#include "compute_reduce_region.h"
#include "compute_erotate_sphere.h"
#include "compute_property_atom.h"
#include "compute_property_local.h"
#include "compute_property_molecule.h"
#include "compute_stress_atom.h"
#include "compute_temp.h"
#include "compute_temp_com.h"
#include "compute_temp_deform.h"
#include "compute_temp_partial.h"
#include "compute_temp_profile.h"
#include "compute_temp_ramp.h"
#include "compute_temp_region.h"
#include "compute_temp_sphere.h"
#endif

#ifdef ComputeClass
ComputeStyle(angle/local,ComputeAngleLocal)
ComputeStyle(bond/local,ComputeBondLocal)
ComputeStyle(centro/atom,ComputeCentroAtom)
ComputeStyle(cna/atom,ComputeCNAAtom)
ComputeStyle(com,ComputeCOM)
ComputeStyle(com/molecule,ComputeCOMMolecule)
ComputeStyle(coord/atom,ComputeCoordAtom)
ComputeStyle(dihedral/local,ComputeDihedralLocal)
ComputeStyle(displace/atom,ComputeDisplaceAtom)
ComputeStyle(group/group,ComputeGroupGroup)
ComputeStyle(gyration,ComputeGyration)
ComputeStyle(gyration/molecule,ComputeGyrationMolecule)
ComputeStyle(heat/flux,ComputeHeatFlux)
ComputeStyle(improper/local,ComputeImproperLocal)
ComputeStyle(ke,ComputeKE)
ComputeStyle(ke/atom,ComputeKEAtom)
ComputeStyle(msd,ComputeMSD)
ComputeStyle(msd/molecule,ComputeMSDMolecule)
ComputeStyle(pair/local,ComputePairLocal)
ComputeStyle(pe,ComputePE)
ComputeStyle(pe/atom,ComputePEAtom)
ComputeStyle(pressure,ComputePressure)
ComputeStyle(rdf,ComputeRDF)
ComputeStyle(reduce,ComputeReduce)
ComputeStyle(reduce/region,ComputeReduceRegion)
ComputeStyle(erotate/sphere,ComputeERotateSphere)
ComputeStyle(property/atom,ComputePropertyAtom)
ComputeStyle(property/local,ComputePropertyLocal)
ComputeStyle(property/molecule,ComputePropertyMolecule)
ComputeStyle(stress/atom,ComputeStressAtom)
ComputeStyle(temp,ComputeTemp)
ComputeStyle(temp/com,ComputeTempCOM)
ComputeStyle(temp/deform,ComputeTempDeform)
ComputeStyle(temp/partial,ComputeTempPartial)
ComputeStyle(temp/profile,ComputeTempProfile)
ComputeStyle(temp/ramp,ComputeTempRamp)
ComputeStyle(temp/region,ComputeTempRegion)
ComputeStyle(temp/sphere,ComputeTempSphere)
#endif

#ifdef DihedralInclude
#endif

#ifdef DihedralClass
#endif

#ifdef DumpInclude
#include "dump_atom.h"
#include "dump_cfg.h"
#include "dump_custom.h"
#include "dump_dcd.h"
#include "dump_local.h"
#include "dump_xyz.h"
#endif

#ifdef DumpClass
DumpStyle(atom,DumpAtom)
DumpStyle(cfg,DumpCFG)
DumpStyle(custom,DumpCustom)
DumpStyle(dcd,DumpDCD)
DumpStyle(local,DumpLocal)
DumpStyle(xyz,DumpXYZ)
#endif

#ifdef FixInclude
#include "fix_addforce.h"
#include "fix_aveforce.h"
#include "fix_ave_atom.h"
#include "fix_ave_histo.h"
#include "fix_ave_spatial.h"
#include "fix_ave_time.h"
#include "fix_box_relax.h"
#include "fix_deform.h"
#include "fix_deposit.h"
#include "fix_drag.h"
#include "fix_dt_reset.h"
#include "fix_efield.h"
#include "fix_enforce2d.h"
#include "fix_evaporate.h"
#include "fix_gravity.h"
#include "fix_heat.h"
#include "fix_indent.h"
#include "fix_langevin.h"
#include "fix_lineforce.h"
#include "fix_minimize.h"
#include "fix_momentum.h"
#include "fix_move.h"
#include "fix_nph.h"
#include "fix_npt.h"
#include "fix_npt_sphere.h"
#include "fix_nve.h"
#include "fix_nve_limit.h"
#include "fix_nve_noforce.h"
#include "fix_nve_sphere.h"
#include "fix_nvt.h"
#include "fix_nvt_sllod.h"
#include "fix_nvt_sphere.h"
#include "fix_planeforce.h"
#include "fix_press_berendsen.h"
#include "fix_print.h"
#include "fix_orient_fcc.h"
#include "fix_recenter.h"
#include "fix_respa.h"
#include "fix_rigid.h"
#include "fix_set_force.h"
#include "fix_shake.h"
#include "fix_shear_history.h"
#include "fix_spring.h"
#include "fix_spring_rg.h"
#include "fix_spring_self.h"
#include "fix_store_coord.h"
#include "fix_store_force.h"
#include "fix_temp_berendsen.h"
#include "fix_temp_rescale.h"
#include "fix_thermal_conductivity.h"
#include "fix_tmd.h"
#include "fix_ttm.h"
#include "fix_viscosity.h"
#include "fix_viscous.h"
#include "fix_wall_lj126.h"
#include "fix_wall_lj93.h"
#include "fix_wall_reflect.h"
#endif

#ifdef FixClass
FixStyle(addforce,FixAddForce)
FixStyle(aveforce,FixAveForce)
FixStyle(ave/atom,FixAveAtom)
FixStyle(ave/histo,FixAveHisto)
FixStyle(ave/spatial,FixAveSpatial)
FixStyle(ave/time,FixAveTime)
FixStyle(box/relax,FixBoxRelax)
FixStyle(deform,FixDeform)
FixStyle(deposit,FixDeposit)
FixStyle(drag,FixDrag)
FixStyle(dt/reset,FixDtReset)
FixStyle(efield,FixEfield)
FixStyle(enforce2d,FixEnforce2D)
FixStyle(evaporate,FixEvaporate)
FixStyle(gravity,FixGravity)
FixStyle(heat,FixHeat)
FixStyle(indent,FixIndent)
FixStyle(langevin,FixLangevin)
FixStyle(lineforce,FixLineForce)
FixStyle(MINIMIZE,FixMinimize)
FixStyle(momentum,FixMomentum)
FixStyle(move,FixMove)
FixStyle(nph,FixNPH)
FixStyle(npt,FixNPT)
FixStyle(npt/sphere,FixNPTSphere)
FixStyle(nve,FixNVE)
FixStyle(nve/limit,FixNVELimit)
FixStyle(nve/noforce,FixNVENoforce)
FixStyle(nve/sphere,FixNVESphere)
FixStyle(nvt,FixNVT)
FixStyle(nvt/sllod,FixNVTSlodd)
FixStyle(nvt/sphere,FixNVTSphere)
FixStyle(orient/fcc,FixOrientFCC)
FixStyle(press/berendsen,FixPressBerendsen)
FixStyle(planeforce,FixPlaneForce)
FixStyle(print,FixPrint)
FixStyle(recenter,FixRecenter)
FixStyle(RESPA,FixRespa)
FixStyle(rigid,FixRigid)
FixStyle(setforce,FixSetForce)
FixStyle(shake,FixShake)
FixStyle(SHEAR_HISTORY,FixShearHistory)
FixStyle(spring,FixSpring)
FixStyle(spring/rg,FixSpringRG)
FixStyle(spring/self,FixSpringSelf)
FixStyle(store/coord,FixStoreCoord)
FixStyle(store/force,FixStoreForce)
FixStyle(temp/berendsen,FixTempBerendsen)
FixStyle(temp/rescale,FixTempRescale)
FixStyle(thermal/conductivity,FixThermalConductivity)
FixStyle(tmd,FixTMD)
FixStyle(ttm,FixTTM)
FixStyle(viscosity,FixViscosity)
FixStyle(viscous,FixViscous)
FixStyle(wall/lj126,FixWallLJ126)
FixStyle(wall/lj93,FixWallLJ93)
FixStyle(wall/reflect,FixWallReflect)
#endif

#ifdef ImproperInclude
#endif

#ifdef ImproperClass
#endif

#ifdef IntegrateInclude
#include "respa.h"
#include "verlet.h"
#endif

#ifdef IntegrateClass
IntegrateStyle(respa,Respa)
IntegrateStyle(verlet,Verlet)
# endif

#ifdef KSpaceInclude
#endif

#ifdef KSpaceClass
#endif

#ifdef MinimizeInclude
#include "min_cg.h"
#include "min_hftn.h"
#include "min_sd.h"
#endif

#ifdef MinimizeClass
MinimizeStyle(cg,MinCG)
MinimizeStyle(hftn,MinHFTN)
MinimizeStyle(sd,MinSD)
# endif

#ifdef PairInclude
#include "pair_buck.h"
#include "pair_buck_coul_cut.h"
#include "pair_coul_cut.h"
#include "pair_coul_debye.h"
#include "pair_dpd.h"
#include "pair_hybrid.h"
#include "pair_hybrid_overlay.h"
#include "pair_lj_cut.h"
#include "pair_lj_cut_coul_cut.h"
#include "pair_lj_cut_coul_debye.h"
#include "pair_lj_expand.h"
#include "pair_lj_gromacs.h"
#include "pair_lj_gromacs_coul_gromacs.h"
#include "pair_lj_smooth.h"
#include "pair_lj96_cut.h"
#include "pair_morse.h"
#include "pair_soft.h"
#include "pair_table.h"
#include "pair_yukawa.h"
#endif

#ifdef PairClass
PairStyle(buck,PairBuck)
PairStyle(buck/coul/cut,PairBuckCoulCut)
PairStyle(coul/cut,PairCoulCut)
PairStyle(coul/debye,PairCoulDebye)
PairStyle(dpd,PairDPD)
PairStyle(hybrid,PairHybrid)
PairStyle(hybrid/overlay,PairHybridOverlay)
PairStyle(lj/cut,PairLJCut)
PairStyle(lj/cut/coul/cut,PairLJCutCoulCut)
PairStyle(lj/cut/coul/debye,PairLJCutCoulDebye)
PairStyle(lj/expand,PairLJExpand)
PairStyle(lj/gromacs,PairLJGromacs)
PairStyle(lj/gromacs/coul/gromacs,PairLJGromacsCoulGromacs)
PairStyle(lj/smooth,PairLJSmooth)
PairStyle(lj96/cut,PairLJ96Cut)
PairStyle(morse,PairMorse)
PairStyle(soft,PairSoft)
PairStyle(table,PairTable)
PairStyle(yukawa,PairYukawa)
#endif

#ifdef RegionInclude
#include "region_block.h"
#include "region_cone.h"
#include "region_cylinder.h"
#include "region_intersect.h"
#include "region_prism.h"
#include "region_sphere.h"
#include "region_union.h"
#endif

#ifdef RegionClass
RegionStyle(block,RegBlock)
RegionStyle(cone,RegCone)
RegionStyle(cylinder,RegCylinder)
RegionStyle(intersect,RegIntersect)
RegionStyle(prism,RegPrism)
RegionStyle(sphere,RegSphere)
RegionStyle(union,RegUnion)
#endif

// package and user add-ons

#include "style_packages.h"
#include "style_user.h"
#include "style_user_packages.h"
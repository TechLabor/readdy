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

#include "string.h"
#include "dump_atom_neuneucharge.h"
#include "domain.h"
#include "atom.h"
#include "update.h"
#include "group.h"
#include "error.h"

using namespace LAMMPS_NS;

/* ---------------------------------------------------------------------- */

DumpAtomNeuneucharge::DumpAtomNeuneucharge(LAMMPS *lmp, int narg, char **arg) : Dump(lmp, narg, arg)
{
  if (narg != 5) error->all("Illegal dump atom command");

  scale_flag = 1;
  image_flag = 0;
  format_default = NULL;
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::init()
{
  if (image_flag == 0) size_one = 6;
  else size_one = 9;

  // default format depends on image flags

  delete [] format;
  if (format_user) {
    int n = strlen(format_user) + 2;
    format = new char[n];
    strcpy(format,format_user);
    strcat(format,"\n");
  } else {
    char *str;
    if (image_flag == 0) str = (char *) "%d %d %g %g %g %g";
    else str = (char *) "%d %d %g %g %g %d %d %d %g";
    int n = strlen(str) + 2;
    format = new char[n];
    strcpy(format,str);
    strcat(format,"\n");
  }

  // setup column string

  if (scale_flag == 0 && image_flag == 0)
    columns = "id type x y z q";
  else if (scale_flag == 0 && image_flag == 1)
    columns = "id type x y z ix iy iz q";
  else if (scale_flag == 1 && image_flag == 0)
    columns = "id type xs ys zs q";
  else if (scale_flag == 1 && image_flag == 1)
    columns = "id type xs ys zs ix iy iz q";

  // setup function ptrs

  if (binary && domain->triclinic == 0)
    header_choice = &DumpAtomNeuneucharge::header_binary;
  else if (binary && domain->triclinic == 1)
    header_choice = &DumpAtomNeuneucharge::header_binary_triclinic;
  else if (!binary && domain->triclinic == 0)
    header_choice = &DumpAtomNeuneucharge::header_item;
  else if (!binary && domain->triclinic == 1)
    header_choice = &DumpAtomNeuneucharge::header_item_triclinic;

  if (scale_flag == 1 && image_flag == 0 && domain->triclinic == 0)
    pack_choice = &DumpAtomNeuneucharge::pack_scale_noimage;
  else if (scale_flag == 1 && image_flag == 1 && domain->triclinic == 0)
    pack_choice = &DumpAtomNeuneucharge::pack_scale_image;
  else if (scale_flag == 1 && image_flag == 0 && domain->triclinic == 1)
    pack_choice = &DumpAtomNeuneucharge::pack_scale_noimage_triclinic;
  else if (scale_flag == 1 && image_flag == 1 && domain->triclinic == 1)
    pack_choice = &DumpAtomNeuneucharge::pack_scale_image_triclinic;
  else if (scale_flag == 0 && image_flag == 0)
    pack_choice = &DumpAtomNeuneucharge::pack_noscale_noimage;
  else if (scale_flag == 0 && image_flag == 1)
    pack_choice = &DumpAtomNeuneucharge::pack_noscale_image;

  if (binary) write_choice = &DumpAtomNeuneucharge::write_binary;
  else if (image_flag == 0) write_choice = &DumpAtomNeuneucharge::write_noimage;
  else if (image_flag == 1) write_choice = &DumpAtomNeuneucharge::write_image;

  // open single file, one time only

  if (multifile == 0) openfile();
  
  // Gerdl: a mapping from our excitation states to a floating point between 0.0 and 1.0:
  AtomVecSRSim           *av  = (AtomVecSRSim*) atom->avec;
  SRSim_ns::SRModel      *srm = av->srmodel;
  SRSim_ns::NamesManager *nm  = srm->names;
  
  nm->getID(SRSim_ns::NamesManager::ModificationName,"a");  // make sure these modifications do actually exist in the nm ;)
  nm->getID(SRSim_ns::NamesManager::ModificationName,"b");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"c");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"d");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"e");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"f");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"g");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"h");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"i");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"j");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"k");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"l");
  nm->getID(SRSim_ns::NamesManager::ModificationName,"x");
  
/*  printf ("modif ~a = %d\n", nm->getID(SRSim_ns::NamesManager::ModificationName,"a") );
  printf ("modif ~b = %d\n", nm->getID(SRSim_ns::NamesManager::ModificationName,"b") );
  printf ("modif ~c = %d\n", nm->getID(SRSim_ns::NamesManager::ModificationName,"c") );
  printf ("modif ~d = %d\n", nm->getID(SRSim_ns::NamesManager::ModificationName,"d") );
  printf ("modif ~e = %d\n", nm->getID(SRSim_ns::NamesManager::ModificationName,"e") );
  printf ("modif ~f = %d\n", nm->getID(SRSim_ns::NamesManager::ModificationName,"f") );
  printf ("modif ~g = %d\n", nm->getID(SRSim_ns::NamesManager::ModificationName,"g") );*/
  
  int max = nm->getMaxID( SRSim_ns::NamesManager::ModificationName );
  modifTranslation = new double [max];
  for (int i=0 ; i<max ; i++) modifTranslation[i] = 0.0;
  
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"a") ] = 0.01;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"b") ] = 0.5;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"c") ] = 0.6;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"d") ] = 0.7;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"e") ] = 0.8;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"f") ] = 0.9;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"g") ] = 0.2;
  
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"x") ] = 1.0;
  
  /*modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"a") ] = 0.01;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"b") ] = 0.2;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"c") ] = 0.4;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"d") ] = 0.6;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"e") ] = 0.8;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"f") ] = 1.0;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"g") ] = 0.8;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"h") ] = 0.7;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"i") ] = 0.5;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"j") ] = 0.3;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"k") ] = 0.2;
  modifTranslation[ nm->getID(SRSim_ns::NamesManager::ModificationName,"l") ] = 0.1;*/
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::modify_param(int narg, char **arg)
{
  if (strcmp(arg[0],"scale") == 0) {
    if (narg < 2) error->all("Illegal dump_modify command");
    if (strcmp(arg[1],"yes") == 0) scale_flag = 1;
    else if (strcmp(arg[1],"no") == 0) scale_flag = 0;
    else error->all("Illegal dump_modify command");
    return 2;
  } else if (strcmp(arg[0],"image") == 0) {
    if (narg < 2) error->all("Illegal dump_modify command");
    if (strcmp(arg[1],"yes") == 0) image_flag = 1;
    else if (strcmp(arg[1],"no") == 0) image_flag = 0;
    else error->all("Illegal dump_modify command");
    return 2;
  }
  return 0;
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::write_header(int ndump)
{
  if (multiproc) (this->*header_choice)(ndump);
  else if (me == 0) (this->*header_choice)(ndump);
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::count()
{
  if (igroup == 0) return atom->nlocal;

  int *mask = atom->mask;
  int nlocal = atom->nlocal;

  int m = 0;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) m++;
  return m;
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::pack()
{
  return (this->*pack_choice)();
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::write_data(int n, double *mybuf)
{
  (this->*write_choice)(n,mybuf);
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::header_binary(int ndump)
{
  fwrite(&update->ntimestep,sizeof(int),1,fp);
  fwrite(&ndump,sizeof(int),1,fp);
  fwrite(&domain->triclinic,sizeof(int),1,fp);
  fwrite(&boxxlo,sizeof(double),1,fp);
  fwrite(&boxxhi,sizeof(double),1,fp);
  fwrite(&boxylo,sizeof(double),1,fp);
  fwrite(&boxyhi,sizeof(double),1,fp);
  fwrite(&boxzlo,sizeof(double),1,fp);
  fwrite(&boxzhi,sizeof(double),1,fp);
  fwrite(&size_one,sizeof(int),1,fp);
  if (multiproc) {
    int one = 1;
    fwrite(&one,sizeof(int),1,fp);
  } else fwrite(&nprocs,sizeof(int),1,fp);
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::header_binary_triclinic(int ndump)
{
  fwrite(&update->ntimestep,sizeof(int),1,fp);
  fwrite(&ndump,sizeof(int),1,fp);
  fwrite(&domain->triclinic,sizeof(int),1,fp);
  fwrite(&boxxlo,sizeof(double),1,fp);
  fwrite(&boxxhi,sizeof(double),1,fp);
  fwrite(&boxylo,sizeof(double),1,fp);
  fwrite(&boxyhi,sizeof(double),1,fp);
  fwrite(&boxzlo,sizeof(double),1,fp);
  fwrite(&boxzhi,sizeof(double),1,fp);
  fwrite(&boxxy,sizeof(double),1,fp);
  fwrite(&boxxz,sizeof(double),1,fp);
  fwrite(&boxyz,sizeof(double),1,fp);
  fwrite(&size_one,sizeof(int),1,fp);
  if (multiproc) {
    int one = 1;
    fwrite(&one,sizeof(int),1,fp);
  } else fwrite(&nprocs,sizeof(int),1,fp);
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::header_item(int ndump)
{
  fprintf(fp,"ITEM: TIMESTEP\n");
  fprintf(fp,"%d\n",update->ntimestep);
  fprintf(fp,"ITEM: NUMBER OF ATOMS\n");
  fprintf(fp,"%d\n",ndump);
  fprintf(fp,"ITEM: BOX BOUNDS\n");
  fprintf(fp,"%g %g\n",boxxlo,boxxhi);
  fprintf(fp,"%g %g\n",boxylo,boxyhi);
  fprintf(fp,"%g %g\n",boxzlo,boxzhi);
  fprintf(fp,"ITEM: ATOMS %s\n",columns);
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::header_item_triclinic(int ndump)
{
  fprintf(fp,"ITEM: TIMESTEP\n");
  fprintf(fp,"%d\n",update->ntimestep);
  fprintf(fp,"ITEM: NUMBER OF ATOMS\n");
  fprintf(fp,"%d\n",ndump);
  fprintf(fp,"ITEM: BOX BOUNDS xy xz yz\n");
  fprintf(fp,"%g %g %g\n",boxxlo,boxxhi,boxxy);
  fprintf(fp,"%g %g %g\n",boxylo,boxyhi,boxxz);
  fprintf(fp,"%g %g %g\n",boxzlo,boxzhi,boxyz);
  fprintf(fp,"ITEM: ATOMS %s\n",columns);
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::pack_scale_image()
{
  int *tag = atom->tag;
  int *type = atom->type;
  int *image = atom->image;
  int *mask = atom->mask;
  double **x = atom->x;
  int nlocal = atom->nlocal;

  double invxprd = 1.0/domain->xprd;
  double invyprd = 1.0/domain->yprd;
  double invzprd = 1.0/domain->zprd;

  int m = 0;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      buf[m++] = tag[i];
      buf[m++] = type[i];
      buf[m++] = (x[i][0] - boxxlo) * invxprd;
      buf[m++] = (x[i][1] - boxylo) * invyprd;
      buf[m++] = (x[i][2] - boxzlo) * invzprd;
      buf[m++] = (image[i] & 1023) - 512;
      buf[m++] = (image[i] >> 10 & 1023) - 512;
      buf[m++] = (image[i] >> 20) - 512;
      buf[m++] = neuCharge(i);
    }

  return m;
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::pack_scale_noimage()
{
  int *tag = atom->tag;
  int *type = atom->type;
  int *mask = atom->mask;
  double **x = atom->x;
  int nlocal = atom->nlocal;

  double invxprd = 1.0/domain->xprd;
  double invyprd = 1.0/domain->yprd;
  double invzprd = 1.0/domain->zprd;
  
  int m = 0;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      buf[m++] = tag[i];
      buf[m++] = type[i];
      buf[m++] = (x[i][0] - boxxlo) * invxprd;
      buf[m++] = (x[i][1] - boxylo) * invyprd;
      buf[m++] = (x[i][2] - boxzlo) * invzprd;
      buf[m++] = neuCharge(i);
    }

  return m;
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::pack_scale_image_triclinic()
{
  int *tag = atom->tag;
  int *type = atom->type;
  int *image = atom->image;
  int *mask = atom->mask;
  double **x = atom->x;
  int nlocal = atom->nlocal;

  double lamda[3];

  int m = 0;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      buf[m++] = tag[i];
      buf[m++] = type[i];
      domain->x2lamda(x[i],lamda);
      buf[m++] = lamda[0];
      buf[m++] = lamda[1];
      buf[m++] = lamda[2];
      buf[m++] = (image[i] & 1023) - 512;
      buf[m++] = (image[i] >> 10 & 1023) - 512;
      buf[m++] = (image[i] >> 20) - 512;
      buf[m++] = neuCharge(i);
    }

  return m;
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::pack_scale_noimage_triclinic()
{
  int *tag = atom->tag;
  int *type = atom->type;
  int *mask = atom->mask;
  double **x = atom->x;
  int nlocal = atom->nlocal;

  double lamda[3];
  
  int m = 0;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      buf[m++] = tag[i];
      buf[m++] = type[i];
      domain->x2lamda(x[i],lamda);
      buf[m++] = lamda[0];
      buf[m++] = lamda[1];
      buf[m++] = lamda[2];
      buf[m++] = neuCharge(i);
    }

  return m;
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::pack_noscale_image()
{
  int *tag = atom->tag;
  int *type = atom->type;
  int *image = atom->image;
  int *mask = atom->mask;
  double **x = atom->x;
  int nlocal = atom->nlocal;

  int m = 0;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      buf[m++] = tag[i];
      buf[m++] = type[i];
      buf[m++] = x[i][0];
      buf[m++] = x[i][1];
      buf[m++] = x[i][2];
      buf[m++] = (image[i] & 1023) - 512;
      buf[m++] = (image[i] >> 10 & 1023) - 512;
      buf[m++] = (image[i] >> 20) - 512;
      buf[m++] = neuCharge(i);
    }

  return m;
}

/* ---------------------------------------------------------------------- */

int DumpAtomNeuneucharge::pack_noscale_noimage()
{
  int *tag = atom->tag;
  int *type = atom->type;
  int *mask = atom->mask;
  double **x = atom->x;
  int nlocal = atom->nlocal;

  int m = 0;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      buf[m++] = tag[i];
      buf[m++] = type[i];
      buf[m++] = x[i][0];
      buf[m++] = x[i][1];
      buf[m++] = x[i][2];
      buf[m++] = neuCharge(i);
    }

  return m;
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::write_binary(int n, double *mybuf)
{
  n *= size_one;
  fwrite(&n,sizeof(int),1,fp);
  fwrite(mybuf,sizeof(double),n,fp);
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::write_image(int n, double *mybuf)
{
  int m = 0;
  for (int i = 0; i < n; i++) {
    fprintf(fp,format,
	    static_cast<int> (mybuf[m]), static_cast<int> (mybuf[m+1]),
	    mybuf[m+2],mybuf[m+3],mybuf[m+4], static_cast<int> (mybuf[m+5]),
	    static_cast<int> (mybuf[m+6]), static_cast<int> (mybuf[m+7]), 
	    static_cast<int> (mybuf[m+8]));
    m += size_one;
  }
}

/* ---------------------------------------------------------------------- */

void DumpAtomNeuneucharge::write_noimage(int n, double *mybuf)
{
  int m = 0;
  for (int i = 0; i < n; i++) {
    fprintf(fp,format,
	    static_cast<int> (mybuf[m]), static_cast<int> (mybuf[m+1]),
	    mybuf[m+2],mybuf[m+3],mybuf[m+4],mybuf[m+5]);
    m += size_one;
  }
}


/*  --------------------- this one is new by Gerdl: --------------------------- */

double DumpAtomNeuneucharge::neuCharge(int n)
{
  AtomVecSRSim *av = (AtomVecSRSim*) atom->avec;
  
  // I'm just guessing now, that our site id is 0... doesn't have to be true...
  int mod = av->site_modified[n][0];
  
  //printf ("   modif(%d) = %d  --> %f\n", n, mod, modifTranslation[mod] );
  
  return modifTranslation[mod];
  
  //smallCounter++;
  //return (smallCounter % 20) / 20.0;
}

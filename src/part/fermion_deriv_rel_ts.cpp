/*
  -------------------------------------------------------------------
  
  Copyright (C) 2006-2017, Andrew W. Steiner

  This file is part of O2scl.
  
  O2scl is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  O2scl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with O2scl. If not, see <http://www.gnu.org/licenses/>.

  -------------------------------------------------------------------
*/
#include <o2scl/fermion_deriv_rel.h>
#include <o2scl/fermion_eff.h>
#include <o2scl/fermion_rel.h>
#include <o2scl/test_mgr.h>
// For access to global convert_units object
#include <o2scl/lib_settings.h>

using namespace std;
using namespace o2scl;
using namespace o2scl_const;

int main(void) {

  cout.setf(ios::scientific);
  
  test_mgr t;
  t.set_output_level(1);
  
  fermion_deriv sf(5.0,2.0);
  fermion_deriv sf2(5.0,2.0);
  fermion ef(5.0,2.0);

  sf.inc_rest_mass=false;

  fermion_deriv_rel snf;
  fermion_eff eff;
  
  t.test_rel(sf.m,5.0,1.0e-5,"mass_inheritance");

  if (false) {
    double psit=-10.0;
    double Tt=5.0;
    sf.mu=psit*Tt+sf.m;
    snf.calc_mu(sf,Tt);
    // 3.4516681584e-02 6.9032459031e-03 8.5396526330e-02
    // 1.3806130168e-03 1.5698274512e-02 1.8229610760e-01
    cout.precision(10);
    cout << sf.pr << " " << sf.n << " " << sf.en << endl;
    cout << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    exit(-1);
  }
  
  // -----------------------------------------------------------------
    
  cout << "----------------------------------------------------" << endl;
  cout << "Compare fermion_deriv_rel to fermion_eff" << endl;
  cout << "----------------------------------------------------" << endl;
  cout << endl;

  double T=1.0;
  sf.non_interacting=false;
  sf.m=2.5;
  sf.nu=1.99*T-sf.m+sf.ms;
  ef.non_interacting=false;
  ef.m=2.5;
  ef.nu=1.99*T+sf.ms;

  cout << "\"Non-degenerate\": " << endl;
  snf.calc_mu(sf,T);
  cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
       << sf.pr << endl;
  cout << "un: " << snf.unc.n << " " << snf.unc.ed << " " << snf.unc.en << " " 
       << snf.unc.pr << endl;
  eff.calc_mu(ef,T);
  cout << "ef: " << ef.n << " " << ef.ed-ef.n*ef.m << " " << ef.en << " " 
       << ef.pr << endl;
  t.test_rel(sf.n,ef.n,1.0e-4,"ndeg density");
  t.test_rel(sf.ed,ef.ed-ef.n*ef.m,1.0e-4,"ndeg energy density");
  t.test_rel(sf.pr,ef.pr,1.0e-4,"ndeg pressure");
  t.test_rel(sf.en,ef.en,1.0e-4,"ndeg entropy");
  cout << endl;
  
  cout << "\"Degenerate\": " << endl;
  sf.nu=2.01*T-sf.m+sf.ms;
  ef.nu=2.01*T+sf.ms;
  snf.calc_mu(sf,T);
  cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
       << sf.pr << endl;
  cout << "un: " << snf.unc.n << " " << snf.unc.ed << " " << snf.unc.en << " " 
       << snf.unc.pr << endl;
  eff.calc_mu(ef,T);
  cout << "ef: " << ef.n << " " << ef.ed-ef.n*ef.m << " " << ef.en << " " 
       << ef.pr << endl;
  t.test_rel(sf.n,ef.n,1.0e-4,"deg density");
  t.test_rel(sf.ed,ef.ed-ef.n*ef.m,1.0e-4,"deg energy density");
  t.test_rel(sf.pr,ef.pr,1.0e-4,"deg pressure");
  t.test_rel(sf.en,ef.en,1.0e-4,"deg entropy");
  cout << endl;

  // -----------------------------------------------------------------
  // Test derivatives for calc() functions
  // -----------------------------------------------------------------

  {
    cout << "Test derivatives (\"non-degenerate\", direct): " << endl;
    snf.method=fermion_deriv_rel::direct;
    double d1, d2, eps=1.0e-4;
    double dndmu, dndT, dsdT, dndm;

    cout << "T: " << T << endl;
    sf.m=5.0;
    sf.ms=5.0;
    sf.nu=1.0*T-sf.m+sf.ms;

    sf.nu=1.0*T-sf.m+sf.ms+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.nu=1.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.n;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.en;
    snf.calc_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;

    sf.ms=5.0+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;

    sf.ms=5.0;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.calc_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"ndeg dir dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"ndeg dir dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"ndeg dir dsdT");
    cout << endl;

    cout << "Test derivatives (\"degenerate\", direct): " << endl;

    sf.nu=4.0*T-sf.m+sf.ms+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.nu=4.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    sf.nu=4.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T+eps);
    d1=sf.n;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.en;
    snf.calc_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;
  
    sf.ms=5.0+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.ms=5.0;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.calc_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"deg dir dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"deg dir dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"deg dir dsdT");
    cout << endl;

    cout << "Test derivatives (\"non-degenerate\", by_parts): " << endl;
    snf.method=fermion_deriv_rel::by_parts;

    sf.nu=1.0*T-sf.m+sf.ms+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.nu=1.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    sf.nu=1.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T+eps);
    d1=sf.n;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.en;
    snf.calc_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;
  
    sf.ms=5.0+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.ms=5.0;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.calc_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"ndeg byp dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"ndeg byp dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"ndeg byp dsdT");
    cout << endl;

    cout << "Test derivatives (\"degenerate\", by_parts): " << endl;

    sf.nu=4.0*T-sf.m+sf.ms+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.nu=4.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    sf.nu=4.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T+eps);
    d1=sf.n;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.en;
    snf.calc_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;
  
    sf.ms=5.0+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.ms=5.0;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.calc_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"deg byp dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"deg byp dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"deg byp dsdT");
    cout << endl;
  }

  // -----------------------------------------------------------------
  // Test derivatives for pair() functions
  // -----------------------------------------------------------------

  if (false) {
    cout << "Test derivatives (pair, \"non-degenerate\", direct): " << endl;
    snf.method=fermion_deriv_rel::direct;
    double d1, d2, eps=1.0e-4;
    double dndmu, dndT, dsdT, dndm;
  
    sf.nu=1.0*T-sf.m+sf.ms+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.nu=1.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    sf.nu=1.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T+eps);
    d1=sf.n;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.pair_mu(sf,T+eps);
    d1=sf.en;
    snf.pair_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;

    sf.ms=5.0+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.ms=5.0;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.pair_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"ndeg dir dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"ndeg dir dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"ndeg dir dsdT");
    cout << endl;

    cout << "Test derivatives (pair, \"degenerate\", direct): " << endl;

    sf.nu=4.0*T-sf.m+sf.ms+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.nu=4.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    sf.nu=4.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T+eps);
    d1=sf.n;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.pair_mu(sf,T+eps);
    d1=sf.en;
    snf.pair_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;
  
    sf.ms=5.0+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.ms=5.0;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.pair_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"deg dir dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"deg dir dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"deg dir dsdT");
    cout << endl;

    cout << "Test derivatives (pair, \"non-degenerate\", by_parts): " << endl;
    snf.method=fermion_deriv_rel::by_parts;

    sf.nu=1.0*T-sf.m+sf.ms+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.nu=1.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    sf.nu=1.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T+eps);
    d1=sf.n;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.pair_mu(sf,T+eps);
    d1=sf.en;
    snf.pair_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;
  
    sf.ms=5.0+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.ms=5.0;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.pair_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << " "
	 << dndm << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"ndeg byp dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"ndeg byp dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"ndeg byp dsdT");
    cout << endl;

    cout << "Test derivatives (pair, \"degenerate\", by_parts): " << endl;

    sf.nu=4.0*T-sf.m+sf.ms+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.nu=4.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    sf.nu=4.0*T-sf.m+sf.ms;
    snf.pair_mu(sf,T+eps);
    d1=sf.n;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.pair_mu(sf,T+eps);
    d1=sf.en;
    snf.pair_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;
  
    sf.ms=5.0+eps;
    snf.pair_mu(sf,T);
    d1=sf.n;
    sf.ms=5.0;
    snf.pair_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;

    snf.pair_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << " "
	 << dndm << endl;
    t.test_rel(dndmu,sf.dndmu,1.0e-4,"deg byp dndmu");
    t.test_rel(dndT,sf.dndT,1.0e-4,"deg byp dndT");
    t.test_rel(dsdT,sf.dsdT,1.0e-4,"deg byp dsdT");
    cout << endl;
  }

  snf.method=fermion_deriv_rel::automatic;

  // -----------------------------------------------------------------

  {
    double t1, t2, t3, t4, t5;

    cout << "----------------------------------------------------" << endl;
    cout << "Compare calc_mu, calc_density, pair_mu, & pair_density" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << endl;

    cout << "Check calc_density()." << endl;
    snf.calc_mu(sf,1.5);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t1=sf.n;
    t2=sf.ed;
    t3=sf.en;
    t4=sf.pr;
    t5=sf.nu;
    snf.calc_density(sf,1.5);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t.test_rel(t1,sf.n,1.0e-6,"n");
    t.test_rel(t2,sf.ed,1.0e-6,"ed");
    t.test_rel(t3,sf.en,1.0e-6,"en");
    t.test_rel(t4,sf.pr,1.0e-6,"pr");
    t.test_rel(t5,sf.nu,1.0e-6,"nu");
    snf.calc_mu(sf,0.7);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t1=sf.n;
    t2=sf.ed;
    t3=sf.en;
    t4=sf.pr;
    t5=sf.nu;
    snf.calc_density(sf,0.7);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t.test_rel(t1,sf.n,1.0e-6,"n");
    t.test_rel(t2,sf.ed,1.0e-6,"ed");
    t.test_rel(t3,sf.en,1.0e-6,"en");
    t.test_rel(t4,sf.pr,1.0e-6,"pr");
    t.test_rel(t5,sf.nu,1.0e-6,"nu");

    cout << "Check pair_density()." << endl;
    snf.pair_mu(sf,1.5);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t1=sf.n;
    t2=sf.ed;
    t3=sf.en;
    t4=sf.pr;
    t5=sf.nu;
    snf.pair_density(sf,1.5);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t.test_rel(t1,sf.n,1.0e-6,"n");
    t.test_rel(t2,sf.ed,1.0e-6,"ed");
    t.test_rel(t3,sf.en,1.0e-6,"en");
    t.test_rel(t4,sf.pr,1.0e-6,"pr");
    t.test_rel(t5,sf.nu,1.0e-6,"nu");
    snf.pair_mu(sf,0.7);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t1=sf.n;
    t2=sf.ed;
    t3=sf.en;
    t4=sf.pr;
    t5=sf.nu;
    snf.pair_density(sf,0.7);
    cout << "sf: " << sf.n << " " << sf.ed << " " << sf.en << " " 
	 << sf.pr << " " << sf.nu << endl;
    t.test_rel(t1,sf.n,1.0e-6,"n");
    t.test_rel(t2,sf.ed,1.0e-6,"ed");
    t.test_rel(t3,sf.en,1.0e-6,"en");
    t.test_rel(t4,sf.pr,1.0e-6,"pr");
    t.test_rel(t5,sf.nu,1.0e-6,"nu");
    cout << endl;
  }

  // -----------------------------------------------------------------
  
  cout << "----------------------------------------------------" << endl;
  cout << "Test calc() and pair() functions w/wo interactions" << endl;
  cout << "----------------------------------------------------" << endl;
  cout << endl;

  double den;
  for(den=1.0e-6;den<=1.01e2;den*=1.0e2) {

    cout << "density: " << den << endl;

    fermion_deriv xsf(0.511/hc_mev_fm,2.0);
    double temper=10.0/hc_mev_fm, mu, d1, d2;
    double t1, t2, t3, t4;

    // -----------------------------------------------------------------

    xsf.non_interacting=false;
    xsf.inc_rest_mass=false;
    cout << "Interacting, removed rest mass: " << endl;
    
    xsf.n=den;
    xsf.nu=0.0;
    snf.calc_density(xsf,temper);
    cout << xsf.nu+xsf.m << " ";
    t1=xsf.nu+xsf.m;
    snf.calc_mu(xsf,temper);
    cout << xsf.n << " " << den << endl;
    t.test_rel(xsf.n,den,1.0e-6,"den1");

    xsf.n=den;
    xsf.nu=0.0;
    snf.pair_density(xsf,temper);
    mu=xsf.nu;
    cout << xsf.nu+xsf.m << " ";
    t2=xsf.nu+xsf.m;
    snf.pair_mu(xsf,temper);
    cout << xsf.n << " " << den << endl;
    t.test_rel(xsf.n,den,1.0e-6,"den2");
    
    xsf.nu=mu;
    snf.calc_mu(xsf,temper);
    d1=xsf.n;
    xsf.nu=-mu-2.0*xsf.m;
    snf.calc_mu(xsf,temper);
    d2=xsf.n;
    cout << den << " " << d1 << " " << d2 << " " << d1-d2 << endl;
	 
    t.test_rel(den,d1-d2,1.0e-6,"den3");
    t3=d1;
    t4=d2;
    cout << endl;

    // -----------------------------------------------------------------

    xsf.non_interacting=true;
    xsf.inc_rest_mass=false;
    cout << "Non-interacting, removed rest-mass: " << endl;
    
    xsf.n=den;
    xsf.mu=0.0;
    snf.calc_density(xsf,temper);
    cout << xsf.mu+xsf.m << " ";
    t.test_rel(xsf.nu+xsf.m,t1,1.0e-6,"nu1");
    snf.calc_mu(xsf,temper);
    cout << xsf.n << " " << den << endl;
    t.test_rel(xsf.n,den,1.0e-6,"den1");
    
    xsf.n=den;
    xsf.mu=0.0;
    snf.pair_density(xsf,temper);
    mu=xsf.mu;
    cout << xsf.mu+xsf.m << " ";
    t.test_rel(xsf.nu+xsf.m,t2,1.0e-6,"nu2");
    snf.pair_mu(xsf,temper);
    cout << xsf.n << " " << den << endl;
    t.test_rel(xsf.n,den,1.0e-6,"den2");
    
    xsf.mu=mu;
    snf.calc_mu(xsf,temper);
    d1=xsf.n;
    xsf.mu=-mu-2.0*xsf.m;
    snf.calc_mu(xsf,temper);
    d2=xsf.n;
    cout << den << " " << d1 << " " << d2 << " " << d1-d2 << endl;
	 
    t.test_rel(den,d1-d2,1.0e-6,"den3");
    t.test_rel(t3,d1,1.0e-6,"den4");
    t.test_rel(t4,d2,1.0e-6,"den5");
    cout << endl;
  }

  // -----------------------------------------------------------------
  // Test the specific heat of degenerate fermions (As a reminder,
  // note that the degenerate specific heat differs for relativistic
  // and non-relativistic particles by a factor of two. Below is the
  // case for relativistic particles.) The code below demonstrates
  // that the computation of the specific heat (and that of the
  // entropy and the pressure) fails for sufficiently low temperatures,
  // i.e. in the extremely degenerate case. [12/20/09 - This now
  // works better]

  snf.method=fermion_deriv_rel::automatic;

  cout << "Test degenerate specific heat:\n" << endl;
  cout << "err T(MeV)     pr           en           "
       << "C            C(deg appx)  C(next term)" << endl;
  sf.mu=0.0;
  for (T=3.0/hc_mev_fm;T>=0.001/hc_mev_fm;T/=3.0) {

    sf.init(o2scl_mks::mass_electron*
	    o2scl_settings.get_convert_units().convert("kg","1/fm",1.0),2.0);
    sf.non_interacting=true;
    sf.n=0.2;
    snf.calc_density(sf,T);
    sf.kf=cbrt(6.0*pi2/sf.g*sf.n);
    cout << T*hc_mev_fm << " " << sf.pr << " " << sf.en << " "
	 << T/sf.n*(sf.dsdT-sf.dndT*sf.dndT/sf.dndmu) << " "
	 << o2scl_const::pi*o2scl_const::pi*T/sf.mu << " " 
	 << pow(o2scl_const::pi/sf.kf,2.0)*T*sf.mu-
      pow(o2scl_const::pi*T/sf.kf,3.0)*o2scl_const::pi/15.0*
      (5.0*pow(sf.m,4.0)+4.0*sf.m*sf.m*sf.mu*sf.mu+14.0*pow(sf.mu,4.0)) 
	 << endl;
    
    t.test_rel(T/sf.n*(sf.dsdT-sf.dndT*sf.dndT/sf.dndmu),
	       o2scl_const::pi*o2scl_const::pi*T/sf.mu,1.0e-2,"sh1");
    t.test_rel(T/sf.n*(sf.dsdT-sf.dndT*sf.dndT/sf.dndmu),
	       sf.en/sf.n,1.0e-3,"sh2");
    
    // Compare with direct differentiation
    snf.calc_density(sf,T);
    double en1=sf.en;
    double h=1.0e-4;
    snf.calc_density(sf,T+h);
    double en2=sf.en;
    if (true || T>0.1/hc_mev_fm) {
      cout << "\t" << (en2-en1)/h*T/sf.n << endl;
      t.test_rel(T/sf.n*(sf.dsdT-sf.dndT*sf.dndT/sf.dndmu),
		 (en2-en1)/h*T/sf.n,1.0e-2,"sh3");
    }
  }
  cout << endl;

  fermion_deriv sfx(1.0,2.0);
  sfx.inc_rest_mass=false;

  cout << "----------------------------------------------------" << endl;
  cout << "Function deriv_calibrate() method=direct." << endl;
  cout << "----------------------------------------------------" << endl;
  cout << endl;
  
  snf.method=fermion_deriv_rel::direct;
  double val2=snf.deriv_calibrate(sfx,1,"../../data/o2scl/fermion_cal2.o2");
  cout << "Deriv_Calibrate: " << val2 << endl;
  t.test_rel(val2,0.0,8.0e-6,"deriv_calibrate direct");

  cout << "----------------------------------------------------" << endl;
  cout << "Function deriv_calibrate() method=by_parts." << endl;
  cout << "----------------------------------------------------" << endl;
  cout << endl;

  snf.method=fermion_deriv_rel::by_parts;
  double val3=snf.deriv_calibrate(sfx,1,"../../data/o2scl/fermion_cal2.o2");
  cout << "Deriv_Calibrate: " << val3 << endl;
  t.test_rel(val3,0.0,1.0e-6,"deriv_calibrate by parts");
  cout << endl;

  cout << "----------------------------------------------------" << endl;
  cout << "Function deriv_calibrate() method=automatic." << endl;
  cout << "----------------------------------------------------" << endl;
  cout << endl;

  snf.method=fermion_deriv_rel::automatic;
  double val4=snf.deriv_calibrate(sfx,1,"../../data/o2scl/fermion_cal2.o2");
  cout << "Deriv_Calibrate: " << val4 << endl;
  t.test_rel(val4,0.0,1.0e-6,"deriv_calibrate auto");
  cout << endl;

  cout << "----------------------------------------------------" << endl;
  cout << "Testing with inc_rest_mass=true" << endl;
  cout << "----------------------------------------------------" << endl;
  cout << endl;

  sf.m=5.0;
  sf2.m=5.0;
  sf.inc_rest_mass=false;
  sf2.inc_rest_mass=true;
  sf.mu=1.2;
  sf2.mu=1.2+sf.m;

  snf.method=fermion_deriv_rel::direct;
    
  cout << "degenerate direct" << endl;

  snf.calc_mu(sf,0.01);
  cout << sf.mu << " " << sf.m << " " << sf.ms << endl;
  cout << sf.n << " " << sf.ed+sf.n*sf.m << " " 
       << sf.pr << " " << sf.en << endl;
  cout << sf.dndT << " " << sf.dndmu << " " << sf.dsdT << endl;
    
  snf.calc_mu(sf2,0.01);
  cout << sf2.mu << " " << sf2.m << " " << sf2.ms << endl;
  cout << sf2.n << " " << sf2.ed << " " << sf2.pr << " " << sf2.en << endl;
  cout << sf2.dndT << " " << sf2.dndmu << " " << sf2.dsdT << endl;

  t.test_rel(sf.n,sf2.n,1.0e-8,"n direct deg");
  t.test_rel(sf.ed+sf.n*sf.m,sf2.ed,1.0e-8,"ed direct deg");
  t.test_rel(sf.en,sf2.en,1.0e-8,"en direct deg");
  t.test_rel(sf.pr,sf2.pr,1.0e-8,"pr direct deg");
  t.test_rel(sf.dndT,sf2.dndT,1.0e-8,"dndT direct deg");
  t.test_rel(sf.dndmu,sf2.dndmu,1.0e-8,"dndmu direct deg");
  t.test_rel(sf.dsdT,sf2.dsdT,1.0e-8,"dsdT direct deg");

  snf.calc_density(sf,0.01);
  snf.calc_density(sf2,0.01);
  t.test_rel(sf.mu+sf.m,sf2.mu,1.0e-8,"calc_density direct deg");

  cout << endl;
  cout << "degenerate by parts" << endl;
    
  snf.method=fermion_deriv_rel::by_parts;
    
  snf.calc_mu(sf,0.01);
  cout << sf.mu << " " << sf.m << " " << sf.ms << endl;
  cout << sf.n << " " << sf.ed+sf.n*sf.m << " " 
       << sf.pr << " " << sf.en << endl;
  cout << sf.dndT << " " << sf.dndmu << " " << sf.dsdT << endl;
    
  snf.calc_mu(sf2,0.01);
  cout << sf2.mu << " " << sf2.m << " " << sf2.ms << endl;
  cout << sf2.n << " " << sf2.ed << " " << sf2.pr << " " << sf2.en << endl;
  cout << sf2.dndT << " " << sf2.dndmu << " " << sf2.dsdT << endl;

  t.test_rel(sf.n,sf2.n,1.0e-8,"n by_parts deg");
  t.test_rel(sf.ed+sf.n*sf.m,sf2.ed,1.0e-8,"ed by_parts deg");
  t.test_rel(sf.en,sf2.en,1.0e-8,"en by_parts deg");
  t.test_rel(sf.pr,sf2.pr,1.0e-8,"pr by_parts deg");
  t.test_rel(sf.dndT,sf2.dndT,1.0e-8,"dndT by_parts deg");
  t.test_rel(sf.dndmu,sf2.dndmu,1.0e-8,"dndmu by_parts deg");
  t.test_rel(sf.dsdT,sf2.dsdT,1.0e-8,"dsdT by_parts deg");

  snf.calc_density(sf,0.01);
  snf.calc_density(sf2,0.01);
  t.test_rel(sf.mu+sf.m,sf2.mu,1.0e-8,"calc_density by_parts deg");

  cout << endl;
  cout << "non-degenerate direct" << endl;

  snf.method=fermion_deriv_rel::direct;
    
  snf.calc_mu(sf,1.0);
  cout << sf.mu << " " << sf.m << " " << sf.ms << endl;
  cout << sf.n << " " << sf.ed+sf.n*sf.m << " " 
       << sf.pr << " " << sf.en << endl;
  cout << sf.dndT << " " << sf.dndmu << " " << sf.dsdT << endl;
    
  snf.calc_mu(sf2,1.0);
  cout << sf2.mu << " " << sf2.m << " " << sf2.ms << endl;
  cout << sf2.n << " " << sf2.ed << " " << sf2.pr << " " << sf2.en << endl;
  cout << sf2.dndT << " " << sf2.dndmu << " " << sf2.dsdT << endl;

  t.test_rel(sf.n,sf2.n,1.0e-8,"n direct ndeg");
  t.test_rel(sf.ed+sf.n*sf.m,sf2.ed,1.0e-8,"ed direct ndeg");
  t.test_rel(sf.en,sf2.en,1.0e-8,"en direct ndeg");
  t.test_rel(sf.pr,sf2.pr,1.0e-8,"pr direct ndeg");
  t.test_rel(sf.dndT,sf2.dndT,1.0e-8,"dndT direct ndeg");
  t.test_rel(sf.dndmu,sf2.dndmu,1.0e-8,"dndmu direct ndeg");
  t.test_rel(sf.dsdT,sf2.dsdT,1.0e-8,"dsdT direct ndeg");
  
  snf.calc_density(sf,1.0);
  snf.calc_density(sf2,1.0);
  t.test_rel(sf.mu+sf.m,sf2.mu,1.0e-8,"calc_density direct ndeg");

  cout << endl;
  cout << "non-degenerate by parts" << endl;

  snf.method=fermion_deriv_rel::by_parts;
    
  snf.calc_mu(sf,1.0);
  cout << sf.mu << " " << sf.m << " " << sf.ms << endl;
  cout << sf.n << " " << sf.ed+sf.n*sf.m << " " 
       << sf.pr << " " << sf.en << endl;
  cout << sf.dndT << " " << sf.dndmu << " " << sf.dsdT << endl;
    
  snf.calc_mu(sf2,1.0);
  cout << sf2.mu << " " << sf2.m << " " << sf2.ms << endl;
  cout << sf2.n << " " << sf2.ed << " " << sf2.pr << " " << sf2.en << endl;
  cout << sf2.dndT << " " << sf2.dndmu << " " << sf2.dsdT << endl;

  t.test_rel(sf.n,sf2.n,1.0e-8,"n by_parts ndeg");
  t.test_rel(sf.ed+sf.n*sf.m,sf2.ed,1.0e-8,"ed by_parts ndeg");
  t.test_rel(sf.en,sf2.en,1.0e-8,"en by_parts ndeg");
  t.test_rel(sf.pr,sf2.pr,1.0e-8,"pr by_parts ndeg");
  t.test_rel(sf.dndT,sf2.dndT,1.0e-8,"dndT by_parts ndeg");
  t.test_rel(sf.dndmu,sf2.dndmu,1.0e-8,"dndmu by_parts ndeg");
  t.test_rel(sf.dsdT,sf2.dsdT,1.0e-8,"dsdT by_parts ndeg");
    
  snf.calc_density(sf,1.0);
  snf.calc_density(sf2,1.0);
  t.test_rel(sf.mu+sf.m,sf2.mu,1.0e-8,"calc_density by_parts ndeg");
  cout << endl;

  cout << "pair direct" << endl;

  snf.method=fermion_deriv_rel::direct;
    
  snf.pair_mu(sf,1.0);
  cout << sf.mu << " " << sf.m << " " << sf.ms << endl;
  cout << sf.n << " " << sf.ed+sf.n*sf.m << " " 
       << sf.pr << " " << sf.en << endl;
  cout << sf.dndT << " " << sf.dndmu << " " << sf.dsdT << endl;
    
  snf.pair_mu(sf2,1.0);
  cout << sf2.mu << " " << sf2.m << " " << sf2.ms << endl;
  cout << sf2.n << " " << sf2.ed << " " << sf2.pr << " " << sf2.en << endl;
  cout << sf2.dndT << " " << sf2.dndmu << " " << sf2.dsdT << endl;

  t.test_rel(sf.n,sf2.n,1.0e-8,"n direct pair");
  t.test_rel(sf.ed+sf.n*sf.m,sf2.ed,1.0e-5,"ed direct pair");
  t.test_rel(sf.en,sf2.en,1.0e-8,"en direct pair");
  t.test_rel(sf.pr,sf2.pr,1.0e-8,"pr direct pair");
  t.test_rel(sf.dndT,sf2.dndT,1.0e-8,"dndT direct pair");
  t.test_rel(sf.dndmu,sf2.dndmu,1.0e-8,"dndmu direct pair");
  t.test_rel(sf.dsdT,sf2.dsdT,1.0e-8,"dsdT direct pair");
  
  snf.pair_density(sf,1.0);
  snf.pair_density(sf2,1.0);
  t.test_rel(sf.mu+sf.m,sf2.mu,1.0e-8,"pair_density direct pair");
  cout << endl;

  cout << "pair by parts" << endl;
  snf.method=fermion_deriv_rel::by_parts;
    
  snf.pair_mu(sf,1.0);
  cout << sf.mu << " " << sf.m << " " << sf.ms << endl;
  cout << sf.n << " " << sf.ed+sf.n*sf.m << " " 
       << sf.pr << " " << sf.en << endl;
  cout << sf.dndT << " " << sf.dndmu << " " << sf.dsdT << endl;
    
  snf.pair_mu(sf2,1.0);
  cout << sf2.mu << " " << sf2.m << " " << sf2.ms << endl;
  cout << sf2.n << " " << sf2.ed << " " << sf2.pr << " " << sf2.en << endl;
  cout << sf2.dndT << " " << sf2.dndmu << " " << sf2.dsdT << endl;

  t.test_rel(sf.n,sf2.n,1.0e-7,"n by_parts pair");
  t.test_rel(sf.ed+sf.n*sf.m,sf2.ed,1.0e-5,"ed by_parts pair");
  t.test_rel(sf.en,sf2.en,1.0e-7,"en by_parts pair");
  t.test_rel(sf.pr,sf2.pr,1.0e-7,"pr by_parts pair");
  t.test_rel(sf.dndT,sf2.dndT,1.0e-7,"dndT by_parts pair");
  t.test_rel(sf.dndmu,sf2.dndmu,1.0e-7,"dndmu by_parts pair");
  t.test_rel(sf.dsdT,sf2.dsdT,1.0e-7,"dsdT by_parts pair");
    
  snf.pair_density(sf,1.0);
  snf.pair_density(sf2,1.0);
  t.test_rel(sf.mu+sf.m,sf2.mu,1.0e-7,"calc_density by_parts pair");
  cout << endl;

  cout << "------------------------------------------------------" << endl;
  {

    cout << "dndm test" << endl;
    snf.method=fermion_deriv_rel::direct;
    double d1, d2, eps=1.0e-4;
    double dndmu, dndT, dsdT, dndm;

    sf.inc_rest_mass=false;
    sf.non_interacting=true;

    T=1.0;
    sf.m=5.0;
    sf.ms=5.0;
    sf.mu=1.0*T-sf.m+sf.ms;

    sf.mu=1.0*T-sf.m+sf.ms+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.mu=1.0*T-sf.m+sf.ms;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.n;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.en;
    snf.calc_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;

    sf.non_interacting=false;
    sf.nu=sf.mu;
    sf.ms=5.0+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;

    sf.ms=5.0;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;
    sf.non_interacting=true;

    snf.calc_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << endl;
    double dndm2=3.0*sf.n/sf.m-(sf.dndT+sf.mu/T*sf.dndmu)*T/sf.m-sf.dndmu;
    cout << dndm2 << endl;
    cout << endl;

  }

  {

    cout << "dndm test" << endl;
    snf.method=fermion_deriv_rel::direct;
    double d1, d2, eps=1.0e-4;
    double dndmu, dndT, dsdT, dndm;

    sf.inc_rest_mass=true;
    sf.non_interacting=true;

    T=1.0;
    sf.m=5.0;
    sf.ms=5.0;
    sf.mu=1.0*T;

    sf.mu=1.0*T+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    sf.mu=1.0*T;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndmu=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.n;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndT=(d1-d2)/eps;

    snf.calc_mu(sf,T+eps);
    d1=sf.en;
    snf.calc_mu(sf,T);
    d2=sf.en;
    dsdT=(d1-d2)/eps;

    sf.non_interacting=false;
    sf.nu=sf.mu;
    sf.ms=5.0+eps;
    snf.calc_mu(sf,T);
    d1=sf.n;
    
    sf.ms=5.0;
    snf.calc_mu(sf,T);
    d2=sf.n;
    dndm=(d1-d2)/eps;
    sf.non_interacting=true;

    snf.calc_mu(sf,T);
    cout << "sf: " << sf.dndmu << " " << sf.dndT << " " << sf.dsdT << endl;
    cout << "un: " << snf.unc.dndmu << " " << snf.unc.dndT << " " 
	 << snf.unc.dsdT << endl;
    cout << "nu: " << dndmu << " " << dndT << " " << dsdT << " "
	 << dndm << endl;
    double dndm2=3.0*sf.n/sf.ms-(sf.dndT*T/sf.ms+sf.nu/sf.ms*sf.dndmu);
    cout << dndm2 << endl;
    cout << endl;

  }

  t.report();

  return 0;
}

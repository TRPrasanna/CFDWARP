/*
SPDX-License-Identifier: BSD-2-Clause

Copyright 2020 Bernard Parent

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
   conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
   of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <model/chem/_chem.h>
#include <model/_model.h>
#include <model/thermo/_thermo.h>
#include <model/metrics/_metrics.h>
#include <model/share/chem_share.h>


#define TOWNSEND TRUE
#define TOWNSEND_IMPLICIT FALSE
#define Estarmin 1e-40


/* set all reactions to true except for testing purposes */
const static bool REACTION[28]=
  {
   TRUE, /* reaction 0 */
   TRUE, /* reaction 1 */
   TRUE, /* reaction 2 */
   TRUE, /* reaction 3 */
   TRUE, /* reaction 4 */
   TRUE, /* reaction 5 */
   TRUE, /* reaction 6 */
   TRUE, /* reaction 7 */
   TRUE, /* reaction 8 */
   TRUE, /* reaction 9 */
   TRUE, /* reaction 10 */
   TRUE, /* reaction 11 */
   TRUE, /* reaction 12 */
   TRUE, /* reaction 13 */
   TRUE, /* reaction 14 */
   TRUE, /* reaction 15 */
   TRUE, /* reaction 16 */
   TRUE, /* reaction 17 */
   TRUE, /* reaction 18 */
   TRUE, /* reaction 19 */ 
   TRUE, /* reaction 20 */
   TRUE, /* reaction 21 */
   TRUE, /* reaction 22 */
   TRUE, /* reaction 23 */
   TRUE, /* reaction 24 */
   TRUE, /* reaction 25 */
   TRUE, /* reaction 26 */
   TRUE, /* reaction 27 */
  };

#define specEND -1

const static long specM1[]=
  {
   specN, specO, specNplus, specOplus, specEND
  };

const static long specM2[]=
  {
   specN2, specO2, specNO, specN2plus, specO2plus, specNOplus, specEND
  };



void write_model_chem_template(FILE **controlfile){
}


void read_model_chem_actions(char *actionname, char **argum, SOAP_codex_t *codex){
}


void find_W ( gl_t *gl, spec_t rhok, double T, double Te, double Tv, double Estar, double Qbeam, spec_t W ) {
  double N[ns];
  double R,kf,Ta;
  long k;
  spec_t X;

  /* find properties needed by add_to_W* functions */
  R=1.9872;
  Ta=sqrt(Tv*T);
  for ( k = 0; k < ns; k++ ) {
    X[k] = rhok[k] / _calM ( k ) * 1.0e-06;     /* mole/cm3 */
    W[k] = 0.0;
    N[k] = rhok[k] / _calM (k ) * 1e-6 * calA;  /* particules/cm^3 */
  }


  if (REACTION[1]) {
    for (k=0; specM1[k]!=specEND; k++)
      add_to_W_fwbw_2r3p ( specN2, specM1[k],   specN, specN, specM1[k], 3.0e22, -1.6, 113200.0*R, Ta, X, W );
  }
      

  if (REACTION[2]){
    for (k=0; specM2[k]!=specEND; k++)
      add_to_W_fwbw_2r3p ( specN2, specM2[k],   specN, specN, specM2[k], 7.0e21, -1.6, 113200.0*R, Ta, X, W );
  }

  if (REACTION[3]){
    add_to_W_fwbw_2r3p ( specN2, speceminus,   specN, specN, speceminus, 1.2e25, -1.6, 113200.0*R, Ta, X, W );
  }

  if (REACTION[4]){
    for (k=0; specM1[k]!=specEND; k++)
      add_to_W_fwbw_2r3p ( specO2, specM1[k],   specO, specO, specM1[k], 1.0e22, -1.5, 59500.0*R, Ta, X, W );
  }

  if (REACTION[5]){
    for (k=0; specM2[k]!=specEND; k++)
      add_to_W_fwbw_2r3p ( specO2, specM2[k],   specO, specO, specM2[k], 2.0e21, -1.5, 59500.0*R, Ta, X, W );
  }

  if (REACTION[6]){
    for (k=0; specM1[k]!=specEND; k++)
      add_to_W_fwbw_2r3p ( specNO, specM1[k],   specN, specO, specM1[k], 1.1e17, 0.0, 75500.0*R, Ta, X, W );
  }

  if (REACTION[7]){
    for (k=0; specM2[k]!=specEND; k++)
      add_to_W_fwbw_2r3p ( specNO, specM2[k],   specN, specO, specM2[k], 5.0e15, 0.0, 75500.0*R, Ta, X, W );
  }

  if (REACTION[8]){
    add_to_W_fwbw_2r2p ( specNO, specO,   specN, specO2, 8.4e12, 0.0, 19450.0*R, T, X, W );
  }

  if (REACTION[9]){
    add_to_W_fwbw_2r2p ( specN2, specO,   specNO, specN, 6.4e17, -1.0, 38400.0*R, T, X, W );
  }

  if (REACTION[10]){
    add_to_W_fwbw_2r2p ( specN, specO,   specNOplus, speceminus, 8.8e8, 1.0, 31900.0*R, T, X, W );
  }

  if (REACTION[11]){
    add_to_W_fwbw_2r2p ( specO, specO,   specO2plus, speceminus, 7.1e2, 2.7, 80600.0*R, T, X, W );
  }

  if (REACTION[12]){
    add_to_W_fwbw_2r2p ( specN, specN,   specN2plus, speceminus, 4.4e7, 1.5, 67500.0*R, T, X, W );
  }

  if (REACTION[13]){
    add_to_W_fwbw_2r2p ( specNOplus, specO,   specNplus, specO2, 1.0e12, 0.5, 77200.0*R, T, X, W );
  }

  if (REACTION[14]){
    add_to_W_fwbw_2r2p ( specNplus, specN2,   specN2plus, specN, 1.0e12, 0.5, 12200.0*R, T, X, W );
  }

  if (REACTION[15]){
    add_to_W_fwbw_2r2p ( specO2plus, specN,   specNplus, specO2, 8.7e13, 0.14, 28600.0*R, T, X, W );
  }

  if (REACTION[16]){
    add_to_W_fwbw_2r2p ( specOplus, specNO,   specNplus, specO2, 1.4e5, 1.90, 26600.0*R, T, X, W );
  }

  if (REACTION[17]){
    add_to_W_fwbw_2r2p ( specO2plus, specN2,   specN2plus, specO2, 9.9e12, 0.00, 40700.0*R, T, X, W );
  }

  if (REACTION[18]){
    add_to_W_fwbw_2r2p ( specO2plus, specO,   specOplus, specO2, 4.0e12, -0.09, 18000.0*R, T, X, W );
  }

  if (REACTION[19]){
    add_to_W_fwbw_2r2p ( specNOplus, specN,   specOplus, specN2, 3.4e13, -1.08, 12800.0*R, T, X, W );
  }

  if (REACTION[20]){
    add_to_W_fwbw_2r2p ( specNOplus, specO2,   specO2plus, specNO, 2.4e13, 0.41, 32600.0*R, T, X, W );
  }

  if (REACTION[21]){
    add_to_W_fwbw_2r2p ( specNOplus, specO,   specO2plus, specN, 7.2e12, 0.29, 48600.0*R, T, X, W );
  }

  if (REACTION[22]){
    add_to_W_fwbw_2r2p ( specOplus, specN2,   specN2plus, specO, 9.1e11, 0.36, 22800.0*R, T, X, W );
  }

  if (REACTION[23]){
    add_to_W_fwbw_2r2p ( specNOplus, specN,   specN2plus, specO, 7.2e13, 0.00, 35500.0*R, T, X, W );
  }

  if (REACTION[24]){
    add_to_W_fwbw_2r3p ( specO, speceminus,   specOplus, speceminus, speceminus, 3.9e33, -3.78, 158500.0*R, Te, X, W );
  }

  if (REACTION[25]){
    add_to_W_fwbw_2r3p ( specN, speceminus,   specNplus, speceminus, speceminus, 2.5e34, -3.82, 168600.0*R, Te, X, W );
  }

  if (REACTION[26]){
    kf=_kf_Arrhenius(2, 1.07e11, -0.52, 0.0, Te);
    add_to_W_2r1p ( specOplus, speceminus,   specO,   kf , N, W);
  }

  if (REACTION[27]){
    kf=_kf_Arrhenius(2, 1.52e11, -0.48, 0.0, Te);
    add_to_W_2r1p ( specNplus, speceminus,   specN,   kf , N, W);
  }

}






/* Verify the validity of the dW terms at node i=10,j=10 using the command ./test -r control.wrp -node 10 10 dSchemdU 
 * Make sure to verify the dW terms over a wide range of temperatures and mass fractions 
 * Note that the verification using ./test is done by comparing the analytical expressions to numerical derivatives
 * The numerical derivatives depend strongly on the values given to Uref[] within Cycle()
 */ 

void find_dW_dx ( gl_t *gl, spec_t rhok, spec_t mu, double T, double Te, double Tv, double Estar, double Qbeam,
                  spec2_t dWdrhok, spec_t dWdT, spec_t dWdTe, spec_t dWdTv, spec_t dWdQbeam ) {
  long k, s, spec;                    
  spec_t N;
  double Ta,R,kf,dkfdTe,dkfdT,dkfdTv,dTadTv,dTadT;
  spec_t dWdTa;
  spec_t X;

  for ( k = 0; k < ns; k++ ) {
    X[k] = rhok[k] / _calM ( k ) * 1.0e-06;     /* mole/cm3 */
  }
  
  
  R=1.9872;
  Ta=sqrt(T*Tv);
  /* initialize all derivatives to zero */
  for ( s = 0; s < ns; s++ ) {
    dWdTa[s] = 0.0;
    dWdT[s] = 0.0;
    dWdTe[s] = 0.0;
    dWdTv[s] = 0.0;
    dWdQbeam[s] = 0.0;
    for ( k = 0; k < ns; k++ ) {
      dWdrhok[s][k] = 0.0;
    }
  }

  /* find properties needed by add_to_dW* functions in proper units */
  for ( k = 0; k < ns; k++ ) {
    N[k] = rhok[k] / _calM ( k ) * 1e-6 * calA;
  }


  if (REACTION[1]) {
    for (k=0; specM1[k]!=specEND; k++)
      add_to_dW_fwbw_2r3p ( specN2, specM1[k],   specN, specN, specM1[k], 3.0e22, -1.6, 113200.0*R, Ta, X, dWdTa, dWdrhok );
  }
      

  if (REACTION[2]){
    for (k=0; specM2[k]!=specEND; k++)
      add_to_dW_fwbw_2r3p ( specN2, specM2[k],   specN, specN, specM2[k], 7.0e21, -1.6, 113200.0*R, Ta, X, dWdTa, dWdrhok);
  }

  if (REACTION[3]){
    add_to_dW_fwbw_2r3p ( specN2, speceminus,   specN, specN, speceminus, 1.2e25, -1.6, 113200.0*R, Ta, X, dWdTa, dWdrhok);
  }

  if (REACTION[4]){
    for (k=0; specM1[k]!=specEND; k++)
      add_to_dW_fwbw_2r3p ( specO2, specM1[k],   specO, specO, specM1[k], 1.0e22, -1.5, 59500.0*R, Ta, X, dWdTa, dWdrhok);
  }

  if (REACTION[5]){
    for (k=0; specM2[k]!=specEND; k++)
      add_to_dW_fwbw_2r3p ( specO2, specM2[k],   specO, specO, specM2[k], 2.0e21, -1.5, 59500.0*R, Ta, X, dWdTa, dWdrhok);
  }

  if (REACTION[6]){
    for (k=0; specM1[k]!=specEND; k++)
      add_to_dW_fwbw_2r3p ( specNO, specM1[k],   specN, specO, specM1[k], 1.1e17, 0.0, 75500.0*R, Ta, X, dWdTa, dWdrhok);
  }

  if (REACTION[7]){
    for (k=0; specM2[k]!=specEND; k++)
      add_to_dW_fwbw_2r3p ( specNO, specM2[k],   specN, specO, specM2[k], 5.0e15, 0.0, 75500.0*R, Ta, X, dWdTa, dWdrhok);
  }

  if (REACTION[8]){
    add_to_dW_fwbw_2r2p ( specNO, specO,   specN, specO2, 8.4e12, 0.0, 19450.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[9]){
    add_to_dW_fwbw_2r2p ( specN2, specO,   specNO, specN, 6.4e17, -1.0, 38400.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[10]){
    add_to_dW_fwbw_2r2p ( specN, specO,   specNOplus, speceminus, 8.8e8, 1.0, 31900.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[11]){
    add_to_dW_fwbw_2r2p ( specO, specO,   specO2plus, speceminus, 7.1e2, 2.7, 80600.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[12]){
    add_to_dW_fwbw_2r2p ( specN, specN,   specN2plus, speceminus, 4.4e7, 1.5, 67500.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[13]){
    add_to_dW_fwbw_2r2p ( specNOplus, specO,   specNplus, specO2, 1.0e12, 0.5, 77200.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[14]){
    add_to_dW_fwbw_2r2p ( specNplus, specN2,   specN2plus, specN, 1.0e12, 0.5, 12200.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[15]){
    add_to_dW_fwbw_2r2p ( specO2plus, specN,   specNplus, specO2, 8.7e13, 0.14, 28600.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[16]){
    add_to_dW_fwbw_2r2p ( specOplus, specNO,   specNplus, specO2, 1.4e5, 1.90, 26600.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[17]){
    add_to_dW_fwbw_2r2p ( specO2plus, specN2,   specN2plus, specO2, 9.9e12, 0.00, 40700.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[18]){
    add_to_dW_fwbw_2r2p ( specO2plus, specO,   specOplus, specO2, 4.0e12, -0.09, 18000.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[19]){
    add_to_dW_fwbw_2r2p ( specNOplus, specN,   specOplus, specN2, 3.4e13, -1.08, 12800.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[20]){
    add_to_dW_fwbw_2r2p ( specNOplus, specO2,   specO2plus, specNO, 2.4e13, 0.41, 32600.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[21]){
    add_to_dW_fwbw_2r2p ( specNOplus, specO,   specO2plus, specN, 7.2e12, 0.29, 48600.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[22]){
    add_to_dW_fwbw_2r2p ( specOplus, specN2,   specN2plus, specO, 9.1e11, 0.36, 22800.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[23]){
    add_to_dW_fwbw_2r2p ( specNOplus, specN,   specN2plus, specO, 7.2e13, 0.00, 35500.0*R, T, X, dWdT, dWdrhok);
  }

  if (REACTION[24]){
    add_to_dW_fwbw_2r3p ( specO, speceminus,   specOplus, speceminus, speceminus, 3.9e33, -3.78, 158500.0*R, Te, X, dWdTe, dWdrhok);
  }

  if (REACTION[25]){
    add_to_dW_fwbw_2r3p ( specN, speceminus,   specNplus, speceminus, speceminus, 2.5e34, -3.82, 168600.0*R, Te, X, dWdTe, dWdrhok);
  }


  if (REACTION[26]){
    kf=_kf_Arrhenius(2, 1.07e11, -0.52, 0.0, Te);
    dkfdT=0.0;
    dkfdTv=0.0;
    dkfdTe=_dkfdT_Arrhenius(2, 1.07e11, -0.52, 0.0, Te);
    add_to_dW_2r1p ( specOplus, speceminus,   specO,   kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }

  if (REACTION[27]){
    kf=_kf_Arrhenius(2, 1.52e11, -0.48, 0.0, Te);
    dkfdT=0.0;
    dkfdTv=0.0;
    dkfdTe=_dkfdT_Arrhenius(2, 1.52e11, -0.48, 0.0, Te);
    add_to_dW_2r1p ( specNplus, speceminus,   specN,   kf , N, dkfdT, dkfdTv, dkfdTe, dWdrhok, dWdT, dWdTv, dWdTe);
  }


  dTadT=0.5/Ta*Tv;
  dTadTv=0.5/Ta*T;
  for (spec=0; spec<ns; spec++){
    dWdT[spec]+=dWdTa[spec]*dTadT;
    dWdTv[spec]+=dWdTa[spec]*dTadTv;
  }
  
}


void find_Qei(spec_t rhok, double Estar, double Te, double *Qei){
  *Qei=0.0;
}



void find_dQei_dx(spec_t rhok, double Estar, double Te, spec_t dQeidrhok, double *dQeidTe){
  long spec;
  
  for (spec=0; spec<ns; spec++) dQeidrhok[spec]=0.0;
  *dQeidTe=0.0;  

}


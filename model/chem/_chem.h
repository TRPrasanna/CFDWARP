#ifndef _CHEM_H
#define _CHEM_H

#include <src/common.h>

void write_model_chem_template(FILE **controlfile);

void read_model_chem_actions(char *actionname, char **argum, SOAP_codex_t *codex);

void find_W(gl_t *gl, spec_t rhok, double T, double Te, double Tv, double Estar, double Qbeam, spec_t W);

void find_dW_dx(gl_t *gl, spec_t rhok, spec_t mu, double T, double Te, double Tv, double Estar, double Qbeam,
              spec2_t dWdrhok, spec_t dWdT, spec_t dWdTe, spec_t dWdTv, spec_t dWdQbeam);

void find_Qei(spec_t rhok, double Estar, double Te, double *Qei);

void find_dQei_dx(spec_t rhok, double Estar, double Te, spec_t dQeidrhok, double *dQeidTe);


#endif /* _CHEM_H */ 

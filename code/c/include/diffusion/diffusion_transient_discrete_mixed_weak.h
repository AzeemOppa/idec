#ifndef _diffusion_transient_discrete_mixed_weak_h
#define _diffusion_transient_discrete_mixed_weak_h

#include "diffusion_transient_continuous.h"

typedef struct diffusion_transient_discrete_mixed_weak
{
  double  * pi_0;
  double  * pi_dm1;
  double  * initial;
  double  * source;
  jagged1 * boundary_dirichlet;
  double  * g_dirichlet;
  jagged1 * boundary_neumann;
  double  * g_neumann;
} diffusion_transient_discrete_mixed_weak;

void diffusion_transient_discrete_mixed_weak_free(
  diffusion_transient_discrete_mixed_weak * data);

diffusion_transient_discrete_mixed_weak *
diffusion_transient_discrete_mixed_weak_from_continuous(
  const mesh * m,
  const double * m_vol_dm1,
  const double * m_vol_d,
  const diffusion_transient_continuous * data_continuous);

#endif /* _diffusion_transient_discrete_mixed_weak_h */

#ifndef _diffusion_transient_discrete_primal_weak_solve_trapezoidal_next_h
#define _diffusion_transient_discrete_primal_weak_solve_trapezoidal_next_h

#include "diffusion_transient_discrete_primal_weak.h"

void diffusion_transient_discrete_primal_weak_solve_trapezoidal_next(
  double * next,
  double * rhs_final,
  const double * current,
  const matrix_sparse * lhs,
  const matrix_sparse * rhs,
  const double * free_part,
  const diffusion_transient_discrete_primal_weak * data);

#endif /* _diffusion_transient_discrete_primal_weak_solve_trapezoidal_next_h */

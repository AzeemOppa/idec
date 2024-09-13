#include <stdio.h>
#include <stdlib.h>

#include "mesh_qc.h"

static void 
mesh_qc_matrix_sparse_from_inner_of_delta_basis_0_cup_pi_1_delta_basis_0_cols_total(
  int * a_cols_total,
  const mesh_qc * m)
{
  int j, m_cn_0;
  jagged2 m_fc_0_1;

  m_cn_0 = m->cn[0];
  mesh_fc_part2(&m_fc_0_1, m, 0, 1);
  a_cols_total[0] = 0;
  for (j = 1; j <= m_cn_0; ++j)
    a_cols_total[j] = a_cols_total[j - 1] + m_fc_0_1.a1[j];
}

static void
mesh_qc_matrix_sparse_from_inner_of_delta_basis_0_cup_pi_1_delta_basis_0_row_indices(
  int * a_row_indices,
  const mesh_qc * m)
{
  int index, i, j, k, k_local, m_cn_0;
  jagged1 m_cf_1_0_k, m_fc_0_1_j;
  jagged2 m_cf_1_0, m_fc_0_1;

  m_cn_0 = m->cn[0];
  mesh_fc_part2(&m_fc_0_1, m, 0, 1);
  mesh_cf_part2(&m_cf_1_0, m, 1, 0);

  index = 0;
  for (j = 0; j < m_cn_0; ++j)
  {
    a_row_indices[index] = j;
    ++index;
    jagged2_part1(&m_fc_0_1_j, &m_fc_0_1, j);
    for (k_local = 0; k_local < m_fc_0_1_j.a0; ++k_local)
    {
      k = m_fc_0_1_j.a1[k_local];
      jagged2_part1(&m_cf_1_0_k, &m_cf_1_0, k);
      i = jagged1_couple_other_object(&m_cf_1_0_k, j);
      a_row_indices[index] = i;
      ++index;
    }
  }
}

static void
mesh_qc_matrix_sparse_from_inner_of_delta_basis_0_cup_pi_1_delta_basis_0_values(
  double * a_values,
  const mesh_qc * m,
  const double * m_inner_1,
  const double * pi_1)
{
  int diagonal_index, index, j, k, k_local, m_cn_0;
  double c_k;
  jagged1 m_fc_0_1_j;
  jagged2 m_fc_0_1;

  m_cn_0 = m->cn[0];
  mesh_fc_part2(&m_fc_0_1, m, 0, 1);

  index = 0;
  for (j = 0; j < m_cn_0; ++j)
  {
    diagonal_index = index;
    a_values[diagonal_index] = 0;
    ++index;
    jagged2_part1(&m_fc_0_1_j, &m_fc_0_1, j);
    for (k_local = 0; k_local < m_fc_0_1_j.a0; ++k_local)
    {
      k = m_fc_0_1_j.a1[k_local];
      c_k = pi_1[k] * m_inner_1[k];
      a_values[index] = -c_k;
      a_values[diagonal_index] += c_k;
      ++index;
    }
  }
}

/* calculate sparse matrix a, a_{i, j} := <delta N^j, pi_1 (delta N^i)> */
matrix_sparse *
mesh_qc_matrix_sparse_from_inner_of_delta_basis_0_cup_pi_1_delta_basis_0(
  const mesh_qc * m,
  const double * m_inner_1,
  const double * pi_1)
{
  int nonzero_max;
  matrix_sparse * a;

  a->rows = m->cn[0];
  a->cols = a->rows;

  a->cols_total = (int *) malloc(sizeof(int) * (a->cols + 1));
  if (a->cols_total == NULL)
  {
    fprintf(stderr,
      "%s:%d: cannot allocate memory for cols_total\n", __FILE__, __LINE__);
    goto end;
  }
  mesh_qc_matrix_sparse_from_inner_of_delta_basis_0_cup_pi_1_delta_basis_0_cols_total(
    a->cols_total, m);

  nonzero_max = a->cols_total[a->cols];

  a->row_indices = (int *) malloc(sizeof(int) * nonzero_max);
  if (a->row_indices == NULL)
  {
    fprintf(stderr,
      "%s:%d: cannot allocate memory for a->row_indices\n", __FILE__, __LINE__);
    goto a_cols_total_free;
  }
  mesh_qc_matrix_sparse_from_inner_of_delta_basis_0_cup_pi_1_delta_basis_0_row_indices(
    a->row_indices, m);

  a->values = (double *) malloc(sizeof(double) * nonzero_max);
  if (a->values == NULL)
  {
    fprintf(stderr,
      "%s:%d: cannot allocate memory for a->values\n", __FILE__, __LINE__);
    goto a_row_indices_free;
  }
  mesh_qc_matrix_sparse_from_inner_of_delta_basis_0_cup_pi_1_delta_basis_0_values(
    a->values, m, m_inner_1, pi_1);

  return a;

  /* cleaning if an error occurs */
a_row_indices_free:
  free(a->row_indices);
a_cols_total_free:
  free(a->cols_total);
a_free:
  free(a);
end:
  return NULL;
}

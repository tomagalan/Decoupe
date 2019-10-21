/* algorithms.c */

#include <stdio.h>            /* C input/output                       */
#include <stdlib.h>           /* C standard library                   */
#include <glpk.h>             /* GNU GLPK linear/mixed integer solver */

double* cut(int varNb, int consNb, double* cons, double** fact){

	/* declare variables */
	glp_prob *lp;
	int ia[1+10000], ja[1+10000];
	double ar[1+10000];

	/* create problem */
  lp = glp_create_prob();
 	glp_set_prob_name(lp, "decoupe");
  glp_set_obj_dir(lp, GLP_MIN);

	/* fill problem */
	int i, j;
	int count = 1;
	glp_add_rows(lp, consNb);
	glp_add_cols(lp, varNb);

	/* rows */
	for(i = 1; i <= consNb; i++){
		glp_set_row_bnds(lp, i, GLP_FX, cons[i-1], cons[i-1]);
	}

	/* columns */
	for(j = 1; j <= varNb; j++) {
 		glp_set_col_bnds(lp, j, GLP_LO, 0.0, 0.0);
 		glp_set_obj_coef(lp, j, 1.0);
 	}

 	/* constraint matrix */
 	for(i = 1; i <= consNb; i++) {
 		for(j = 1; j <= varNb; j++) {
 			printf("%g ", fact[j-1][i-1]);
 			ia[count] = i, ja[count] = j, ar[count] = fact[j-1][i-1];
 			count ++;
 		}
 		printf("\n");
 	}
 	printf("\n");

 	glp_load_matrix(lp, consNb * varNb, ia, ja, ar);

 	/* solve problem */
	glp_simplex(lp, NULL);

	/* recover and display results */
  double* duals = (double*) malloc(consNb * sizeof(double));
	printf("z = %g; ", glp_get_obj_val(lp));
	printf("\n");
	for(i = 1; i <= consNb; i++) {
    duals[i-1] = glp_get_row_dual(lp,i);
    printf("dual %d = %g; ", i, duals[i-1]);
	}
	printf("\n");

	/* housekeeping */
	glp_delete_prob(lp);
	glp_free_env();

  return duals;
}

double* backPack(int varNb, int consNb, double* cons, double** fact, double* coefs) {

  /* declare variables */
  glp_prob *lp;
  int ia[1+1000], ja[1+1000];
  double ar[1+1000];

  /* create problem */
  lp = glp_create_prob();
  glp_set_prob_name(lp, "sac_a_dos");
  glp_set_obj_dir(lp, GLP_MAX);

  /* fill problem */
  int i, j;
  int count = 1;
  glp_add_rows(lp, consNb);
  glp_add_cols(lp, varNb);

  /* rows */
  for(i = 1; i <= consNb; i++) {
    glp_set_row_bnds(lp, i, GLP_UP, 0.0, cons[i-1]);
  }

  /* columns */
  for(j = 1; j <= varNb; j++) {
    glp_set_col_kind(lp, j, GLP_IV);
    glp_set_col_bnds(lp, j, GLP_LO, 0.0, 0.0);
    glp_set_obj_coef(lp, j, coefs[j-1]);
  }

  /* constraint matrix */
  for(i = 1; i <= consNb; i++) {
    for(j = 1; j <= varNb; j++) {
      printf("%g ", fact[j-1][i-1]);
      ia[count] = i, ja[count] = j, ar[count] = fact[j-1][i-1];
      count ++;
    }
    printf("\n");
  }
  printf("\n");

  glp_load_matrix(lp, consNb * varNb, ia, ja, ar);

  /* solve problem */
  glp_simplex(lp, NULL);
  glp_intopt(lp, NULL);

  /* recover and display results */
  double* primals = (double*) malloc(varNb * sizeof(double));
  double result = glp_mip_obj_val(lp);

  printf("z = %g; ", result);
  printf("\n");
  for(j = 1; j <= varNb; j++) {
    primals[j-1] = glp_mip_col_val(lp, j);
    printf("primal %d = %g; ", j, primals[j-1]);
  }
  printf("\n");

  /* housekeeping */
  glp_delete_prob(lp);
  glp_free_env();

  if(result <= 1.0005){
    free(primals);
    return NULL;
  }

  return primals;
}
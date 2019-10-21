/* optimalCut.c */

#include <stdio.h>            /* C input/output                       */
#include <stdlib.h>           /* C standard library                   */
#include "algorithms.h"

int main(void)
{
  /* Ouverture du fichier */
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen("instances/instance1", "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  double longueurIni;
  double* longueursFin = malloc(0);
  double* demandes = malloc(0);

  /* Récupération de la longueur ini */
  getline(&line, &len, fp);
  getline(&line, &len, fp);
  sscanf(line, "%lf", &longueurIni);
  printf("Longueur initiale: %lf\n", longueurIni);
  getline(&line, &len, fp);

  double longueurCourante, demandeCourante;
  int nbLongueurs = 0;

  /* Récupération des longueurs finales et des demandes */
  while ((read = getline(&line, &len, fp)) != -1) {
      sscanf(line, "%lf %lf", &longueurCourante, &demandeCourante);
      nbLongueurs ++;
      printf("Longueur finale : %lf, demande : %lf\n", longueurCourante, demandeCourante);
      longueursFin = realloc(longueursFin, nbLongueurs * sizeof(double));
      longueursFin[nbLongueurs-1] = longueurCourante;
      demandes[nbLongueurs-1] = demandeCourante; 
  }

  /* Création des motifs de départ */
  int i, j;
  double** decFact = (double**)calloc(nbLongueurs, sizeof(double*));
  for(i = 0; i < nbLongueurs; i++) {
      decFact[i] = (double*)calloc(nbLongueurs, sizeof(double));
  }
  
  for(i = 0; i < nbLongueurs; i++) {
      decFact[i][i] = (double) ((int)longueurIni / (int)longueursFin[i]);
  }

  /* Impression de la matrice de départ correspondante */
  for(i = 0; i < nbLongueurs; i++) {
      for(j=0; j < nbLongueurs; j++) {
          printf("%g ", decFact[i][j]);
      }
      printf("\n");
  }
  printf("\n");

  /* Définition des variables de départ pour la découpe et le sac à dos */
  int decVarNb = nbLongueurs;
  int decConsNb = nbLongueurs;
  double* decCons = demandes;

  int bagVarNb = nbLongueurs;
  int bagConsNb = 1;
  double bagCons[] = {longueurIni};

  double** bagFact = (double**)calloc(bagVarNb, sizeof(double*));
  for(i = 0; i < bagVarNb; i++) {
    bagFact[i] = (double*)calloc(bagConsNb, sizeof(double));
  }

  for(i = 0; i < bagVarNb; i++){
    bagFact[i][0] = longueursFin[i];
  }

  double* duals;
  double* primals;
  double** verif;

  while(1) {

    printf("\n----------------- Decoupe ---------------------\n\n");

    duals = cut(decVarNb, decConsNb, decCons, decFact);

    printf("\n----------------- Sac A Dos -------------------\n\n");

    primals = backPack(bagVarNb, bagConsNb, bagCons, bagFact, duals);

    if(primals == NULL)
      break;

    decVarNb ++;

    verif = realloc(decFact, decVarNb * sizeof(double*));
    verif[decVarNb-1] = (double*)calloc(decConsNb, sizeof(double));

    if(verif == NULL){
      printf("ARR KROSSE PROBLEME\n");
      return 1;
    }

    decFact = verif;

    for(j = 0; j < decConsNb; j++) {
      decFact[decVarNb-1][j] = primals[j];
    }
  }

  fclose(fp);
  /*if (line)
      free(line);
  exit(EXIT_SUCCESS);*/

  return 0;
}
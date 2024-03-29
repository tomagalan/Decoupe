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

  fp = fopen("instances/instance5", "r");
  if (fp == NULL){
    printf("Le fichier spécifié n'existe pas !\n");
    exit(EXIT_FAILURE);
  }

  double longueurIni;
  double* longueursFin = malloc(0);
  double* demandes = malloc(0);

  /* Récupération de la longueur ini */
  getline(&line, &len, fp);
  getline(&line, &len, fp);
  sscanf(line, "%lf", &longueurIni);
  printf("Longueur initiale: %g\n", longueurIni);
  getline(&line, &len, fp);

  double longueurCourante, demandeCourante;
  int nbLongueurs = 0;

  /* Récupération des longueurs finales et des demandes */
  while ((read = getline(&line, &len, fp)) != -1) {
      sscanf(line, "%lf %lf", &longueurCourante, &demandeCourante);
      nbLongueurs ++;
      printf("Longueur finale : %g, demande : %g\n", longueurCourante, demandeCourante);
      longueursFin = realloc(longueursFin, nbLongueurs * sizeof(double));
      demandes = realloc(demandes, nbLongueurs * sizeof(double));
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
    for(j=0; j < nbLongueurs; j++){
      if(i == j){
        decFact[i][j] = (double) ((int)longueurIni / (int)longueursFin[i]);
      }
      else{
        decFact[i][j] = 0;
      }
    }
  }

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

  while(1) {

    printf("\n----------------- Decoupe ---------------------\n\n");

    duals = cut(decVarNb, decConsNb, decCons, decFact);

    printf("\n----------------- Sac A Dos -------------------\n\n");

    primals = backPack(bagVarNb, bagConsNb, bagCons, bagFact, duals);
    free(duals);

    if(primals == NULL){
      break;
    }
      
    decVarNb ++;

    decFact = realloc(decFact, decVarNb * sizeof(double*));
    decFact[decVarNb-1] = primals;
  }

  printf("Terminé !\n");

  for(i = 0; i < bagVarNb; i++) {
    free(bagFact[i]);
  }
  free(primals);
  for(i = 0; i < decVarNb; i++) {
    free(decFact[i]);
  }
  free(bagFact);
  free(decFact);
  free(longueursFin);
  free(demandes);

  fclose(fp);
  free(line);

  return 0;
}
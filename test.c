#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double calc(void){
  return 1.0;
}

int main(void){
  int xsize = 2, xsize2 = 2,
      ysize = 2, ysize2 = 2;
  int i,j,k,l;
  double ****t = (double****)malloc(xsize * sizeof(double***));
  t[0] = (double***)malloc(xsize * ysize * sizeof(double**));
  t[0][0] = (double**)malloc(xsize * ysize * xsize2 * sizeof(double*));
  t[0][0][0] = (double*)malloc(xsize * ysize * xsize2 * ysize2 * sizeof(double));
  //init
  for (i = 0; i < xsize; i++) {
    t[i] = t[0] + i * ysize;
    for (j = 0; j < ysize; j++) {
      t[i][j] = t[0][0] + i*ysize*xsize2 + j*xsize2;
      for (k = 0; k < xsize2; k++) t[i][j][k] = t[0][0][0] + i*ysize*xsize2*ysize2 + j*xsize2*ysize2 + k*ysize2;
    }
  }


  /*            
  i : keido
  j : ido
  k : keido2
  l : ido2
               (keido2,ido2)
	       -------
   (keido,ido) |  |  |
               -------
               |  |  |
               -------
   */

  //calc
  for(i=0;i<xsize;i++){
    for(j=0;j<ysize;j++){
      for(k=0;k<xsize2;k++){
	for(l=0;l<ysize2;l++){
	  t[i][j][k][l] = calc();
	}//l
      }//k
      printf("%d,%d\n",i,j);
    }//j
  }//i

  /*
  //check
  for(i=0;i<xsize;i++){
    for(j=0;j<ysize;j++){
      for(k=0;k<xsize2;k++){
	for(l=0;l<ysize2;l++){
	}
      }
    }
  }
  */
  //free
  free(t[0][0][0]);
  free(t[0][0]);
  free(t[0]);
  free(t);  
  
  return 0;
}

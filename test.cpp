#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

typedef struct{
  double *dist;
  int *hope;
  int *num;
  int state; //-1:non-marriaged 1:marriaged 
}Estdata;


//return sum of dist
double gale_shapel(double *keido,double *ido,double *lon,double *lat,int dnum);
double calc_dist(double keido,double ido,double lon,double lat,int dnum);
void swap(double &a,double &b);
void swap(int &a, int &b);
void MakeHope(Estdata arr, int left, int right);
int compare(Estdata data, int b, int dnum);

int main(void){
  int i,j,k,l,dnum=8;
  double *keido,*ido,*lat,*lon,val;  

  keido = (double*)malloc(sizeof(double)*dnum);
  ido = (double*)malloc(sizeof(double)*dnum);
  lon = (double*)malloc(sizeof(double)*dnum);
  lat = (double*)malloc(sizeof(double)*dnum);
  
  for(i=0;i<dnum;i++){
    keido[i] = 100 + rand()%dnum;
    ido[i] = 10 + rand()%dnum;
    lon[i] = 110 + rand()%10;
    lat[i] = 20 + rand()%10;
  }

  //check
  for(i=0;i<dnum;i++){
    printf("%f %f %f %f\n",keido[i],
	   ido[i],
	   lon[i],
	   lat[i]);
  }

  val = gale_shapel(keido,ido,lon,lat,dnum);

  std::cout << val << std::endl;

  free(lat);
  free(lon);
  free(ido);
  free(keido);

  return 0;
}

/********************************************
typedef struct{
  double *dist;
  int *hope;
  int *num;
  int state; //-1:non-marriaged 1:marriaged 
}Estdata;

double gale_shapel(double *keido,double *ido,double *lon,double *lat,int dnum);
double calc_dist(double keido,double ido,double lon,double lat,int dnum);
void swap(double &a,double &b);
void swap(int &a, int &b);
void MakeHope(Estdata arr, int left, int right);
int compare(Estdata data, int b, int dnum);
 *******************************************/

double gale_shapel(double *keido,double *ido,double *lon,double *lat,int dnum){
  int i,j,k,like,cng,flag;
  double dist_sum,tmp,min;
  Estdata *list, *list2;

  //alloc
  list = (Estdata*)malloc(sizeof(Estdata)*dnum);
  list2 = (Estdata*)malloc(sizeof(Estdata)*dnum);
  for(i=0;i<dnum;i++){
    list[i].dist = (double*)malloc(sizeof(double)*dnum);
    list[i].hope = (int*)malloc(sizeof(int)*dnum);
    list[i].num = (int*)malloc(sizeof(int)*dnum);
    list2[i].dist = (double*)malloc(sizeof(double)*dnum);
    list2[i].hope = (int*)malloc(sizeof(int)*dnum);
    list2[i].num = (int*)malloc(sizeof(int)*dnum);
  }  

  //make dist
  for(i=0;i<dnum;i++){
    list[i].state  = -1; //mikon
    list2[i].state = -1;
    for(j=0;j<dnum;j++){
      list[i].num[j] = j;
      list[i].hope[j] = list[i].num[j];
      list[i].dist[j] = calc_dist(keido[i],ido[i],lon[j],lat[j],dnum);
      list2[i].num[j] = j;
      list2[i].hope[j] = list[i].num[j];
      list2[i].dist[j] = calc_dist(keido[j],ido[j],lon[i],lat[i],dnum);
    }
  }
  
  //make hopelist
  for(i=0;i<dnum;i++){
    MakeHope(list[i], 0, dnum-1);
    MakeHope(list2[i], 0, dnum-1);
  }  

  
  //check
  for(i=0;i<dnum;i++){
    printf("list:");
    for(k=0;k<dnum;k++){
      printf("%d ",list[i].hope[k]);
    }
    printf("\nlist2:");
    for(k=0;k<dnum;k++){
      printf("%d ",list2[i].hope[k]);
    }
    puts("\n");
  }
  

  //gale shapley 
  flag=1;
  while(flag){
    for(i=0;i<dnum;i++){
      for(j=0;j<dnum;j++){
	if(list[i].state == -1){
	  like = compare(list2[list[i].hope[j]],i,dnum);
	  if(list2[list[i].hope[j]].state == -1){
	    list2[list[i].hope[j]].state = i;
	    list[i].state = list[i].hope[j];
	    printf("%d %d marriage\n",i,list[i].hope[j]);
	  }else if(like == 1){
	    cng = list2[list[i].hope[j]].state;
	    list[cng].state = -1;
	    printf("%d %d divorce and ",cng,list[i].hope[j]);
	    list2[list[i].hope[j]].state = i;
	    list[i].state = list[i].hope[j];
	    printf("%d %d marriage\n",i,list[i].hope[j]);
	  }else{
	    printf("%d %d failed\n",i, list[i].hope[j]);
	  }
	}
    }
      flag = 0;
      for(j=0;j<dnum;j++){
	if(list[j].state == -1){
	  flag = 1;
	}
      }
    }
  }

  /*
 //check : pairs are in state array
 for(i=0;i<dnum;i++){
   printf("%d | %d   %d\n",i,list[i].state, list2[i].state);
  }
  */
 //sum
 dist_sum=0;
 for(i=0;i<dnum;i++){
   tmp = calc_dist(keido[i],ido[i],lon[list[i].state],lat[list[i].state],dnum);
   dist_sum = dist_sum + tmp;
 }
  for(i=0;i<dnum;i++){
    free(list[i].num);
    free(list[i].hope);
    free(list[i].dist);
    free(list2[i].num);
    free(list2[i].hope);
    free(list2[i].dist);
  }
  free(list);
  free(list2);

  return dist_sum;
}

double calc_dist(double a,double b,double c,double d,int dnum){
  double val,tmp,tmp2;
  tmp  = pow(a-c,2.0);
  tmp2 = pow(b-d,2.0);
  val = sqrt(tmp+tmp2);
  return val;
}

void MakeHope(Estdata arr, int left, int right) {
      int i = left, j = right;
      double tmp;
      double pivot = arr.dist[(left + right) / 2];
 
      while (i <= j) {
	while (arr.dist[i] < pivot)
                  i++;
	while (arr.dist[j] > pivot)
                  j--;
            if (i <= j) {
	      swap(arr.dist[i],arr.dist[j]);
	      swap(arr.hope[i],arr.hope[j]);
                  i++;
                  j--;
            }
      };
 

      if (left < j)
            MakeHope(arr, left, j);
      if (i < right)
            MakeHope(arr, i, right);
}

void swap(double &a,double &b){
  double tmp;
  tmp = a;
  a = b;
  b = tmp;
}

void swap(int &a, int &b){
  int tmp;
  tmp = a;
  a = b;
  b = tmp;
}

int compare(Estdata data, int b, int dnum){
  int i,tmp,tmp2;
  for(i=0;i<dnum;i++){
    if(data.hope[i] == data.state){
      tmp = i;
    }
    if(data.hope[i] == b){
      tmp2 = i;
    }
  }
  
  if(tmp2 < tmp){
    return 1;
  }  
  return -1;
  
}

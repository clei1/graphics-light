#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;

  normalize(normal);
  normalize(view);
  normalize(light[LOCATION]);

  color a = calculate_ambient(alight, areflect);
  color d = calculate_diffuse(light, dreflect, normal);
  color s = calculate_specular(light, sreflect, view, normal);

  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;

  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;

  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;

  double nl = dot_product(light[LOCATION], normal);
  
  d.red = light[COLOR][RED] * dreflect[RED] * nl;
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * nl;
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * nl;
  
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  int expt = 10;
  double tmp[3];

  tmp[0]= (2 * dot_product(normal, light[LOCATION]) * normal[0]) - light[LOCATION][0];
  tmp[1]= (2 * dot_product(normal, light[LOCATION]) * normal[1]) - light[LOCATION][1];
  tmp[2]= (2 * dot_product(normal, light[LOCATION]) * normal[2]) - light[LOCATION][2];
  
  double nl = pow(dot_product(tmp, view), expt);
  
  s.red = light[COLOR][RED] * sreflect[RED] * nl;
  s.green = light[COLOR][GREEN] * sreflect[GREEN] * nl;
  s.blue = light[COLOR][BLUE] * sreflect[BLUE] * nl;
  
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if(c->red > 255){
    c->red = 255;
  }
  if(c->blue > 255){
    c->blue = 255;
  }
  if(c->green > 255){
    c->green = 255;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double mag = 0;
  
  for(int i = 0; i < 3; i++){
    mag += vector[i] * vector[i];
  }
  mag = sqrt(mag);

  for(int j = 0; j < 3; j++){
    vector[j] = vector[j]/mag;
  }
}

//Return the dot product of a . b
double dot_product( double *a, double *b ) {
  return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}

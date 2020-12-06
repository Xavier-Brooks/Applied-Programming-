/*--------------------------------------------------------------------------
  Simple scanf test program
  student copy
--------------------------------------------------------------------------*/
#include <stdio.h>                    /* ex2a_scan.c */

const double mi2km = 1.609;
double convert(double mi) {return (mi * mi2km);}
int main() {
   double miles;
   int rc;

   printf("Miles to Km Conversion (enter negative number to end)\n");
   do {
      printf("Input distance in miles: ");
      rc = scanf("%f", &miles);
      if (rc == -1){
	  printf("error, nothings been parsed\n");
          return 1;
      }
      printf("\n%f miles = %f km\n", miles, convert(miles));
   } while (miles > 0.0);

   return 0;
}

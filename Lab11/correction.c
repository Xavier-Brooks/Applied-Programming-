/******************************************************************
 * Program to correct the data for the sensor
 * Note: Each student gets unique data, so this exact equation will
 * differ for each students solution
 * Be sure to use Honer's factorization
 * ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Runs the data through the fitting line */

int main(int argc, char *argv[])
{
    int res, real, ideal;
    float ans;

    while(scanf("%d %d", &ideal, &real) != EOF)
    {
     /* Insert your polynomial here, be sure to round properly */
	ans = -48.9559 + (0.293484*real) + (-5.31075e-05)*real*real;
	res = round(ans);

	//round res
	res = real - res;
        printf("%d %d\n", ideal, res);
    }
    return 0;
}

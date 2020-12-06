/* ==========================================================================
      ODEsolvers module - student version
      Version: 1.01
      Juan C. Cockburn, Nov. 2009, Nov. 2012
      08/10/2015 - R. Repka     Minor text changes
      05/12/2016 - R. Repka     Better calculations
      07/21/2017 - R. Repka     Added ClassErrors.h
      06/24/2020 - R. Repka     created HW 14
   ======================================================================= */   
#include <stdio.h>
#include "ClassErrors.h"
#include "ODEsolvers.h"


/*---------------------------------------------------------------------------
  This perform a single time step of Euler's method to approximate 
  the solution of a system of ODE's
  
    k1 = f(tk, xk)
    xk+1 = xk+hk1


  Where: simParm *sim - simulation constants
         double  t0 - value of independent variable at current time
         double *x0 - value of state variables at current time, AND
                      the approximate solution time t0+h is returned
                      (x0 is overwritten)
         funArgs  f - function which defines the differential equations
                     (see ode_solvers.h for details)
  Returns: nothing
  Errors:  prints an error and exits with return code 99
---------------------------------------------------------------------------*/
void eu(simParm *sim, double t0, double *x0, funArgs f )
{
     int i;         /* loop counter     */
     double *xp;    /* temporary vector */

     xp = malloc(sim->neq * sizeof(double));
     if (NULL == xp){
         printf("Error: Could not allocate memory in '%s' at line %d\n", __FILE__, __LINE__);
         exit(MALLOC_ERROR);
     }

     /* Compute Euler update - return in x0 */

     /* Evaluate the Differential equations at the current time   */
     /*  k1 = f(tk, xk)  */
     f(sim, t0, x0, xp);

     /* Add the values to the current integration solution */
     /* xk+1 = xk+hk1    */
     for ( i = 0; i < sim->neq; i++ ){
         x0[i] += sim->h * xp[i];
     }

    /* clean up temporary variables */
     free(xp);
} /* End eu */


/*---------------------------------------------------------------------------
  This performs a single time step of the classical 2nd-order Runge-Kutta
  method to approximate the solution of a system of ODE's
  rk2 - Runge-Kuta 2nd order method  ( trapezoid integration)
  
    k1 = f(tk, xk)
    k2 = f(tk+ h, xk+hk1)
    xk+1 = xk+h(½k1+½k2)

        
  Where: simParm *sim - simulation constants
         double  t0 - value of independent variable at current time
         double  td - Time for the .5 second disturbance           
         double *x0 - value of state variables at current time, AND
                      the approximate solution time t0+h is returned
                      (x0 is overwritten)
         funArgs f - function which defines the right-hand side
                     of the differential equation of the form:
                     void f ( double t, double *x, double *xp )
                    (see ode_solvers.h for details)
  Returns: nothing
  Errors:  prints an error and exits with return code 99
---------------------------------------------------------------------------*/
void rk2( simParm *sim, double t0, double *x0, funArgs f )
{
     int i;                            /* Loop counter */
     double *xtilde, *k1, *k2;         /* temporary vectors */

     xtilde = malloc(sim->neq*sizeof(double));
     k1 = malloc(sim->neq*sizeof(double));
     k2 = malloc(sim->neq*sizeof(double));

     if ((NULL == xtilde) ||(NULL == k1) ||(NULL == k2)){
         printf("Error: Could not allocate memory in '%s' at line %d\n", __FILE__, __LINE__);
         exit(MALLOC_ERROR);
     }

     /* evaluate rhs of equations - returns dx in xp */

     /* Evaluate the Differential equations at the current time   */
     /* k1 = f(tk, xk)  */
     f(sim, t0, x0, k1); /* updates k1 */


     /* Compute RK corrections     */
     for ( i = 0; i < sim->neq; i++ ) {
         /* Build k2 function parameters   xk+hk1*/
         xtilde[i] = x0[i] + sim->h*k1[i];
     }
    /* Evaluate the Differential equations at the current time   */
    /* k2 = f(tk+ h, xk+hk1) */
     f(sim, t0+sim->h, xtilde, k2);

     /* update dx xk+1 = xk+h(½k1+½k2)  */
     for ( i = 0; i < sim->neq; i++ ) {
         x0[i] += sim->h*(k1[i] + k2[i])/2.0;
     }

    /* clear temporary variables */
     free(xtilde);
     free(k1);
     free(k2);

} /* End rk2 */


/*---------------------------------------------------------------------------
  This performs a single time step of the classical 3rd-order Runge-Kutta
  method to approximate the solution of a system of ODE's

    k1 = f(tk, xk)
    k2 = f(tk+ 2⁄3h, xk+2⁄3hk1)
    k3 = f(tk+ 2⁄3h, xk+2⁄3hk2)
    xk+1 = xk+h(1⁄4k1+3⁄8k2+3⁄8k3)

  Where: simParm *sim - simulation constants
         double  t0 - value of independent variable at current time
         double *x0 - value of state variables at current time, AND
                      the approximate solution time t0+h is returned
                      (x0 is overwritten)
         funArgs f - function which defines the right-hand side
                     of the differential equation of the form:
                     void f ( double t, double *x, double *xp )
                    (see ode_solvers.h for details)
  Returns: nothing
  Errors:  prints an error and exits with return code 99
---------------------------------------------------------------------------*/
void rk3(simParm *sim, double t0, double *x0, funArgs f )
{
	int i; //loop counter
	double con = (2.0/3.0);
	double *xtilde, *k1, *k2, *k3; //temporary vectors

	xtilde = malloc(sim->neq * sizeof(double));
	k1 = malloc(sim->neq * sizeof(double));
	k2 = malloc(sim->neq * sizeof(double));
	k3 = malloc(sim->neq * sizeof(double));

	if((NULL == xtilde) || (NULL == k1) || (NULL == k2) || (NULL == k3)){
		printf("Error: could not allocate memory in '%s' at line %d\n", __FILE__, __LINE__);
		exit(MALLOC_ERROR);
	}

	//initial evaluation
	f(sim, t0, x0, k1);

	//first correction
	for ( i = 0; i < sim->neq; i++ ) {
		/* Build k2 function parameters   xk+(2/3)hk1*/
		xtilde[i] = x0[i] + (con)*(sim->h*k1[i]);
	}

        /* Evaluate the Differential equations at the current time   */
        /* k2 = f(tk+ 2/3h, xk+2/3hk1) */
        f(sim, t0 +(con)*(sim->h), xtilde, k2);

	//second correction
	for ( i = 0; i < sim->neq; i++ ) {
                /* Build k2 function parameters   xk+(2/3)hk2*/
                xtilde[i] = x0[i] + (con)*(sim->h*k2[i]);
        }

	//evaluate k3 = f(tk + 2/3h, xk + 2/3hk2)
	f(sim, t0 + (con)*(sim->h), xtilde, k3);

	//update dx, xk+1 = xk +h(1/4k1 + 3/8k2 + 3/8k3)
	for ( i = 0; i < sim->neq; i++ ) {
              x0[i] += (sim->h)*( (3.0/8.0)*(k2[i]+k3[i]) + (k1[i]/4.0) );
        }

	free(xtilde);
	free(k1);
	free(k2);
	free(k3);

} /* End rk3 */



/*---------------------------------------------------------------------------
  This performs a single time step of the classical 4th-order Runge-Kutta
  method to approximate the solution of a system of ODE's
      rk4    - Runge-Kuta 4th order method  (classical)
    
       k1 = f(tk, xk)
    k2 = f(tk+ ½h, xk+½hk1)
    k3 = f(tk+ ½h, xk+½hk2)
    k4 = f(tk+ h, xk+hk3)
    xk+1 = xk+h(1⁄6k1+1⁄3k2+1⁄3k3+1⁄6k4)


 Where:  simParm *sim - simulation constants
         double  t0 - value of independent variable at current time
         double *x0 - value of state variables at current time, AND
                      the approximate solution time t0+h is returned
                      (x0 is overwritten)
         funArgs f - function which defines the right-hand side
                      of the differential equation of the form:
                      void f ( double t, double *x, double *xp )
                     (see ode_solvers.h for details)
  Returns: nothing
  Errors:  prints an error and exits with return code 90
---------------------------------------------------------------------------*/
void rk4(simParm *sim, double t0, double *x0, funArgs f )
{

        int i; //loop counter
        double *xtilde, *k1, *k2, *k3 , *k4; //temporary vectors

        xtilde = malloc(sim->neq * sizeof(double));
        k1 = malloc(sim->neq * sizeof(double));
        k2 = malloc(sim->neq * sizeof(double));
        k3 = malloc(sim->neq * sizeof(double));
	k4 = malloc(sim->neq * sizeof(double));

        if((NULL == xtilde) || (NULL == k1) || (NULL == k2) || (NULL == k3)){
                printf("Error: could not allocate memory in '%s' at line %d\n", __FILE__, __LINE__);
                exit(MALLOC_ERROR);
        }

        //initial evaluation
        f(sim, t0, x0, k1);

        //first correction
        for ( i = 0; i < sim->neq; i++ ) {
                /* Build k2 function parameters   xk+(1/2)hk1*/
                xtilde[i] = x0[i] + ((1.0/2.0)*(sim->h*k1[i]));
        }

	/* Evaluate the Differential equations at the current time   */
        /* k2 = f(tk+ 1/2h, xk+1/2hk1) */
        f(sim, t0 +((1.0/2.0)*(sim->h)), xtilde, k2);

        //second correction
        for ( i = 0; i < sim->neq; i++ ) {
                /* Build k2 function parameters   xk+(1/2)hk2*/
                xtilde[i] = x0[i] + (1.0/2.0)*(sim->h*k2[i]);
        }

        //evaluate k3 = f(tk + 1/2h, xk + 1/2hk2)
        f(sim, t0 + ((1.0/2.0)*(sim->h)), xtilde, k3);

	//third correction
        for ( i = 0; i < sim->neq; i++ ) {
                /* Build k2 function parameters   xk+hk3*/
                xtilde[i] = x0[i] + (sim->h*k3[i]);
        }

        f(sim, t0 + (sim->h), xtilde, k4);

        //update dx, xk+1 = xk +h(1/4k1 + 3/8k2 + 3/8k3)
        for ( i = 0; i < sim->neq; i++ ) {
              x0[i] += (sim->h)*( ((1.0/6.0)*k1[i]) + ((1.0/3.0)*k2[i]) + ((1.0/3.0)*k3[i]) + ((1.0/6.0)*k4[i]));
        }

	free(xtilde);
        free(k1);
        free(k2);
        free(k3);
	free(k4);
} /* End rk4 */

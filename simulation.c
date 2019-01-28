
#include <stdio.h> //printf
#include <stdlib.h> //srand, rand, RAND_MAX
#include <time.h> //time(0) (for seeding rand)
#include <math.h> //sqrt, pow

/* 
 * in molecular dynamics simulations, pairwise potential 
 * energies are given by the van der Waals potential
 * V(x)=a*r^(-n)-b*r^(-m). Usually, m=6 and n=12. 
 * I choose m=2, n=3 to make the potential much softer.
 */
const double A=1.0,B=2.0,N=3.0,M=2.0;

//Note that I promote NPOINTS to a global constant, so I no longer
//need to pass it in as an argument.
const int NPOINTS=100; 
const int NTIMESTEPS=300;

/* Generate a random number between a and b. */
double myRandom(double a, double b){
    return (b-a)*(rand()*1.0)/RAND_MAX+a;
}

/* "van der Waals" acceleration. This is -V'(x), where 
 * V(x)=A*x^(-N)-B*x^(-M) */
double vdwAcceleration(double dist){
    return -B*M*pow(dist,-1-M)+A*N*pow(dist,-1-N);
}

/* Acceleration function which only has NPOINTS*(NPOINTS-1)/2 
 * distance calculations. Still O(N^2), but we get a factor of two speedup. */
void calculateAcceleration(double ax[], double ay[], double x[], double y[]){
    //First, zero all points.
    for(int i=0;i<NPOINTS;i++){
        ax[i]=0;
        ay[i]=0;
    }
    //i index runs through all points
    for(int i=0;i<NPOINTS;i++){
        //j index only runs through some points.
        //Visualization: an N by N matrix. (i,j) runs through every lower diagonal. 
        for(int j=0;j<i;j++){
            //(diffx,diffy) is a vector which points FROM particle i, TO particle j. 
            //Subtracting (x[j],y[j]) puts the jth particle at the origin.
            double diffx=x[i]-x[j];
            double diffy=y[i]-y[j];
            double d=sqrt(diffx*diffx+diffy*diffy);

            //Calculate the acceleration. I assume every particle has unit mass, so force=acceleration.
            double force=vdwAcceleration(d);

            //This gives a vector with magnitude "force" in the direction pointing to particle i from particle j.
            double forcex=diffx*force/d;
            double forcey=diffy*force/d;

            //If the force is positive, the particles should be pushed away. So particle "i" should feel a
            //force in direction (diffx,diffy)/d, and particle "j" should feel the negative of this force.
            //Note that momentum will be conserved exactly (to within floating point accuracy).
            ax[i]+=forcex;
            ay[i]+=forcey;
            ax[j]-=forcex;
            ay[j]-=forcey;
        }
    }
}

/* Note that this differs from the original leapfrog code: we do only one 
 * force calculation per timestep, because the force calculation is 
 * expensive. This is still exactly the same leapfrog algorithm! 
 * Now I just assume the x,y values are at full timesteps, and the vx,vy values are 
 * at half timesteps.
 * */
void leapstep(double x[], double y[], double vx[], double vy[], double dt){
    double ax[NPOINTS];
    double ay[NPOINTS];

    calculateAcceleration(ax,ay,x,y);
    for(int i=0;i<NPOINTS;i++){
        vx[i]+=ax[i]*dt;
        vy[i]+=ay[i]*dt;
    }
    for(int i=0;i<NPOINTS;i++){
        x[i]+=vx[i]*dt;
        y[i]+=vy[i]*dt;
    }
}

/* If we have initial conditions x(0)=1, v(0)=0, we can't just plug this into the leapfrog algorithm. 
 * The leapfrog algorithm needs velocity to be initialized at v(-dt/2). So, we take a timestep backwards
 * to get the correct initial velocities.
*/
void initLeapstep(double x[], double y[], double vx[], double vy[], double dt){
    double ax[NPOINTS];
    double ay[NPOINTS];
    calculateAcceleration(ax,ay,x,y);
    for(int i=0;i<NPOINTS;i++){
        vx[i]-=ax[i]*dt/2;
        vy[i]-=ay[i]*dt/2;
    }
}

void printstate(double x[],double y[], double tnow) {
    for (int i=0;i<NPOINTS;i++)
            printf("%8.4f%4d%12.6f%12.6f\n", tnow, i, x[i], y[i]);
}



int main(){
    double dt=0.01;

    double x[NPOINTS];
    double y[NPOINTS];
    double vx[NPOINTS];
    double vy[NPOINTS];

    //Initialize the x,y,vx,vy positions and velocities.
    for(int row=0;row<10;row++){
        for(int col=0;col<10;col++){
            int i=col+row*10;
            //Arrange the positions in a grid
            x[i]=row;
            y[i]=col;
            //Add in some randomness - start the velocities near zero but a bit chaotic.
            vx[i]=myRandom(-0.01,0.01);
            vy[i]=myRandom(-0.01,0.01);
        }
    }

    //Initialize the velocities: we were given v(0), so this step finds v(-dt/2). 
    initLeapstep(x,y,vx,vy,dt);

    //Run the simulation and output the results
    for(int k=0;k<NTIMESTEPS;k++){
        //Take five timesteps for every output. 
        for(int substeps=0;substeps<5;substeps++)
            leapstep(x,y,vx,vy,dt);

        //Print all particles to the console.
        printstate(x,y,k*dt);
    }

    return 0;
}




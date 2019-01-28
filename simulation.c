
#include <stdio.h> //printf
#include <stdlib.h> //srand, rand, RAND_MAX
#include <time.h> //time(0) (for seeding rand)
#include <math.h> //sqrt, pow

/*

Pairwise potential energies are generally 

a*r^(-n)-b*r^(-m)

For the van der Waals potential, m=6 and n=12. 
I choose m=2, n=3 to make the potential much softer.
 */
const double A=1.0,B=2.0,N=3.0,M=2.0;
const int NPOINTS=100;
const int NTIMESTEPS=300;

double myRandom(double a, double b){
    return (b-a)*(rand()*1.0)/RAND_MAX+a;
}

double vdwAcceleration(double dist){
    return -B*M*pow(dist,-1-M)+A*N*pow(dist,-1-N);
}

void calculateAcceleration(double ax[], double ay[], double x[], double y[]){
    //First, zero all points.
    for(int i=0;i<NPOINTS;i++){
        ax[i]=0;
        ay[i]=0;
    }
    for(int i=0;i<NPOINTS;i++){
        for(int j=0;j<i;j++){
            double diffx=x[i]-x[j];
            double diffy=y[i]-y[j];
            double d=sqrt(diffx*diffx+diffy*diffy);
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

/* Assuming 
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
    for(int row=0;row<10;row++){
        for(int col=0;col<10;col++){
            int i=col+row*10;
            x[i]=row;
            y[i]=col;
            vx[i]=myRandom(-0.01,0.01);
            vy[i]=myRandom(-0.01,0.01);
        }
    }
    initLeapstep(x,y,vx,vy,dt);
    for(int k=0;k<NTIMESTEPS;k++){
        for(int substeps=0;substeps<5;substeps++)
            leapstep(x,y,vx,vy,dt);
        printstate(x,y,k*dt);
    }

    return 0;
}




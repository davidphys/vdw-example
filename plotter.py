import matplotlib.pyplot as plt
import pylab
from matplotlib.collections import PatchCollection

NTIMESTEPS=300
NPOINTS=100

data=pylab.loadtxt("out.txt")


#5 second video at 30 fps = 150 frames
for timestep in range(NTIMESTEPS):
    plt.close('all')
    fig, axes = plt.subplots(figsize=(5,5))
    #adding the Circles to the plot using a PatchCollection is much faster.
    patches=[]
    #some cool motion
    for i in range(NPOINTS):
        index=timestep*NPOINTS+i;
        patches.append(plt.Circle((data[index,2],data[index,3]),0.15))
    axes.add_collection(PatchCollection(patches, alpha=0.95))
    axes.set_xlim(-1,10)
    axes.set_ylim(-1,10)
    axes.set_xlabel("x (m)")
    axes.set_ylabel("y (m)")
    axes.set_title("t=%.2f seconds" % data[timestep*NPOINTS,0])
    axes.set_aspect('equal')
    plt.savefig("out/out%03d.jpg"%timestep,dpi=80)



all: simulation.c plotter.py
	gcc simulation.c -o simulation -lm
	./simulation > out.txt
	-mkdir out
	python plotter.py
	convert -delay 3 -loop 0 out/out*.jpg animation.gif

clean : 
	rm simulation out.txt
	rm -r out


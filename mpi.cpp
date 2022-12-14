
#include "asg2.h"
#include <stdio.h>
#include <mpi.h>


int rank;
int world_size;


int main(int argc, char *argv[]) {
	if ( argc == 4 ) {
		X_RESN = atoi(argv[1]);
		Y_RESN = atoi(argv[2]);
		max_iteration = atoi(argv[3]);
	} else {
		X_RESN = 1000;
		Y_RESN = 1000;
		max_iteration = 100;
	}

	if (rank == 0) {
		#ifdef GUI
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(500, 500); 
		glutInitWindowPosition(0, 0);
		glutCreateWindow("MPI");
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0, X_RESN, 0, Y_RESN);
		glutDisplayFunc(plot);
		#endif
	}

	/* computation part begin */
	MPI_Init(&argc, &argv);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
    initData();

	if (rank == 0){
		t1 = std::chrono::high_resolution_clock::now();
	}

    total_size = X_RESN * Y_RESN;
    int my_size = total_size / world_size;
    Point *my_data = new Point[my_size];
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(data, my_size * sizeof(Point), MPI_CHAR, my_data, my_size * sizeof(Point), MPI_CHAR, 0, MPI_COMM_WORLD);
    Point *p = my_data;
    for (int i = 0; i < my_size; i++) {
        compute(p);
        p++;
    }
	MPI_Gather(my_data, my_size * sizeof(Point), MPI_CHAR, data, my_size * sizeof(Point), MPI_CHAR, 0, MPI_COMM_WORLD);

	if (rank == 0){
		t2 = std::chrono::high_resolution_clock::now();  
		time_span = t2 - t1;
	}

	if (rank == 0){
		printf("Student ID: 119010319\n"); // replace it with your student id
		printf("Name: Yuancheng Wang\n"); // replace it with your name
		printf("Assignment 2 MPI\n");
		printf("Run Time: %f seconds\n", time_span.count());
		printf("Problem Size: %d * %d, %d\n", X_RESN, Y_RESN, max_iteration);
		printf("Process Number: %d\n", world_size);
	}

	MPI_Finalize();
	/* computation part end */

	if (rank == 0){
		#ifdef GUI
		glutMainLoop();
		#endif
	}

	return 0;
}
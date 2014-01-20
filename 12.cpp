/*
О молекуле воды
Атомы водорода и кислорода колеблются в пространстве, пытаясь собраться в молекулы 
воды. Для этого нужна взаимная синхронизация двух атомов водорода и одного атома кислорода. 
Пусть атомы кислорода (O) и водорода (H) моделируются потоками управления. Каждый атом 
(H) вызывает процедуру Hready, когда он готов объединиться в молекулу. Аналогично готовый к 
соединению атом кислорода O вызывает процедуру Oready.
Напишите эти две процедуры, пользуясь для синхронизации семафорами. Атом H в 
процедуре Hready должен приостановиться, пока другой атом H и один атом O не вызовут 
соответственно процедуры Hready и Oready. Тогда один из потоков (например O) должен вызвать 
процедуру makeWater. После выхода из процедуры makeWater все три процесса должны 
вернуться из своих вызовов Hready и Oready. Не пользуйтесь в решении активным ожиданием, 
обеспечьте отсутствие взаимной блокировки и зависаний
*/
#include <mpi.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
using namespace std;

bool check(int * disp) {
	for (int i = 0; i < 3; ++i) 
		if (disp[i] == 0) return false;
	return true;
}

int main(int argc, char *argv[]) {        

    int rank, nprocs;
    int TAG = 0;
    int iO = 0, iH = 0;
    int disp[3] = {0, 0, 0};
    std::vector<int> vH, vO; 
    
    MPI_Init(&argc,&argv); 
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 

    if (rank != 0) {
    	sleep(1);
    	MPI_Request request;
        MPI_Status st;
        if(rank % 2 == 0) {
        	iH = rank;
        	MPI_Isend( &iH, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &request);
            MPI_Wait( &request, &st);
           	cout << "Get H " << rank << endl;
        }
        else {
        	iO = rank;
        	MPI_Isend( &iO, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &request);
            MPI_Wait( &request, &st);
		    cout << "Get O " << rank << endl;
        }
        sleep(1);
    }

    else {
    	sleep(5);
		for (int i = 1; i < nprocs; i++) {

	        int buf = 0;
			int flag;
			MPI_Request request;
			MPI_Status st;
    		
    		MPI_Irecv( &buf, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &request);
    		MPI_Test( &request, &flag, &st);
    		//cout << "buf: " << buf << endl;
    		if (buf % 2 == 0)  { // H

    			if (disp[1] == 0) disp[1] = buf;
    			else if (disp[2] == 0) disp[2] = buf;
    			else vH.push_back(buf);
			}
			else { //O
				if (disp[0] == 0) disp[0] = buf;
				else vO.push_back(buf);
			}

    		
    		if (check(disp)) {
    			printf("makeWater: O[%d], H[%d], H[%d]\n", disp[0], disp[1], disp[2]);
    			for (int i = 0; i < 3; ++i) disp[i] = 0;
    		}

			if(!vO.empty() && disp[0] == 0) {
				disp[0] = vO.back();
				vO.pop_back();
			} 
			if(!vH.empty() && disp[1] == 0) {
				disp[1] = vH.back();
				vH.pop_back();
			} 
			if(!vH.empty() && disp[2] == 0) {
				disp[2] = vH.back();
				vH.pop_back();
			} 

			buf = 0;
			sleep(1);
		}

	}
    
    MPI_Finalize(); 
    return 0; 
}

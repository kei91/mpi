/*
10 студентов РХТУ опаздывают на занятия и, отталкивая друг друга, пытаются 
войти в трамвай на станции метро «Сходненская». Для этого они посылают сообщение 
электронному кондуктору «у меня есть билет». Кондуктор получает эти сообщения в 
случайном порядке и пропускает студентов по одному, посылая каждому из студентов 
сообщение «проходи». Получив сообщение, студент проходит в трамвай и выдает на экран «я 
внутри».
*/

#include <mpi.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {        

    MPI_Status status;       
    int TAG = 0;
    int p, myrank;
    int students[10];
    for(int i = 0; i < 10; i++) { students[i] = i + 1; }

    MPI_Init( &argc, &argv );
    MPI_Comm_size(MPI_COMM_WORLD, &p); //получить количество всех процессов
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank); // получить ранг (номер) процесса в группе (коммуникаторе)
    if (myrank != 0){
        cout << "student " << myrank << " have a ticket!" << endl;
        MPI_Send( &myrank, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        // myrank – адрес буфера с данными;
		// 1 – число элементов (типа datatype) в буфере;
		// MPI_INT – тип данных каждого элемента в буфере;
		// 0 – ранг (номер) получателя в коммуникаторе (группе) comm;
		// TAG – тег сообщения;
		// MPI_COMM_WORLD – коммуникатор (группа процессов).
    } else {
        int student = 0;
        for (int i = 0; i < 10; i++){
            MPI_Recv( &student, 1, MPI_INT, students[i], TAG, MPI_COMM_WORLD, &status );
		// student – адрес буфера с данными, в котором следует разместить полученное сообщение;
		// 1 – максимальное число элементов (типа datatype) в буфере;
		// MPI_INT – тип данных каждого элемента в буфере;
		// students[i] – ранг (номер) отправителя в коммуникаторе (группе) comm (может быть MPI_ANY_SOURCE);
		// TAG – тег сообщения (можно использовать MPI_ANY_TAG);
		// MPI_COMM_WORLD – коммуникатор (группа процессов);
		// status – информация о полученном сообщении.

            cout << "student " << student << " come!" << endl;
        }
        cout << "Ended!" << endl;
    }
    MPI_Finalize();
}

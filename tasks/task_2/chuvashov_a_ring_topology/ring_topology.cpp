// Copyright 2023 Chuvashov Andrey

#include "../tasks/task_2/chuvashov_a_ring_topology/ring_topology.h"

/*
* data - ������������ ������
* count - ���������� ������
* datatype - ��� ������ data
* source - ����-�����������
* destination - ����-����������
* tag - ��� ��� ������������� ���������
* communicator - ������������
* 
* MPI_Send
    (
        void* buffer - ����� ������������� ���������,
        int count - ���������� ��������� ������ � ���������,
        MPI_Datatype type - ��� ������ ������������� ���������,
        int destination - ���� ��������, �������� ������������ ���������,
        int tag - ��������-��� ��� ������������� ���������,
        MPI_Comm communicator - ������������
    );
*
* MPI_Recv
    (
        void* buffer - ����� ��� ����� ���������,
        int count - ���������� ��������� ������ � ���������,
        MPI_Datatype type - ��� ������ ������������� ���������,
        int source - ���� ��������, �� �������� ������ ���� �������� ���� ���������,
        int tag - ��������-��� ��� ������������� ���������,
        MPI_Comm communicator - ������������,
        MPI_Status * status - ��������� �� ��������� ������ � ����������� � ���������� ����� ���������
    );
*/
int nextNode(int currentNode, int size) {
    return (currentNode + 1) % size;
}

int previousNode(int currentNode, int size) {
    return (currentNode + size - 1) % size;
}

bool checker(int currentNode, int source, int destination, int size) {
    return currentNode >= source && currentNode <= destination ||
        currentNode <= (size - 1) && currentNode >= 0;
}

void SendDataByRing(void* data, int count, MPI_Datatype datatype,
    int source, int destination, int tag, MPI_Comm communicator) {

    int rank, size;
    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);

    if (!(source < size && destination < size)) return;
    if (!checker(rank, source, destination, size)) return;
    if (size == 1) return;

    if (rank == source) {
        MPI_Send(data, count, datatype, nextNode(rank, size),
            tag, communicator);
    } else if (checker(rank,
        nextNode(source, size), previousNode(destination, size), size)) {
        MPI_Recv(data, count, datatype, previousNode(rank, size),
            tag, communicator, MPI_STATUS_IGNORE);
        MPI_Send(data, count, datatype, nextNode(rank, size),
            tag, communicator);
    } else {
        MPI_Recv(data, count, datatype, previousNode(rank, size),
            tag, communicator, MPI_STATUS_IGNORE);
    }
}

#define WrongInput wcout << L"Неверная команда" << endl
#define Output wcout << L"Сортированный масив:" <<endl;for(int i = 0; i <n;i++) wcout << mas[i] << " "; break

#include <iostream>
#include <math.h>
#include <algorithm>
#include <random>
#include <io.h>
#include <fcntl.h>
#include <bits/stdc++.h>

using std::wcin;
using std::wcout;
using std::swap;
using std::endl;
using std::string;
using std::vector;

namespace ten {
    void merge(int mas[], int first, int cut, int last) {
        int n1 = cut - first + 1;
        int n2 = last - cut;
        int L[n1], M[n2];

        for (int i = 0; i < n1; i++)
            L[i] = mas[first + i];
        for (int j = 0; j < n2; j++)
            M[j] = mas[cut + 1 + j];

        int i = 0, j = 0, k = first;
        while (i < n1 && j < n2) {
            if (L[i] <= M[j]) {
                mas[k] = L[i];
                i++;
            } else {
                mas[k] = M[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            mas[k] = L[i];
            i++;
            k++;
        }
        while (j < n2) {
            mas[k] = M[j];
            j++;
            k++;
        }
    }

    void mergeSort(int mas[], int left, int right) {
        if (left < right) {
            int midle = left + (right - left) / 2;

            mergeSort(mas, left, midle);
            mergeSort(mas, midle + 1, right);

            merge(mas, left, midle, right);
        }
    }
}
struct MinHeapNode {
    int element;
    int i;
};

void swap(MinHeapNode* x, MinHeapNode* y)
{
    MinHeapNode temp = *x;
    *x = *y;
    *y = temp;
}

class MinHeap {
    MinHeapNode* harr;
    int heap_size;
public:
    MinHeap(MinHeapNode a[], int size);
    void MinHeapify(int);
    int left(int i) {
        return (2 * i + 1);
    }
    int right(int i) {
        return (2 * i + 2);
    }
    MinHeapNode getMin() {
        return harr[0];
    }
    void replaceMin(MinHeapNode x) {
        harr[0] = x;
        MinHeapify(0);
    }
};

MinHeap::MinHeap(MinHeapNode a[], int size) {

    heap_size = size;
    harr = a;
    int i = (heap_size - 1) / 2;
    while (i >= 0) {
        MinHeapify(i);
        i--;
    }
}

void MinHeap::MinHeapify(int i) {

    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l].element < harr[i].element)
        smallest = l;
    if (r < heap_size && harr[r].element < harr[smallest].element)
        smallest = r;
    if (smallest != i) {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}


FILE* openFile(char* fileName, char* mode)
{
    FILE* fp = fopen(fileName, mode);
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void mergeData(char* opFile, int n, int k) {

    FILE* in[k];
    for (int i = 0; i < k; i++) {
        char fileName[2];
        snprintf(fileName, sizeof(fileName), "%d", i);
        in[i] = openFile(fileName, "r");
    }
    FILE* out = openFile(opFile, "w");
    MinHeapNode* harr = new MinHeapNode[k];
    int l;
    for (l = 0; l < k; l++) {
        if (fscanf(in[l], "%d ", &harr[l].element) != 1)
            break;
        harr[l].i = l;
    }
    MinHeap hp(harr, l);
    int count = 0;
    while (count != l) {
        MinHeapNode root = hp.getMin();
        fprintf(out, "%d ", root.element);
        if (fscanf(in[root.i], "%d ",
                   &root.element)
            != 1) {
            root.element = INT_MAX;
            count++;
        }
        hp.replaceMin(root);
    }
    for (int i = 0; i < k; i++)
        fclose(in[i]);

    fclose(out);
}

void initialiseData( char* ipFile, int memory, int num_ways) {

    FILE* in = openFile(ipFile, "r");
    FILE* out[num_ways];
    char fileName[2];
    for (int i = 0; i < num_ways; i++) {

        snprintf(fileName, sizeof(fileName), "%d", i);
        out[i] = openFile(fileName, "w");
    }
    int* arr = (int*)malloc( memory * sizeof(int));
    bool more_input = true;
    int next_opFile = 0;

    int l;
    while (more_input) {
        for (l = 0; l < memory; l++) {
            if (fscanf(in, "%d ", &arr[l]) != 1) {
                more_input = false;
                break;
            }
        }
        ten::mergeSort(arr, 0, l - 1);
        for (int j = 0; j < l; j++)
            fprintf(out[next_opFile], "%d ", arr[j]);
        next_opFile++;
    }
    for (int i = 0; i < num_ways; i++)
        fclose(out[i]);

    fclose(in);
}

void externalSort( char* ipFile, char* opFile, int num_ways, int memory) {

    initialiseData(ipFile, memory, num_ways);
    mergeData(opFile, memory, num_ways);
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin),  _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    int num_ways = 20;
    int memory;

    wcout << L"Внешняя многофазная" << endl;
    wcout << L"Введите доступную память (в целых Кб):";
    wcin >> memory;

    char ipFile[] = "inputFile.txt";
    char opFile[] = "outputFile.txt";

    FILE* in = openFile(ipFile, "w");

    srand(time(NULL));
    for (int i = 0; i < num_ways * memory; i++)
        fprintf(in, "%d ", rand());
    fclose(in);
    externalSort(ipFile, opFile, num_ways, memory);

    for (int i = 0; i < num_ways * memory; i++)
        fprintf(in, "%d ", rand());
    fclose(in);
    return 0;
}

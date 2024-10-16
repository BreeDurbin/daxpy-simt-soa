#include <vector>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

struct SOA_DAXPY{
    SOA_DAXPY(int n) : a(n), X(n), Y1(n), Y2(n), s(n) {}
    double a;
    vector<double> X;
    vector<double> Y1;
    vector<double> Y2;
    int s;
};

void DAXPY(int s, int e, SOA_DAXPY *soa){
    for(; s<e; s++){
        soa->Y2[s] = (soa->a * soa->X[s]) + soa->Y1[s];
    }
}

int main(int argc, char *argv[])
{
    int array_size = 10000;
    int thread_count = 100;
    int interval_size = thread_count / array_size;
    int rand_lower = 0;
    int rand_upper = 10000;

    cout << "Initializing: thread_count=" << thread_count << " array_size=" << array_size << 
        " rand_lower=" << rand_lower << " rand_upper=" << rand_upper << endl;

    srand(time(nullptr)); // use current time as seed for random generator

    SOA_DAXPY *soa = new SOA_DAXPY(array_size);

    soa->a = rand() % rand_upper;

    for(int i=0; i<array_size; i++){
        soa->X[i] = rand() % rand_upper;
    }

    std::vector<std::jthread> threads;
    for(int i=0; i<thread_count; i++){
        cout << "Spinning up thread i: " << i << endl;
        threads.emplace_back(DAXPY, i*interval_size, i*(interval_size+1), soa);
    }

    for(int i=0; i<threads.size(); i++){
        threads[i].join();
        cout << "Thread: " << i << " processing finished." << endl;
    }

    cout << "Printing out to example.txt." << endl;

    //print out all results into a file
    ofstream myfile;
    myfile.open ("./examples/example.txt");
    myfile << "Thread count: " << thread_count << " Array size: " << array_size << " Interval size: " << interval_size << endl;
    for(int i=0; i<array_size; i++){
        myfile << "Y2[" << i << "]: " << soa->Y2[i] << " = " << soa->a << " * " << soa->X[i] << " + " << soa->Y1[i] << endl;
    }
    myfile.close();

    cout << "Complete" << endl;
 
    return 0;
}
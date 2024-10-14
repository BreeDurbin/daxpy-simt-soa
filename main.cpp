#include <vector>

using namespace std;

struct SOA_DAXPY(){
    SOA_DAXPY(int n) : a(n), X(n), Y1(n), Y2(n), s(n) {}
    double a;
    vector<double> X;
    vector<double> Y1;
    vector<double> Y2;
    int s;
}

void DAXPY(int s, int e, SOA_DAXPY& soa){
    for(; s<e; s++){
        soa.Y2[s] = (soa.a * soa.X[s]) + soa.Y1[s];
    }
}

int main(int argc, char *argv[])
{
    int array_size = 10000;
    int thread_count = 100;
    int interval_size = thread_count / array_size;
    double rand_lower = 0;
    double rand_upper = 10000;

    cout << "Initializing: thread_count=" << thread_count << " array_size=" << array_size << 
        " rand_lower=" << rand_lower << " rand_upper=" << rand_upper << endl;

    srand(time(nullptr)); // use current time as seed for random generator

    SOA_DAXPY soa = new SOA_DAXPY(n);
    soa.a = rand() % rand_upper;

    for(int i=0; i<array_size; i++){
        soa.X = rand() % rand_upper;
    }

    int interval_size = thread_count / array_size;

    vector<jthread> threads;
    for(int i=0; i<thread_count; i++){
        cout << "Spinning up thread i: " << i << endl;
        threads.push_back(
            jthread{[]{
            int s = i*interval_size;
            int e = s + interval_size; 
            DAXPY(s, e, soa);
            }};
        )
    }

    for(int i=0; i<threads.size(); i++){
        thread.join();
        cout << "Thread: " << i << " processing finished." << endl;
    }

    //print out all results into a file

    cout << "Complete" << endl;
 
    return 0;
}
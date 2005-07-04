
#include <eq/base/barrier.h>
#include <eq/base/clock.h>
#include <eq/base/thread.h>
#include <iostream>

using namespace eqBase;
using namespace std;

#define MAXTHREADS 256

volatile size_t nThreads;
Barrier*        barrier;
Clock           timer;

class Test : public Thread
{
public:
    Test() : Thread( Thread::PTHREAD ) {}
    virtual int run()
        {
            int nLoops = 100000000 / nThreads;
            const bool master = ( barrier->enter( nThreads ) == 0 );
            if( master )
                timer.reset();

            while( nLoops-- )
            {}

            barrier->enter( nThreads );
            if( master )
                cerr << nThreads << " threads, " << timer.getTime() << "ms"
                     << endl;
        }
};

int main( int argc, char **argv )
{
    barrier = new Barrier( Thread::PTHREAD );
    Test threads[MAXTHREADS];

    for( nThreads = MAXTHREADS; nThreads>1; nThreads = nThreads>>1 )
    {
        for( size_t i=0; i<nThreads; i++ )
            threads[i].start();
        for( size_t i=0; i<nThreads; i++ )
            threads[i].join();
    }

    delete barrier;
    return EXIT_SUCCESS;
}


#include "concurrent_value.h"
#include "thread.h"
#include "logging.h"
#include <string>
#include <vector>


struct S
{
     int i_;
     std::string s_;
};

constexpr int count_ = 100'000;

int main()
{
     ThisThread::SetName( "Main" );

     ConcurrentValue< S > v;
     v.Get() = { 0, "Hello Big Safety World!" };

     std::chrono::microseconds d1;
     std::chrono::microseconds d2;
     std::chrono::microseconds d3;
     Thread t1( "1", [ &v, &d1 ]{
               auto t1 = std::chrono::high_resolution_clock::now();
               for ( int i = 0; i < count_; ++i )
               {
                    v.Get()->i_++;
               }
               auto t2 = std::chrono::high_resolution_clock::now();
               d1 = std::chrono::duration_cast< std::chrono::microseconds >( t2 - t1 );
          } );
     Thread t2( "2", [ &v, &d2 ]{
               auto t1 = std::chrono::high_resolution_clock::now();
               int l;
               for ( int i = 0; i < count_; ++i )
               {
                    l = v.Get()->i_;
               }
               LOG_ERROR( "%i", l );
               auto t2 = std::chrono::high_resolution_clock::now();
               d2 = std::chrono::duration_cast< std::chrono::microseconds >( t2 - t1 );
          } );
     Thread t3( "3", [ &v, &d3 ]{
               auto t1 = std::chrono::high_resolution_clock::now();
               for ( int i = 0; i < count_; ++i )
               {
                    v.Get()->i_++;
               }
               auto t2 = std::chrono::high_resolution_clock::now();
               d3 = std::chrono::duration_cast< std::chrono::microseconds >( t2 - t1 );
          } );
     t1.Start();
     t2.Start();
     t3.Start();
     t1.WaitStop();
     t2.WaitStop();
     t3.WaitStop();

     float sec = ( d1.count() + d2.count() + d3.count() ) / 1'000'000.0;
     LOG_INFO( "%i: %.1f ops", v.Get()->i_, count_*3 / sec );

     return 0;
}

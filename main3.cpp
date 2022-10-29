#include "concurrent_value.h"
#include "thread.h"
#include "logging.h"
#include <string>
#include <unordered_map>
#include <atomic>
#include <memory>


constexpr int count_ = 100'000;
std::mutex m;

struct S
{
     int i_;
     bool b_;
};


void Test()
{
     m.lock();
     Test();
}

int main()
{
     ThisThread::SetName( "Main" );

     ConcurrentValue< std::unordered_map< std::string, std::string > > v;
     v.Get()->emplace( "Hello", "World" );

     LOG_INFO( "%s", v.Get()->at( "Hello" ).c_str() );

     std::atomic< S > a;
     a = { 0, false };

     Test();

     return 0;
}

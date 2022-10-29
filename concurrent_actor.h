#pragma once

#include "concurrent_queue.h"
#include "thread.h"
#include <exception>
#include <functional>
#include <future>


class ConcurrentActor
{
private:
     using Func = std::function< void() >;

     struct QueueItem
     {
          Func func;
          std::promise< void > p;
     };

public:
     ConcurrentActor( const std::string& name = "ConcurrentActor", size_t queueMaxSize = 1'000'000 )
          : thread_( name ), queue_( queueMaxSize )
     {
     }

     ~ConcurrentActor()
     {
          Stop();
     }

     void Start()
     {
          thread_.Start( [ this ]{
                    DoExecute();
               } );
     }

     void Stop()
     {
          queue_.Disable();
          thread_.WaitStop();
     }

     std::future< void > Execute( Func func )
     {
          std::promise< void > p;
          auto fut = p.get_future();

          queue_.WaitPush( QueueItem{
                    .func = func,
                    .p = std::move( p )
               } );
          return fut;
     }

protected:
     void DoExecute()
     {
          while ( true )
          {
               QueueItem impl;
               if ( !queue_.WaitPop( impl ) )
               {
                    break;
               }
               try
               {
                    impl.func();
                    impl.p.set_value();
               }
               catch ( const std::exception& )
               {
                    try
                    {
                         impl.p.set_exception( std::current_exception() );
                    }
                    catch ( const std::exception& e )
                    {
                         LOG_ERROR( "%s", e.what() );
                    }
               }
          }
     }

private:
     Thread thread_;
     ConcurrentQueue< QueueItem > queue_;
};

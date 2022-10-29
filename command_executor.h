#pragma once

#include "concurrent_queue.h"
#include "thread.h"
#include <exception>
#include <functional>


class CommandExecutor
{
private:
     using Func = std::function< void() >;

public:
     CommandExecutor( const std::string& name = "CommandExecutor", size_t queueMaxSize = 1'000'000 )
          : thread_( name ), queue_( queueMaxSize )
     {
     }

     ~CommandExecutor()
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

     void Execute( Func func )
     {
          queue_.WaitPush( func );
     }

protected:
     void DoExecute()
     {
          while ( true )
          {
               Func func;
               if ( !queue_.WaitPop( func ) )
               {
                    break;
               }
               try
               {
                    func();
               }
               catch ( const std::exception& e )
               {
                    LOG_ERROR( "%s", e.what() );
               }
          }
     }

private:
     Thread thread_;
     ConcurrentQueue< Func > queue_;
};

#pragma once

#include <chrono>
#include <string>
#include <stdio.h>
#include <unistd.h>


class ThisThread
{
public:
     ThisThread() = delete;
     ThisThread( const ThisThread& ) = delete;
     ThisThread& operator=( const ThisThread& ) = delete;

     static void SetName( const std::string& name )
     {
          GetSetName( name );
     }

     static const char* GetName()
     {
          auto name = GetSetName();
          if ( name[0] )
          {
               return name;
          }
          return "Unknown";
     }

protected:
     static const char* GetSetName( const std::string& name = {} )
     {
          static thread_local std::string threadName_;

          if ( !name.empty() )
          {
               threadName_ = name;
          }
          return threadName_.c_str();
     }
};

#define TIMESTAMP std::chrono::system_clock::now().time_since_epoch().count() / 1'000'000'000.0
#define THREAD_ID gettid()

// printf гарантирует многопоточную корректность вывода сообщений
#define LOG_INFO( fmt, ... ) \
     { fprintf( stdout, "INFO %.3f %s[%i]: " fmt "\n", TIMESTAMP, ThisThread::GetName(), THREAD_ID, ##__VA_ARGS__); fflush( stdout ); }

#define LOG_ERROR( fmt, ... ) \
     fprintf( stderr, "ERROR %.3f %s[%i]: " fmt " [%s:%i]\n", TIMESTAMP, ThisThread::GetName(), THREAD_ID, ##__VA_ARGS__, __FILE__, __LINE__);

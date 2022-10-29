#pragma once

#include "logging.h"
#include <functional>
#include <string>
#include <thread>


/// @brief Класс потока исполнения
class Thread
{
public:
     /// @brief Конструктор
     /// @param name имя потока
     Thread( const std::string& name ) : name_(name)
     {
     }

     /// @brief Конструктор
     /// @param name имя потока
     /// @param f функция исполнения
     Thread( const std::string& name, std::function< void() > f ) :
          name_(name),
          f_( std::move(f) )
     {
     }

     /// @brief Деструктор
     ~Thread()
     {
          WaitStop();
     }

     Thread( Thread&& ) = default;
     Thread& operator=( Thread&& ) = default;

     /// @brief Запустить поток
     void Start()
     {
          WaitStop(); // на случай перезапуска потока, который еще не завершился из-за команды асинхронной остановки
          if ( !f_ )
          {
               throw std::bad_function_call();
          }
          t_ = std::thread( []( std::string name, std::function< void() > f )
               {
                    ThisThread::SetName( name );
                    LOG_INFO( "Thread started" );
                    f();
                    LOG_INFO( "Thread finishes" );
               }, name_, f_ );
     }

     /// @brief Запустить поток
     /// @param f функция исполнения
     void Start( std::function< void() > f )
     {
          f_ = std::move(f);
          Start();
     }

     /// @brief Остановить поток
     void WaitStop()
     {
          if ( t_.joinable() )
          {
               t_.join();
          }
     }

     /// @brief Остановить поток
     /// @tparam Func тип функции остановки
     /// @param func функция остановки потока
     /// @param sync остановить, дожидаясь реальной остановки или нет
     template< typename Func >
     void Stop( Func&& func, bool sync = false )
     {
          func();
          if ( sync )
          {
               WaitStop();
          }
     }

     /// @brief Проверить запущен ли поток
     /// @return true, если запущен или false, если нет
     bool IsStarted() const
     {
          return t_.joinable();
     }

     std::string GetName() const
     {
          return name_;
     }

protected:
     std::string name_;          ///< Имя потока
     std::function< void() > f_; ///< Функция исполнения потока
     std::thread t_;             ///< Объект потока
};

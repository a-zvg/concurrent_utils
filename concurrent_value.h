#pragma once

#include <mutex>


template< typename T >
class ConcurrentValue
{
     class Accessor
     {
          friend ConcurrentValue;
     public:
          ~Accessor()
          {
               m_.unlock();
          }

          Accessor( Accessor&& ) = default;
          Accessor& operator=( Accessor&& ) = default;

          operator const T&() const
          {
               return value_;
          }

          void operator=( const T& value )
          {
               value_ = value;
          }

          T& operator*()
          {
               return value_;
          }

          T* operator->()
          {
               return &value_;
          }

     protected:
          Accessor( T& value, std::mutex& m ) : value_( value ), m_( m )
          {
               m.lock();
          }

     private:
          T& value_;
          std::mutex& m_;
     };

public:
     ConcurrentValue() = default;

     Accessor Get()
     {
          return { value_, m_ };
     }

private:
     T value_{};
     std::mutex m_;
};

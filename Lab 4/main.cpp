
#include "rccregisters.hpp"
#include "gpiocregisters.hpp"
#include "gpioaregisters.hpp"
#include <iostream>
#include "stkregisters.hpp"
#include "tim2registers.hpp"

constexpr std::uint32_t CpuClock = 8'000'000U;
auto delay_ms(std::uint32_t ButPeriod)
{
TIM2::CR1::URS::OverflowEvent::Set();
TIM2::PSC::Set(7999U);
TIM2::ARR::Write(ButPeriod - 1U);
TIM2::SR::UIF::NoInterruptPending::Set();
TIM2::CNT::Write(0);
TIM2::CR1::CEN::Enable::Set();
while (!TIM2::SR::UIF::NoInterruptPending::IsSet() )
      {

      }
      TIM2::CR1::CEN::Disable::Set();
      }

 int main()
    {
      unsigned int ButPeriod = 100U;
      unsigned int LedsPeriod = 500U;
      unsigned int counter = 0;

    bool flag = false;
    RCC::CR::HSEON::On::Set();
    while(!RCC::CR::HSERDY::Ready::IsSet())
    {
    }

        RCC::CFGR::SW::Hse::Set();
    while(!RCC::CFGR::SWS::Hse::IsSet())
    {
    }
        RCC::AHB1ENR::GPIOCEN::Enable::Set();
        RCC::AHB1ENR::GPIOAEN::Enable::Set();
        GPIOC::MODER::MODER5::Output::Set();
        GPIOC::MODER::MODER13::Input::Set();
        GPIOA::MODER::MODER5::Output::Set();
        GPIOC::MODER::MODER9::Output::Set();
        GPIOC::MODER::MODER8::Output::Set();
        RCC::APB1ENR::TIM2EN::Enable::Set();

    for(;;)
    {
 delay_ms(ButPeriod);
    if(GPIOC::IDR::IDR13::Low::IsSet())
    {
      LedsPeriod += 100U;
      counter = 0;
    }
    counter = counter + ButPeriod;
    if(counter >= LedsPeriod)
      {
        if(!flag)
      {
      GPIOC::BSRR::BS8::High::Write();
      GPIOC::BSRR::BS9::High::Write();
      GPIOC::BSRR::BS5::High::Write();
      GPIOA::BSRR::BS5::High::Write();
      flag = true;
    }
    else
    {
      GPIOC::BSRR::BR9::Low::Write();
      GPIOC::BSRR::BR8::Low::Write();
      GPIOC::BSRR::BR5::Low::Write();
      GPIOA::BSRR::BR5::Low::Write();
      flag = false;
    }
    counter = 0;

    }
    }
    return 1;
    }
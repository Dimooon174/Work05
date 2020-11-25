#include "rccregisters.hpp"
#include "gpiocregisters.hpp"
#include "gpioaregisters.hpp"
#include <iostream>
#include "stkregisters.hpp"

constexpr std::uint32_t CpuClock = 8'000'000U;


auto delay_ms(std::uint32_t period)
{

bool result = false;
{
const std::uint32_t timerValue = (CpuClock / 1000) * period - 1U;
STK::LOAD::Write(timerValue);
STK::VAL::Write(0U);
STK::CTRL::ENABLE::Enable::Set();
while(STK::CTRL::COUNTFLAG::NoOverflow::IsSet());
{
}
STK::CTRL::ENABLE::Disable::Set();

result = true;
}
return true;
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

STK::LOAD::Write(CpuClock - 1U);
STK::CTRL::CLKSOURCE::CpuClock::Set();
RCC::AHB1ENR::GPIOAEN::Enable::Set();
RCC::AHB1ENR::GPIOCEN::Enable::Set();
GPIOC::MODER::MODER5::Output::Set();
GPIOC::MODER::MODER13::Input::Set();
GPIOA::MODER::MODER5::Output::Set();
GPIOC::MODER::MODER9::Output::Set();
GPIOC::MODER::MODER8::Output::Set();

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
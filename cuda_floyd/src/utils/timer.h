//
// Created by orest on 10/08/18.
//
#ifndef CUDAFLOYD2_TIMER_H
#define CUDAFLOYD2_TIMER_H

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
#define duration_cast(A) (std::chrono::duration_cast<std::chrono::nanoseconds>(A))
#ifdef _MSC_VER
// Windows
#include <Windows.h>
#else
// Linux
#define time std_time
#include <ctime>
#undef time
#endif

namespace utils
{
#ifdef _MSC_VER
	class Timer
        {
        private:
                LARGE_INTEGER start;
                double frequency;
        public:
                Timer()
                {
                        LARGE_INTEGER freq;
                        ::QueryPerformanceFrequency(&freq);
                        frequency = 1.0 / freq.QuadPart;
                        QueryPerformanceCounter(&start);
                }

                void reset()
                {
                        QueryPerformanceCounter(&start);
                }

                float elapsed()
                {
                        LARGE_INTEGER current;
                        ::QueryPerformanceFrequency(&current);
                        unsigned __int64 cycles = current.QuadPart - start.QuadPart;
                        return (float)(cycles * freequency);
                }
        };
#else

	class Timer
	{
	private:
		Clock::time_point start;
	public:
		Timer()
		{
			start = Clock::now();
		}

		void reset()
		{
			start = Clock::now();
		}

		float elapsed()
		{
			return (float) (duration_cast(Clock::now() - start).count()) / CLOCKS_PER_SEC;
		}
	};

#endif
}

#endif //CUDAFLOYD2_TIMER_H

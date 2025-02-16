#include "util.h"

//--------------------------------------------------------------------

void initGenMT(unsigned long seed)
{
	init_genrand(seed);
}


//--------------------------------------------------------------------

// void initGenSSE(unsigned int seed)
// {
// 	srand_sse(seed);
// }


//--------------------------------------------------------------------

real getRandomNumberMT(real x, real y)
{
	real result;
	
	result = (real) genrand_real1();

	return(x + ((y - x) * result));
}


//--------------------------------------------------------------------

unsigned long getRandomIntMT(int x, int y)
{
	double resultReal;

	resultReal = genrand_real1();

	return((unsigned long) floor(x + ((y - x) * resultReal) + 0.5));
}


//--------------------------------------------------------------------

// real getRandomNumberSSE(real x, real y)
// {
// 	real result;

//     result = rand_sse() * INV_MAX_INT; 

// 	return(x + ((y - x) * result));
// }


//--------------------------------------------------------------------

void printProgressBar(double progress, double elapsedTime, double remainingTime, 
                      long long sampleRays, long long shadowRays, long long indirectRays, 
                      double raysPerSec) 
{
    int barWidth = 50;  // Length of the progress bar
    int pos = static_cast<int>(barWidth * progress);

    // ANSI color codes
    const std::string CYAN = "\033[36m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RESET = "\033[0m";

    // Move cursor up exactly 2 lines and clear both lines
    std::cout << "\033[F\033[F\r";  // Move cursor up two lines

    // Progress Bar Line
    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << CYAN << "=";  // Progress
        else if (i == pos) std::cout << CYAN << ">";
        else std::cout << " ";
    }
    std::cout << RESET << "] "  // Reset color
              << std::fixed << std::setprecision(2) 
              << ((progress >= 1.0) ? GREEN : "")  // Turn green if 100%
              << (progress * 100.0) << "%" << RESET  // Percentage
              << "        \n";  // Extra spaces to clear previous text

    // Stats Line
    std::cout << "Elapsed: " << std::fixed << std::setprecision(1) << elapsedTime << "s | "
              << "Remaining: " << std::fixed << std::setprecision(1) << remainingTime << "s \n"
              << "Rays (Sample: " << sampleRays 
              << ", Shadow: " << shadowRays 
              << ", Indirect: " << indirectRays 
              << ") | " << YELLOW << std::fixed << std::setprecision(2) << raysPerSec << " MRays/s" << RESET
              << "          "  // Extra spaces clear lingering text
              << std::flush; // Force immediate update
}


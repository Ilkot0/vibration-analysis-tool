#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <valarray>

const double PI = 3.14159265358979323846;
const double THRESHOLD = 0.5;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


void fft(CArray& x) {
    const size_t N = x.size();
    if (N <= 1) return;

    CArray even = x[std::slice(0, N / 2, 2)];
    CArray  odd = x[std::slice(1, N / 2, 2)];

    fft(even);
    fft(odd);

    for (size_t k = 0; k < N / 2; ++k) {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
    }
}

int main() {
   
    const int N = 64; 
    std::vector<double> time_domain(N);
    
    std::cout << "--- Machine Vibration Analysis Tool ---\n";
    std::cout << "Collecting data samples...\n";

    for (int i = 0; i < N; ++i) {
       
        double t = (double)i / 1000.0;
        time_domain[i] = 0.4 * sin(2 * PI * 50 * t) + 0.6 * sin(2 * PI * 120 * t);
    }

    
    Complex test[N];
    for (int i = 0; i < N; ++i) test[i] = Complex(time_domain[i], 0.0);
    CArray data(test, N);

    
    fft(data);

   
    bool problemDetected = false;
    std::cout << "\nFrequency Spectrum Analysis:\n";
    std::cout << "----------------------------\n";
    
    for (int i = 0; i < N / 2; ++i) {
        double magnitude = std::abs(data[i]) / N;
        double frequency = i * (1000.0 / N); 

        if (magnitude > 0.1) {
            std::cout << "Freq: " << frequency << " Hz | Mag: " << magnitude;
            
            if (magnitude > THRESHOLD) {
                std::cout << " [!] ALERT: HIGH VIBRATION DETECTED";
                problemDetected = true;
            }
            std::cout << std::endl;
        }
    }

    
    std::cout << "\n----------------------------\n";
    if (problemDetected) {
        std::cout << "STATUS: CRITICAL - Potential machine fault detected.\n";
        std::cout << "Action: Inspect bearings and alignment.\n";
    } else {
        std::cout << "STATUS: NORMAL - Machine operating within parameters.\n";
    }

    return 0;
}

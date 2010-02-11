#ifndef RANDOM_H
#define RANDOM_H
#include <cstdlib>
#include <ctime>
class Random {
  
  public:
    Random() {
      srand( time(0) );
    }
    int NextInt() {
      return rand();
    }
    /** max is exclusive. example: NextInt(2) will give random result of 0 and 1 */
    int NextInt(int max) {
      return NextInt(0, max);
    }
    int NextInt(int min, int max) {
      return ((rand() * (double(max - min) / RAND_MAX)) + min);
    }
    double NextDouble() {
      return ( double(rand()) / double(RAND_MAX) );
    }
    double NextDouble(double max) {
      return NextDouble(0, max);
    }
    double NextDouble(double min, double max) {
      return ( NextDouble() * (max - min) + min );
    }
};

#endif // RANDOM_H
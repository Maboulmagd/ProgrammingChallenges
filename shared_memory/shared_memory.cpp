#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main( int argc, char* argv[] ) {
    if ( argc<3 ) {
        printf( "Usage: map <index> <divisor>\n" );
        return 0;
    }
    int index = ::atoi(argv[1]);
    int divisor  = ::atoi(argv[2]);
  const char *filename = "/tmp/shared.dat";
  int mapsize = 4096;
  int fid = ::open(filename, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
  ftruncate(fid, mapsize);
  void* ptr = ::mmap(nullptr, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fid, 0);
  printf("ptr address: %x\n", ptr);
//   volatile uint32_t *ptr = (volatile uint32_t *)::mmap(nullptr, mapsize, PROT_READ | PROT_WRITE,
//                                    MAP_SHARED, fid, 0);
//   for ( int j=0; j<10; ++j ) {
//       while ( *ptr % divisor != index );
//       uint32_t value = (*ptr)++;
//       printf( "Process %d/%d incremented to %u\n", index, divisor, value );
//   }
  return 0;
}
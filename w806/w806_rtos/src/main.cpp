#include "main.h"


const HeapRegion_t xHeapRegions[] = 
{ 
    { ( uint8_t * ) 0x20020000UL, 0x5000 },
    { NULL, 0 } 
};

int main(void) 
{
  hwInit();
  apInit();

	vPortDefineHeapRegions( xHeapRegions );
  apMain();
  return 0;
}


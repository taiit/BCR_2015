/*
 * main.c
 *
 * Created: 7/20/2015 8:07:28 PM
 *  Author: Vo Huu Tai
 */ 


#include "../lib/inc/hardware_config.h"
#include "../lib/inc/hardware_feature.h"
#include "unit_test/uint_test.h"

#define UNIT_TEST

int main(void)
{
	vInitProgram();
    while(1)
    {
        #ifdef UNIT_TEST
		vUnitTest();
		#endif // UNIT_TEST		
    }
}
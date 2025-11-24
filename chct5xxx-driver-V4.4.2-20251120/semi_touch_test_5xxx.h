#ifndef __TEST_SUPPRT_5xxx__
#define __TEST_SUPPRT_5xxx__
#include "platform.h"
#include "head_def.h"

#if SEMI_TOUCH_FACTORY_TEST_EN
int semi_touch_start_factory_test(char *filebuff, int *lenth);

struct rawdata_th
{
    unsigned int rawdata_min[ARRAY_SIZE_MAX];
    unsigned int rawdata_max[ARRAY_SIZE_MAX];
};

#endif

#endif
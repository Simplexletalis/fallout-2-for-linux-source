#include "stdafx.h"
#include <stdlib.h>
#include <math.h>

extern "C" {
	double ff_d1, ff_d2;
	double CCONV1(stdcall) h_pow() CCONV2(stdcall);
	double CCONV1(stdcall) h_atan2() CCONV2(stdcall);
}

double CCONV1(stdcall) h_pow() {
	return pow(ff_d1, ff_d2);
}

double CCONV1(stdcall) h_atan2() {
	return atan2(ff_d1, ff_d2);
}

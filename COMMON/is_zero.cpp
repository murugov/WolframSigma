#include "is_zero.h"

int is_zero(double x) { return (fabs(x) < 1e-10); }
#include <stdio.h>
#include <string>

#include <nng/nng.h>

void fatal(const std::string& func, int rv)
{
  fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
}

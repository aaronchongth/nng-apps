#ifndef CORE__SRC__UTILITIES_HPP
#define CORE__SRC__UTILITIES_HPP

#include <stdio.h>
#include <string>

#include <nng/nng.h>

void fatal(const std::string& func, int rv)
{
  fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
}

#endif // CORE__SRC__UTILITIES_HPP

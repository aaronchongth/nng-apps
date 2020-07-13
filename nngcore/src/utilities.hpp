#ifndef CORE__SRC__UTILITIES_HPP
#define CORE__SRC__UTILITIES_HPP

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>

#include <nng/nng.h>

void fatal(const std::string& func, int rv)
{
  fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
}

char* date()
{
  time_t now = time(&now);
  struct tm *info = localtime(&now);
  char *text = asctime(info);
  text[strlen(text)-1] = '\0'; // remove '\n'
  return text;
}

#endif // CORE__SRC__UTILITIES_HPP

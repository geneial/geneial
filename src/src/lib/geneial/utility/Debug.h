#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>

/**
 *
 * @brief Print a demangled stack backtrace of the caller function to FILE* out.
 *
 * See https://panthema.net/2008/0901-stacktrace-demangled/
 * This function does not adhere global coding standards,
 * no namespace is intended as this function should only be used when debugging
 *
 */
void print_stacktrace(FILE *out = stderr);

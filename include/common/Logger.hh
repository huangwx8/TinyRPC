#pragma once

#include <stdarg.h>
#include <common/Logger.hh>

extern FILE* logfile;

void start_log(const char* filename);

void log_dev(const char *format, ...);
void log_err(const char *format, ...);

void stop_log();

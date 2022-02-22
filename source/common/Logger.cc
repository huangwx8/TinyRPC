#include <common/Logger.hh>

FILE* logfile = nullptr;

void start_log(const char* filename)
{
    logfile = fopen(filename, "w+");
}

void log_dev(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    if (logfile)
    {
        vfprintf(logfile, format, args);
        fflush(logfile);
    }
    
    va_end(args);
}

void log_err(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_dev("Disastrous error! ");
    log_dev(format, args);

    va_end(args);
}

void stop_log()
{
    fclose(logfile);
}

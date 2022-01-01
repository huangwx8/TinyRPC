#pragma once

/** Parse command arguments with given flag name as string */
const char* parse_str(int argc, char* argv[], const char* flag);

/** Parse command arguments with given flag name as integer */
int parse_int(int argc, char* argv[], const char* flag);

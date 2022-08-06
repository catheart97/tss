#pragma once

#include <stdio.h>
#include <stdlib.h>

#define FILELIB_ERROR_NONE 0
#define FILELIB_NO_INPUT 42
#define FILELIB_NO_OUTPUT -1

typedef struct
{
    FILE * in_ = NULL;
    FILE * out_ = NULL;
    int error = FILELIB_ERROR_NONE;
} file_state;

extern "C++"
{
// #include <LocalStorage.hpp>
// LocalStorage<file_state> fstate;
// #define STATE (*(fstate))
thread_local file_state STATE;
}

namespace FileLib
{

void open_file_in(const char * filepath)
{
    STATE.in_ = fopen(filepath, "r");
    if (!STATE.in_) STATE.error = FILELIB_NO_INPUT;
}

void open_file_out(const char * filepath) { STATE.out_ = fopen(filepath, "w"); }

char * read_line()
{
    if (STATE.in_)
    {
        char * line = NULL;
        size_t length = 0;
        if (getline(&line, &length, STATE.in_) != -1) // NOTE: ownership on line to user
            return line;
        else
            return NULL;
    }
    else
    {
        STATE.error = FILELIB_NO_INPUT;
        return NULL;
    }
}

void write_line(const char * line, size_t length)
{
    if (STATE.out_)
    {
        fwrite((void *)line, sizeof(char), length, STATE.out_);
    }
    else
    {
        STATE.error = FILELIB_NO_OUTPUT;
    }
}

void close()
{
    if (STATE.in_) fclose(STATE.in_);

    if (STATE.out_) fclose(STATE.out_);
}

} // namespace FileLib
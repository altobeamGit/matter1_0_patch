/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <lib/shell/Engine.h>
#include <lib/shell/streamer.h>

#include <lib/core/CHIPError.h>
#include <stdio.h>
#include <string.h>

#include "atbm_general.h"

extern "C" void Console_Putc(unsigned char Data);

namespace chip {
namespace Shell {

static int chip_command_handler(int argc, char ** argv)
{
    CHIP_ERROR err;

    if (argc > 0)
    {
        err = Engine::Root().ExecCommand(argc - 1, argv + 1);
    }
    else
    {
        err = CHIP_ERROR_INVALID_ARGUMENT;
    }
    return static_cast<int>(err.AsInteger());
}

int streamer_atbm_init(streamer_t * streamer)
{
    atbm_console_matter_cmd_register(chip_command_handler);
    return 0;
}

ssize_t streamer_atbm_read(streamer_t * streamer, char * buf, size_t len)
{
    return 0;
}

ssize_t streamer_atbm_write(streamer_t * streamer, const char * buf, size_t len)
{
    size_t i = 0;
    char *p = (char *)buf;
    while (i++ < len){
    	Console_Putc(*p++);
    }
    return 0;
}

static streamer_t streamer_stdio = {
    .init_cb  = streamer_atbm_init,
    .read_cb  = streamer_atbm_read,
    .write_cb = streamer_atbm_write,
};

streamer_t * streamer_get()
{
    return &streamer_stdio;
}

} // namespace Shell
} // namespace chip

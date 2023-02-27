/*
 * Copyright (C) 2009 Niek Linnenbank
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "Wait.h"
#include "lib/libposix/sys/wait.h"

Wait::Wait(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Stop executing until the specified process finished");
    parser().registerPositional("PID", "Stop executing until the process with this ID finished executing");
}

Wait::~Wait()
{
}

Wait::Result Wait::exec()
{
    //This is all of the sleep specific information that we will need to change
    //I am leaving it here as frame of reference

    int pid = 0;

        // Convert input to pid (using seconds like sleep)
        if ((pid = atoi(arguments().get("PID"))) <= 0)
        {
            ERROR("invalid wait pid `" << arguments().get("SECONDS") << "'");
            return InvalidArgument;
        }

        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            ERROR("failed to wait: " << strerror(errno));
            return IOError;
        }

        // Done
        return Success;

    /*
    int sec = 0;

    // Convert input to seconds
    if ((sec = atoi(arguments().get("SECONDS"))) <= 0)
    {
        ERROR("invalid wait time `" << arguments().get("SECONDS") << "'");
        return InvalidArgument;
    }

    // Sleep now
    if (wait(sec) != 0)
    {
        ERROR("failed to sleep: " << strerror(errno));
        return IOError;
    }
    */

    // Done
    return Success;
}

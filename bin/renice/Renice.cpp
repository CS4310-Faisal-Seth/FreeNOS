/*
 * Copyright (C) 2015 Niek Linnenbank
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


#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <FreeNOS/System.h>
#include <FreeNOS/Kernel.h>
#include <FreeNOS/Config.h>
#include <FreeNOS/Process.h>
#include <FreeNOS/ProcessEvent.h>
#include <FreeNOS/ProcessManager.h>
#include <Log.h>
#include <Types.h>

#include <Types.h>
#include <Macros.h>
#include <stdio.h>
#include <unistd.h>
#include <ProcessClient.h>

#include "Renice.h"

Renice::Renice(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Output system process list");

    parser().registerFlag('n', "CHANGE_PRIORITY", "Indicates changing the priority of a process");
    parser().registerPositional("PRIORITY", "priority we want to set to");
    parser().registerPositional("PID", "process we want to change");
}

Renice::Result Renice::exec()
{
    //Only checks for -n
    const char *priority = arguments().get("CHANGE_PRIORITY");
    if (priority != NULL){

        //Pass in the arguments from command line
        int PID = 0;
        int newpriority = 0;

        PID = atoi(arguments().get("PID"));
        newpriority = atoi(arguments().get("PRIORITY"));

        //input validation
        if(newpriority < 1 || newpriority > 5){
            ERROR("invalid priority `" << arguments().get("PRIORITY") << "'");
            return InvalidArgument;
        }

        ProcessClient::Info info;

        //Calls the function in processCtl using the output to pass in the newpriority
        const API::Result result = ProcessCtl(PID, ChangePriority, (Address) &info.kernelState, newpriority);

        return Success;
    }

    //returns success if not -n argument
    return Success;

}

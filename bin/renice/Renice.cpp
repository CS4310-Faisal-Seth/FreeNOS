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
    parser().registerPositional("PRIORITY", "designated seconds to stop for");
}

Renice::Result Renice::exec()
{
     const ProcessClient process;
    //All of this is getting changed
    int newpriority = 0;

    newpriority = atoi(arguments().get("PRIORITY"));

    //input validation
    if(newpriority < 1 || newpriority > 5){
        ERROR("invalid priority `" << arguments().get("PRIORITY") << "'");
        return InvalidArgument;
    }


    //now the actual implementation
    //ProcessClient::Info info;

    const API::Result result = ProcessCtl(pid, Renice, newpriority);
    /*if (result == ProcessClient::Success)
    {

    }else{
        ERROR("PID does not exisit");
        return InvalidArgument;
    }*/

    //WTF is this




    return Success;
}

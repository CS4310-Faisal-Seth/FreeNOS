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
    parser().registerPositional("PID", "designated seconds to stop for");
    parser().registerPositional("PRIORITY", "designated seconds to stop for");
}

Renice::Result Renice::exec()
{
     //const ProcessClient process;
    //All of this is getting changed
    int PID = 0;
    int newpriority = 0;

    PID = atoi(arguments().get("PRIORITY"));
    newpriority = atoi(arguments().get("PRIORITY"));

    //input validation
    if(newpriority < 1 || newpriority > 5){
        ERROR("invalid priority `" << arguments().get("PRIORITY") << "'");
        return InvalidArgument;
    }


    //now the actual implementation

     const ProcessClient process;

     ProcessClient::Info info;
                                                     //process is a process client
     //const ProcessClient::Result result = process.processInfo(pid, info);

     const Arch::MemoryMap map;
     const Memory::Range range = map.range(MemoryMap::UserArgs);
     char cmd[128];


     //const API::Result result = ProcessCtl(pid, InfoPID, (Address) &info.kernelState);
     //const ProcessID procID,
                                         //const ProcessOperation action,
                                         //const Address addr,
                                         //const Address output)
     Process *proc = ZERO;
     ProcessInfo *info = (ProcessInfo *) (address);
     ProcessManager *procs = Kernel::instance()->getProcessManager();
     Timer *timer;

     DEBUG("#" << procs->current()->getID() << " " << action << " -> " << PID << " (" << (address) << ")");

     // Does the target process exist?
     if(action != GetPID && action != Spawn)
     {
         if (PID == SELF)
             proc = procs->current();    //current process
         else if (!(proc = procs->get(PID)))
             return InvalidArgument;
     }
     proc -> changePriority(newpriority);






    return Success;
}

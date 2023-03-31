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
    parser().registerPositional("PID", "process we want to change");
    parser().registerPositional("PRIORITY", "priority we want to set to");
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

    //Make sure that the process exisits

    ProcessID procID = PID;

    const ProcessClient process;

    ProcessClient::Info info;

    const API::Result result = ProcessCtl(procID, IncreasePriority, (Address) &info.kernelState);

/*
    Kernel k = Kernel::instance();

    // Does the target process exist?
    const Arch::MemoryMap map;
    Process *proc = ZERO;
    //ProcessInfo *info = (ProcessInfo *) addr;
    ProcessManager *procs = Kernel::instance()->getProcessManager();
    //Timer *timer;

    if (procID == SELF)
        proc = procs->current();    //current process
    else if (!(proc = procs->get(procID))){
        ERROR("Couldnt find process");
        return InvalidArgument;
    }

    else    {
        proc = procs->get(procID);
    }

    proc->setPriority(newpriority);

*/
    //Call the process's setPriority(newpriority)



    //now the actual implementation

     //He says we should follow how ps is structured

     //ps code below
        /*
            const ProcessClient process;

            // Loop processes
                                            //Using process client to find the number of processes
            for (ProcessID pid = 0; pid < ProcessClient::MaximumProcesses; pid++)
            {
                ProcessClient::Info info;
                                                        //process is a process client
                const ProcessClient::Result result = process.processInfo(pid, info);
                if (result == ProcessClient::Success)
                {
                    //info is a struct in process client
                    //prints stuff from info and info.kernelstate
                }
            }
        */

    /*process.processInfo makes sure that the process exisits then runs process CTL on it
        it uses this ProcessCtl(pid, InfoPID, (Address) &info.kernelState);, but process CTL takes in
          ProcessCtlHandler(const ProcessID procID, const ProcessOperation action, const Address addr, const Address output)
        I think addr and output are initialized as 0 through a weird code loop?
    processctl then does a bunch of stuff I dont understand, but is able to call the processes functions.
        I think we have two options
            1. Figure out how process ctl works to bypass going through all these classes and access the process
            2. call process ctl and add an increment and decrement action to ctl, then loop that by the difference between the new priority and current priority

    */

    return Success;
}

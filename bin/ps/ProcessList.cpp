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

//process Ctrl kernel API
//process Client lib libruntime

#include <Types.h>
#include <Macros.h>
#include <stdio.h>
#include <unistd.h>
#include <ProcessClient.h>
#include "ProcessList.h"

ProcessList::ProcessList(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().registerFlag('l', "priority", "Print out priorities");
    //parser().registerOption("-l", "--priority", "Output priorities");
    parser().setDescription("Output system process list");
    //parser().registerPositional("Priorities", "List only priority levels");
}

ProcessList::Result ProcessList::exec()
{
    const ProcessClient process;
    String out;


    const char *priority = arguments().get("priority");
    // Print header
    if (priority != NULL){
        out << "ID  Priority  PARENT  USER GROUP STATUS     CMD\r\n";
    } else {
        out << "ID  PARENT  USER GROUP STATUS     CMD\r\n";
    }
    //out << "ID  Priority  PARENT  USER GROUP STATUS     CMD\r\n";


    //parser().addFlag(priority, 'l', "priority", "prints the level of priority");


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
            DEBUG("PID " << pid << " state = " << *info.textState);

            // Output a line
            char line[128];

                if (priority != NULL){
                    snprintf(line, sizeof(line),
                                    "%3d %7d %7d %4d %5d %10s %32s\r\n",
                                     pid, info.kernelState.priority, info.kernelState.parent,
                                     0, 0, *info.textState, *info.command); //add one more for priority
                                    out << line;
                } else {
                    snprintf(line, sizeof(line),
                                    "%3d %7d %4d %5d %10s %32s\r\n",
                                     pid, info.kernelState.parent,
                                     0, 0, *info.textState, *info.command); //add one more for priority
                                    out << line;
                }

                /*snprintf(line, sizeof(line),
                "%3d %7d %7d %4d %5d %10s %32s\r\n",
                 pid, info.kernelState.priority, info.kernelState.parent,
                 0, 0, *info.textState, *info.command); //add one more for priority
                out << line;
                */

        }
    }

    // Output the table
    write(1, *out, out.length());
    return Success;
}

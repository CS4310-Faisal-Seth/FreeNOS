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

#include <Log.h>
#include "Kernel.h"
#include "Scheduler.h"

Scheduler::Scheduler()
{
    DEBUG("");
}

Scheduler::Result Scheduler::enqueue(Process *proc, bool ignoreState)
{
    if (proc->getState() != Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " not in Ready state");
        return InvalidArgument;
    }
    m_queues[(proc->getPriority())-1].push(proc);

    return Success;
}

//When changing priority we remove it from the old queue (if it was ready) change the priority then enqueue it to the new queue
Scheduler::Result Scheduler::syncQueues(Process *proc, int newPriority) {

     Process *savedProc = proc;
    if(proc->getState() == Process::Ready) {
        dequeue(proc, true);
        savedProc->setPriority(newPriority);
        enqueue(savedProc, true);
    } else {
        savedProc->setPriority(newPriority);
    }


     return Success;
}

Scheduler::Result Scheduler::dequeue(Process *proc, bool ignoreState)
{
    if (proc->getState() == Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " is in Ready state");
        return InvalidArgument;
    }

    int currentProcId = proc->getPriority();
    int ProcId = proc->getID();

    Size count = m_queues[currentProcId-1].count();

    // Traverse the Queue to remove the Process
    for (Size i = 0; i < count; i++)
    {
        Process *p = m_queues[currentProcId-1].pop();

        if (p == proc)
            return Success;
        else
            m_queues[currentProcId-1].push(p);
    }

    FATAL("process ID " << proc->getID() << " is not in the schedule, currentProcId " << currentProcId);
    return InvalidArgument;
}

//Currently the Scheduler will just select the highest priority to run
Process * Scheduler::select()
{

    for(int i = 4; i >= 0; i--){

        if (m_queues[i].count() > 0)
            {
                Process *p = m_queues[i].pop();
                m_queues[i].push(p);

                return p;
            }

    }

    return (Process *) NULL;
}

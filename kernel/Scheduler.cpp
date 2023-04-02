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

//Size Scheduler::count() const
//{
//    return m_queue.count();
//}

Scheduler::Result Scheduler::enqueue(Process *proc, bool ignoreState)
{
    if (proc->getState() != Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " not in Ready state");
        return InvalidArgument;
    }

    if (proc->getPriority() == 1) {
        m_queue1.push(proc);
    } else if (proc->getPriority() == 2) {
        m_queue2.push(proc);
    } else if (proc->getPriority() == 3) {
        m_queue3.push(proc);
    } else if (proc->getPriority() == 4) {
        m_queue4.push(proc);
    } else if (proc->getPriority() == 5) {
        m_queue5.push(proc);
    } else {
        ERROR("process ID " << proc->getID() << " has invalid priority level");
            return InvalidArgument;
    }

    return Success;
}

// At this point, assume proc has new priority
Scheduler::Result Scheduler::syncQueues(Process *proc, int oldPriority, int newPriority) {

    Process *foundProc;

    if (oldPriority == 1) {
       Size count = m_queue1.count();

       // Traverse the Queue to remove the Process
       for (Size i = 0; i < count; i++)
        {
          Process *p = m_queue1.pop();
           if (p == proc)
              foundProc = p;
           else
              m_queue1.push(p);
       }
    } else if (oldPriority == 2) {
       Size count = m_queue2.count();

       // Traverse the Queue to remove the Process
       for (Size i = 0; i < count; i++)
       {
          Process *p = m_queue2.pop();
          if (p == proc)
             foundProc = p;
          else
             m_queue2.push(p);
       };
    } else if (oldPriority == 3) {
        Size count = m_queue3.count();

        // Traverse the Queue to remove the Process
       for (Size i = 0; i < count; i++)
       {
          Process *p = m_queue3.pop();
          if (p == proc)
             foundProc = p;
          else
             m_queue3.push(p);
       };
    } else if (oldPriority == 4) {
       Size count = m_queue4.count();

       // Traverse the Queue to remove the Process
       for (Size i = 0; i < count; i++)
       {
          Process *p = m_queue4.pop();
          if (p == proc)
              foundProc = p;
          else
             m_queue4.push(p);
       };
    } else if (oldPriority == 5) {
       Size count = m_queue5.count();

       // Traverse the Queue to remove the Process
       for (Size i = 0; i < count; i++)
       {
          Process *p = m_queue5.pop();
          if (p == proc)
             foundProc = p;
          else
             m_queue5.push(p);
        };
     }

     // At this point, foundProc should be found
     if (!foundProc) {
        ERROR("process ID " << proc->getID() << " not found, old prio: " << oldPriority);
        return InvalidArgument;
     }

     // Proc has its new priority, so enqueue will add appropriately
     foundProc->setPriority(newPriority);
     return this->enqueue(foundProc, true);
}

Scheduler::Result Scheduler::dequeue(Process *proc, bool ignoreState)
{
    if (proc->getState() == Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " is in Ready state");
        return InvalidArgument;
    }

    int currentProcId = proc->getPriority();

    if (currentProcId == 1) {
        Size count = m_queue1.count();

        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process *p = m_queue1.pop();

            if (p == proc)
                return Success;
            else
                m_queue1.push(p);
        }
    }

    else if (currentProcId == 2) {
        Size count = m_queue2.count();

        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process *p = m_queue2.pop();

            if (p == proc)
                return Success;
            else
                m_queue2.push(p);
        }
    }

    else if (currentProcId == 3) {
        Size count = m_queue3.count();

        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process *p = m_queue3.pop();

            if (p == proc)
                return Success;
            else
                m_queue3.push(p);
        }

    }

    else if (currentProcId == 4) {
        Size count = m_queue4.count();

        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process *p = m_queue4.pop();

            if (p == proc)
                return Success;
            else
                m_queue4.push(p);
        }

    }

    else if (currentProcId == 5) {
        Size count = m_queue5.count();

        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process *p = m_queue5.pop();

            if (p == proc)
                return Success;
            else
                m_queue5.push(p);
        }

    }

    FATAL("process ID " << proc->getID() << " is not in the schedule, currentProcId " << currentProcId);
    return InvalidArgument;
}

//Major part to the second part
Process * Scheduler::select()
{
    if (m_queue5.count() > 0)
    {
        Process *p = m_queue5.pop();
        m_queue5.push(p);

        return p;
    }

    else if (m_queue4.count() > 0)
    {
        Process *p = m_queue4.pop();
        m_queue4.push(p);

        return p;
    }

    else if (m_queue3.count() > 0)
    {
        Process *p = m_queue3.pop();
        m_queue3.push(p);

        return p;
    }

    else if (m_queue2.count() > 0)
    {
        Process *p = m_queue2.pop();
        m_queue2.push(p);

        return p;
    }

    else if (m_queue1.count() > 0)
    {
        Process *p = m_queue1.pop();
        m_queue1.push(p);

        return p;
    }

    return (Process *) NULL;
}

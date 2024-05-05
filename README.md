# Ice-Cream-Factory-Problem

OPERATING SYSTEM PROJECT REPORT

Group Members:
- Ali Jafar 
- Rafay Ahmad 
- Ahmad Ali Ansari

Requirements:

- COURSE: OPERATING SYSTEM
- OS: LINUX 6.5.1
- LANGUAGE: C

Objectives:
- Create a system call to monitor thread activities
- Integrate the system call into the ice cream factory project
- Use synchronization to make sure threads function effectively
- Effectively Handle Deadlocks

- For Creating the system calls and configurating the kernal we followed the following link:
  [YOUTUBE LINK]: https://youtu.be/85qGRoaOkhQ?feature=shared

System Calls:
The Project Consist of 5 System Calls:
- Ice Cream Count
- Number of Flavors
- Out of Bounds
- Revenue
- Shop Closure
  
-  Ice Cream Count:
Ice cream count system calls takes 4 arguments Prints the amount of flavors left at the end of the day
- Number of Flavors:
Number of Flavors prints the prompt when there are no more flavors available.
- Out of Bounds:
Check if the number of tickets provided by the user is within the set limits.
- Revenue:
Prints all the Statistics of the day on closure i.e number of customers, total revenue etc
- Shop Closure:
Indicates that the shop is now closed.

Synchronization and Deadlocks:
We Implemented Semaphores to counter deadlocks and Synchronize all the threads to avoid race condition problem.

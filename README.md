# factoryscheduling
This program will schedule a machine works in a factory. Program will read three text files and create an output file for your schedule result.

> Below you can find the description and format of each file;
## Operations.txt : 
This file shows the operations can be done by the machine. This file includes two 
information on each row. First one is the operation code and the second one is its speed (work can be 
done per minute).
#### Format: (OperationCode;Amount\n\r)
Ex:
* 1;10
* 2;15
* 3;100
* 4;50
> First row means that this machine can do operation code 1 and its speed is 10m per minute.

## SetupDuration.txt : 
This file shows the setup durations for operation changes. Starting operation has zero 
setup duration. However when you pass the machine from one operation to another one it takes this setup 
time to start second operation. The setup times are equal in two way since each operation couple has one 
record in this file.
#### Format: (OperationCode1;OperationCode2;SetupDuration\n\r)
Ex:
* 1;2;10
* 1;3;10
* 1;4;12
* 2;3;5
* 2;4;3
* 3;4;20
> First row means that if you pass from operation1 to operation2 it takes 10 minutes to prepare the machine.

## Orders.txt : 
This files include the clients’ orders that must be schedule on our machine. We can simple 
schedule all the work one by one but we want to increase the machine performance by decreasing the 
setup durations. On the other hand each order has a deadline to be finished. So we must conform the 
deadlines and increase the machine’s performance. This file gives us the order code, amount of the order 
in meters, the operation type and the deadline limit in minutes.
#### Format: (OrderCode;AmountOfWork;OperationCode;Deadline\n\r)
Ex:
* 1;100;2;300
* 2;100;3;300
* 3;200;1;500
* 4;200;1;500
* 5;100;2;1000
* 6;100;2;1000
* 7;100;4;1000

## The Output of the Program
Your program will create the scheduling output as a text mode file and shows the total duration of the 
schedule on the screen. The description for the output file is below;
## Schedule.txt : 
Includes your schedule result like the input files format.
#### Format: (ScheduleTime;OperationCode;OrderCode;AmountOfWork;SetupOverhead\n\r)
Ex:
* 0;2;1;100;0
* 7;2;6;100;0
* 14;2;5;100;0
* 26;3;2;100;5
* 37;1;4;200;10
* 57;1;3;200;0
* 89;4;7;100;12


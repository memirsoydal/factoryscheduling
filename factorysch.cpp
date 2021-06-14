#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <cassert>
#include <cmath>
#include <algorithm>
using namespace std;
class Operation
{
public:
    int OperationCode;
    int Amount;
    Operation() {}
    Operation(int opCode, int amount) : OperationCode(opCode), Amount(amount) {}
};

class Order
{
public:
    int OrderCode;
    int AmountOfWork;
    int OperationCode;
    int Deadline;
    Order() {}
    Order(int ordCod, int AOW, int OPCode, int dedline) : OrderCode(ordCod), AmountOfWork(AOW), OperationCode(OPCode), Deadline(dedline) {}
};
class Schedule
{
public:
    long ScheduleTime;
    int OperationCode;
    int OrderCode;
    int AmountOfWork;
    int SetupOverhead;
    Schedule() {}
    Schedule(long ST, int OPC, int ORDC, int AOW, int STPOH) : ScheduleTime(ST), OperationCode(OPC), OrderCode(ORDC), AmountOfWork(AOW), SetupOverhead(STPOH) {}
};
void STIP(string str, vector<int> &array)
{
    int temp = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ';')
        {
            array.push_back(temp);
            temp = 0;
        }
        else
        {
            temp = (temp * 10) + (str[i] - 48);
        }
    }
    array.push_back(temp);
}
void readOP(map<int, Operation> &op)
{
    fstream infile;
    string data;
    vector<int> datainInt;
    infile.open("Operations.txt");
    if (!infile.is_open())
    {
        cout << "Unable to open the Operations.";
        assert(false);
    }
    while (!infile.eof())
    {
        getline(infile, data);
        datainInt.clear();
        STIP(data, datainInt);
        Operation opr(datainInt[0], datainInt[1]);
        op[datainInt[0]] = opr;
    }
    infile.close();
}
void readSCH(map<int, map<int, int>> &op)
{
    fstream infile;
    string data;
    vector<int> datainInt;
    infile.open("SetupDuration.txt");
    if (!infile.is_open())
    {
        cout << "Unable to open the SetupDuration.";
        assert(false);
    }
    while (!infile.eof())
    {
        getline(infile, data);
        datainInt.clear();
        STIP(data, datainInt);
        op[datainInt[0]].insert(make_pair(datainInt[1], datainInt[2]));
    }
    infile.close();
}
void readORD(map<int, Order> &op)
{
    fstream infile;
    string data;
    vector<int> datainInt;
    infile.open("Orders.txt");
    if (!infile.is_open())
    {
        cout << "Unable to open the Orders.";
        assert(false);
    }
    while (!infile.eof())
    {
        getline(infile, data);
        datainInt.clear();
        STIP(data, datainInt);
        Order opr(datainInt[0], datainInt[1], datainInt[2], datainInt[3]);
        op[datainInt[0]] = opr;
    }
    infile.close();
}

struct less_than_key
{
    inline bool operator()(const Order &struct1, const Order &struct2)
    {
        return (struct1.Deadline > struct2.Deadline);
    }
};
void Machines(vector<vector<Order>> &queues, map<int, Order> orders, map<int, Operation> operations)
{

    map<int, Order>::iterator MORD;
    map<int, Operation>::iterator MOP;
    vector<vector<Order>>::iterator VORD;

    queues.resize(operations.size());

    for (MORD = orders.begin(); MORD != orders.end(); ++MORD)
        queues[MORD->second.OperationCode - 1].push_back(MORD->second);
    for (VORD = queues.begin(); VORD != queues.end(); ++VORD)
        sort(VORD->begin(), VORD->end(), less_than_key());
}

void scheduling(map<int, Operation> op, map<int, map<int, int>> stpDrtn, map<int, Order> ord, vector<vector<Order>> &queues, vector<Schedule> &sch)
{
    double Sndtime = 0;
    int setupOverhead = 0, prevOP = 0, time = 0, index = 0;
    long tick = 0;
    for (int i = 0, tempDeadline = 0, x = 0; i < op.size(); ++i)
    {
        if (queues[i].size() != 0)
        {
            tempDeadline = ceil((double)queues[i].back().AmountOfWork / (double)(op.find(queues[i].back().OperationCode)->second.Amount));
            x = (x == 0) ? tempDeadline - queues[i].back().Deadline : x;
            if (tempDeadline - queues[i].back().Deadline >= x)
            {
                index = i;
                prevOP = index;
                x = tempDeadline - queues[i].back().Deadline;
            }
        }
    }

    time = ceil((double)queues[index].back().AmountOfWork / (double)(op.find(queues[index].back().OperationCode)->second.Amount));
    Schedule scheduleElements(tick, queues[index].back().OperationCode, queues[index].back().OrderCode, queues[index].back().AmountOfWork, setupOverhead);
    tick += (time + setupOverhead);
    sch.push_back(scheduleElements);
    queues[index].pop_back();

    for (int j = 1; j < ord.size(); ++j)
    {
        if (queues[index].size() != 0)
        {
            for (int i = 0, work = 0, x = 2147483647; i < op.size(); ++i)
            {
                if (queues[i].size() != 0)
                {
                    work = ceil((double)queues[i].back().AmountOfWork / (double)(op.find(queues[i].back().OperationCode)->second.Amount));
                    setupOverhead = (prevOP == i) ? 0 : (stpDrtn[prevOP + 1][queues[i].back().OperationCode] == NULL) ? stpDrtn[queues[i].back().OperationCode][prevOP + 1]
                                                                                                                      : stpDrtn[prevOP + 1][queues[i].back().OperationCode];

                    if ((work + setupOverhead) >= (queues[i].back().Deadline - tick) / (op.size() * op.size() / 2) && (queues[i].back().Deadline - work - tick - setupOverhead) < x)
                    {
                        index = i;
                        x = (queues[i].back().Deadline - work - tick - setupOverhead);
                    }
                }
            }
        }
        else
        {
            for (int i = 0, work = 0, x = 2147483647; i < op.size(); ++i)
            {
                if (queues[i].size() != 0)
                {
                    work = ceil((double)queues[i].back().AmountOfWork / (double)(op.find(queues[i].back().OperationCode)->second.Amount));
                    setupOverhead = (prevOP == i) ? 0 : (stpDrtn[prevOP + 1][queues[i].back().OperationCode] == NULL) ? stpDrtn[queues[i].back().OperationCode][prevOP + 1]
                                                                                                                      : stpDrtn[prevOP + 1][queues[i].back().OperationCode];

                    if (queues[i].back().Deadline - work - setupOverhead < x)
                    {
                        index = i;
                        x = queues[i].back().Deadline - work - setupOverhead;
                    }
                }
            }
        }

        time = ceil((double)queues[index].back().AmountOfWork / (double)(op.find(queues[index].back().OperationCode)->second.Amount));
        setupOverhead = (prevOP == index) ? 0 : (stpDrtn[prevOP + 1][queues[index].back().OperationCode] == NULL) ? stpDrtn[queues[index].back().OperationCode][prevOP + 1]
                                                                                                                  : stpDrtn[prevOP + 1][queues[index].back().OperationCode];
        tick += setupOverhead;
        Schedule scheduleElements(tick, queues[index].back().OperationCode, queues[index].back().OrderCode, queues[index].back().AmountOfWork, setupOverhead);
        tick += time;
        sch.push_back(scheduleElements);
        queues[index].pop_back();
        prevOP = index;
    }
}

void output(vector<Schedule> &sch)
{
    ofstream outfile("Schedule.txt");
    vector<Schedule>::iterator it;
    for (it = sch.begin(); it != sch.end(); ++it)
    {
        outfile << it->ScheduleTime << ";" << it->OperationCode << ";" << it->OrderCode << ";" << it->AmountOfWork << ";" << it->SetupOverhead << endl;
    }
}

int main()
{
    int n = 0;
    map<int, Operation> operations;
    map<int, map<int, int>> setupDurations;
    map<int, Order> orders;
    vector<Schedule> schedules;
    vector<vector<Order>> queues;
    readOP(operations);
    readSCH(setupDurations);
    readORD(orders);
    Machines(queues, orders, operations);
    scheduling(operations, setupDurations, orders, queues, schedules);
    output(schedules);
    return 0;
}
#include "myentry.hpp"
#include "myentryindex.hpp"
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>

int main(int argc, char* argv[])
{   //argv: 1 = input, 2 = startingline, 3 = endingline, 4 = output, 5 = mode, 6 = field
    double t1, t2, cpu_time;
    struct tms tb1, tb2;
    double ticspersec;

    if(argc != 7)   //check the number of arguments
        cout << "Wrong input to sorter (1)" << endl;
    else
    {
        stringstream ss;
        int startingline, endingline, mode, field;

        ss << argv[2];
        if(!(ss >> startingline))   //check if the input is correct
            cout << "wrong input to sorter (2)" << endl;
        else
        {
            ss.str("");
            ss.clear();
            ss << argv[3];

            if(!(ss >> endingline)) //check if the input is correct
                cout << "wrong input to sorter (3)" << endl;
            else
            {
                ss.str("");
                ss.clear();
                ss << argv[5];

                if(!(ss >> mode) || (mode != 1 && mode != 2))   //check if the input is correct
                    cout << "wrong input to sorter (4)" << endl;
                else
                {
                    ss.str("");
                    ss.clear();
                    ss << argv[6];

                    if(!(ss >> field) || field < 0 || field > 3)    //check if the input is correct
                        cout << "wrong input to sorter (5)" << endl;
                    else
                    {
                        ss.str("");
                        ss.clear();

                        FILE* inp;
                        FILE* outp;

                        outp = fopen(argv[4], "w");     //open the output fifo
                        inp = fopen(argv[1], "r");      //open the input file

                        if(inp != NULL && outp != NULL) //check if both files are open
                        {
                            EntryIndex ei(endingline - startingline);   //create an index of the proper size
                            char buff[100];
                            int counter = 0;                    //line counter

                            while(fgets(buff, 100, inp) > 0)    //while there are still lines to read
                            {
                                counter++;                      //add to the line counter
                                if(counter > startingline)      //if you are in the lines that are allocated to you
                                {
                                    ss << buff;                 //put the line you just read in the stream

                                    string idnumber;
                                    string surname;
                                    string name;
                                    string inc;

                                    unsigned long id;
                                    unsigned long income;

                                    getline(ss, idnumber, ' ');     //get the id in string form
                                    id = atol(idnumber.c_str());    //get the id in long form
                                    getline(ss, surname, ' ');      //get the surname
                                    getline(ss, name, ' ');         //get the name
                                    getline(ss, inc);               //get the income in string form
                                    income = atol(inc.c_str());     //get the income in long form

                                    ss.str("");
                                    ss.clear();

                                    Entry* e = new Entry(id, surname, name, income);    //create a new entry
                                    ei.insertEntry(e);                  //insert it in the index
                                }
                                if(counter == endingline)           //once you reach the end of your allocated area stop reading
                                    break;
                            }

                            ticspersec = (double) sysconf(_SC_CLK_TCK);

                            t1 = (double) times(&tb1);  //get the time before sorting

                            ei.mysort(mode, field); //sort the index

                            t2 = (double) times(&tb2);  //get the time after sorting
                            cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));    //find the time needed to sort

                            fprintf(outp, "%lf sec (REAL time) | CPU for %lf sec (CPU time).\n", (t2 - t1) / ticspersec, cpu_time / ticspersec);    //print the you needed to sort
               //             cout << "starting line: " << startingline << ", ending line: " << endingline << "ending line - starting line = " << endingline - startingline << endl;
                            ei.printToFifo(outp);   //put the index in the fifo

                            fclose(inp);    //close input file
                            fclose(outp);   //close output fifo
                            exit(0);        //exit
                        }
                    }
                }
            }
        }
    }
}

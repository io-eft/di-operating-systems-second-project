#include "myfunction.hpp"

void sorter(char* d, char* f, char* a)
{
    stringstream ss;
    ss << d;
    int depth;
    if(!(ss >> depth) || (depth < 1 || depth > 6))  //check input is correct
        cout << "Wrong input, please try again using the correct format.(1)" << endl;
    else
    {
        ss.str("");
        ss.clear();
        int attribute;
        ss << a;
        if(!(ss >> attribute) || (attribute < 0 || attribute > 3))  //check if the input is correct
            cout << "Wrong input, please try again using the correct format.(2)" << endl;
        else
        {
            cout << "Starting!" << endl;    //print something to indicate the program is running
            ss.str("");
            ss.clear();
            ifstream myfile;
            myfile.open(f);     //open the input file
            if(myfile.is_open())
            {
                string numberoflines;
                int lines = 0;
                while(getline(myfile, numberoflines))
                    lines++;    //count the number of lines in the file

                myfile.close(); //close the input file

                int pid;
                string fifoname = "myfifo"; //choose the fifo name

                ss << lines;
                ss >> numberoflines;

                int fd;
                if((fd = mkfifo(fifoname.c_str(), 0666)) < 0)   //create fifo
                    cout << "Failed to open the fifo." << endl; //let the user know if the creation failed
                else
                {
                    pid = fork();
                    if(pid < 0)     //if the fork failed, let the user know
                    {
                        cout << "Fork failed, quiting!" << endl;
                    }
                    else
                    {
                        if(pid != 0)    //if you are in the parent
                        {
                            ofstream result;
                            ofstream timeresult;
                            FILE* fifo;
                            result.open("sortedoutputlily.txt");    //open the file which will contain the sorted results
                            timeresult.open("timeslily.txt");       //and the file with the times
                            fifo = fopen(fifoname.c_str(), "r");    //open the fifo
                            if(result.is_open() && timeresult.is_open() && fifo != NULL)
                            {
                                char buff[100];
                                int rows = 0;
                                int timerows = power(atoi(d));  //find the number of rows that contain time results
                                while(fgets(buff, 100, fifo) > 0)
                                {
                                    rows++;
                                    timeresult << "Node " << rows;  //print the number of node
                                    if(rows% 2 == 0)
                                        timeresult << " (Shellsort): "; //the type of node
                                    else
                                        timeresult << " (Quicksort): ";
                                     timeresult << buff;  //and the results in the appropriate file
                                    if(rows == timerows)    //once you stop reading times, stop
                                        break;
                                }

                                while(fgets(buff, 100, fifo) > 0)   //print the rest of the results in the appropriate file
                                    result << buff;

                                waitpid(pid, NULL, 0);  //wait for the child process to finish

                                result.close();     //close the result file
                                timeresult.close(); //and the time result file
                                fclose(fifo);       //close the fifo
                                unlink(fifoname.c_str());   //and unlink it
                                cout << "Finished!" << endl;    //let the user know it's completed
                            }
                        }
                        else    //if in the child
                        {   //call the internal node function with the proper arguements
                            if(execl("./internalnode", "internalnode", f, "0", numberoflines.c_str(), fifoname.c_str(), d, a, NULL) < 0)
                            {
                                cout << "exec failed" << endl;  //and let the user know if it failed
                            }
                        }
                    }
                }
            }
            else
                cout << "Failed to open the file." << endl;
        }
    }
}

int power(int a)    //simple function to find a-power of 2
{
    int n = 1;
    for(int i = 0; i < a; i++)
        n = n*2;
    return n;
}

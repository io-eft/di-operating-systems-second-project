#include "myentryqueue.hpp"
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include "myfunction.hpp"

int main(int argc, char* argv[])
{   //argv: 1 = input, 2 = startingline, 3 = endingline, 4 = output, 5 = depth, 6 = field
    if(argc != 7)   //check the number of arguments
        cout << "Wrong input to internal node" << endl;
    else
    {
        int startingline, middleline, endingline, depth, mode, field;
        stringstream ss;
        ss << argv[2];

        if(!(ss >> startingline))   //verify the input
            cout << "wrong input to sorter (2)" << endl;
        else
        {
            ss.str("");
            ss.clear();
            ss << argv[3];

            if(!(ss >> endingline))     //verify the input
                cout << "wrong input to sorter (3)" << endl;
            else
            {
                ss.str("");
                ss.clear();
                ss << argv[5];

                if(!(ss >> depth))      //verify the input
                    cout << "wrong input to sorter (4)" << endl;
                else
                {
                    ss.str("");
                    ss.clear();
                    ss << argv[6];

                    if(!(ss >> field) || field < 0 || field > 3)    //verify the input
                        cout << "wrong input to sorter (5)" << endl;
                    else
                    {
                        ss.str("");
                        ss.clear();

                        middleline = startingline + (endingline - startingline)/2;  //calculate where the input file will be split for the children
                        int lpid, rpid, lf, rf;
                        string leftfifo(argv[4]);   //get the name of the output fifo
                        leftfifo += "l";            //add a l to its end for the left child
                        string rightfifo(argv[4]);  //get the name of the output fifo
                        rightfifo += "r";           //add a r to its end for the right child

                        if((lf = mkfifo(leftfifo.c_str(), 0666) < 0) || ((rf = mkfifo(rightfifo.c_str(), 0666)) < 0))   //create the 2 fifos
                            cout << "Failed to open the fifo." << endl;     //let the user know if either of the fails
                        else
                        {
                            lpid = fork();  //for for the left child
                            if(lpid < 0)    //if the fork fails, let the user know
                                cout << "Fork failed, quiting!" << endl;
                            else    //if it succeeds
                            {
                                if(lpid != 0)   //if you are in the parent
                                {
                                    rpid = fork();  //fork for the right child
                                    if(rpid < 0)    //if the fork fails
                                        cout << "Fork failed, quiting!" << endl;    //let the user know
                                    else    //if it succeeds
                                    {
                                        if(rpid != 0)   //if you are still the parent
                                        {
                                            EntryQueue leftq, rightq;   //create the 2 queues to store the input from children
                                            FILE* leftinput;            //a file pointer for the input from the left child
                                            FILE* rightinput;           //a file pointer for the input from the right child
                                            FILE* output;               //a file pointer for the output to the parent

                                            int field = atoi(argv[6]);  //get the field that the results are sorted at

                                            leftinput = fopen(leftfifo.c_str(), "r");   //open the fifo of the left child
                                            rightinput = fopen(rightfifo.c_str(), "r"); //open the fifo of the right child
                                            output = fopen(argv[4], "w");               //open the fifo to the parent

                                            if(leftinput != NULL && rightinput != NULL && output != NULL)
                                            {   //if all fifos are open
                                                char leftbuff[100];         //set a buffer for left input
                                                char rightbuff[100];        //set a buffer for the right input
                                                stringstream leftstream;    //set a stringstream to use for the left input
                                                stringstream rightstream;   //set a stringstream to use for the right input

                                                int timerows = power(depth - 1);    //calculate the number of rows containing time results
                                                int leftrows = 0;                   //set a counter for the left input
                                                int rightrows = 0;                  //set a counter for the right input

                                                while(fgets(leftbuff, 100, leftinput) > 0)  //start reading from the left child
                                                {
                                                    fprintf(output, "%s", leftbuff);    //print the times to the output
                                                    leftrows++;                         //increase the left counter
                                                    if(leftrows == timerows)            //if you've read the number of lines containing time results
                                                        break;                          //stop
                                                }
                                                while(fgets(rightbuff, 100, rightinput) > 0)    //start reading from the right child
                                                {
                                                    fprintf(output, "%s", rightbuff);           //print the times to the output
                                                    rightrows++;                                //increase the right counter
                                                    if(rightrows == timerows)                   //if you've read the number of lines containing time results
                                                        break;                                  //stop
                                                }
                                                while(fgets(leftbuff, 100, leftinput) > 0 && fgets(rightbuff, 100, rightinput) > 0)
                                                {       //read from both fifo while they are both open
                                                        leftstream << leftbuff;     //put the left input to stringstream to extract the proper information

                                                        string leftidnumber;
                                                        string leftsurname;
                                                        string leftname;
                                                        string leftinc;

                                                        unsigned long leftid;
                                                        unsigned long leftincome;
                                                        //get the proper info from each line
                                                        getline(leftstream, leftidnumber, ' ');
                                                        leftid = atol(leftidnumber.c_str());
                                                        getline(leftstream, leftsurname, ' ');
                                                        getline(leftstream, leftname, ' ');
                                                        getline(leftstream, leftinc);
                                                        leftincome = atol(leftinc.c_str());
                                                        //clear the stringstream
                                                        leftstream.str("");
                                                        leftstream.clear();
                                                        //create a new entry and insert it in the left queue
                                                        Entry* le = new Entry(leftid, leftsurname, leftname, leftincome);
                                                        leftq.insertEntry(le);

                                                        rightstream << rightbuff;   //put the right input to stringstream to extract the proper information

                                                        string rightidnumber;
                                                        string rightsurname;
                                                        string rightname;
                                                        string rightinc;

                                                        unsigned long rightid;
                                                        unsigned long rightincome;
                                                        //get the proper info from each line
                                                        getline(rightstream, rightidnumber, ' ');
                                                        rightid = atol(rightidnumber.c_str());
                                                        getline(rightstream, rightsurname, ' ');
                                                        getline(rightstream, rightname, ' ');
                                                        getline(rightstream, rightinc);
                                                        rightincome = atol(rightinc.c_str());
                                                        //clear the stringstream
                                                        rightstream.str("");
                                                        rightstream.clear();
                                                        //create a new entry and insert it in the right queue
                                                        Entry* re = new Entry(rightid, rightsurname, rightname, rightincome);
                                                        rightq.insertEntry(re);
                                                }//this will stop when one of the 2 fifos reaches its end
                                                if(fgets(leftbuff, 100, leftinput) > 0)
                                                {   //check if the left fifo still has more information in it
                                                    do  //same as above, only with do-while instead of while, since you've already read a line
                                                    {
                                                        leftstream << leftbuff;

                                                        string leftidnumber;
                                                        string leftsurname;
                                                        string leftname;
                                                        string leftinc;

                                                        unsigned long leftid;
                                                        unsigned long leftincome;

                                                        getline(leftstream, leftidnumber, ' ');
                                                        leftid = atol(leftidnumber.c_str());
                                                        getline(leftstream, leftsurname, ' ');
                                                        getline(leftstream, leftname, ' ');
                                                        getline(leftstream, leftinc);
                                                        leftincome = atol(leftinc.c_str());

                                                        leftstream.str("");
                                                        leftstream.clear();

                                                        Entry* le = new Entry(leftid, leftsurname, leftname, leftincome);
                                                        leftq.insertEntry(le);
                                                    }while(fgets(leftbuff, 100, leftinput) > 0);
                                                }
                                                if(fgets(rightbuff, 100, rightinput) > 0)
                                                {   //check if the fifo has more information it it
                                                    do  //same as above, only with do-while instead of while, since you've already read a line
                                                    {
                                                        rightstream << rightbuff;

                                                        string rightidnumber;
                                                        string rightsurname;
                                                        string rightname;
                                                        string rightinc;

                                                        unsigned long rightid;
                                                        unsigned long rightincome;

                                                        getline(rightstream, rightidnumber, ' ');
                                                        rightid = atol(rightidnumber.c_str());
                                                        getline(rightstream, rightsurname, ' ');
                                                        getline(rightstream, rightname, ' ');
                                                        getline(rightstream, rightinc);
                                                        rightincome = atol(rightinc.c_str());

                                                        rightstream.str("");
                                                        rightstream.clear();

                                                        Entry* re = new Entry(rightid, rightsurname, rightname, rightincome);
                                                        rightq.insertEntry(re);
                                                    }while(fgets(rightbuff, 100, rightinput) > 0);
                                                }

                                                waitpid(lpid, NULL, 0);     //wait for the left child
                                                waitpid(rpid, NULL, 0);     //wait for the right child

                                                Entry* leftcandidate;   //set a left candidate to compare
                                                Entry* rightcandidate;  //set a right candidate to compare

                                                while(!leftq.isEmpty() && !rightq.isEmpty())    //while there are still entries in both queues
                                                {
                                                    leftcandidate = leftq.getTop();     //set the left candidate to be the top entry in the left queue
                                                    rightcandidate = rightq.getTop();   //set the right candidate to be the top entry in the right queue
                                                    switch(field)   //switch depending on the field indicated by the input
                                                    {   //depending on the field, compare the 2 candidates. Then, print the corrent one and then pop it from the queue
                                                        case(0):if(leftcandidate->getId() < rightcandidate->getId())
                                                                {
                                                                    leftq.printTopToFifo(output);
                                                                    leftq.popTop();
                                                                }
                                                                else
                                                                {
                                                                    rightq.printTopToFifo(output);
                                                                    rightq.popTop();
                                                                }
                                                                break;
                                                        case(1): if(leftcandidate->getSurname() < rightcandidate->getSurname())
                                                                {
                                                                    leftq.printTopToFifo(output);
                                                                    leftq.popTop();
                                                                }
                                                                else
                                                                {
                                                                    rightq.printTopToFifo(output);
                                                                    rightq.popTop();
                                                                }
                                                                break;
                                                        case(2): if(leftcandidate->getName() < rightcandidate->getName())
                                                                {
                                                                    leftq.printTopToFifo(output);
                                                                    leftq.popTop();
                                                                }
                                                                else
                                                                {
                                                                    rightq.printTopToFifo(output);
                                                                    rightq.popTop();
                                                                }
                                                                break;
                                                        case(3): if(leftcandidate->getIncome() < rightcandidate->getIncome())
                                                                {
                                                                    leftq.printTopToFifo(output);
                                                                    leftq.popTop();
                                                                }
                                                                else
                                                                {
                                                                    rightq.printTopToFifo(output);
                                                                    rightq.popTop();
                                                                }
                                                                break;
                                                    }
                                                }   //this happens while both queues have entries left
                                                if(!leftq.isEmpty())    //if the left queue has some remaining entries
                                                {
                                                    while(!leftq.isEmpty())     //print and pop them until it's empty
                                                    {
                                                        leftq.printTopToFifo(output);
                                                        leftq.popTop();
                                                    }
                                                }
                                                if(!rightq.isEmpty())   //if the right queue has some remaining entries
                                                {
                                                    while(!rightq.isEmpty())    //print and pop them until it's empty
                                                    {
                                                        rightq.printTopToFifo(output);
                                                        rightq.popTop();
                                                    }
                                                }
                                            }

                                            fclose(leftinput);  //close left input fifo
                                            fclose(rightinput); //close right input fifo
                                            fclose(output);     //close output fifo

                                            unlink(leftfifo.c_str());   //unlink left input fifo
                                            unlink(rightfifo.c_str());  //unlink right input fifo
                                            exit(0);                    //exit
                                        }
                                        else    //if you are in the right child
                                        {
                                            string newstartingline;     //create a string with the new starting line
                                            ss << (middleline);
                                            ss >> newstartingline;
                                            ss.str("");
                                            ss.clear();

                                            if(depth > 1)               //if you're not in height 1, meaning you need to call more internal nodes
                                            {
                                                depth--;                //calculate the new depth
                                                string dpth;
                                                ss << depth;
                                                ss >> dpth;
                                                ss.str("");
                                                ss.clear();
                                                                    //call in the next interal node with the new arguments
                                                if(execl("./internalnode", "internalnode", argv[1], newstartingline.c_str(), argv[3], rightfifo.c_str(), dpth.c_str(), argv[6], NULL) < 0)
                                                    cout << "exec failed (internalnode)" << endl;   //let the user know if the exec failed
                                            }
                                            else    //if your depth is one, call the sorter. Since it's the right child, set it to quicksort
                                            {
                                                if(execl("./mysorter", "mysorter", argv[1], newstartingline.c_str(), argv[3], rightfifo.c_str(), "2", argv[6], NULL) < 0)
                                                    cout << "exec failed (sorter)" << endl;     //if the exec fails, let the user know
                                            }
                                        }
                                    }
                                }
                                else    //if you are in the left child
                                {
                                    string newendingline;   //create a string with the new ending line
                                    ss << middleline;
                                    ss >> newendingline;
                                    ss.str("");
                                    ss.clear();

                                    if(depth > 1)       //if you're not in height 1, meaning you need to call more internal nodes
                                    {
                                        depth--;        //calculate the new depth
                                        string dpth;
                                        ss << depth;
                                        ss >> dpth;
                                        ss.str("");
                                        ss.clear();
                                                        //call in the next interal node with the new arguments
                                        if(execl("./internalnode", "internalnode", argv[1], argv[2], newendingline.c_str(), leftfifo.c_str(), dpth.c_str(), argv[6], NULL) < 0)
                                            cout << "exec failed (internalnode)" << endl;   //let the user know if the exec failed
                                    }
                                    else    //if your depth is one, call the sorter. Since it's the left child, set it to shellsort
                                    {
                                        if(execl("./mysorter", "mysorter", argv[1], argv[2], newendingline.c_str(), leftfifo.c_str(), "1", argv[6], NULL) < 0)
                                            cout << "exec failed (sorter)" << endl; //if the exec fails, let the user know
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

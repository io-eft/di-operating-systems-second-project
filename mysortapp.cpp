#include "myfunction.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 7)   //check if the number of arguments is correct
        cout << "Wrong input, please try again using the correct format." << endl;
    else    //check if the flags are correct, and call the function with the right arguments, depending on flag order
    {
        if(strcmp(argv[1],"-d") == 0 && strcmp(argv[3], "-f") == 0 && strcmp(argv[5], "-a") == 0 )
        {
            sorter(argv[2], argv[4], argv[6]);
        }
        else
        {
            if(strcmp(argv[1],"-d") == 0 && strcmp(argv[3], "-a") == 0 && strcmp(argv[5], "-f") == 0 )
            {
                sorter(argv[2], argv[6], argv[4]);
            }
            else
            {
                if(strcmp(argv[1],"-f") == 0 && strcmp(argv[3], "-d") == 0 && strcmp(argv[5], "-a") == 0 )
                {
                    sorter(argv[4], argv[2], argv[6]);
                }
                else
                {
                    if(strcmp(argv[1],"-f") == 0 && strcmp(argv[3], "-a") == 0 && strcmp(argv[5], "-d") == 0 )
                    {
                        sorter(argv[6], argv[2], argv[4]);
                    }
                    else
                    {
                        if(strcmp(argv[1],"-a") == 0 && strcmp(argv[3], "-f") == 0 && strcmp(argv[5], "-d") == 0 )
                        {
                            sorter(argv[6], argv[4], argv[2]);
                        }
                        else
                        {
                            if(strcmp(argv[1],"-a") == 0 && strcmp(argv[3], "-d") == 0 && strcmp(argv[5], "-f") == 0 )
                            {
                                sorter(argv[4], argv[6], argv[2]);
                            }
                            else
                            {
                                cout << "Wrong input, please try again using the correct format!!." << endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

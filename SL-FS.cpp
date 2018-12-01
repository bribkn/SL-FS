#include <sys/stat.h>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;


int main()
{
   string aux,folder_route_string;
   int option;
   while(1){
      cout << "1.Add Folder: \n";
      cout << "2.Exit\n\n";
      cout << "Enter an option: ";
      cin >> option;
      
      if(option == 1){
         folder_route_string = "./";
         cout << "Enter the name of the folder: ";
         cin >> aux;
         folder_route_string += aux;
         const char *folder_route = folder_route_string.c_str();
         mkdir(folder_route, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

      }else if(option == 2){
         break;
      }


   }

   return 0;
}

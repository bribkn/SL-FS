#include <sys/stat.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string>
using namespace std;

int counter_id = 0;

class Folder{
public:
   int id;
   int size;
   string name;
   Folder *father = NULL;
   vector <Folder *> sons;
   vector <string> files;

   Folder(string name){
      this->id = counter_id;
      this->name = name;
      this->size = 0;
      ++counter_id;
   }
   void setFather(Folder *f){
      father = f;
   }
   void addSubFolder(Folder *f){
      sons.push_back(f);
   }
   void addSize(Folder *f, int s){
      f->size += s;
      if(f->father !=NULL){
         addSize(f->father,s);
      }
   }
   string CurrentPath(Folder *f){
      if(f->father != NULL){
         return CurrentPath(f->father) + "/" + name;
      }else{
         return "root";
      }
   }
   void addFile(string f){
      files.push_back(f);
   }
   void printStatus(){
      cout << "Name\tID\tSize\n";
      cout << name << "\t" << id << "\t" << size << "\n";
   }
   void printCurrentFolder(){
      for(int i = 0; i < sons.size(); ++i){
         cout << sons[i]->name << "    ";
      }
      for(int i = 0; i < files.size(); ++i){
         cout << files[i] << "    ";
      }
      cout << endl;
   }
   Folder *changeFolder(string n){
      for(int i = 0; i < sons.size(); ++i){
         if(sons[i]->name == n){
            return sons[i];
         }
      }
      return NULL;
   }
};

int main()
{

   string command;
   string folder_name;
   string file_name;
   Folder *Root = new Folder("root");
   Folder *CurrentFolder = Root;

   while(1){
      cin >> command;
      // cout << "Command in progress: " << command << endl;
      if(command == "exit"){
         break;

      }else if(command == "mkdir"){
         cin >> folder_name;
         Folder *aux = new Folder(folder_name);
         aux->setFather(CurrentFolder);
         CurrentFolder->addSubFolder(aux);

      }else if(command == "ls"){
         CurrentFolder->printCurrentFolder();

      }else if(command == "cd"){
         cin >> folder_name;
         if(folder_name == "root"){
            CurrentFolder = Root;
         }else if(folder_name == ".."){
            CurrentFolder = CurrentFolder->father;
         }else{
            CurrentFolder = CurrentFolder->changeFolder(folder_name);
         }

      }else if(command == "create"){
         cin >> file_name;
         CurrentFolder->addFile(file_name);
         CurrentFolder->addSize(CurrentFolder,file_name.size());
      }else if(command == "status"){
         CurrentFolder->printStatus();
      }else if(command == "pwd"){
         cout << CurrentFolder->CurrentPath(CurrentFolder) << endl;
      }
   }
   return 0;
}

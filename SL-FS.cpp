#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;

int counter_id = 0;

class File{
public:
    int id;
    int size;
    string name;

    File(string name){
        this->id = counter_id;
        this->name = name;
        this->size = name.length();

        ++counter_id;
    }
};

class Folder{
public:
    int id;
    int size;
    string name;

    Folder *father;

    vector <Folder *> childFolders;
    vector <File *> files;

    Folder( string name ){
        this->id = counter_id;
        this->name = name;
        this->size = 0;
        this->father = NULL;

        ++counter_id;
    }

    void setFather( Folder *f ){ father = f; }

    string getCurrentPath( Folder *f ){
        if(f->father != NULL)
            return getCurrentPath(f->father) + "/" + name;
        else
            return "root";
    }

    void addSubFolder( Folder *f ){ childFolders.push_back(f); }
    void addFile( File *f ){ files.push_back( f ); }

    void updateSize( Folder *f, int s ){
        f->size += s;

        if(f->father != NULL)
            updateSize(f->father,s);
    }

    void printStatus(){
        cout << "Name\ti-node\tSize\n";

        cout << name << "\t" << id << "\t" << size << "\n";
    }

    void printChilds(){
        for(int i = 0; i < childFolders.size(); ++i)
            cout << childFolders[i]->name << "    ";

        for(int i = 0; i < files.size(); ++i)
            cout << files[i]->name << "    ";

        if ( !childFolders.size() && !files.size() ) {
            cout << "[LS] No folders or files were found";
        }

        cout << endl;
    }

    Folder *changeFolder( string n ){
        for(int i = 0; i < childFolders.size(); ++i)
            if( childFolders[i]->name == n ){
                cout << "[CD] Folder swapped to: " << childFolders[i]->getCurrentPath(childFolders[i]) << "\n";
                return childFolders[i];
            }

        cout << "[CD] Folder not found\n";
        return this;
    }
};

int main(){
    string command;
    string file_name;
    string folder_name;

    Folder *root = new Folder("root");
    Folder *CurrentFolder = root;

    cout << "Commands list:\n";
    cout << "\tmkdir foldername: makes a new folder\n";
    cout << "\tls: shows current folder items\n";
    cout << "\tcd foldername: navigates to foldername\n";
    cout << "\tcreate filename.extension: makes a new file\n";
    cout << "\tstatus: shows current status\n";
    cout << "\tpwd: shows the absolute route of the current folder\n\n\n";

    while(1){
        // cout << "Command in progress: " << command << endl;
        cout << "-> " << CurrentFolder->getCurrentPath( CurrentFolder ) << "$ ";
        cin >> command;

        if(command == "exit"){
            break;
        }else if(command == "mkdir"){
            cin >> folder_name;
            cout << "Creating new folder with name: " << folder_name << "\n\n";

            Folder *aux = new Folder(folder_name);

            aux->setFather(CurrentFolder);
            CurrentFolder->addSubFolder(aux);
        }else if(command == "ls"){
            CurrentFolder->printChilds();
        }else if(command == "cd"){
            cin >> folder_name;

            if(folder_name == "root" || folder_name == ""){
                CurrentFolder = root;
            }else if(folder_name == ".."){
                CurrentFolder = CurrentFolder->father;
            }else{
                CurrentFolder = CurrentFolder->changeFolder(folder_name);
            }
        }else if(command == "create"){
            cin >> file_name;
            File *newFile = new File(file_name);

            CurrentFolder->addFile( newFile );
            CurrentFolder->updateSize( CurrentFolder, newFile->size );
        }else if(command == "status"){
            CurrentFolder->printStatus();
        }else if(command == "pwd"){
            cout << CurrentFolder->getCurrentPath(CurrentFolder) << endl;
        }
    }

    return 0;
}

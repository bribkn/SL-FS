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
    vector <File *> childFiles;

    Folder( string name ){
        this->id = counter_id;
        this->name = name;
        this->size = 0;
        this->father = NULL;

        ++counter_id;
    }

    void setFather( Folder *folder ){
        father = folder;
    }

    bool checkDuplicatedName( string name ){
        for(int i = 0; i < this->childFolders.size(); ++i)
            if ( childFolders[i]->name == name )
                return false;

        for(int i = 0; i < this->childFiles.size(); ++i)
            if ( childFiles[i]->name == name )
                return false;

        return true;
    }

    string getCurrentPath( Folder *folder ){
        if(folder->father != NULL)
            return getCurrentPath(folder->father) + "/" + folder->name;
        else
            return "root";
    }

    bool addSubFolder( Folder *f ){
        if( checkDuplicatedName(f->name) ){
            childFolders.push_back(f);
            return true;
        }else{
            cout << "SL-FS: There already exists a folder/file with that name";
            return false;
        }
    }

    bool addFile( File *f ){
        if( checkDuplicatedName(f->name) ){
            childFiles.push_back( f );
            return true;
        }else{
            cout << "SL-FS: There already exists a folder/file with that name";
            return false;
        }
    }

    void updateSize( Folder *f, int s ){
        f->size += s;

        if(f->father != NULL)
            updateSize(f->father,s);
    }

    void printStatus(){
        cout << "NAME\t\tI-NODE\t\tSIZE\n";

        cout << name << "\t\t" << id << "\t\t" << size << "\n";

        for(int i = 0; i < childFolders.size(); ++i)
            cout << childFolders[i]->name << "\t\t" << childFolders[i]->id << "\t\t" << childFolders[i]->size << "\n";

        for(int i = 0; i < childFiles.size(); ++i)
            if ( i != childFiles.size()-1 )
                cout << childFiles[i]->name << "\t\t" << childFiles[i]->id << "\t\t" << childFiles[i]->size << "\n";
            else
                cout << childFiles[i]->name << "\t\t" << childFiles[i]->id << "\t\t" << childFiles[i]->size << "";
    }

    void printChilds(){
        for(int i = 0; i < childFolders.size(); ++i)
            cout << "\033[1;36m" << childFolders[i]->name << "\t";

        for(int i = 0; i < childFiles.size(); ++i)
            cout << "\033[0m" << childFiles[i]->name << "\t";

        if ( !childFolders.size() && !childFiles.size() ) {
            cout << "SL-FS: No folders or files were found";
        }
    }

    Folder *changeFolder( string n ){
        for(int i = 0; i < childFolders.size(); ++i)
            if( childFolders[i]->name == n ){
                cout << "SL-FS: Folder swapped to: " << childFolders[i]->getCurrentPath(childFolders[i]) << "";
                return childFolders[i];
            }

        cout << "SL-FS: Folder not found";
        return this;
    }
};

int main(){
    string command;
    string file_name;
    string folder_name;

    Folder *root = new Folder("root");
    Folder *currentFolder = root;

    cout << "Commands list:\n";
    cout << "\tmkdir foldername: makes a new folder\n";
    cout << "\tls: shows current folder items\n";
    cout << "\tcd foldername: navigates to foldername\n";
    cout << "\tcreate filename.extension: makes a new file\n";
    cout << "\tstatus: shows current status\n";
    cout << "\tpwd: shows the absolute route of the current folder\n";
    cout << "\texit: terminates SL-FS\n\n\n";

    while(1){
        cout << "\033[1;31m-> " << currentFolder->getCurrentPath( currentFolder ) << "$\033[0m ";
        cin >> command;

        if(command == "exit"){
            break;
        }else if(command == "mkdir"){
            cin >> folder_name;

            Folder *aux = new Folder(folder_name);

            if( currentFolder->addSubFolder(aux) ){
                aux->setFather(currentFolder);
                cout << "SL-FS: Creating new folder with name: " << folder_name << "";
            }
        }else if(command == "ls"){
            currentFolder->printChilds();
        }else if(command == "cd"){
            cin >> folder_name;

            if(folder_name == "root" || folder_name == ""){
                currentFolder = root;
            }else if(folder_name == ".."){
                if ( currentFolder->father != NULL ) {
                    currentFolder = currentFolder->father;

                    cout << "SL-FS: Folder swapped to: " << currentFolder->getCurrentPath( currentFolder ) << "";
                }else{
                    cout << "SL-FS: Father folder does not exists.";
                }
            }else{
                currentFolder = currentFolder->changeFolder( folder_name );
            }
        }else if(command == "create"){
            cin >> file_name;

            File *newFile = new File(file_name);

            if ( currentFolder->addFile( newFile ) ) {
                currentFolder->updateSize( currentFolder, newFile->size );
                cout << "SL-FS: Creating new file with name: " << file_name << "";
            }
        }else if(command == "status"){
            currentFolder->printStatus();
        }else if(command == "pwd"){
            cout << currentFolder->getCurrentPath(currentFolder) << "";
        }else{
            cout << "SL-FS: Command '" << command << "' not found";
        }

        cout << "\n";
    }

    return 0;
}

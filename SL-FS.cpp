#include <time.h>
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
    int creationTime;

    File(string name){
        this->id = counter_id;
        this->name = name;
        this->size = name.length();
        this->creationTime = time(0);

        ++counter_id;
    }
};

class Folder{
public:
    int id;
    int size;
    string name;
    int creationTime;

    Folder *father;

    vector <Folder *> childFolders;
    vector <File *> childFiles;

    Folder( string name ){
        this->id = counter_id;
        this->name = name;
        this->size = 0;
        this->father = NULL;
        this->creationTime = time(0);

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
            return "\033[1;31mIron\033[0m\033[1;33mMan\033[0m";
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
        cout << "NAME\t\t\t\tI-NODE\t\t\tSIZE\t\t\tCREATION TIME\n";

        cout << name << "\t\t\t\t" << id << "\t\t\t" << size << "\t\t\t" << creationTime;

        vector<string> allData;

        if ( childFiles.size() || childFolders.size() )
            cout << "\n";

        for(int i = 0; i < childFolders.size(); ++i)
            allData.push_back( childFolders[i]->name + "\t\t\t\t" + to_string(childFolders[i]->id) + "\t\t\t" + to_string(childFolders[i]->size) + "\t\t\t" + to_string(childFolders[i]->creationTime) );

        for(int i = 0; i < childFiles.size(); ++i)
            allData.push_back( childFiles[i]->name + "\t\t\t" + to_string(childFiles[i]->id) + "\t\t\t" + to_string(childFiles[i]->size) + "\t\t\t" + to_string(childFiles[i]->creationTime) );

        for(int i = 0; i < allData.size(); ++i){
            cout << allData[i];
            i != allData.size()-1 ? cout << "\n" : cout << "";
        }
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

    system("clear");

    cout << "Commands list:\n";
    cout << "\t" << "\033[1;32mHulk\033[0m" << " foldername: makes a new folder\n";
    cout << "\t" << "\033[1;31mSpiderman\033[0m" << ": shows current folder items\n";
    cout << "\t" << "\033[1;33mThor\033[0m" << " foldername: navigates to foldername\n";
    cout << "\t" << "\033[1;34mCap\033[0m" << " filename.extension: makes a new file\n";
    cout << "\t" << "\033[1;35mHawkeye\033[0m" << ": shows current status\n";
    cout << "\t" << "\033[1;36mBlackwidow\033[0m" << ": shows the absolute route of the current folder\n";
    cout << "\t" << "\033[1;31mT\033[0m" << "\033[1;32mh\033[0m" << "\033[1;33ma\033[0m"  << "\033[1;34mn\033[0m"   << "\033[1;35mo\033[0m"   << "\033[1;36ms\033[0m" << ": terminates SL-FS\n\n\n";

    while(1){
        cout << "\033[1;31m->\033[0m" <<currentFolder->getCurrentPath( currentFolder ) << "\033[1;33m$ \033[0m";
        cin >> command;

        if(command == "Thanos"){
            break;
        }else if(command == "Hulk"){
            cin >> folder_name;

            Folder *aux = new Folder(folder_name);

            if( currentFolder->addSubFolder(aux) ){
                aux->setFather(currentFolder);
                cout << "SL-FS: Creating new folder with name: " << folder_name << "";
            }
        }else if(command == "Spiderman"){
            currentFolder->printChilds();
        }else if(command == "Thor"){
            cin >> folder_name;

            if(folder_name == "root" || folder_name == ""){
                currentFolder = root;

                cout << "SL-FS: Folder swapped to: " << currentFolder->getCurrentPath( currentFolder ) << "";
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
        }else if(command == "Cap"){
            cin >> file_name;

            File *newFile = new File(file_name);

            if ( currentFolder->addFile( newFile ) ) {
                currentFolder->updateSize( currentFolder, newFile->size );
                cout << "SL-FS: Creating new file with name: " << file_name << "";
            }
        }else if(command == "Hawkeye"){
            currentFolder->printStatus();
        }else if(command == "Blackwidow"){
            cout << currentFolder->getCurrentPath(currentFolder) << "";
        }else{
            cout << "SL-FS: Command '" << command << "' not found";
        }

        cout << "\n";
    }

    return 0;
}

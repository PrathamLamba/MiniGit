#include "ModularLink.h"



int main(int argc, char* argv[]){

    if(argc<2){
        cout<<"Usage: minigit <command>\n";
        return 1;
    }

    string command = argv[1];


    if(command=="test"){

        FileManager fileManager;
        ObjectStore objectStore(".minigit");

        string content=fileManager.read_file("hello.txt");

        string hash = objectStore.create_object("blob", content);
        cout << "Object ID: " << hash << "\n";
        cout << "Object stored\n";

        return 0;
    }

    if(command == "init"){
        
        if(filesystem::exists(".minigit")){
            cout<<"MiniGit repository already exists\n";
        }
        else{
            filesystem::create_directory(".minigit");
            filesystem::create_directory(".minigit/objects");
            filesystem::create_directory(".minigit/refs");
            filesystem::create_directory(".minigit/refs/heads");
            
            ofstream head(".minigit/HEAD");
            head<< "ref: refs/heads/main\n";
            head.close();
        
            cout<<"Initialized MiniGit repository\n";
        }
    }
    else if(command=="add"){
        
        if(argc<3){
            cout<<"Usage: minigit add <file>\n";
            return 1;
        }

        string filename= argv[2];
        
        if(!filesystem::exists(filename)){
            cout<<"File does not exist: "<<filename<<"\n";
            return 1;
        }

        FileManager fileManager;
        ObjectStore objectStore(".minigit");
        StagingArea stagingArea(".minigit");
        
        string content=fileManager.read_file(filename);

        string hash=objectStore.create_object("blob",content);
        
        stagingArea.add(filename, hash);

        cout<<"Added "<<filename<<"\n";
        cout<<"Object ID: "<<hash<<"\n";
    }
    else if(command=="status"){

        StagingArea stagingArea(".minigit");

        if(stagingArea.empty()){
            cout<<"No files are staged.\n";
            return 0;
        }

        for(const auto& entry:stagingArea.getEntries()){

            string filename=entry.first;
            string stagedHash=entry.second;
            FileManager fileManager;
            if(!fileManager.exists(filename)){
                cout<<filename<<" - deleted\n";

                continue;
            }

            string current_hash=fileManager.get_file_hash(filename);

            if(current_hash==stagedHash){
                cout<<filename<<" - staged, unchanged\n";
            }
            else{
                cout<<filename<<" - modified\n";
            }
        }

        return 0;
    }
    else if(command=="tree-test"){

    StagingArea stagingArea(".minigit");

    if(stagingArea.empty()){
        cout << "Nothing is staged.\n";
        return 0;
    }

    string tree_data;

    for(const auto& entry: stagingArea.getEntries()){
        tree_data+=entry.first;
        tree_data+=" ";
        tree_data+=entry.second;
        tree_data+="\n";
    }

    ObjectStore objectStore(".minigit");
    string tree_hash = objectStore.create_object("tree", tree_data);

    cout << "Tree object ID: " << tree_hash << "\n";
    cout << "Tree data:\n";
    cout << tree_data;

    return 0;
}
else if(command=="commit"){


    if(argc<3){
        cout<<"Usage: minigit commit <message>\n";
        return 1;
    }

    string message=argv[2];

    StagingArea stagingArea(".minigit");

    if(stagingArea.empty()){
        cout << "Nothing is staged.\n";
        return 0;
    }

    Tree tree;

    for(const auto& entry: stagingArea.getEntries()){
        tree.addFile(entry.first, entry.second);
    }

    BranchManager branchManager(".minigit");
    ObjectStore objectStore(".minigit");
    string tree_hash=tree.createObject(objectStore);

    string parent_hash=branchManager.get_current_commit();
    
    Commit commit(tree_hash, parent_hash, "Pratham", message);

    string commit_hash=commit.createObject(objectStore);

    branchManager.updateCurrentBranch(commit_hash);

    cout << "Tree object ID: " << tree_hash << "\n";
    cout << "Commit object ID: " << commit_hash << "\n";

    cout << "Commit data:\n";
    cout << commit.serialize();

    return 0;

    }

    else if(command=="tree-view"){
        StagingArea stagingArea(".minigit");

        if(stagingArea.empty()){
            cout<<"Nothing is staged.\n";
            return 0;
        }

        Tree tree;

        for(const auto& entry:stagingArea.getEntries()){
            string filename=entry.first;
            string hash=entry.second;

            tree.addFile(filename, hash);
        }

        cout<<"File Tree:\n";
        tree.printDFS(tree.getRoot());

        cout<<"\nSerialized Tree:\n";
        cout<<tree.serialize();
        cout<<"\n";

        ObjectStore objectStore(".minigit");

        string treeHash= tree.createObject(objectStore);

        cout<<"Tree Object ID: "<<treeHash<<"\n";

        return 0;

    }

    else if (command == "log") {
    try {
        ObjectStore objectStore(".minigit");
        BranchManager branchManager(".minigit");

        string currentHash = branchManager.get_current_commit();

        if (currentHash.empty()) {
            cout << "No commits yet.\n";
            return 0;
        }

        while (!currentHash.empty()) {
            string content = objectStore.read_object_content(currentHash);

            ParsedCommit commit = CommitParser::parse(content);

            cout << "Commit: " << currentHash << '\n';
            cout << "Author: " << commit.author << '\n';
            cout << "Message: " << commit.message << '\n';
            cout << "Tree: " << commit.treeHash << '\n';
            cout << "-----------------------------------\n";

            currentHash = commit.parentHash;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}

    else if(command=="restore"){
        if(argc<4){
            cout<<"Usage: minigit restore <commit-hash> <file-path>\n";
            return 1;
        }

        try{
            ObjectStore objectStore(".minigit");
            RestoreManager restoreManager(objectStore);

            string commitHash=argv[2];
            string filepath=argv[3];

            restoreManager.restoreFile(commitHash, filepath);
        }
        catch(const exception& e){
            cerr<<"Restore failed!: "<<e.what()<<endl;
            return 1;
        }
    }

    else{
        cout<<"Unknown command: "<<command<<"\n";
    }

    return 0;
}





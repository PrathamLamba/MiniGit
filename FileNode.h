#include "ModularLink.h"


class FileNode{
    private:
        string name;
        string hash;
        bool directory;
        vector<FileNode> children;

    public:
        FileNode(const string& name, bool directory=false, const string& hash=""):name(name), hash(hash), directory(directory){}

        void addChild(const FileNode& child){
            children.push_back(child);
        }

        const string& getName() const{
            return name;
        }

        const string& getHash() const{
            return hash;
        }

        bool isDirectory() const{
            return directory;
        }

        vector<FileNode>& getChildren(){
            return children;
        }

        const vector<FileNode>& getChildren() const{
            return children;
        }
};

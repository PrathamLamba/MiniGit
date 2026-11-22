#include "ModularLink.h"

class RestoreManager{
    private:
        ObjectStore& objectStore;
        CommitParser commitParser;
        TreeParser treeParser;
    
    public:
        RestoreManager(ObjectStore& store):objectStore(store), treeParser(store){}

        string getBlobHash(const string& commitHash, const string& filepath){

            string commitContent=objectStore.read_object_content(commitHash);

            ParsedCommit commit=CommitParser::parse(commitContent);

            if(commit.treeHash.empty()){
                throw runtime_error("Commit does not contain a Tree hash.");
            }

            string blobHash=treeParser.findBlobHash(commit.treeHash, filepath);

            if(blobHash.empty()){
                throw runtime_error("File not found in the specific commit: "+filepath);
            }

            return blobHash;
        }

        void restoreFile(const string& commitHash, const string& filepath){
            string blobHash=getBlobHash(commitHash, filepath);

            string content=objectStore.read_object_content(blobHash);

            ofstream file(filepath, ios::binary);

            if(!file.is_open()){
                throw runtime_error("Could not open file for restoration: "+filepath);
            }

            file.write(content.data(), content.size());

            if(!file){
                throw runtime_error("Failed to write restored file: "+filepath);
            }

            file.close();
            
            cout<<"Restored: "<<filepath<<endl;
            
        }
};
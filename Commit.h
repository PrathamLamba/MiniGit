#include "ModularLink.h"


class Commit{
    private:
        string treeHash;
        string parentHash;
        string author;
        string message;

    public:
        Commit(const string& treeHash, const string& parentHash, const string& author, const string& message):treeHash(treeHash), parentHash(parentHash), author(author), message(message){}

        string serialize() const{
            string data;

            data+="tree ";
            data+=treeHash;
            data+="\n";

            if(!parentHash.empty()){
                data+="parent ";
                data+=parentHash;
                data+="\n";
            }

            data+="Author: ";
            data+=author;
            data+="\n";

            data+="Message: ";
            data+=message;
            data+="\n";

            return data;
        }

        string createObject(ObjectStore& objectStore) const{
            return objectStore.create_object("commit", serialize());
        }
};

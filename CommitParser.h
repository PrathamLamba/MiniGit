#include "ModularLink.h"

struct ParsedCommit{
    string treeHash;
    string parentHash;
    string author;
    string message;
};

class CommitParser{
    public:
        static ParsedCommit parse(const string& content){
            ParsedCommit commit;
            istringstream input(content);
            string line;

            while(getline(input, line)){
                if(line.rfind("tree ",0)==0){
                    commit.treeHash=line.substr(5);
                }
                else if(line.rfind("parent ",0)==0){
                    commit.parentHash=line.substr(7);
                }
                else if(line.rfind("Author: ",0)==0){
                    commit.author=line.substr(8);
                }
                else if(line.rfind("Message: ",0)==0){
                    commit.message=line.substr(9);
                }
            }

            return commit;
        }
};
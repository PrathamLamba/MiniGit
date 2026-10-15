#include "ModularLink.h"


class StagingArea{
    private:
        unordered_map<string,string> entries;
        string indexPath;

    public:
        StagingArea(const string& repositoryPath): indexPath(repositoryPath + "/index"){
            load();
        }

        void load(){
            entries.clear();

            ifstream index(indexPath);

            if(!index.is_open()){
                return;
            }

            string filename;
            string hash;

            while(index>>filename>>hash){
                entries[filename]=hash;
            }

            index.close();
        }


        void save() const{
            ofstream index(indexPath);
            
            if(!index.is_open()){
                throw runtime_error("Could not open index");
            }

            for(const auto& entry:entries){
                index<<entry.first<<" "<<entry.second<<"\n";
            }

            index.close();
        }

        void add(const string& filename, const string& hash){
            entries[filename]=hash;
            save();
        }

        bool empty() const{
            return entries.empty();
        }

        bool contains(const string& filename) const{
            return entries.find(filename)!=entries.end();
        }

        string getHash(const string& filename) const{
            auto it=entries.find(filename);

            if(it==entries.end()){
                return "";
            }
            return it->second;
        }

        const unordered_map<string, string>& getEntries() const{
            return entries;
        }

};

#include "ModularLink.h"


class ObjectStore{
    private:
        string objectsPath;
    
    public:
        ObjectStore(const string& repositoryPath):objectsPath(repositoryPath + "/objects/")
        {}

        // function to write object:-
        void write_object(const string& hash, const string& data){
    
            string path = objectsPath + hash;

            ofstream file(path, std::ios::binary);

            if (!file.is_open()) {
                throw runtime_error("Could not create object");
            }

            file.write(data.data(), data.size());

            file.close();
        }

        // fuction to create object:-
        string create_object(const string& type, const string& content){

            string object_data=type+" "+to_string(content.size())+'\0'+content;

            string hash=SHA1::sha1(object_data);
            write_object(hash, object_data);

            return hash;
        }

        string read_object(const string& hash) const {
            string path = objectsPath + hash;

            ifstream file(path, ios::binary);

            if (!file.is_open()) {
                throw runtime_error("Could not read object: " + hash);
            }

            string data{istreambuf_iterator<char>(file), istreambuf_iterator<char>()};

            return data;
        }


        string read_object_content(const string& hash) const {
            string objectData = read_object(hash);

            size_t separator = objectData.find('\0');

            if (separator == string::npos) {
                throw runtime_error("Invalid object format: " + hash);
            }

            return objectData.substr(separator + 1);
        }

};

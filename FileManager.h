#include "ModularLink.h"


class FileManager{
    public:
        bool exists(const string& filename) const{
            return filesystem::exists(filename);
        }

        string read_file(const string& filename) const{
            ifstream file(filename);

            if(!file.is_open()){
                throw runtime_error("Could not open file: " + filename);
            }

            return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
        }
        
        string get_file_hash(const string& filename) const{
            
            string content=read_file(filename);
            string blob_data="blob "+to_string(content.size())+'\0'+content;

            return SHA1::sha1(blob_data);
        }
};

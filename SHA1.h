#include "ModularLink.h"


class SHA1{
    private:
        // function to rotate the digits of unsigned int for hashing.
        static uint32_t left_rotate(uint32_t value, uint32_t amount){
            return (value<<amount) | (value>>(32-amount));
        }

        //function for adding padding to the data for hashing.
        static vector<uint8_t> sha1_pad(const string& input){

            vector<uint8_t> data(input.begin(), input.end());

            uint64_t original_bit_length= data.size()*8;

            // append the '1' bit followed by seven '0' bits.
            data.push_back(0x80);

            // add zero bytes until the length is 56 bytes modulo 64.
            while(data.size()%64!=56){
                data.push_back(0x00);
            }

            // append the original length as a 64-bit endian integer.
            for(int i=7; i>=0; i--){
                data.push_back(static_cast<uint8_t>((original_bit_length>>(i*8)) & 0xFF ));
            }
        return data;
        }   

        static vector<uint32_t> create_words(const vector<uint8_t>& block){
            vector<uint32_t> words(16);

            for(int i = 0; i < 16; i++){
                words[i] =
                (static_cast<uint32_t>(block[i * 4]) << 24) |
                (static_cast<uint32_t>(block[i * 4 + 1]) << 16) |
                (static_cast<uint32_t>(block[i * 4 + 2]) << 8) |
                static_cast<uint32_t>(block[i * 4 + 3]);
            }

            return words;
        }

        static void expand_words(vector<uint32_t>& words){
            words.resize(80);

            for(int t=16; t<80; t++){
                words[t]=left_rotate(words[t-3]^words[t-8]^words[t-14]^words[t-16],1);
            }
        }

        static void process_block(const vector<uint8_t>& block, uint32_t& h0, uint32_t& h1, uint32_t& h2, uint32_t& h3, uint32_t& h4){
            vector<uint32_t> words = create_words(block);
            expand_words(words);

            uint32_t A = h0;
            uint32_t B = h1;
            uint32_t C = h2;
            uint32_t D = h3;
            uint32_t E = h4;

            for (int t = 0; t < 80; t++) {

                uint32_t f;
                uint32_t K;

                if (t < 20) {
                    f = (B & C) | ((~B) & D);
                    K = 0x5A827999;
                }
                else if (t < 40) {
                    f = B ^ C ^ D;
                    K = 0x6ED9EBA1;
                }
                else if (t < 60) {
                    f = (B & C) | (B & D) | (C & D);
                    K = 0x8F1BBCDC;
                }
                else {
                    f = B ^ C ^ D;
                    K = 0xCA62C1D6;
                }

                uint32_t temp =
                    left_rotate(A, 5)
                    + f
                    + E
                    + words[t]
                    + K;

                E = D;
                D = C;
                C = left_rotate(B, 30);
                B = A;
                A = temp;
            }

            h0 += A;
            h1 += B;
            h2 += C;
            h3 += D;
            h4 += E;
        }

    public:
        static string sha1(const string& input) {

            uint32_t h0 = 0x67452301;
            uint32_t h1 = 0xEFCDAB89;
            uint32_t h2 = 0x98BADCFE;
            uint32_t h3 = 0x10325476;
            uint32_t h4 = 0xC3D2E1F0;

            vector<uint8_t> data = sha1_pad(input);

            for (size_t i = 0; i < data.size(); i += 64) {

                vector<uint8_t> block(
                    data.begin() + i,
                    data.begin() + i + 64
                );

                process_block(block, h0, h1, h2, h3, h4);
            }

            // Convert the five 32-bit values into hexadecimal.
            std::stringstream result;

            result << hex
                << setw(8) << setfill('0') << h0
                << setw(8) << setfill('0') << h1
                << setw(8) << setfill('0') << h2
                << setw(8) << setfill('0') << h3
                << setw(8) << setfill('0') << h4;

            return result.str();
        }

        
};


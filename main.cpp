// CS 356 PA1     Bolin Liu   830487624

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;

int checkArguments (char, char);



int main (int argc, char* argv[]) {
    
    // check the number of arguments
    if (argc != 6) {
        cerr << "Expect 5 Arguments!" << endl;
        cerr << "Usage: <B/S>  <inputFileName>  <OutputFileName>  <Keyfile>  <D/E>" << endl;
        return -1;
    }
    
    // check funciton and operation
    if (checkArguments (*argv[1], *argv[5]) == -1) return -1;
    
    // check input file and key file
    ifstream input_file;
    input_file.open( argv[2], ios::binary);
    //cout << argv[2] << endl;
    if (input_file.fail()) {
        cerr << "Error Opening Input File!" << endl;
        return -1;
    }
    
    ifstream key_file;
    key_file.open( argv[4], ios::binary);
    //cout << argv[4]  << endl;
    if (key_file.fail()) {
        cerr << "Error Opening Key File!" << endl;
        return -1;
    }
    
    // ******** KEY FILE OPENED *********
    
    key_file.seekg(0, ios::end);
    const int key_size = key_file.tellg();
    //   cout << "size of key file: " << key_size << endl;
    char key_buffer [key_size];
    //key_buffer [key_size] = '\0';
    key_file.seekg(0, ios::beg);
    
    key_file.read (key_buffer, key_size);
    
    
    
    
    // ************* OUTPUT TO FILE *************
    // ************* IF:     STREAM.
    // ************* ELSE:   BLOCK
    ofstream output(argv[3], ios::binary );
    if (output.fail()) {
        cerr << "Error Opening Outputfile!" << endl;
    }
    
    if (*argv[1] == 'S' || *argv[1] == 's') {
        //ofstream output(argv[3], ios::binary );
        char input_buffer [1];
        int key_index = 0;
        input_file.read(input_buffer, 1);
        while (!input_file.eof()) {
            if (key_index >= key_size) {
                key_index = 0;
            }
            
            char cipher [] = {(char) (key_buffer[key_index++] ^ input_buffer[0])};
            output.write(cipher, 1);
            input_file.read(input_buffer, 1 );
            
        }
    } else { // BLOCK CIPHER
        if (*argv[5] == 'E') {       // IF BLOCK ENCIPHER
            vector <char> output_buffer;
            char input_buffer[1];
            //char input_array[8];
            input_file.read(input_buffer, 1);
            //cout << input_buffer[0] << "********";
            int index = 0;          // really is Key_index
            while (!input_file.eof()) {
                //cout << input_buffer[0];
                if (index >= 8) {
                    index = 0;
                }
                char cipher = (char) (key_buffer[index] ^ input_buffer[0]);
                output_buffer.push_back (cipher);
                input_file.read(input_buffer, 1);
                index ++;
            }
            //         cout << "****** PADDING ******" << endl;
            while (output_buffer.size()%8 != 0) {
                int pad = 0x80;
                char cipher = (char) (key_buffer[index++] ^ (char) pad);
                output_buffer.push_back(cipher);
            }
            
            //cout << "Vector size: " << output_buffer.size() << endl;
            //            for (vector<char>::iterator it = output_buffer.begin(); it != output_buffer.end(); ++it) {
            //            output.write(&(*it),1);
            //            }
            
            index = 0;
            int beg = 0;
            int las = output_buffer.size() - 1;
            for (unsigned i = 0; i < output_buffer.size(); i++) {
                if (key_buffer[index++] % 2 != 0) {
                    char temp = output_buffer.at(beg);
                    output_buffer[beg] = output_buffer[las];
                    output_buffer[las] = temp;
                    beg ++;
                    las --;
                } else {
                    beg ++;
                }
                if (beg >= las) break;
                if (index >= 8) index = 0;
            }
            //cout << "beg: " << beg << endl;
            for (vector<char>::iterator it = output_buffer.begin(); it != output_buffer.end(); ++it) {
                output.write(&(*it),1);
            }
        } else {       // IF BLOCK DECIPHER
            vector <char> decipher_buffer;
            char buffer[1];
            input_file.read(buffer, 1);
            while (!input_file.eof()) {
                decipher_buffer.push_back(buffer[0]);
                input_file.read(buffer, 1);
            }
            //cout << "vector size: " << decipher_buffer.size() << endl;
            //            for (vector<char>::iterator it = decipher_buffer.begin(); it != decipher_buffer.end(); ++it) {
            //               cout << *it;
            //            }
            // Find the index for beg and las.
            int index = 0;
            int beg = 0;
            int las = decipher_buffer.size() - 1;
            for (unsigned i = 0; i < decipher_buffer.size(); i++) {
                if (key_buffer[index++] % 2 != 0) {
                    char temp = decipher_buffer.at(beg);
                    decipher_buffer[beg] = decipher_buffer[las];
                    decipher_buffer[las] = temp;
                    beg ++;
                    las --;
                } else {
                    beg ++;
                }
                if (beg >= las) break;
                if (index >= 8) index = 0;
            }
            //cout << "beg: " << beg << endl;
            
            
            // ******** XOR *********
            index = 0;
            for (unsigned i = 0; i < decipher_buffer.size(); i++) {
                char cipher = (char) (key_buffer[index++] ^ decipher_buffer[i]);
                if ((int) abs(cipher) != 0x80) {
                    output.write (&cipher, 1);
                }
                if (index >= 8) index = 0;
            }
            
        }
    }
    return 0;
}


// check function, cipher/decipher operation
int checkArguments(char function, char operation) {
    //char function = *argv[1];
    //cout << "Function: " << function << endl;
    if (function != 'B' && function != 'b' && function != 'S' && function != 's') {
        cerr << "Cipher Function Error! <B/S>" << endl;
        return -1;
    }
    
    // check cipher/decipher operation
    //char operation = *argv[5];
    //cout << "Operation: " << operation << endl;
    if (operation != 'D' && operation != 'E' && operation != 'd' && operation != 'e') {
        cerr << "Operation Error!" << endl;
        cerr << "Usage: <B/S>  <inputFileName>  <OutputFileName>  <Keyfile>  <D/E>" << endl;
        return -1;
    }
    
    return 0;
    
}

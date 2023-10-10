#include <algorithm>
#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"


int main(int argc, char** argv) {

    std::ifstream file; // create file object to read in data
    file.open(argv[1]); // open our sample json file
    nlohmann::json MainObject; // create object to store data in 
    if (file.is_open()) { // if the file is open, do this
        file >> MainObject; // transfer data into an object
    }  
    file.close(); // close the file to prevent memory leaks


    int arraylength = MainObject["metadata"]["arraySize"]; // create integer value for the length of the arrays to traverse
    int num_samples = MainObject["metadata"]["numSamples"]; // create integer value for number of samples to check
    nlohmann::json OutputObject; // store output data in this object
    int counter = 0; // create counter to check for number of inversions
    std::string samplenum; // create string for sample number 
    bool inversion_happened = false; // prevent multiple inversions counts with a single sample

    for (int i = 1; i < num_samples + 1; i++) { // iterate through the number of samples
        if (i >= 10) {  // formatting the sample part of the output object
                samplenum = "Sample";  // if the sample number is >= 10 then label it like so
            } else {
                samplenum = "Sample0";  // if the sample number is < 10 then label it like so
            }
            samplenum += std::to_string(i); // add the index we are at to find sample number
        for (int j = 0; j < arraylength - 1; j++) { // iterate through each sample through the entire array
            if (MainObject[samplenum][j] > MainObject[samplenum][j+1]) { // compare index and next and if it is less than continue
                inversion_happened = true; // change boolean to true to show inversion happened
                int vals[2] = { MainObject[samplenum][j], MainObject[samplenum][j+1] }; // create array to store in output object
                OutputObject[samplenum]["ConsecutiveInversions"][std::to_string(j)] = vals; // store that array in output object
                OutputObject[samplenum]["sample"] = MainObject[samplenum]; // store what sample had the inversion and put in output object
                
            }
        }
        if (inversion_happened) { // prevent multiple inversions counts in same sample
            counter++; // add to our counter of inversions
            inversion_happened = false; // reset boolean and go again with next sample
        }
    }


    OutputObject["metadata"] = MainObject["metadata"]; // add our metadata part into output object for printing
    OutputObject["metadata"]["file"] = argv[1]; // add the name of the file to the output object
    OutputObject["metadata"]["samplesWithInversions"] = counter; // add the number of samples 

    std::cout << OutputObject.dump(2) << std::endl; // print out our output object using dump to make it look nice

    return 0;
}
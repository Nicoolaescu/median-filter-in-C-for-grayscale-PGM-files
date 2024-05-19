#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

void bubbleSort(vector<uint8_t>& vect)
{
    bool swapp = true;
    while(swapp){
        swapp = false;
        for (size_t i = 0; i < vect.size()-1; i++) {
            if (vect[i] > vect[i+1] ){
                vect[i] += vect[i+1];
                vect[i+1] = vect[i] - vect[i+1];
                vect[i] -= vect[i+1];
                swapp = true;
            }
        }
    }
}

void merge_sort(std::vector<uint8_t>& array) {
    if (1 < array.size()) {
        std::vector<uint8_t> array1(array.begin(), array.begin() + array.size() / 2);
        merge_sort(array1);
        std::vector<uint8_t> array2(array.begin() + array.size() / 2, array.end());
        merge_sort(array2);
        merge(array1.begin(),array1.end(),array2.begin(),array2.end(),array.begin());
    }
}

bool is_out_of_bounds(unsigned x, unsigned y, unsigned border, unsigned height, unsigned width){
    if(x < border / 2 || y < border / 2) return true;
    if(x + border / 2 + 1 > height || y + border / 2 + 1 > width) return true;

    return false;
}

vector<uint8_t> window(unsigned x, unsigned y, unsigned border, vector<vector<uint8_t>> &image){
    vector<uint8_t> wndw;

    for(unsigned i = (x - border / 2) ; i <= x + border / 2; i++ ){
        for (unsigned j = (y - border / 2); j <= y + border / 2; j++){
            wndw.push_back(image[i][j]);
        }
    }

    return wndw;
}

void citire_img(ifstream &file_in,unsigned &height, unsigned &width, unsigned &max_color_value, vector<vector<uint8_t>> &image){

    string line;
    unsigned counter = 0;

    while(counter < 3){
        getline(file_in,line);

        if(line[0] != '#'){
            stringstream line_stream(line);

            if(counter == 1) line_stream >> width >> height;
            else if (counter == 2) line_stream >> max_color_value;

            counter++;
        }

    }

    for (unsigned i = 0; i < height; i ++){

        image.emplace_back();

        for(unsigned j = 0; j < width; j++){

            unsigned pixel;
            file_in >> pixel;

            image[i].push_back(uint8_t(pixel));

        }
    }

}

void scriere_img(ofstream &file_out,unsigned height, unsigned width, unsigned max_color_value, vector<vector<uint8_t>> &image){

    file_out << "P2" << endl;
    file_out << width << ' ' << height << endl;
    file_out << max_color_value << endl;



    for (unsigned i = 0; i < height; i ++){

        file_out << endl;

        for(unsigned j = 0; j < width; j++){

            file_out << (unsigned)image[i][j] << ' ';

        }
    }
}

void bordered_img(unsigned &border, unsigned &height, unsigned &width ,vector<vector<uint8_t>> &image, vector<vector<uint8_t>> &new_image){
    unsigned surplus = border / 2;

    for(unsigned i = 0; i < height + border - 1; i++){

        new_image.emplace_back();

        for(unsigned j = 0; j < width + border - 1; j++){

            if (i < surplus && j < surplus) new_image[i].push_back(image[0][0]);
            else if (i < surplus && j > width + surplus - 1) new_image[i].push_back(image[0][width - 1]);
            else if (i > height + surplus - 1 && j < surplus) new_image[i].push_back(image[height - 1][0]);
            else if (i > height + surplus - 1 && j > width + surplus - 1) new_image[i].push_back(image[height - 1][width - 1]);
            else if (i < surplus) new_image[i].push_back(image[0][j - surplus]);
            else if (i > height + surplus - 1) new_image[i].push_back(image[height - 1][j - surplus]);
            else if (j < surplus) new_image[i].push_back(image[i - surplus][0]);
            else if (j > width + surplus - 1) new_image[i].push_back(image[i - surplus][width - 1]);
            else new_image[i].push_back(image[i - surplus][j - surplus]);

        }
    }


}


int main() {

    string algo;
    cin >> algo;

    unsigned border;
    cin >> border;

    string file;
    cin >> file;
    ifstream file_in(file);

    cin >> file;
    ofstream file_out(file);

    unsigned width, height;
    unsigned max_color_value;

    vector<vector<uint8_t>> image;
    vector<vector<uint8_t>> new_image;
    vector<vector<uint8_t>> repaired_image;

    citire_img(file_in,height,width,max_color_value,image);

    bordered_img(border,height,width,image,new_image);

    for (unsigned i = 0; i < height; i++){

        repaired_image.emplace_back();

        for(unsigned j = 0; j < width; j++){

            vector<uint8_t> wndw = window(i + border / 2, j + border / 2, border, new_image);

            if(algo == "merge"){
                merge_sort(wndw);

            }else if(algo == "bubble"){
                bubbleSort(wndw);
            }

            repaired_image[i].push_back(wndw[wndw.size() / 2 + 1]);

        }

    }

//    for(unsigned i = 0 ; i < height; i++){
//
//        new_image.emplace_back();
//
//        for(unsigned j = 0; j < width; j++){
//
//            if(!is_out_of_bounds(i, j, border, height, width)){
//
//                vector<uint8_t> wndw = window(i,j,border,image);
//
//                if(algo == "merge"){
//                    merge_sort(wndw);
//
//                }else if(algo == "bubble"){
//                    bubbleSort(wndw);
//
//                }
//
//                new_image[i].push_back(wndw[wndw.size() / 2 + 1]);
//
//            } else {
//                new_image[i].push_back(image[i][j]);
//            }
//
//        }
//
//    }

    scriere_img(file_out, height, width, max_color_value, repaired_image);




    return 0;
}

// bubble 3 glassware_noisy_in.pgm glassware_noisy_out.pgm
// merge 3 lena_noise_in.pgm lena_noise_out.pgm
// merge 3 lena_noise2_in.pgm lena_noise2_out.pgm
// merge 3 test_in.pgm test_out.pgm


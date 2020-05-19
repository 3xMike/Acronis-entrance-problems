#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

#include <conio.h>
#include <math.h>
using namespace std;

int k=0;


class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str, line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }

            if (!lineStream && cell.empty())
            {

                m_data.push_back("");
            }
        }
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}


long double func_average(CSVRow head, CSVRow row){
    k++;
    vector<float>   quantity;
    vector<int>   request_duration;
    double sred=0;
    for (int i =2; i< row.size(); i++){
        if (k<2594){

            quantity.push_back(stof(row[i]));

        }
        else{
            quantity.push_back(0);
        }
        request_duration.push_back(stoi(head[i]));

    }
    sort(quantity.begin(), quantity.end());
    sort(request_duration.begin(), request_duration.end());

    vector<float>  current_quantity;
    current_quantity.push_back(quantity[0]);
    for (int i=1;i< quantity.size();i++ ){
        current_quantity.push_back(quantity[i] - quantity[i-1]);
    }
    double sum=0;

    for (int i=0; i< current_quantity.size(); i++)
        sum+=double(current_quantity[i])*double(request_duration[i]);
    if (row[1]!="")  {
        sred=sum / stoi(row[1]);
    }
    return sred;
}

double dispersion (double m, CSVRow head, CSVRow row){



//    k++;
    vector<float>   quantity;
    vector<int>   request_duration;
    double disp=0;
    for (int i =2; i< row.size(); i++){
        if (k<2594){
            quantity.push_back(stof(row[i]));
        }
        else{
            quantity.push_back(0);
        }
        request_duration.push_back(stoi(head[i]));

    }
    sort(quantity.begin(), quantity.end());
    sort(request_duration.begin(), request_duration.end());

    vector<float>  current_quantity;

    for (int i=1;i< quantity.size();i++ ){
        current_quantity.push_back(quantity[i] - quantity[i-1]);
    }
    double sum=0;
    for (int i=0; i< current_quantity.size(); i++)
        sum+= current_quantity[i] * (request_duration[i] - m) * (request_duration[i] - m);
    if (k<2594)  {
        disp=sum / stoi(row[1]);
    }

    return disp;

}
int main()
{
    std::ifstream file("/Users/user/source/repos/ms_bucket.csv"); //Your path
    CSVRow row;
    CSVRow description;
    vector<time_t> time;
    vector<float>  average;
    vector<double>  dispers;
    double current_average = 0;
    double current_disp = 0;
    while (file >> row)
    {
        if (row[0] == "timestamp")
            description = row;
        else {
            current_average = func_average(description, row);
            current_disp = dispersion(current_average, description, row);
            time.push_back((time_t)stoi(row[0]));
            average.push_back(current_average);
            dispers.push_back(current_disp);
        }
    }
    vector<time_t>::iterator ti = time.begin();
    vector<float>::iterator av = average.begin();
    vector<double>::iterator di = dispers.begin();
    int count = 0;
    while (count < 2593) {
        count++;
        cout << "time = " << *ti << ", average = " << *av << ", disp = " << *di << endl;
        ti++;
        av++;
        di++;
    }

    return 0;
}

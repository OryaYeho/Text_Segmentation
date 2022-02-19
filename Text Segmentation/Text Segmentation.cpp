// Text Segmentation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <list>

#define SYMBOLS_AMOUNT 4
#define WORD_SIZE 3
#define MIN_BLOCK_SIZE 4
#define MAX(a,b) (((a)>(b))?(a):(b))

using namespace std;


void segmentation(string fileName) {
    ifstream f_in;
    ofstream f_out;

    int counters[SYMBOLS_AMOUNT][WORD_SIZE];
    int counters_2[SYMBOLS_AMOUNT][WORD_SIZE];
    char tempWords[MIN_BLOCK_SIZE][WORD_SIZE];
    char ch, chMax;
    int objectiveFunction, objectiveFunctionTemp;
    int sum = 0, i_chMax;
    int repetitions = 0;
    list<int> confines;
    char nameDocForm[40];


    f_in.open( (fileName + ".fasta"), ios::in);
    f_out.open(fileName + "_out.txt", ios::out);

    confines.push_back(0);

 
    for (int i = 0; i < SYMBOLS_AMOUNT; i++) {
        for (int j = 0; j < WORD_SIZE; j++) {
            counters[i][j] = 0;
            //counters_2[i][j] = 0;
        }
    } 

    for (int i = 0; (i < MIN_BLOCK_SIZE) && f_in.good(); i++) {
        for (int position = 0; (position < WORD_SIZE) && f_in.good(); position++) {

            ch = f_in.get();

            switch (ch) {
                case 'A':
                    counters[0][position]++;
                    f_out << ch;
                    break;
                case 'C':
                    counters[1][position]++;
                    f_out << ch;
                    break;
                case 'G':
                    counters[2][position]++;
                    f_out << ch;
                    break;
                case 'T':
                    counters[3][position]++;
                    f_out << ch;
                    break;
                case '\n':
                    position--;
                    break;
                default:
                    f_out << "?";
            }
        }

        repetitions++;
        //f_out << "\t";
        f_out << " ";
    }

    for (int i = 0; i < SYMBOLS_AMOUNT; i++)
        for (int j = 0; j < WORD_SIZE; j++)
            counters_2[i][j] = counters[i][j];

    while (f_in.good()) {
        repetitions = MIN_BLOCK_SIZE;    // repetitions = read WORD_SIZE = MIN_BLOCK_SIZE chars

        sum = MAX(MAX(counters[0][0], counters[1][0]), MAX(counters[2][0], counters[3][0]));
        sum += MAX(MAX(counters[0][1], counters[1][1]), MAX(counters[2][1], counters[3][1]));
        sum += MAX(MAX(counters[0][2], counters[1][2]), MAX(counters[2][2], counters[3][2]));
        objectiveFunction = sum / repetitions;

        for (int i = 0; (i < MIN_BLOCK_SIZE) && f_in.good(); i++) {
            for (int position = 0; (position < WORD_SIZE) && f_in.good(); position++) {
                ch = f_in.get();

                switch (ch) {
                    case 'A':
                        counters_2[0][position]++;
                        tempWords[i][position] = ch;
                        break;
                    case 'C':
                        counters_2[1][position]++;
                        tempWords[i][position] = ch;
                        break;
                    case 'G':
                        counters_2[2][position]++;
                        tempWords[i][position] = ch;
                        break;
                    case 'T':
                        counters_2[3][position]++;
                        tempWords[i][position] = ch;
                        break;
                    case '\n':
                        position--; 
                        break;
                    default:
                        tempWords[i][position] = '?' ;
                }
            }

            sum = MAX(MAX(counters_2[0][0], counters_2[1][0]), MAX(counters_2[2][0], counters_2[3][0]));
            sum += MAX(MAX(counters_2[0][1], counters_2[1][1]), MAX(counters_2[2][1], counters_2[3][1]));
            sum += MAX(MAX(counters_2[0][2], counters_2[1][2]), MAX(counters_2[2][2], counters_2[3][2]));
            objectiveFunctionTemp = sum / repetitions;
            
            if (objectiveFunctionTemp > objectiveFunction) {
                repetitions++;
                for (int x = 0; x < SYMBOLS_AMOUNT; x++)
                    for (int t = 0; t < WORD_SIZE; t++)
                        counters[x][t] = counters_2[x][t];

                for (int x = 0; x <= i; x++)
                    f_out << tempWords[x][0] << tempWords[x][1] << tempWords[x][2] << " ";
                i = -1;
            }
        }

        f_out << "\t\t => *";
        for (int i = 0; i < WORD_SIZE; i++) {
            i_chMax = 0;
            for (int s = 1; s < SYMBOLS_AMOUNT; s++)
            {
                if (counters[i_chMax][i] < counters[s][i])
                    i_chMax = s;
            }

            switch (i_chMax) {
            case 0: chMax = 'A'; break;
            case 1: chMax = 'C'; break;
            case 2: chMax = 'G'; break;
            case 3: chMax = 'T'; 
            }
            f_out << chMax;
        }
        f_out << "*\n";

        confines.push_back(repetitions);
        //repetitions == MIN_BLOCK_SIZE;

        for (int i = 0; i < SYMBOLS_AMOUNT; i++) {
            for (int j = 0; j < WORD_SIZE; j++) {
                counters[i][j] = counters_2[i][j] - counters[i][j];
                counters_2[i][j] = counters[i][j];
            }
        }

        for (int x = 0; x < MIN_BLOCK_SIZE; x++) {
            for (int t = 0; t < WORD_SIZE; t++) {
                f_out << tempWords[x][t];
            }
            f_out << " ";
        }
       
    }


    std::cout << "Segmentation was performed on file " << fileName << 
        " and the output in " << fileName << "_out.txt\n";
}

int main()
{  
    const char *fName[4] = { "covid", "hiv", "Mers", "sars"};
    for (int i = 0; i < 4; i++)
        segmentation(fName[i]);
}
#include <iostream>
#include <fstream>
#include <string>
#include<time.h>
#include<stdlib.h>
#include <chrono>
using namespace std;
using namespace std::chrono;


ifstream readFile() //wczytuje podany plik
{
    string name, data;

    cout << "Enter the name of the instance you wish to read:\n";
    cin >> name;

    ifstream file(name);

    file >> data; //wczytanie nazwy pliku zawartej w środku

    if (data != name) //sprawdza czy nazwa podana przez użytkownika jest taka sama jak nazwa pliku który został wczytany
        cout << "Error: Couldn't read the file!\n";

    else
        cout << "File loaded succesfully!\n";

    return file;
}

int** edgeMatrix(ifstream& file, int n, int** matrix) {

    string data;

    for (int i = 0; i < n; i++) {      //wypełnienie tabeli 2D macierzą krawędzi
        for (int j = 0; j < n; j++) {
            file >> data;
            matrix[i][j] = stoi(data);      //tworzenie macierzy, stoi(data)-konwersja string do int
        }
    }

    return matrix;
}

double calculatePRD(int hamilton, int optimum)
{
    double prd = 100 * (double)hamilton / (double)optimum;
    return prd;
}
int* nearestNeighbour(int** matrix, int n, int optimum) {

    int* met = new int[n]; //dynamiczna tablica na już sprawdzone elementy
    int* toCheck = new int[n]; //dynamiczna tablica z elementami które jeszcze są do sprawdzenia

    for (int i = 0; i < n; i++) {//wypełnienie tablicy 1 - żaden element nie został sprawony
        toCheck[i] = 1;          //gdy sprawdzi to w miejsce 1 wstawia 0
    }

    int curr, min = 1000, minPos, hamilton = 0;
    curr = 0;               //element startowy
    met[0] = curr;              //elem startowy doodany do odwiedzonych
    toCheck[curr] = 0;          //elem startowy usuwany z tablicy do sprawdzenia
    for (int j = 1; j < n; j++) { //pętla idzie po kolei przez macierz krawedzi i szuka min. dla kolejnych elementów

        min = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (matrix[curr][i] < min && toCheck[i] != 0) {
                min = matrix[curr][i];
                minPos = i;

            }
        }
        toCheck[minPos] = 0;
        met[j] = minPos;
        hamilton += min;
        curr = minPos;
    }

    hamilton += matrix[met[0]][minPos];

    return met;
}
int* swap_random(int* base, int n) {
    //generuje 2 różne liczby losowe
    int rand_x = rand() % n;
    int rand_y = rand() % n;
    int* temp = new int[n];


    //zapisuje wartosci z base do temp
    for (int i = 0; i < n; i++) {
        temp[i] = base[i];
    }
    //zamienia wartosci o 2 losowych indeksach w temp
    temp[rand_x] = base[rand_y];
    temp[rand_y] = base[rand_x];
    return temp;



}
int* invert_random(int* base, int n) {
    int rand_x = rand() % n;
    int rand_y = rand() % n;
    int* temp = new int[n];

    //zapisuje wartosci z base do temp
    for (int i = 0; i < n; i++) {
        temp[i] = base[i];
    }
    //jesli Y wieksze od X, to iteruje od X do Y jednocześniej idąc od końca indeksem j
    if (rand_y > rand_x) {
        int j = rand_y;
        for (int i = rand_x; i <= rand_y; i++) {
            temp[i] = base[j];
            j--;
        }
    }
    //jeśli X > Y to iteruje od Y do X jednocześnie idąc od końca indeksem j
    else {
        int j = rand_x;
        for (int i = rand_y; i <= rand_x; i++) {
            temp[i] = base[j];
            j--;
        }
    }

    return temp;
}
int* insert_random(int* base, int n) {
    int rand_x = rand() % n;
    int rand_y = rand() % n;;
    while (rand_y == rand_x)
        rand_y = rand() % n;

    int* temp = new int[n];

    //zapisuje wartosci z base do temp
    for (int i = 0; i < n; i++) {
        temp[i] = base[i];
    }

    int index = temp[rand_x];
    if (rand_y > rand_x) {
        for (int i = rand_x; i < rand_y; i++) {
            temp[i] = temp[i + 1];
        }
        temp[rand_y] = index;
    }
    else {
        for (int i = rand_x; i > rand_y; i--) {
            temp[i] = temp[i - 1];
        }
        temp[rand_y] = index;
    }

    return temp;
}
int calculateTour(int** matrix, int n, int* result_matrix) {
    int tour = 0;
    for (int i = 1; i < n; i++) {
        tour += matrix[result_matrix[i - 1]][result_matrix[i]];
    }
    tour += matrix[result_matrix[n - 1]][result_matrix[0]];
    return tour;
}
void bubbleSort(int** population, int* population_cost, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 1; j < n - i; j++) //pętla wewnętrzna
            if (population_cost[j - 1] > population_cost[j]) {
                //zamiana miejscami
                int temporary = population_cost[j - 1];
                population_cost[j - 1] = population_cost[j];
                population_cost[j] = temporary;
                int* temp = population[j - 1];
                population[j - 1] = population[j];
                population[j] = temp;
            }
}
int* crossing(int** population, int rand_x, int rand_y, int n) {
    bool* checkIfVisited = new bool[n];
    int* result = new int[n];
    int tmp = rand() % n;
    for (int i = 0; i < n; i++) {
        checkIfVisited[i] = false;
        result[i] = 0;
    }

    for (int i = 0; i < tmp; i++) {
        result[i] = population[rand_x][i];
        checkIfVisited[result[i]] = true;
    }
    for (int i = 0; i < n; i++) {
        if (checkIfVisited[population[rand_y][i]] != true) {
            result[tmp] = population[rand_y][i];
            checkIfVisited[population[rand_y][i]] = true;
            tmp++;
        }
    }
    return result;
}
int geneticAlgorithm(int** matrix, int** population, int n, int optimum, int pop_size, int iterations, int pop_child, int pop_mut) {
    for (int i = 0; i < n; i++) {
        population[0][i] = i;
    }
    population[0] = insert_random(population[0], n);
    int bestTour = calculateTour(matrix, n, population[0]);
    int rand_x, rand_y;
    int* population_cost = new int[pop_size + pop_child];
    population_cost[0] = bestTour;
    cout << "Start " << bestTour << " (" << calculatePRD(bestTour, optimum) << "%)\n";
    /*
        wygenerowanie początkowej populacji
    */
    for (int i = 1; i < pop_size; i++) {
        rand_x = rand() % 3;
        switch (rand_x % 3) {
        case 0:
            population[i] = swap_random(population[i - 1], n);
            population_cost[i] = calculateTour(matrix, n, population[i]);


            break;
        case 1:
            population[i] = invert_random(population[i - 1], n);
            population_cost[i] = calculateTour(matrix, n, population[i]);

            break;
        case 2:
            population[i] = insert_random(population[i - 1], n);
            population_cost[i] = calculateTour(matrix, n, population[i]);

            break;
        default:
            population[i] = insert_random(population[i - 1], n);
            population_cost[i] = calculateTour(matrix, n, population[i]);
            break;
        }
    }

    /*
    główna pętla
    */

    for (int i = 0; i < iterations; i++) {

        /*
        generowanie potomków
        */
        for (int j = 0; j < pop_child; j++) {
            rand_x = rand() % (pop_size + j);
            rand_y = rand() % (pop_size + j);
            while (rand_y == rand_x) {
                rand_y = rand() % (pop_size + j);
            }
            population[pop_size + j] = crossing(population, rand_x, rand_y, n);
            population_cost[pop_size + j] = calculateTour(matrix, n, population[pop_size + j]);

        }

        /*
            mutacje
        */
        for (int j = 0; j < pop_mut; j++) {
            rand_x = rand() % (pop_size + pop_child);
            population[rand_x] = swap_random(population[rand_x], n);
            population_cost[rand_x] = calculateTour(matrix, n, population[rand_x]);
        }

        bubbleSort(population, population_cost, pop_size + pop_child);
        if (population_cost[0] < bestTour) {
            cout << population_cost[0] << " (" << calculatePRD(population_cost[0], optimum) << "%) \n";
            bestTour = population_cost[0];
        }

    }
    cout << population[0][0];
    for (int i = 1; i < n; i++) {
        cout << "->" << population[0][i];
    }
    cout << endl;
    return 0;
}
int main() {

    srand(time(NULL));
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    string data;
    ifstream file;
    int n, optimum;

    file = readFile(); //wczytanie wybranego pliku
    file >> data;     //wczytanie liczby miast z pliku
    n = stoi(data); // zmiana liczby miast ze string na int
    int** matrix = new int* [n];//tworzenie dynamicznej tablicy 2 wymiarowej o wymiarach n x n
    for (int i = 0; i < n; i++) { //do któej wprowadzimy macierz krawędzi
        matrix[i] = new int[n];
    }
    matrix = edgeMatrix(file, n, matrix);// wczytanie macierzy z pliku
    int pop_size, iterations, pop_child, pop_mut;
    cout << "\n Initial population size:";
    cin >> pop_size;
    cout << "\n How many iterations:";
    cin >> iterations;
    cout << "\n How many crossovers per iteration:";
    cin >> pop_child;
    cout << "\n How many mutations per iteration:";
    cin >> pop_mut;

    int** population = new int* [pop_size + iterations];
    for (int i = 0; i < pop_size + iterations; i++) {
        population[i] = new int[n];
    }


    system("PAUSE");

    file >> optimum;

    start = high_resolution_clock::now();
    geneticAlgorithm(matrix, population, n, optimum, pop_size, iterations, pop_child, pop_mut);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "\nCalculation time: " << (double)duration.count() / 1000 << "ms" << endl;
    system("PAUSE");


    file.close();
    system("PAUSE");
    delete matrix;
    return 0;
}
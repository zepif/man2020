#include<time.h>
#include<math.h>
#include<iostream>
#include<ctime>
using namespace std;
//#define double long double
struct neuron
{
    double value;
    double error;
    void act()   /// функция активации
    {
        value = (1 / (1 + pow(2.71828, (-value))));
        //value = tg(value);
    }
};

class network
{
public:
    int layers;/// количество слоев
    neuron** neurons; /// массив нейронов
    double*** weights; /** массив синапсов,
    [1] слой выходного нейрона
    [2] означает положение нейрона в слою
    [3] положение нейрона в следующем слою
    **/
    int* size; /// количество нейронов в каждом слою
    double sigm_pro(double x)
    {
        double res = x * (1.0 - x);
        return res;
    }
    void setLayersNotStudy(int n, int* p, string filename)  /// загружение весов
    {
        ifstream fin;
        fin.open(filename);
        //srand(time(0));
        layers = n;
        neurons = new neuron * [n];
        weights = new double** [n - 1];
        size = new int[n];
        for (int i = 0; i < n; i++)
        {
            size[i] = p[i];
            neurons[i] = new neuron[p[i]];
            if (i < n - 1)
            {
                weights[i] = new double* [p[i]];
                for (int j = 0; j < p[i]; j++)
                {
                    weights[i][j] = new double[p[i + 1]];
                    for (int k = 0; k < p[i + 1]; k++)
                    {
                        fin >> weights[i][j][k];
                    }
                }
            }
        }
    }
    void setLayers(int n, int* p)   /// генерация случайных весов
    {
        std::mt19937 gen;  /// вихрь Мерсенна
        gen.seed(time(0));
       // srand(time(0));
        layers = n;
        neurons = new neuron * [n];
        weights = new double** [n - 1];
        size = new int[n];
        for (int i = 0; i < n; i++)
        {
            size[i] = p[i];
            neurons[i] = new neuron[p[i]];
            if (i < n - 1)
            {
                weights[i] = new double* [p[i]];
                for (int j = 0; j < p[i]; j++)
                {
                    weights[i][j] = new double[p[i + 1]];
                    for (int k = 0; k < p[i + 1]; k++)
                    {
                        weights[i][j][k] = ((gen() % 100)) * 0.01 / size[i];
                    }
                }
            }
        }
    }
    void set_input(double p[])   /// запись входных данных во входной слой
    {
        for (int i = 0; i < size[0]; i++)
        {
            neurons[0][i].value = p[i];
        }
    }


    void LayersCleaner(int LayerNumber, int start, int stop)   /// отчистка значений нейронов
    {
        for (int i = start; i < stop; i++)
        {
            neurons[LayerNumber][i].value = 0;
        }
    }

    void ForwardFeeder(int LayerNumber, int start, int stop)   /// подсчет значений в нейронах
    {
        for (int j = start; j < stop; j++)
        {
            for (int k = 0; k < size[LayerNumber - 1]; k++)
            {
                neurons[LayerNumber][j].value += neurons[LayerNumber - 1][k].value * weights[LayerNumber - 1][k][j];
            }
            neurons[LayerNumber][j].act();
        }
    }

    double ForwardFeed()   /// вычесление ответа
    {
        setlocale(LC_ALL, "ru");
        for (int i = 1; i < layers; i++)
        {
            LayersCleaner(i, 0, size[i]);
            ForwardFeeder(i, 0, size[i]);
        }
        double max = -10000;
        double prediction = 0;
        for (int i = 0; i < size[layers - 1]; i++)
        {

            if (neurons[layers - 1][i].value > max)
            {
                max = neurons[layers - 1][i].value;
                prediction = i;
            }
        }
        return prediction;
    }

     double ForwardFeed(string param)   /// вычесление ответа
    {
        setlocale(LC_ALL, "ru");
        for (int i = 1; i < layers; i++)
        {
            LayersCleaner(i, 0, size[i]);
            ForwardFeeder(i, 0, size[i]);
        }
        double max = -10000;
        double prediction = 0;
        for (int i = 0; i < size[layers - 1]; i++)
        {
            cout << neurons[layers - 1][i].value << " " << rek[i] << endl;
            if (neurons[layers - 1][i].value > max)
            {
                max = neurons[layers - 1][i].value;
                prediction = i;
            }
        }
        return prediction;
    }

    void ErrorCounter(int LayerNumber, int start, int stop, double prediction, double rresult, double lr)   
    {
        if (LayerNumber == layers - 1)
        {
            for (int j = start; j < stop; j++)
            {
                if (j != int(rresult))
                {
                    neurons[LayerNumber][j].error = -neurons[LayerNumber][j].value;
                }
                else
                {
                    neurons[LayerNumber][j].error = 1.0 - neurons[LayerNumber][j].value;
                }
            }
        }
        else
        {
            for (int j = start; j < stop; j++)
            {
                double error = 0.0;
                for (int k = 0; k < size[LayerNumber + 1]; k++)
                {
                    error += neurons[LayerNumber + 1][k].error * weights[LayerNumber][j][k];
                }
                neurons[LayerNumber][j].error = error;
            }
        }

    }

    void WeightsUpdater(int start, int stop, int LayerNum, double lr)
    {
        int i = LayerNum;
        for (int j = start; j < stop; j++)
        {
            for (int k = 0; k < size[i + 1]; k++)
            {
                weights[i][j][k] += lr * neurons[i + 1][k].error * sigm_pro(neurons[i + 1][k].value) * neurons[i][j].value;
            }
        }
    }

    void BackPropogation(double prediction, double rresult, double lr)
    {
        for (int i = layers - 1; i > 0; i--)
        {
            ErrorCounter(i, 0, size[i], prediction, rresult, lr);
       }
        for (int i = 0; i < layers - 1; i++)
        { 
            WeightsUpdater(0, size[i], i, lr);
        }
    }

    bool SaveWeights()   /// сохранение весов
    {
        ofstream fout;
        fout.open("weights.txt");
        for (int i = 0; i < layers; i++)
        {
            if (i < layers - 1)
            {
                for (int j = 0; j < size[i]; j++)
                {
                    for (int k = 0; k < size[i + 1]; k++)
                    {
                        fout << weights[i][j][k] << " ";
                    }
                }
            }
        }
        fout.close();
        return 1;
    }
    double CountError(int start, int stop, int LayerNum)
    {
        double error = 0.0;
        for (int i = start; i < stop; i++)
            error += neurons[LayerNum][i].error * neurons[LayerNum][i].error;
        return error;
    }
};

//mdyq6440

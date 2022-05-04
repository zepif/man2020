#include <bits/stdc++.h>
using namespace std;
#pragma GCC optimization("O3")
#pragma GCC optimization("unroll-loops")
map<string, double> job, marital, education, def, month, day, rec, genn;
map<double, string> rek;
#include "network.h"
struct data_one {
    double info[9];
    double rresult;
    int ind;
};
int myrand(int i) {
    std::mt19937 gen; /// вихрь Мерсенна
    gen.seed(time(0));
    return gen() % i;
}
void tr_ed() {
    education["basic.4y"] = 3;
    education["basic.6y"] = 3;
    education["basic.9y"] = 3;
    education["high.school"] = 4;
    education["illiterate"] = 5;
    education["professional.course"] = 6;
    education["university.degree"] = 7;
    education["unknown"] = 8;
}
void tr_mat() {
    marital["divorced"] = 1;
    marital["married"] = 2;
    marital["single"] = 3;
    marital["unknown"] = 4;
}
void tr_gen() {
    genn["man"] = 1;
    genn["woman"] = 2;
    genn["unknown"] = 3;
}
void tr_job() {
    job["admin."] = 1;
    job["blue-collar"] = 1;
    job["entrepreneur"] = 3;
    job["housemaid"] = 4;
    job["management"] = 5;
    job["retired"] = 6;
    job["self-employed"] = 7;
    job["services"] = 8;
    job["student"] = 9;
    job["technician"] = 10;
    job["unemployed"] = 11;
    job["unknown"] = 13;
}
void tr_def() {
    def["no"] = 1;
    def["yes"] = 2;
    def["unknown"] = 3;
}
void tr_mon() {
    month["jan"] = 1;
    month["feb"] = 2;
    month["mar"] = 3;
    month["apr"] = 4;
    month["may"] = 5;
    month["jun"] = 6;
    month["jul"] = 7;
    month["aug"] = 8;
    month["sep"] = 9;
    month["oct"] = 10;
    month["nov"] = 11;
    month["dec"] = 12;
    month["unknown"] = 13;
}
void tr_day() {
    day["mon"] = 1;
    day["tue"] = 2;
    day["wed"] = 3;
    day["thu"] = 4;
    day["fri"] = 5;
    day["unknown"] = 6;
}
void tr_rek() {
    rek[5] = "policy";
    rek[1] = "technique";
    rek[2] = "games";
    rek[3] = "music";
    rek[4] = "internet-services";
    rek[0] = "fashion";
    rek[6] = "rest";
    rek[7] = "education";
    for (int i = 0; i < 8; i++)
        rec[rek[i]] = i;
}
int main() {
    // srand(static_cast<unsigned int>(time(0)));
    setlocale(LC_ALL, "Russian");
    ifstream fin;
    ofstream fout;
    fout.open("log.txt");
    tr_ed();
    tr_mat();
    tr_job();
    tr_def();
    tr_gen();
    tr_mon();
    tr_rek();
    tr_day();
    const int l = 3;        /// количество слоев в сети
    const int input_l = 10; /// количество входных нейронов
    int size[l] = { input_l, 120, 8 }; /// массив у которого каждый индекс означает кол-во нейронов в слою
    network nn;
    double input[input_l];
    double rresult;
    double result;
    double ra = 0;
    int maxra = 0;
    int maxraepoch = 0;
    const int n = 900;                /// количество тестов
    data_one* data = new data_one[n]; /// массив тестов
    bool to_study = 0;
    cout << "Производить обучение?";
    cin >> to_study;
    double time = 0;

    if (to_study) {
        fin.open("lib.da");         /// файл с тестами
        for (int i = 0; i < n; i++) /// считывание данных
        {
            fin >> data[i].info[0];
            string s;
            fin >> s;
            data[i].info[1] = job[s];
            fin >> s;
            data[i].info[2] = marital[s];
            fin >> s;
            data[i].info[3] = education[s];
            fin >> s;
            data[i].info[4] = def[s];
            fin >> s;
            data[i].info[5] = genn[s];
            fin >> s;
            data[i].info[6] = def[s];
            fin >> s;
            data[i].info[7] = month[s];
            fin >> s;
            data[i].info[8] = day[s];
            fin >> s;
            data[i].rresult = rec[s];
            // for (int j = 0;j < 8; j++)cout << data[i].info[j] << " ";cout <<
            // data[i].rresult << endl;
        }
        // for (int k = 0; k < 100; k++)
        random_shuffle(data, data + n, myrand);

        nn.setLayers(l, size); /// задаем случайные веса
        for (int e = 0; ra / double(n) < 1.0; e++) {
            fout << "Epoch # " << e << "\t\t\tRa #  " << ra / n * 100 << endl;
            ra = 0;
            double w_delta = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < 9; j++) {
                    input[j] = data[i].info[j]; /// ввод тестов
                }
                rresult = data[i].rresult;
                nn.set_input(input); /// ввод теста в нейронную сеть

                result = nn.ForwardFeed(); /// вычесляем ответ

                if (result == rresult) /// сравниваем ответы, если ответ
                                       /// правильный, то выводим его
                {
                    // cout << " " << rek[result] << "\t\t\t****" << endl;
                    ra++;
                } else {
                    nn.BackPropogation(result, rresult,
                                       0.01); /// если ответ неверный, то корректируем веса
                }
            }
            cout << "Right answers: " << ra / n * 100 << "% \t Max RA: " << double(maxra) / n * 100 << "(epoch "
                 << maxraepoch << " )" << endl; /// вывод процент максимальных ответов
            if (ra > maxra) {
                maxra = ra;
                maxraepoch = e;
            }
        }
        if (nn.SaveWeights()) {
            cout << "Веса сохранены!";
        }
    } else {
        nn.setLayersNotStudy(l, size, "weights.txt");
    }
    fin.close();
    cout << "Начать тест:(1/0) ";
    bool to_start_test = 0;
    cin >> to_start_test;
    char right_res;
    if (to_start_test) {
        fin.open("test.da");
        fin >> input[0];
        string s;
        fin >> s;
        input[1] = job[s];
        fin >> s;
        input[2] = marital[s];
        fin >> s;
        input[3] = education[s];
        fin >> s;
        input[4] = def[s];
        fin >> s;
        input[5] = genn[s];
        fin >> s;
        input[6] = def[s];
        fin >> s;
        input[7] = month[s];
        fin >> s;
        input[8] = day[s];
        nn.set_input(input);
        result = nn.ForwardFeed(string("show results"));
        cout << "Какой ответ на самом деле?";
        cin >> s;
        int right_res = rec[s];
        if (right_res != result) {
            cout << "Исправляю ошибку!";

            nn.BackPropogation(result, right_res, 0.005);
            nn.SaveWeights();
        }
    }

    cout << "Начать проверку: ";
    bool start_universal = 0;
    cin >> start_universal;

    if (start_universal) {
        fin.close();
        int length;
        int rightAnswers = 0;
        cout << "Введите кол-во примеров в тесте: ";
        cin >> length;
        fin.open("check.txt");
        for (int i = 0; i < length; i++) {
            fin >> input[0];
            string s;
            fin >> s;
            input[1] = job[s];
            fin >> s;
            input[2] = marital[s];
            fin >> s;
            input[3] = education[s];
            fin >> s;
            input[4] = def[s];
            fin >> s;
            input[5] = genn[s];
            fin >> s;
            input[6] = def[s];
            fin >> s;
            input[7] = month[s];
            fin >> s;
            input[8] = day[s];
            fin >> s;
            right_res = rec[s];
            // cout << s << endl;
            nn.set_input(input);
            // result = nn.ForwardFeed((string)"write answer");
            result = nn.ForwardFeed();
            if (right_res == result) {
                rightAnswers++;
            }
        }
        cout << "Тест пройден! Процент правильных ответов: " << double(rightAnswers) / double(length) * 100 << " %\n";
    }
    return 0;
}

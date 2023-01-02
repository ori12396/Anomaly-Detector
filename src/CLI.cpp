#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->menu = vector<Command*>();
    info = new SharedInfo();

    Command* c1 = new CommandUploadTimeSeries(dio, info);
    menu.push_back(c1);
    Command* c2 = new CommandSettings(dio, info);
    menu.push_back(c2);
    Command* c3 = new CommandDetectAnomalies(dio, info);
    menu.push_back(c3);
    Command* c4 = new CommandDisplayResults(dio, info);
    menu.push_back(c4);
    Command* c5 = new CommandUploadAnomaliesAnalyzeResults(dio, info);
    menu.push_back(c5);
}

void CLI::start(){
    float option;

    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    for (int i = 0; i < 5; i++) {
        dio->write(i+1);
        dio->write(".");
        menu[i]->print();
    }
    dio->write("6.exit\n");

    dio->read(&option);

    while(option != 6) {
        menu[(int)option - 1]->execute();
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < 5; i++) {
            dio->write(i+1);
            dio->write(".");
            menu[i]->print();
        }
        dio->write("6.exit\n");
        dio->read(&option);
    }
}

CLI::~CLI() {
    for (auto c : menu)
        delete c;
    delete info;
}


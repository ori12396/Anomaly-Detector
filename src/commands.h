#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <iomanip>
#include <string.h>

#include <fstream>
#include <utility>
#include <vector>
#include <tuple>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
    virtual void write(int i)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};


class StandartIO: public DefaultIO {
    string read() override {
        string text;
        cin >> text;
        return text;
    }

    void write(string text) override {
        cout << text;
    }

    void write(float f) override {
        cout << f;
    }

    void read(float* f) override {
        float num;
        cin >> num;
        *f = num;
    }
};

// you may add here helper classes
class SharedInfo {
    HybridAnomalyDetector* ad;
    TimeSeries* trainTS;
    TimeSeries* testTS;
    vector<AnomalyReport>* reports;

public:
    void setTrainTs(const char* path) {
        trainTS = new TimeSeries(path);
    }

    void setTestTs(const char* path) {
        testTS = new TimeSeries(path);
    }

    void setDetector() {
        ad = new HybridAnomalyDetector();
    }

    float getThreshold() {
        return ad->getThreshold();
    }

    void setThreshold(float f) {
        ad->setThreshold(f);
    }

    void setReports() {
        reports = new vector<AnomalyReport>();
    }

    vector<AnomalyReport> getReports() {
        return *reports;
    }

    void learnNormal() {
        ad->learnNormal(*trainTS);
    }

    void detect() {
        *reports = ad->detect(*testTS);
    }

    unsigned long getTSsize() {
        return testTS->dataSize();
    }

    ~SharedInfo() {
        delete ad;
        delete trainTS;
        delete testTS;
        delete reports;
    }
};

// you may edit this class
class Command{
    string description;
protected:
	DefaultIO* dio;
    SharedInfo* info;

public:
	Command(DefaultIO* dio, SharedInfo* info, string desc):
    dio(dio), info(info), description(std::move(desc)){
    }
    void print() {
        dio->write(description);
    }
    virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes

class CommandUploadTimeSeries: public Command {
public:
    CommandUploadTimeSeries(DefaultIO* dio, SharedInfo* info):
    Command(dio, info, "upload a time series csv file\n") {};

    void execute() override {
        dio->write("Please upload your local train CSV file.\n");

        std::string line;
        ofstream out;
        out.open("train.csv");
        line = dio->read();
        while (line != "done" && line != "Done" && line != "Done\n" && line != "done\n") {
            out << line << endl;
            line = dio->read();
        }
        out.close();
        info->setTrainTs("train.csv");
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");

        out.open("test.csv");
        line = dio->read();
        while (line != "done" && line != "Done" && line != "Done\n" && line != "done\n") {
            out << line << endl;
            line = dio->read();
        }
        out.close();
        info->setTestTs("test.csv");
        dio->write("Upload complete.\n");
        info->setDetector();
    }
};

class CommandSettings: public Command {
public:
    CommandSettings(DefaultIO* dio, SharedInfo* info):
    Command(dio, info, "algorithm settings\n") {};

    void execute() override {
        float num;
        dio->write("The current correlation threshold is ");
        //dio->write(*this->threshold);
        dio->write(info->getThreshold());
        dio->write("\n");
        dio->write("Type a new threshold\n");

        dio->read(&num);
        while (num < 0 || num > 1) {
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(&num);
        }
        info->setThreshold(num);
    }
};

class CommandDetectAnomalies: public Command {
public:
    CommandDetectAnomalies(DefaultIO* dio, SharedInfo* info):
    Command(dio, info, "detect anomalies\n") {};
    void execute() override {
        info->setReports();
        info->learnNormal();
        info->detect();
        dio->write("anomaly detection complete.\n");
    }
};

class CommandDisplayResults: public Command {
public:
    CommandDisplayResults(DefaultIO* dio, SharedInfo* info):
    Command(dio, info, "display results\n") {};
    void execute() override {
        for (auto & report : info->getReports()) {
            dio->write((float)report.timeStep);
            dio->write("\t");
            dio->write(report.description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};



class CommandUploadAnomaliesAnalyzeResults: public Command {
public:
    CommandUploadAnomaliesAnalyzeResults(DefaultIO* dio, SharedInfo* info):
    Command(dio, info,"upload anomalies and analyze results\n") {};
    void execute() override {
        vector<pair<int, int>> anomalyFile;

        dio->write("Please upload your local anomalies file.\n");
        std::string line, cell1, cell2;

        // iterate on each line
        line = dio->read();
        while (line != "done" && line != "Done" && line != "Done\n" && line != "done\n") {
            // open each line as string stream
            std::stringstream ss(line);

            // add the data to the tuple vectors.
            getline(ss,cell1, ',');
            getline(ss,cell2, ',');
            //tuple<int, int> t = {stoi(cell1), stoi(cell2)};
            pair<int, int> t = make_pair(stoi(cell1), stoi(cell2));
            anomalyFile.push_back(t);
            line = dio->read();
        }

        dio->write("Upload complete.\n");

        vector<pair<pair<long, long>, string>> mergedReports;
        bool flag = true;
        string desc;
        long start, finish;
        for (auto & r : info->getReports()) {

            if (flag) {
                start = r.timeStep;
                desc = r.description;
                flag = false;
            } else {
                if (desc == r.description) {
                    finish = r.timeStep;
                } else {
                    mergedReports.push_back(make_pair(make_pair(start, finish), desc));
                    //flag = true;
                    start = r.timeStep;
                    desc = r.description;
                }
            }
        }
        mergedReports.push_back(make_pair(make_pair(start, finish), desc));


        unsigned long P, N, FP = 0, TP = 0;
        P = anomalyFile.size();
        N = info->getTSsize();
        for (auto & t : anomalyFile) {
            N = N - (t.second - t.first + 1);
        }

        for (auto & m : mergedReports) {
            bool flagg = true;
            for (auto & t : anomalyFile) {
                if (!(m.first.second < t.first || m.first.first > t.second)){
                    flagg = false;
                    break;
                }
            }
            if (flagg){
                FP++;
            }
        }

        for (auto & m : mergedReports) {
            for (auto & t : anomalyFile) {
                if (!(m.first.second < t.first || m.first.first > t.second)) {
                    TP++;
                    break;
                }
            }
        }

        float TPR, FPR;
        TPR = (float)TP / (float)P;
        FPR = (float)FP / (float)N;
        TPR = floorf(TPR * 1000) / 1000;
        FPR = floorf(FPR * 1000) / 1000;
        dio->write("True Positive Rate: ");
        dio->write(TPR);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(FPR);
        dio->write("\n");
    }
};

#endif /* COMMANDS_H_ */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> 


using namespace std;

struct fit {
    vector<string> modules;
    vector<int> quantity;
    string ammo_and_drones;
    void addtofit(string toadd) {
        int found = 0;
        int multiQuant = 1;
        if (toadd == "") {
            return;
        }
        found = toadd.rfind(",");
        if (found > 0 ) {
            found = toadd.size() - found;
            for (int i = 0; i < found; i++) {
                toadd.pop_back();
            }
        }
        if (isdigit(toadd[toadd.size() - 1])) {
            ammo_and_drones = "";
            for (int multi = toadd.size() - 1; multi > 1; multi--) {
                if (isdigit(toadd[multi])) {
                    ammo_and_drones += toadd[multi];
                }
                else {
                    break;
                }
            }
            for (int subString = 0; subString < ammo_and_drones.size() + 2; subString++) { // +2 is for x and " "
                toadd.pop_back();
            }
            reverse(ammo_and_drones.begin(), ammo_and_drones.end());
            multiQuant = stoi(ammo_and_drones);
        }
        for (int i = 0; i < modules.size(); i++) {
            if (toadd == modules[i]) {
                quantity[i] += multiQuant;
                return;
            }
        }
        quantity.push_back(multiQuant);
        modules.push_back(toadd);
    }
};

fit readInOne(ifstream& is) {
    string line;
    fit singleFit;
    int newline = 0;
    while (true) {
        getline(is, line);
        if (line.size() != 0) {
            if (line[0] == '[') {
                if (line == "[Empty Med slot]" || line == "[Empty High slot]" || line == "[Empty Low slot]" || line == "[Empty Rig slot]") {
                    continue;
                }
                else {
                    return singleFit;
                }
            }
        }
        if (line.size() == 0) {
            newline++;
        }
        if (is.eof()) {
            return singleFit;
        }
        else {
            singleFit.addtofit(line);
            newline = 0;
        }
        if (newline > 4) {
            return singleFit;
        }
    }
    return singleFit;

}

vector<fit> readIn(ifstream& is) {
    vector<fit> allFits;
    string throwAway;
    getline(is, throwAway); // throw away the first fit name
    while (true) {
        fit aFit = readInOne(is);
        allFits.push_back(aFit);
        if (is.eof()) {
            break;
        }
    }
    return allFits;
}
void PrintOutput(ofstream& os, vector<fit>& fits) {
    os << "These are all the fits!\n";
    for (int i = 0; i < fits.size(); i++) {
        os << "Fit number " << i << "\n\n\n";
        for (int i2 = 0; i2 < fits[i].modules.size(); i2++) {
            os << fits[i].modules[i2] << "   " << fits[i].quantity[i2] << '\n';
        }
    }

}
void PrintLegalOutput(ofstream& os, fit& fits) {
    //os << "[Nergal, BigBuy]\n\nExpanded Cargohold II\nExpanded Cargohold II\nExpanded Cargohold II\nExpanded Cargohold II\n\n[Empty Med slot]\n[Empty Med slot]\n[Empty Med slot]\n\n[Empty High slot]\n[Empty High slot]\n\n[Empty Rig slot]\n[Empty Rig slot]\n\n\n";
    os << "[Iteron Mark V, test]\n\n[Empty Low slot]\n[Empty Low slot]\n[Empty Low slot]\n[Empty Low slot]\n[Empty Low slot]\n\n[Empty Med slot]\n[Empty Med slot]\n[Empty Med slot]\n[Empty Med slot]\n\n[Empty High slot]\n[Empty High slot]\n\n[Empty Rig slot]\n[Empty Rig slot]\n[Empty Rig slot]\n\n\n";
    // add each module on a new line
    for (int i = 0; i < fits.modules.size(); i++) {
        if (fits.modules[i] == "") {
            continue;
        }
        os << fits.modules[i] << " x" << fits.quantity[i] << '\n';
    }
}

fit UnifiedFit(vector<fit>& fits) {
    bool found = false;
    fit everything;
    for (int i = 0; i < fits.size(); i++) { //list of fits
        for (int i2 = 0;i2 < fits[i].modules.size(); i2++) { // indivdual fit
            found = false;
            for (int i3 = 0; i3 < everything.modules.size();i3++) { //compare fit to main list
                if (fits[i].modules[i2] == everything.modules[i3]) {
                    found = true;
                    if (fits[i].quantity[i2] > everything.quantity[i3]) {
                        everything.quantity[i3] = fits[i].quantity[i2];
                    }
                    else {
                        break;
                    }
                }
            }
            if (!found) {
                everything.modules.push_back(fits[i].modules[i2]);
                everything.quantity.push_back(fits[i].quantity[i2]);
            }
        }
    }
    return everything;
}

int main() {
    string ifname;
    string outName;
    cout << "This program will take in multiple eve fitting in the form of eft from a single text file and produce one fit for an itty 5 containing all componenets with no double ups\n";
    cout << "Input the input file name including the extension ie \"test.txt\"\n";
    getline(cin, ifname);
    cout << "Input the output file name including the .txt bit!\n";
    getline(cin, outName);
    ifstream Input(ifname);
    if (!Input.is_open()) {
        cerr << "Potentially invalid file name try copy pasting it next time!\n";
        return 1;
    }
    vector<fit> bunchofFits = readIn(Input);
    ofstream Output(outName);
    fit Unifit = UnifiedFit(bunchofFits);
    PrintLegalOutput(Output, Unifit);
    return 0;
}
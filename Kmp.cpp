#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
using namespace std;

vector<long long> v1;

void computeLPSArray(const string &pat, int M, vector<int> &lps) {
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            lps[i] = ++len;
            i++;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i] = 0;
            i++;
        }
    }
}

// returns match count, fills matches with "line:pos" style pairs if you want them later
int KMPSearch(const string &pat, const string &txt) {
    int M = pat.length(), N = txt.length();
    vector<int> lps(M);
    computeLPSArray(pat, M, lps);

    int i = 0, j = 0, count = 0;
    while ((N - i) >= (M - j)) {
        if (pat[j] == txt[i]) { j++; i++; }
        if (j == M) {
            count++;
            j = lps[j - 1];
        } else if (i < N && pat[j] != txt[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "{\"error\":\"usage: ./kmp <textfile> <pattern>\"}" << endl;
        return 1;
    }
    string filename = argv[1];
    string pattern = argv[2];

    ifstream newfile(filename);
    string text;
    if (newfile.is_open()) {
        string line;
        while (getline(newfile, line)) {
            text += line;
            text += ' ';
        }
        newfile.close();
    } else {
        cerr << "{\"error\":\"could not open file\"}" << endl;
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();
    int matches = KMPSearch(pattern, text);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    // single JSON line -> this is what Node.js will read
    cout << "{"
         << "\"algorithm\":\"KMP\","
         << "\"matches\":" << matches << ","
         << "\"time_ms\":" << ms << ","
         << "\"complexity\":\"O(n+m)\""
         << "}" << endl;

    return 0;
}

#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <io.h>
#include <direct.h>

#define b_picture vector<vector<bool>>

using namespace std;


void getFiles(string path, vector<string>& files);
void save_all(int kk, int z, int k, vector<b_picture> share, b_picture sa, b_picture sb, int num);
void savemask(vector<b_picture> mask, string path);
void savepic(vector<b_picture> mask, int k, string path, int num_val, b_picture original, b_picture original2);

void kk1(b_picture sa, b_picture sb, int k, int n, int z, int num);
void kk2(b_picture sa, b_picture sb, int k, int n, int z, int num);
void kk3(b_picture sa, b_picture sb, int k, int n, int z, int num);

int gen(int i) { return rand() % i; }


int main() {
    int k = 2;
    vector<b_picture> SA, SB;
    vector<string> pic_path, pic_path2;
    string path = "./pic";
    string path2 = "./pic2";

    getFiles(path, pic_path);
    getFiles(path2, pic_path2);

    if (pic_path.empty() || pic_path.size() != pic_path2.size()) {
        cout << "錯誤：圖片數量不匹配或資料夾為空！" << endl;
        system("pause");
        return -1;
    }

    for (int i = 0; i < (int)pic_path.size(); i++) {
        cv::Mat img = cv::imread(pic_path[i], cv::IMREAD_GRAYSCALE);
        cv::Mat img2 = cv::imread(pic_path2[i], cv::IMREAD_GRAYSCALE);
        if (img.empty() || img2.empty()) continue;

        int w = img.cols, h = img.rows;
        b_picture b_data(h, vector<bool>(w)), b_data2(h, vector<bool>(w));
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                b_data[r][c] = (img.at<uchar>(r, c) < 128); // 黑點為 1
                b_data2[r][c] = (img2.at<uchar>(r, c) < 128);
            }
        }
        SA.push_back(b_data); SB.push_back(b_data2);
    }

    int choice = 0, num_val = 0;
    cout << "========================================" << endl;
    cout << "1:KK1, 2:KK2, 3:KK3, 4:執行全部\n請輸入選擇: "; cin >> choice;
    cout << "請輸入 num 值 (區間分割數): "; cin >> num_val;
    cout << "========================================" << endl;

    for (int i = 0; i < (int)SA.size(); i++) {
        if (choice == 1 || choice == 4) kk1(SA[i], SB[i], k, k, i + 1, num_val);
        if (choice == 2 || choice == 4) kk2(SA[i], SB[i], k, k, i + 1, num_val);
        if (choice == 3 || choice == 4) kk3(SA[i], SB[i], k, k, i + 1, num_val);
    }

    cout << "\n處理完成！結果儲存於 s1/ 資料夾下。" << endl;
    system("pause");
    return 0;
}

// --- 核心演算法實作 ---

void kk1(b_picture sa, b_picture sb, int k, int n, int z, int num) {
    srand((unsigned int)time(NULL));
    int h = (int)sa.size(), w = (int)sa[0].size(), sw = w / num;
    b_picture G1(h, vector<bool>(w, 0)), G2(h, vector<bool>(w, 0));
    vector<int> b, c;
    for (int i = 0; i < sw; i++) b.push_back(i);
    for (int i = 0; i < h; i++) c.push_back(i);
    random_shuffle(b.begin(), b.end(), gen);
    random_shuffle(c.begin(), c.end(), gen);

    cout << "執行 KK1 (組別 " << z << ")..." << endl;
    for (int k_idx = 0; k_idx < sw; k_idx++) {
        for (int l = 0; l < h; l++) {
            int i = c[l], j = b[k_idx], y = rand() % num + 1;
            int target = j + sw * (y - 1);
            if (target < w) {
                G1[i][target] = rand() % 2;
                G2[i][target] = sa[i][target] ^ G1[i][target];
            }
            for (int a = 1; a < num; a++) {
                if (y == num) {
                    G2[i][j] = sb[i][j + sw * (num - 1)] ^ G1[i][j + sw * (num - 1)];
                    G1[i][j] = sa[i][j] ^ G2[i][j]; y = 1;
                }
                else {
                    int nxt = j + sw * y;
                    if (nxt < w) {
                        G2[i][nxt] = sb[i][j + sw * (y - 1)] ^ G1[i][j + sw * (y - 1)];
                        G1[i][nxt] = sa[i][nxt] ^ G2[i][nxt];
                    }
                    y++;
                }
            }
        }
    }
    save_all(1, z, k, { G1, G2 }, sa, sb, num);
}

void kk2(b_picture sa, b_picture sb, int k, int n, int z, int num) {
    srand((unsigned int)time(NULL));
    int h = (int)sa.size(), w = (int)sa[0].size(), sw = w / num;
    b_picture G1(h, vector<bool>(w, 0)), G2(h, vector<bool>(w, 0));
    vector<int> b, c;
    for (int i = 0; i < sw; i++) b.push_back(i);
    for (int i = 0; i < h; i++) c.push_back(i);
    random_shuffle(b.begin(), b.end(), gen);
    random_shuffle(c.begin(), c.end(), gen);

    cout << "執行 KK2 (組別 " << z << ")..." << endl;
    for (int k_idx = 0; k_idx < sw; k_idx++) {
        for (int l = 0; l < h; l++) {
            int i = c[l], j = b[k_idx], y = rand() % num + 1;
            int target = j + sw * (y - 1);
            if (target < w) {
                G1[i][target] = rand() % 2;
                G2[i][target] = (sa[i][target]) ? (sa[i][target] ^ G1[i][target]) : (rand() % 2);
            }
            // RG 傳遞邏輯
            for (int a = 1; a < num; a++) {
                if (y == num) {
                    G2[i][j] = sb[i][j + sw * (num - 1)] ^ G1[i][j + sw * (num - 1)];
                    G1[i][j] = sa[i][j] ^ G2[i][j]; y = 1;
                }
                else {
                    int nxt = j + sw * y;
                    if (nxt < w) {
                        G2[i][nxt] = sb[i][j + sw * (y - 1)] ^ G1[i][j + sw * (y - 1)];
                        G1[i][nxt] = sa[i][nxt] ^ G2[i][nxt];
                    }
                    y++;
                }
            }
        }
    }
    save_all(2, z, k, { G1, G2 }, sa, sb, num);
}

void kk3(b_picture sa, b_picture sb, int k, int n, int z, int num) {
    srand((unsigned int)time(NULL));
    int h = (int)sa.size(), w = (int)sa[0].size(), sw = w / num;
    b_picture G1(h, vector<bool>(w, 0)), G2(h, vector<bool>(w, 0));
    vector<int> b, c;
    for (int i = 0; i < sw; i++) b.push_back(i);
    for (int i = 0; i < h; i++) c.push_back(i);
    random_shuffle(b.begin(), b.end(), gen);
    random_shuffle(c.begin(), c.end(), gen);

    cout << "執行 KK3 (組別 " << z << ")..." << endl;
    for (int k_idx = 0; k_idx < sw; k_idx++) {
        for (int l = 0; l < h; l++) {
            int i = c[l], j = b[k_idx], y = rand() % num + 1;
            int target = j + sw * (y - 1);
            if (target < w) {
                G1[i][target] = rand() % 2;
                G2[i][target] = (!sa[i][target]) ? (sa[i][target] ^ G1[i][target]) : (rand() % 2);
            }
            // RG 傳遞邏輯
            for (int a = 1; a < num; a++) {
                if (y == num) {
                    G2[i][j] = sb[i][j + sw * (num - 1)] ^ G1[i][j + sw * (num - 1)];
                    G1[i][j] = sa[i][j] ^ G2[i][j]; y = 1;
                }
                else {
                    int nxt = j + sw * y;
                    if (nxt < w) {
                        G2[i][nxt] = sb[i][j + sw * (y - 1)] ^ G1[i][j + sw * (num - 1)];
                        G1[i][nxt] = sa[i][nxt] ^ G2[i][nxt];
                    }
                    y++;
                }
            }
        }
    }
    save_all(3, z, k, { G1, G2 }, sa, sb, num);
}

// --- 數據統計與影像儲存 ---

void savepic(vector<b_picture> mask, int k, string path, int num_val, b_picture original, b_picture original2) {
    int h = (int)mask[0].size(), w = (int)mask[0][0].size();
    int shift = w / num_val;
    int rev_shift = w - shift;

    ofstream fout(path + "output.txt");
    auto calculate = [&](string label, b_picture& res, b_picture& orig) {
        double black_pic = 0, white_pic = 0, black_hit = 0, white_hit = 0;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (orig[i][j]) { // 原圖黑點
                    black_pic++;
                    if (res[i][j] == orig[i][j]) black_hit++;
                }
                else { // 原圖白點
                    white_pic++;
                    if (res[i][j] == orig[i][j]) white_hit++;
                }
            }
        }
        // 格式化輸出數據 
        fout << label << " : " << fixed << setprecision(5) << (black_hit + white_hit) / (h * w) << endl;
        fout << label.substr(0, label.find("-")) << "-black" << label.substr(label.find("-")) << " : " << black_hit / black_pic << endl;
        fout << label.substr(0, label.find("-")) << "-white" << label.substr(label.find("-")) << " : " << white_hit / white_pic << endl;
        };

    b_picture sa_or(h, vector<bool>(w)), sa_xor(h, vector<bool>(w));
    b_picture sb_or(h, vector<bool>(w)), sb_xor(h, vector<bool>(w));

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            sa_or[i][j] = mask[0][i][j] || mask[1][i][j];
            sa_xor[i][j] = mask[0][i][j] ^ mask[1][i][j];
            // 位移還原 SB 
            bool m2_s = (j < rev_shift) ? mask[1][i][j + shift] : mask[1][i][j - rev_shift];
            sb_or[i][j] = mask[0][i][j] || m2_s;
            sb_xor[i][j] = mask[0][i][j] ^ m2_s;
        }
    }

    auto save = [&](string name, b_picture& data) {
        cv::Mat m(h, w, CV_8UC1);
        for (int r = 0; r < h; r++) for (int c = 0; c < w; c++) m.at<uchar>(r, c) = data[r][c] ? 0 : 255;
        cv::imwrite(path + name + ".png", m);
        };

    save("SA_OR", sa_or); save("SA_XOR", sa_xor);
    save("SB_OR", sb_or); save("SB_XOR", sb_xor);

    calculate("OR-all-SA", sa_or, original);
    calculate("OR-all-SB", sb_or, original2);
    calculate("XOR-all-SA", sa_xor, original);
    calculate("XOR-all-SB", sb_xor, original2);
    fout.close();
}

void save_all(int kk, int z, int k, vector<b_picture> share, b_picture sa, b_picture sb, int num) {
    char dir[256]; snprintf(dir, sizeof(dir), "./s%d/kk%d/", z, kk);
    _mkdir("./s1"); _mkdir("./s1/kk1"); _mkdir("./s1/kk2"); _mkdir("./s1/kk3"); _mkdir(dir);
    string p = string(dir);
    savemask(share, p);
    savepic(share, k, p, num, sa, sb);
    cv::Mat mSA(sa.size(), sa[0].size(), CV_8UC1), mSB(sb.size(), sb[0].size(), CV_8UC1);
    for (int i = 0; i < sa.size(); i++) for (int j = 0; j < sa[0].size(); j++) {
        mSA.at<uchar>(i, j) = sa[i][j] ? 0 : 255; mSB.at<uchar>(i, j) = sb[i][j] ? 0 : 255;
    }
    cv::imwrite(p + "SA_original.png", mSA); cv::imwrite(p + "SB_original.png", mSB);
}

void savemask(vector<b_picture> mask, string path) {
    for (int k = 0; k < (int)mask.size(); k++) {
        cv::Mat img(mask[k].size(), mask[k][0].size(), CV_8UC1);
        for (int i = 0; i < img.rows; i++)
            for (int j = 0; j < img.cols; j++)
                img.at<uchar>(i, j) = mask[k][i][j] ? 0 : 255;
        cv::imwrite(path + to_string(k + 1) + ".png", img);
    }
}

void getFiles(string path, vector<string>& files) {
    intptr_t hFile = 0; struct _finddata_t fileinfo; string p;
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
        do {
            if (!(fileinfo.attrib & _A_SUBDIR)) files.push_back(p.assign(path).append("/").append(fileinfo.name));
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}
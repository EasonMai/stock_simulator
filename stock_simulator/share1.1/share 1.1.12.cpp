/*
	share v1.1.12���¹��棺 
	fix bug;
*/ 

#include<fstream>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <algorithm>
using namespace std;

int pl_money;
int flag;
int day = 0; 

struct share_info {
    string name;
    string introduction;
    int kind; 
    double now[10000];
    int days;
    double price_itself; 
    bool up_down;        
    float range;         
    float probability;   
};

struct TradeRecord {
    int type;        // 1-���� 2-����
    int stock_idx;   // ��Ʊ������0-19��
    int shares;      // ��������
    double price;    // ���׼۸�����ɱ�/�������ۣ�
    int trade_day;   // ���׷�������
    double profit_pct; // ӯ���ٷֱȣ�����ʱ��Ч��
    double profit;    // ӯ��������ʱ��Ч��
};

vector<TradeRecord> tradeHistory;  // ȫ�ֽ��׼�¼����

struct pl_have {
    int hand;    
    double cost; 
    pl_have() : hand(0), cost(0) {} 
};
struct pl_have s_have[20]; 
int my_stocks[20];       
int my_stocks_count = 0; 
struct share_info share[20];
struct share_info big_share[4]; 
string name1[20] = {
    "���Ƽ�", "��ҫ�Ļ�", "��ʢ��ó", "δ������",
    "��̩����", "�ں�����", "��������", "����彨",
    "�������", "̩ͨ����", "����ҽ��", "��������",
    "���Ž���", "���Ƽ�", "������Ʒ", "����ũҵ",
    "�����ʱ�", "�۾���Ϣ", "���ݼҾ�", "�˰���ѯ"
};

string intro[20] = {
    "�Ƽ�����ҵ����������Դ���", "�Ļ���ý����ý��", "��ó��ҵ��ͷ֮һ", "������ѵ��˾",
    "���йɷ�,������˾", "���繥������������", "������Ʒ����Ӫ����Ʒ����", "������˾���ٶ����",
    "��ƹ�˾��ӵ������������ʦ", "������˾��������ͷ", "�¿Ƽ�ҽ�ƣ����ն��������", "�����磬�Լ۱�֮��",
    "֤ȯ��˾��������", "���ӿƼ���ǰ�ؼ���", "������Ʒ�꣬������˾", "ũҵ�Ƽ������ж����ӽ���ר��",
    "ӵ�о����ʱ���ǿ��ķ����Ŷӣ�������㹫˾����ʹͶ����", "�����ݷ�����ҵ���ǻ۳��еĵ�����", "�Ҿ����죬ӵ����๤��", "��ѯ��˾��������ҵ"
};

void error(string a) {
    cout << a;
    system("pause");
    system("cls");
}

void error1(string a) {
    cout << a;
    system("pause");
}

void init_shares() {
    for (int i = 0; i < 20; i++) {
        share[i].name = name1[i];
        share[i].introduction = intro[i];
        share[i].kind = rand() % 4; 
        share[i].days = 1;          
        share[i].price_itself = rand() % 100 + 50; 
        share[i].now[0] = share[i].price_itself * (0.8 + (rand() % 41) / 100.0); 
        for (int j = 1; j < 1000; j++) share[i].now[j] = 0;
    }
}

void show_holdings() {
    bool has_holdings = false;
    for (int i = 0; i < 20; i++) {
        if (s_have[i].hand > 0) {
            has_holdings = true;
            double current_price = share[i].now[share[i].days - 1];
            double avg_cost = s_have[i].cost / s_have[i].hand;
            double profit_pct = ((current_price - avg_cost) / avg_cost) * 100;
            double profit = s_have[i].hand * (current_price - avg_cost);
            
            printf("%2d: %-8s ���� %4d �� | �ɱ���: %6.2f | ��ǰ��: %6.2f | ӯ��: %+6.2f%% (%+8.2fԪ)\n",
                   i, share[i].name.c_str(), s_have[i].hand, avg_cost, current_price, 
                   profit_pct, profit);
        }
    }
    cout<<endl; 
    if (!has_holdings) cout << "���޳ֲּ�¼��\n";
}

void show_market() {
    cout << "��������ʾ���������е���ţ�0-19�����ɽ�������\n";
    for (int i = 0; i < 20; i++) {
        int current_day = share[i].days - 1;
        double current_price = share[i].now[current_day];
        bool has_yesterday = (current_day > 0);
        double yesterday_price = 0.0;
        double change_pct = 0.0;
        char yesterday_str[20];
        char change_str[20];

        if (has_yesterday) {
            yesterday_price = share[i].now[current_day - 1];
            change_pct = (current_price - yesterday_price) / yesterday_price * 100;
            sprintf(yesterday_str, "%8.2f", yesterday_price);
            sprintf(change_str, "%6.2f%%", change_pct);
        } else {
            sprintf(yesterday_str, "   ��");
            sprintf(change_str, "  ��");
        }

        printf("[%2d]: %-8s ���ռ�: %s ��ǰ��: %8.2f Ԫ �ǵ���: %6s�����: %s\n",
               i, share[i].name.c_str(), yesterday_str, current_price, 
               change_str, share[i].introduction.c_str());
    }
}

void simulate_day() {
    for (int i = 0; i < 20; i++) {
        if (share[i].days % 5 == 0) {
            share[i].price_itself += rand() % 3 - 1; 
        }
        
        double last_price = share[i].now[share[i].days - 1];
        
        double ratio = share[i].price_itself / last_price;
        double deviation = (ratio - 1.0) * 0.05; 
        share[i].probability = 0.5 + deviation;
        if (share[i].probability < 0.35) share[i].probability = 0.35;
        else if (share[i].probability > 0.65) share[i].probability = 0.65;
        
        share[i].up_down = (rand() % 100) < (share[i].probability * 100);
        
        switch (share[i].kind) {
            case 0: 
            case 1: 
                share[i].range = (rand() % 2000) / 10000.0; 
                break;
            case 2: 
            case 3: 
                share[i].range = (rand() % 1000) / 10000.0; 
                break;
        }
        
        double new_price;
        if (share[i].up_down) {
            new_price = last_price * (1 + share[i].range);
        } else {
            new_price = last_price * (1 - share[i].range);
        }
        new_price = floor(new_price * 100 + 0.5) / 100; 
        
        share[i].now[share[i].days] = new_price;
        share[i].days++;
    }
}

void show_point_stock(int stock_idx){
    int current_day = share[stock_idx].days - 1;
    double current_price = share[stock_idx].now[current_day];
    bool has_yesterday = (current_day > 0);
    double yesterday_price = 0.0;
    double change_pct = 0.0;
    char yesterday_str[20];
    char change_str[20];

    if (has_yesterday) {
        yesterday_price = share[stock_idx].now[current_day - 1];
        change_pct = (current_price - yesterday_price) / yesterday_price * 100;
        sprintf(yesterday_str, "%8.2f", yesterday_price);
        sprintf(change_str, "%6.2f%%", change_pct);
    } else {
        sprintf(yesterday_str, "   ��");
        sprintf(change_str, "  ��");
    }

    printf("%2d: %-8s ���ռ�: %s ��ǰ��: %8.2f Ԫ �ǵ���: %6s�����: %s\n",
        stock_idx, share[stock_idx].name.c_str(), yesterday_str, current_price, 
        change_str, share[stock_idx].introduction.c_str());
} 

void show_max(bool flag, int stock_idx) {
    if (flag) { 
        double price = share[stock_idx].now[share[stock_idx].days - 1]; 
        if (price <= 0) {
            cout << "��Ʊ�۸��쳣���޷����롣";
            return;
        }
        int max_shares = floor(pl_money / (price * 1.0003));
        cout << "�������" << max_shares << "�ɡ�" << endl;
    } else { 
        cout << "�������" << s_have[stock_idx].hand << "�ɡ�" << endl;
    }
}

void trade_stock() {
    int stock_idx, action, shares;
    cout << "�����Ʊ��� (0-19): ";
    cin >> stock_idx;
    if (stock_idx < 0 || stock_idx >= 20) {
        error1("��Ч��Ʊ��ţ�");
        return;
    }
    show_point_stock(stock_idx);
    cout << "1. ����  2. ����: ";
    cin >> action;
    double price = share[stock_idx].now[share[stock_idx].days - 1];
    int current_day = day;

    if (action == 1) {
        show_max(true, stock_idx); 
        cout<<"�����ʽ�"<<pl_money<<"Ԫ��"<<endl; 
        cout << "�������: ";
        cin >> shares;
        
        double total = price * shares * 1.0003; 
        if (total > pl_money) {
            error1("�ʽ��㣡"); 
        } else {
            pl_money -= total;
            s_have[stock_idx].hand += shares;
            s_have[stock_idx].cost += total;
            cout << "�ɹ����� " << shares << " �ɡ�\n";
            
            TradeRecord record;
            record.type = 1;
            record.stock_idx = stock_idx;
            record.shares = shares;
            record.price = price * 1.0003;
            record.trade_day = current_day;
            record.profit_pct = 0;
            record.profit = 0;
            tradeHistory.push_back(record);
        }
    } else {
        show_max(false, stock_idx); 
        cout << "��������: ";
        cin >> shares;
        if (shares > s_have[stock_idx].hand) {
            error1("�ֲֲ��㣡"); 
        } else {
            double total = price * shares * 0.995; 
            pl_money += total;
            double avg_cost = s_have[stock_idx].cost / s_have[stock_idx].hand;
            s_have[stock_idx].hand -= shares;
            s_have[stock_idx].cost = avg_cost * s_have[stock_idx].hand;
            cout << "�ɹ����� " << shares << " �ɡ�\n";
            
            double sell_price = price * 0.995;
            double profit = shares * (sell_price - avg_cost);
            double profit_pct = ((sell_price - avg_cost) / avg_cost) * 100;
            
            TradeRecord record;
            record.type = 2;
            record.stock_idx = stock_idx;
            record.shares = shares;
            record.price = price;
            record.trade_day = current_day;
            record.profit_pct = profit_pct;
            record.profit = profit;
            tradeHistory.push_back(record);
        }
    }
}

int check_game_status() {
    if (pl_money >= 1000000) {
        cout << "\n��ϲ�����ѳ�Ϊ�����̣�\n";
        return 1;
    }
    if (pl_money <= -1) {
        cout << "\n�ʽ��㣬��Ϸ������\n";
        return -1;
    }
    return 0;
}

void start(){
    for(int i=0;i<=10;i++){
        simulate_day();
    }
}

struct StockChange {
    string name;
    double change;
    int index;
};

bool compareGainers(const StockChange& a, const StockChange& b) {
    return a.change > b.change;
}

bool compareLosers(const StockChange& a, const StockChange& b) {
    return a.change > b.change;
}

void dt() {
    StockChange gainers[20];
    StockChange losers[20];
    int gainer_count = 0, loser_count = 0;

    for (int i = 0; i < 20; i++) {
        if (share[i].days < 2) continue;
        
        double prev = share[i].now[share[i].days - 2];
        double curr = share[i].now[share[i].days - 1];
        double change = (curr - prev) / prev * 100;

        if (change > 0) {
            gainers[gainer_count].name = share[i].name;
            gainers[gainer_count].change = change;
            gainers[gainer_count].index = i;
            gainer_count++;
        } else if (change < 0) {
            losers[loser_count].name = share[i].name;
            losers[loser_count].change = -change;
            losers[loser_count].index = i;
            loser_count++;
        }
    }

    sort(gainers, gainers + gainer_count, compareGainers);
    sort(losers, losers + loser_count, compareLosers);

    cout << "\n�����������񡿣�ֱ���������е�[���]����������Ӧ��Ʊ��\n";
    
    cout << "�X�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�[\n"
         << "�U ����  �� ���  �� ��Ʊ����         �� �ǵ���     �U\n"
         << "�c���������������੤�������������੤�����������������������������������੤�����������������������f\n";
    if (gainer_count == 0) {
        cout << "�U �޷����������ݣ����������ǹ�Ʊ��                  �U\n";
    } else {
        for (int i = 0; i < min(gainer_count, 5); i++) {
            printf("�U %-4d�� [%2d] �� %-14s�� %+6.2f%% �U\n",
                  i+1, gainers[i].index, gainers[i].name.c_str(), gainers[i].change);
        }
    }
    cout << "�^�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�a\n\n";

    cout << "�X�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�[\n"
         << "�U ����  �� ���  �� ��Ʊ����         �� �ǵ���     �U\n"
         << "�c���������������੤�������������੤�����������������������������������੤�����������������������f\n";
    if (loser_count == 0) {
        cout << "�U �޷����������ݣ��������µ���Ʊ��                  �U\n";
    } else {
        for (int i = 0; i < min(loser_count, 5); i++) {
            printf("�U %-4d�� [%2d] �� %-14s�� %-6.2f%%  �U\n",
                  i+1, losers[i].index, losers[i].name.c_str(), losers[i].change);
        }
    }
    cout << "�^�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�a\n\n";

    cout << "�����������...";
    system("pause > nul");
}

void mystock() {
    int choice;
    while(true){
        
        cout << "\n==================================================== ��ѡ�ɹ��� =======================================================\n";
        cout << "[1] �����ѡ��\t[2] �Ƴ���ѡ��\n";
        cout << "[3] �鿴��ѡ��\t[4] �������˵�\n";
        cout << "������������: ";
        cin >> choice;

        if(choice == 1){
            if(my_stocks_count >= 20){
                error1("��ѡ���Ѵ�����(20֧)��");
                continue;
            }
            
            int stock_idx;
            cout << "����Ҫ��ӵĹ�Ʊ��� (0-19): ";
            cin >> stock_idx;
            
            if(stock_idx < 0 || stock_idx >= 20){
                error1("��Ч��Ʊ��ţ�");
                continue;
            }
            
            bool exists = false;
            for(int i=0; i<my_stocks_count; i++){
                if(my_stocks[i] == stock_idx){
                    exists = true;
                    break;
                }
            }
            if(exists){
                error1("�ù�Ʊ������ѡ�б��У�");
            }
            else{
                my_stocks[my_stocks_count++] = stock_idx;
                cout << "�ɹ���� " << share[stock_idx].name << " ����ѡ�ɣ�\n";
            }
            system("pause");
        }
        
        else if(choice == 2){
            if(my_stocks_count == 0){
                error1("��ѡ���б�Ϊ�գ�");
                continue;
            }
            
            cout << "\n��ǰ��ѡ���б�:\n";
            for(int i=0; i<my_stocks_count; i++){
                printf("%2d: %-8s (���%d)\n", 
                      i+1, share[my_stocks[i]].name.c_str(), my_stocks[i]);
            }
            
            int del_idx;
            cout << "����Ҫ�Ƴ����б���� (1-" << my_stocks_count << "): ";
            cin >> del_idx;
            
            if(del_idx < 1 || del_idx > my_stocks_count){
                error1("��Ч�б���ţ�");
            }
            else{
                cout << "���Ƴ� " << share[my_stocks[del_idx-1]].name << endl;
                for(int i=del_idx-1; i<my_stocks_count-1; i++){
                    my_stocks[i] = my_stocks[i+1];
                }
                my_stocks_count--;
            }
            system("pause");
        }
        
        else if(choice == 3){
            if(my_stocks_count == 0){
                error1("��ѡ���б�Ϊ�գ�");
                continue;
            }
            
            cout << "\n============= ��ѡ������ =============\n";
            for(int i=0; i<my_stocks_count; i++){
                show_point_stock(my_stocks[i]);
                cout << endl;
            }
            system("pause");
        }
        
        else if(choice == 4){
            break;
        }
        
        else{
            error1("��Ч������ţ�");
        }
    }
}

void showTradeHistory() {
    if (tradeHistory.empty()) {
        cout << "���޽��׼�¼��\n";
        system("pause");
        return;
    }

    cout << "\n����ʷ���׼�¼������ʱ�䵹����ʾ��\n";
    cout << "�X�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�[\n"
         << "�U ���  �� ����  �� ��Ʊ����   �� ����(��) �� ����(Ԫ) �� ��������    �� ӯ��(Ԫ/%)  �U\n"
         << "�c���������������੤�������������੤�����������������������੤�������������������੤�������������������੤�������������������������੤�����������������������f\n";

    for (int i = tradeHistory.size() - 1; i >= 0; i--) {
        const TradeRecord& rec = tradeHistory[i];
        string type = (rec.type == 1) ? "����" : "����";
        string stock_name = share[rec.stock_idx].name;
        char profit_str[30];

        if (rec.type == 1) {
            sprintf(profit_str, "��");
        } else {
            sprintf(profit_str, "%+8.2fԪ (%+6.2f%%)", rec.profit, rec.profit_pct);
        }

        printf("�U %-4d�� %-4s�� %-10s�� %-8d�� %-8.2f�� ��%-4d��    �� %-12s�U\n",
              tradeHistory.size() - i,
              type.c_str(),
              stock_name.c_str(),
              rec.shares,
              rec.price,
              rec.trade_day,
              profit_str);
    }
    cout << "�^�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�a\n";
    system("pause");
}

void save() {
    int slot;
    cout << "��ѡ��浵λ�ã�1-1000��: ";
    cin >> slot;
    if(slot < 1 || slot > 1000) {
        error1("��Ч�浵λ�ã�������1-1000֮�������");
        return;
    }
    
    char filename[20];
    sprintf(filename, "game%d.sav", slot);
    
    ofstream file(filename, ios::binary);
    if (!file) {
        error1("�޷������浵�ļ���");
        return;
    }

    file.write(reinterpret_cast<char*>(&pl_money), sizeof(pl_money));
    file.write(reinterpret_cast<char*>(&day), sizeof(day));

    for (int i = 0; i < 20; ++i) {
        size_t name_len = share[i].name.size();
        file.write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
        file.write(share[i].name.c_str(), name_len);

        size_t intro_len = share[i].introduction.size();
        file.write(reinterpret_cast<const char*>(&intro_len), sizeof(intro_len));
        file.write(share[i].introduction.c_str(), intro_len);

        file.write(reinterpret_cast<const char*>(&share[i].kind), sizeof(share[i].kind));
        file.write(reinterpret_cast<const char*>(&share[i].days), sizeof(share[i].days));
        file.write(reinterpret_cast<const char*>(&share[i].price_itself), sizeof(share[i].price_itself));

        for (int j = 0; j < share[i].days; ++j) {
            file.write(reinterpret_cast<const char*>(&share[i].now[j]), sizeof(double));
        }
    }

    for (int i = 0; i < 20; ++i) {
        file.write(reinterpret_cast<const char*>(&s_have[i].hand), sizeof(s_have[i].hand));
        file.write(reinterpret_cast<const char*>(&s_have[i].cost), sizeof(s_have[i].cost));
    }

    file.write(reinterpret_cast<char*>(&my_stocks_count), sizeof(my_stocks_count));
    for (int i = 0; i < my_stocks_count; ++i) {
        file.write(reinterpret_cast<char*>(&my_stocks[i]), sizeof(int));
    }

    int history_size = tradeHistory.size();
    file.write(reinterpret_cast<char*>(&history_size), sizeof(history_size));
    for (int i = 0; i < history_size; ++i) {
        const TradeRecord& rec = tradeHistory[i];
        file.write(reinterpret_cast<const char*>(&rec.type), sizeof(rec.type));
        file.write(reinterpret_cast<const char*>(&rec.stock_idx), sizeof(rec.stock_idx));
        file.write(reinterpret_cast<const char*>(&rec.shares), sizeof(rec.shares));
        file.write(reinterpret_cast<const char*>(&rec.price), sizeof(rec.price));
        file.write(reinterpret_cast<const char*>(&rec.trade_day), sizeof(rec.trade_day));
        file.write(reinterpret_cast<const char*>(&rec.profit_pct), sizeof(rec.profit_pct));
        file.write(reinterpret_cast<const char*>(&rec.profit), sizeof(rec.profit));
    }

    cout << "�浵" << slot << "�ѱ��棡" << endl;
    system("pause");
}

void read() {
    int slot;
    cout << "��ѡ����صĴ浵��1-1000��: ";
    cin >> slot;
    if (slot < 1 || slot > 1000) {
        error1("��Ч�浵λ�ã�������1-1000֮�������");
        return;
    }

    char filename[20];
    sprintf(filename, "game%d.sav", slot);

    ifstream file(filename, ios::binary);
    if (!file) {
        error1("�ô浵�����ڣ�");
        return;
    }

    try {
        // �������ݶ�ȡУ��
        if (!file.read(reinterpret_cast<char*>(&pl_money), sizeof(pl_money)) ||
            !file.read(reinterpret_cast<char*>(&day), sizeof(day))) {
            throw runtime_error("�ļ�ͷ��");
        }

        // ��Ʊ���ݰ�ȫ��ȡ
        for (int i = 0; i < 20; ++i) {
            // ��֤���Ƴ���
            size_t name_len;
            if (!file.read(reinterpret_cast<char*>(&name_len), sizeof(name_len)) || name_len > 255) {
                throw runtime_error("��Ʊ���Ƴ����쳣");
            }
            
            // Ԥ���仺����
            vector<char> name_buf(name_len + 1);
            if (!file.read(name_buf.data(), name_len)) {
                throw runtime_error("��Ʊ���ƶ�ȡʧ��");
            }
            share[i].name.assign(name_buf.data(), name_len);

            // ��֤��鳤��
            size_t intro_len;
            if (!file.read(reinterpret_cast<char*>(&intro_len), sizeof(intro_len)) || intro_len > 1024) {
                throw runtime_error("��Ʊ��鳤���쳣");
            }

            vector<char> intro_buf(intro_len + 1);
            if (!file.read(intro_buf.data(), intro_len)) {
                throw runtime_error("��Ʊ����ȡʧ��");
            }
            share[i].introduction.assign(intro_buf.data(), intro_len);

            // ��֤��������
            if (!file.read(reinterpret_cast<char*>(&share[i].kind), sizeof(share[i].kind)) ||
                !file.read(reinterpret_cast<char*>(&share[i].days), sizeof(share[i].days)) ||
                !file.read(reinterpret_cast<char*>(&share[i].price_itself), sizeof(share[i].price_itself))) {
                throw runtime_error("��Ʊ����������");
            }

            // ��֤��ʷ����
            if (share[i].days < 0 || share[i].days > 10000) {
                throw runtime_error("��ʷ���������쳣");
            }

            // ��ȡ�۸���ʷ
            for (int j = 0; j < share[i].days; ++j) {
                if (!file.read(reinterpret_cast<char*>(&share[i].now[j]), sizeof(double))) {
                    throw runtime_error("�۸���ʷ������");
                }
            }
        }

        // �ֲ�����У��
        for (int i = 0; i < 20; ++i) {
            if (!file.read(reinterpret_cast<char*>(&s_have[i].hand), sizeof(s_have[i].hand)) ||
                !file.read(reinterpret_cast<char*>(&s_have[i].cost), sizeof(s_have[i].cost))) {
                throw runtime_error("�ֲ�������");
            }
        }

        // ��ѡ������У��
        if (!file.read(reinterpret_cast<char*>(&my_stocks_count), sizeof(my_stocks_count)) ||
            my_stocks_count < 0 || my_stocks_count > 20) {
            throw runtime_error("��ѡ�������쳣");
        }

        for (int i = 0; i < my_stocks_count; ++i) {
            if (!file.read(reinterpret_cast<char*>(&my_stocks[i]), sizeof(int))) {
                throw runtime_error("��ѡ��������");
            }
        }

        // ���׼�¼��ȫ��ȡ
        int history_size;
        if (!file.read(reinterpret_cast<char*>(&history_size), sizeof(history_size)) ||
            history_size < 0 || history_size > 1000000) {
            throw runtime_error("���׼�¼�����쳣");
        }

        tradeHistory.clear();
        for (int i = 0; i < history_size; ++i) {
            TradeRecord rec;
            if (!file.read(reinterpret_cast<char*>(&rec.type), sizeof(rec.type)) ||
                !file.read(reinterpret_cast<char*>(&rec.stock_idx), sizeof(rec.stock_idx)) ||
                !file.read(reinterpret_cast<char*>(&rec.shares), sizeof(rec.shares)) ||
                !file.read(reinterpret_cast<char*>(&rec.price), sizeof(rec.price)) ||
                !file.read(reinterpret_cast<char*>(&rec.trade_day), sizeof(rec.trade_day)) ||
                !file.read(reinterpret_cast<char*>(&rec.profit_pct), sizeof(rec.profit_pct)) ||
                !file.read(reinterpret_cast<char*>(&rec.profit), sizeof(rec.profit))) {
                throw runtime_error("���׼�¼��");
            }
            tradeHistory.push_back(rec);
        }

        cout << "�浵" << slot << "�Ѽ��أ���ǰ������" << day << endl;
    }
    catch (const exception& e) {
        cerr << "\n�浵����ʧ�ܣ�" << e.what() << endl;
        // ������Ϸ״̬
        init_shares();
        start();
        pl_money = rand() % 100000 + 100000;
        memset(s_have, 0, sizeof(s_have));
        my_stocks_count = 0;
        tradeHistory.clear();
    }

    system("pause");
    system("cls");
}

void more(int choice){
    if(choice==1){
        dt();
    }
    else if(choice==2){
        mystock(); 
    }
    else if(choice==3){
        pl_money = rand() % 100000 + 100000;
        cout<<"������"<<endl;
        system("pause");
        system("cls"); 
    }
    else if(choice==4){ 
        save();
    }
    else if(choice==5){ 
        read();
    }
    else if(choice==6){ 
        showTradeHistory();
    }
    else {
        error("δ������ȷ��ָ��!"); 
    }
}

int main() {
    srand(time(NULL));
    init_shares();
    start(); 
    pl_money = rand() % 100000 + 100000;
    cout << "��ӭ����������Ϸ��\nĿ�꣺׬ȡ100��Ԫ��\n";
    int day = 0;
    while (true) {
        cout << "\n=============================================== �� " << ++day << " �� =====================================================\n";
        while(true){
            cout << "��ǰ�ʽ�: " << pl_money << "Ԫ\n";
            show_holdings();
            show_market();
            cout << "\n[1] ���н��� [2] �������� [3]���๦��: ";
            int choice;
            flag=0;
            cin >> choice;
            
            if (choice == 1) {
                while(flag==0){
                    trade_stock();
                    cout<<"�Ƿ��������?[0]��[1]��:" ;
                    cin>>flag; 
                } 
                simulate_day();
                break;
            }

            else if (choice ==2){
                simulate_day();
                break;
            }
            else if(choice == 3){
                cout<<"[1]������[2]�л�����ѡ�ɳ�[3]������Ϸ[4]�浵[5]����[6]�鿴���׼�¼: ";
                cin>>choice;
                more(choice); 
            }
            
            else error("δ������ȷ��ָ��!");
            
        } 
        
        int status = check_game_status();
        if (status != 0) break;

        system("pause");
        system("cls");
    }
    return 0;
}



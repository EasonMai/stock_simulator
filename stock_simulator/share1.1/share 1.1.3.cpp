/*
	share v1.1.3���¹��棺 
	����������&��ѡ�ɹ��� 
	������main 
*/ 



#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<string>
#include <vector>
#include <algorithm> 
using namespace std;
int pl_money;
int flag;
struct share_info {
    string name;
    string introduction;
    int kind; // ���ͣ�0-�߼۸����,1-�ͼ۸����,2-�߼۵����,3-�ͼ۵����
    double now[10000];
    int days;
    double price_itself; // ���ڼ�ֵ
    bool up_down;        // �ǵ�
    float range;         // ��������
    float probability;   // ����
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

struct share_info share[20];
struct share_info big_share[4]; // ��������

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

void init_shares() {
    for (int i = 0; i < 20; i++) {
        share[i].name = name1[i];
        share[i].introduction = intro[i];
        share[i].kind = rand() % 4; // ����0-3
        share[i].days = 1;          // ��ʼ����һ������
        share[i].price_itself = rand() % 100 + 50; // ���ڼ�ֵ50-150
        share[i].now[0] = share[i].price_itself * (0.8 + (rand() % 41) / 100.0); // ��ʼ�۸�80%-120%
        for (int j = 1; j < 1000; j++) share[i].now[j] = 0;
    }
}

struct pl_have {
    int hand = 0;    // ���й���
    double cost = 0; // �ܳɱ�
};
struct pl_have s_have[20]; // �ֲּ�¼
vector<int> my_stocks; // �洢��ѡ��������ȫ������
void show_holdings() {
    bool has_holdings = false;
    for (int i = 0; i < 20; i++) {
        if (s_have[i].hand > 0) {
            has_holdings = true;
            double current_price = share[i].now[share[i].days - 1];
            double avg_cost = s_have[i].cost / s_have[i].hand;
            double profit = ((current_price - avg_cost) / avg_cost) * 100;
            printf("%2d: %-8s ���� %4d �ɣ��ɱ���: %6.2f����ǰ��: %6.2f��ӯ����: %6.2f%%��ӯ���%6.2f\n",
                   i, share[i].name.c_str(), s_have[i].hand, avg_cost, current_price, profit,s_have[i].hand*(current_price-avg_cost));
        }
    }
    cout<<endl; 
    if (!has_holdings) cout << "���޳ֲּ�¼��\n";
}

void show_market() {
    for (int i = 0; i < 20; i++) {
        int current_day = share[i].days - 1;
        double current_price = share[i].now[current_day];
        bool has_yesterday = (current_day > 0);
        double yesterday_price = 0.0;
        double change_pct = 0.0;
        char yesterday_str[20];
        char change_str[20];

        // �������ռ۸���ʾ
        if (has_yesterday) {
            yesterday_price = share[i].now[current_day - 1];
            change_pct = (current_price - yesterday_price) / yesterday_price * 100;
            sprintf(yesterday_str, "%8.2f", yesterday_price);
            sprintf(change_str, "%6.2f%%", change_pct);
        } else {
            sprintf(yesterday_str, "   ��");
            sprintf(change_str, "  ��");
        }

        printf("%2d: %-8s ���ռ�: %s ��ǰ��: %8.2f Ԫ �ǵ���: %6s�����: %s\n",
               i, share[i].name.c_str(), yesterday_str, current_price, 
               change_str, share[i].introduction.c_str());
    }
}
void simulate_day() {
    for (int i = 0; i < 20; i++) {
        // ÿ5��������ڼ�ֵ
        if (share[i].days % 5 == 0) {
            share[i].price_itself += rand() % 3 - 1; // -1, 0, +1
        }
        
        double last_price = share[i].now[share[i].days - 1];
        
        // �����ǵ����ʣ��������ڼ�ֵ�뵱ǰ�۸��ϵ��
        double ratio = share[i].price_itself / last_price;
        double deviation = (ratio - 1.0) * 0.05; // ÿ20%�۸�ƫ�����1%����
        share[i].probability = 0.5 + deviation;
        // ���Ƹ�����35%~65%֮��
        if (share[i].probability < 0.35) share[i].probability = 0.35;
        else if (share[i].probability > 0.65) share[i].probability = 0.65;
        
        // ����ǵ�
        share[i].up_down = (rand() % 100) < (share[i].probability * 100);
        
        // ��������ȷ����������
        switch (share[i].kind) {
            case 0: // �߼۸������0-20%
            case 1: // �ͼ۸������0-20%
                share[i].range = (rand() % 2000) / 10000.0; // 0%~20%
                break;
            case 2: // �߼۵������0-10%
            case 3: // �ͼ۵������0-10%
                share[i].range = (rand() % 1000) / 10000.0; // 0%~10%
                break;
        }
        
        // �����¼۸����ƾ���
        double new_price;
        if (share[i].up_down) {
            new_price = last_price * (1 + share[i].range);
        } else {
            new_price = last_price * (1 - share[i].range);
        }
        new_price = floor(new_price * 100 + 0.5) / 100; // ������λС��
        
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

        // �������ռ۸���ʾ
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
    if (flag) { // ����
        double price = share[stock_idx].now[share[stock_idx].days - 1]; // ��ȡ��ǰ�۸�
        if (price <= 0) {
            cout << "��Ʊ�۸��쳣���޷����롣";
            return;
        }
        // ���㿼�������Ѻ�����������
        int max_shares = floor(pl_money / (price * 1.0003));
        cout << "�������" << max_shares << "�ɡ�" << endl;
    } else { // ����
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

    if (action == 1) {
    	show_max(true, stock_idx); // ��ʾ�����޶�
    	cout<<"�����ʽ�"<<pl_money<<"Ԫ��"<<endl; 
        cout << "�������: ";
        cin >> shares;
        
        double total = price * shares * 1.0003; // 0.03%������
        if (total > pl_money) {
            error1("�ʽ��㣡"); 
			 
        } else {
            pl_money -= total;
            s_have[stock_idx].hand += shares;
            s_have[stock_idx].cost += total;
            cout << "�ɹ����� " << shares << " �ɡ�\n";
            
        }
    } else {
    	show_max(false, stock_idx); // ��ʾ�����޶�
        cout << "��������: ";
        cin >> shares;
        if (shares > s_have[stock_idx].hand) {
        	error1("�ֲֲ��㣡"); 
        } else {
            double total = price * shares * 0.995; // 0.5%������
            pl_money += total;
            double avg_cost = s_have[stock_idx].cost / s_have[stock_idx].hand;
            s_have[stock_idx].hand -= shares;
            s_have[stock_idx].cost = avg_cost * s_have[stock_idx].hand;
            cout << "�ɹ����� " << shares << " �ɡ�\n";
            
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
void dt() {
    using namespace std;
    vector<pair<string, double>> topGainers;   // �Ƿ�������+�Ƿ���
    vector<pair<string, double>> topLosers;    // ����������+������

    // �ռ���������ʷ���ݵĹ�Ʊ
    for (int i = 0; i < 20; i++) {
        if (share[i].days < 2) continue;  // ������Ҫ2������
        
        double prev = share[i].now[share[i].days - 2];
        double curr = share[i].now[share[i].days - 1];
        double change = (curr - prev) / prev * 100;  // �ǵ�����%��

        if (change > 0) {
            topGainers.emplace_back(share[i].name, change);
        } else if (change < 0) {
            topLosers.emplace_back(share[i].name, -change);  // �洢������
        }
    }

    // �����Ƿ��Ӹߵ���
    sort(topGainers.begin(), topGainers.end(), 
         [](const auto& a, const auto& b) { return a.second > b.second; });

    // ���򣺵����Ӹߵ���
    sort(topLosers.begin(), topLosers.end(), 
         [](const auto& a, const auto& b) { return a.second > b.second; });


    // ��ʽ�����
    auto printHeader = [](const string& title) {
        cout << "\n�X�T�T�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�h�T�T�T�T�T�T�T�T�T�T�T�T�[\n"
             << "�U ����       �� ��Ʊ����         �� �ǵ���     �U\n"
             << "�c�������������������������੤�����������������������������������੤�����������������������f\n";
        cout << "�U " << title << " �U\n";
        cout << "�^�T�T�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�k�T�T�T�T�T�T�T�T�T�T�T�T�a\n\n";
    };

    // ��ʾ�Ƿ���
    cout << "\n������������\n\n";
    printHeader("�Ƿ�ǰ��");
    for (int i = 0; i < min(5, (int)topGainers.size()); i++) {
        printf("�U %-10d�� %-14s�� %+6.2f%% �U\n", 
              i+1, topGainers[i].first.c_str(), topGainers[i].second);
    }
    if (topGainers.empty()) cout << "�U ������                  �U\n";


    // ��ʾ������
    printHeader("����ǰ��");
    for (int i = 0; i < min(5, (int)topLosers.size()); i++) {
        printf("�U %-10d�� %-14s�� %6.2f%%  �U\n", 
              i+1, topLosers[i].first.c_str(), topLosers[i].second);
    }
    if (topLosers.empty()) cout << "�U ������                  �U\n";


    cout << "\n�����������...";
    system("pause > nul");  // ��Ĭ��ͣ
}
void mystock(){//��ѡ�ɹ���
    int choice;
    while(true){
        system("cls");
        cout << "\n=============== ��ѡ�ɹ��� ===============\n";
        cout << "[1] �����ѡ��\t[2] �Ƴ���ѡ��\n";
        cout << "[3] �鿴��ѡ��\t[4] �������˵�\n";
        cout << "������������: ";
        cin >> choice;

        // �����ѡ��
        if(choice == 1){
            int stock_idx;
            cout << "����Ҫ��ӵĹ�Ʊ��� (0-19): ";
            cin >> stock_idx;
            
            // ��Ч�Լ��
            if(stock_idx < 0 || stock_idx >= 20){
                error1("��Ч��Ʊ��ţ�");
                continue;
            }
            
            // ���ؼ��
            if(find(my_stocks.begin(), my_stocks.end(), stock_idx) != my_stocks.end()){
                error1("�ù�Ʊ������ѡ�б��У�");
            }
            else{
                my_stocks.push_back(stock_idx);
                cout << "�ɹ���� " << share[stock_idx].name << " ����ѡ�ɣ�\n";
            }
            system("pause");
        }
        
        // �Ƴ���ѡ��
        else if(choice == 2){
            if(my_stocks.empty()){
                error1("��ѡ���б�Ϊ�գ�");
                continue;
            }
            
            // ��ʾ��ǰ��ѡ��
            cout << "\n��ǰ��ѡ���б�:\n";
            for(int i=0; i<my_stocks.size(); i++){
                printf("%2d: %-8s (���%d)\n", 
                      i+1, share[my_stocks[i]].name.c_str(), my_stocks[i]);
            }
            
            int del_idx;
            cout << "����Ҫ�Ƴ����б���� (1-" << my_stocks.size() << "): ";
            cin >> del_idx;
            
            // ��Ч�Լ��
            if(del_idx < 1 || del_idx > my_stocks.size()){
                error1("��Ч�б���ţ�");
            }
            else{
                cout << "���Ƴ� " << share[my_stocks[del_idx-1]].name << endl;
                my_stocks.erase(my_stocks.begin() + del_idx-1);
            }
            system("pause");
        }
        
        // �鿴��ѡ��
        else if(choice == 3){
            if(my_stocks.empty()){
                error1("��ѡ���б�Ϊ�գ�");
                continue;
            }
            
            cout << "\n============= ��ѡ������ =============\n";
            for(int idx : my_stocks){
                show_point_stock(idx);  // ���ø�������չʾ����
                cout << endl;
            }
            system("pause");
        }
        
        // �������˵�
        else if(choice == 4){
            break;
        }
        
        else{
            error1("��Ч������ţ�");
        }
    }
}
void more(int choice){
	if(choice==1){
		dt();
	}
	else if(choice==2){
		mystock(); 
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
	
	        // ģ����һ��
	        else if (choice ==2){
	        	simulate_day();
	        	break;
			}
			else if(choice == 3){
				cout<<"[1]������[2]�л�����ѡ�ɳ�";
				cin>>choice;
				more(choice); 
			}
			
			else error("δ������ȷ��ָ��!");
			
		} 
		
        // �����Ϸ״̬
        int status = check_game_status();
        if (status != 0) break;

        system("pause");
        system("cls");
    }
    return 0;
}

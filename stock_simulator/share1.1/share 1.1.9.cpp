/*
	share v1.1.9更新公告： 
	修复龙虎榜显示错误（增加明确排名序号）
	增加多存档储存读取功能（支持1-1000号存档位）
	修复C++03兼容性问题（替换to_string和string文件名）
*/ 

#include<fstream>
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
int day = 0; 
struct share_info {
    string name;
    string introduction;
    int kind; // 类型：0-高价高振幅,1-低价高振幅,2-高价低振幅,3-低价低振幅
    double now[10000];
    int days;
    double price_itself; // 内在价值
    bool up_down;        // 涨跌
    float range;         // 波动幅度
    float probability;   // 概率
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
struct share_info big_share[4]; // 大盘类型

string name1[20] = {
    "瑞恒科技", "星耀文化", "宏盛商贸", "未来教育",
    "丰泰环保", "众合网络", "新韵拍卖", "金瑞峰建",
    "逸韵设计", "通泰物流", "康瑞医疗", "锦程万里",
    "恒信金融", "瀚宇科技", "新尚优品", "嘉润农业",
    "鑫润资本", "慧聚信息", "优逸家居", "兴邦咨询"
};

string intro[20] = {
    "科技类企业，归属新能源板块", "文化传媒，新媒体", "外贸企业寡头之一", "教育培训公司",
    "国有股份,环保公司", "网络攻防，技术保镖", "拍卖物品，主营艺术品拍卖", "基建公司，速度奇快",
    "设计公司，拥有许多优秀设计师", "物流公司，服务龙头", "新科技医疗，掌握多项基因技术", "旅游社，性价比之王",
    "证券公司，波动大", "电子科技，前沿技术", "连锁优品店，蓝海公司", "农业科技，已有多项杂交稻专利",
    "拥有巨量资本，强大的分析团队，许多优秀公司的天使投资人", "大数据分析企业，智慧城市的缔造者", "家具制造，拥有许多工厂", "咨询公司，服务行业"
};

void init_shares() {
    for (int i = 0; i < 20; i++) {
        share[i].name = name1[i];
        share[i].introduction = intro[i];
        share[i].kind = rand() % 4; // 类型0-3
        share[i].days = 1;          // 初始已有一天数据
        share[i].price_itself = rand() % 100 + 50; // 内在价值50-150
        share[i].now[0] = share[i].price_itself * (0.8 + (rand() % 41) / 100.0); // 初始价格80%-120%
        for (int j = 1; j < 1000; j++) share[i].now[j] = 0;
    }
}

struct pl_have {
    int hand;    // 持有股数
    double cost; // 总成本
    pl_have() : hand(0), cost(0) {} // 添加构造函数初始化
};
struct pl_have s_have[20]; // 持仓记录
int my_stocks[20];       // 自选股数组
int my_stocks_count = 0; // 自选股数量
void show_holdings() {
    bool has_holdings = false;
    for (int i = 0; i < 20; i++) {
        if (s_have[i].hand > 0) {
            has_holdings = true;
            double current_price = share[i].now[share[i].days - 1];
            double avg_cost = s_have[i].cost / s_have[i].hand;
            double profit = ((current_price - avg_cost) / avg_cost) * 100;
            printf("%2d: %-8s 持有 %4d 股，成本价: %6.2f，当前价: %6.2f，盈亏率: %6.2f%%，盈亏额：%6.2f\n",
                   i, share[i].name.c_str(), s_have[i].hand, avg_cost, current_price, profit,s_have[i].hand*(current_price-avg_cost));
        }
    }
    cout<<endl; 
    if (!has_holdings) cout << "暂无持仓记录。\n";
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

        if (has_yesterday) {
            yesterday_price = share[i].now[current_day - 1];
            change_pct = (current_price - yesterday_price) / yesterday_price * 100;
            sprintf(yesterday_str, "%8.2f", yesterday_price);
            sprintf(change_str, "%6.2f%%", change_pct);
        } else {
            sprintf(yesterday_str, "   无");
            sprintf(change_str, "  无");
        }

        printf("%2d: %-8s 昨日价: %s 当前价: %8.2f 元 涨跌幅: %6s，简介: %s\n",
               i, share[i].name.c_str(), yesterday_str, current_price, 
               change_str, share[i].introduction.c_str());
    }
}
void simulate_day() {
    for (int i = 0; i < 20; i++) {
        if (share[i].days % 5 == 0) {
            share[i].price_itself += rand() % 3 - 1; // -1, 0, +1
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
        sprintf(yesterday_str, "   无");
        sprintf(change_str, "  无");
    }

    printf("%2d: %-8s 昨日价: %s 当前价: %8.2f 元 涨跌幅: %6s，简介: %s\n",
        stock_idx, share[stock_idx].name.c_str(), yesterday_str, current_price, 
        change_str, share[stock_idx].introduction.c_str());
} 
void show_max(bool flag, int stock_idx) {
    if (flag) { 
        double price = share[stock_idx].now[share[stock_idx].days - 1]; 
        if (price <= 0) {
            cout << "股票价格异常，无法买入。";
            return;
        }
        int max_shares = floor(pl_money / (price * 1.0003));
        cout << "至多可买" << max_shares << "股。" << endl;
    } else { 
        cout << "至多可卖" << s_have[stock_idx].hand << "股。" << endl;
    }
}
void trade_stock() {
    int stock_idx, action, shares;
    cout << "输入股票序号 (0-19): ";
    cin >> stock_idx;
    if (stock_idx < 0 || stock_idx >= 20) {
        error1("无效股票序号！");
        return;
    }
    show_point_stock(stock_idx);
    cout << "1. 买入  2. 卖出: ";
    cin >> action;
    double price = share[stock_idx].now[share[stock_idx].days - 1];

    if (action == 1) {
    	show_max(true, stock_idx); 
    	cout<<"现有资金："<<pl_money<<"元。"<<endl; 
        cout << "买入股数: ";
        cin >> shares;
        
        double total = price * shares * 1.0003; 
        if (total > pl_money) {
            error1("资金不足！"); 
        } else {
            pl_money -= total;
            s_have[stock_idx].hand += shares;
            s_have[stock_idx].cost += total;
            cout << "成功买入 " << shares << " 股。\n";
            
        }
    } else {
    	show_max(false, stock_idx); 
        cout << "卖出股数: ";
        cin >> shares;
        if (shares > s_have[stock_idx].hand) {
        	error1("持仓不足！"); 
        } else {
            double total = price * shares * 0.995; 
            pl_money += total;
            double avg_cost = s_have[stock_idx].cost / s_have[stock_idx].hand;
            s_have[stock_idx].hand -= shares;
            s_have[stock_idx].cost = avg_cost * s_have[stock_idx].hand;
            cout << "成功卖出 " << shares << " 股。\n";
            
        }
    }
}

int check_game_status() {
    if (pl_money >= 1000000) {
        cout << "\n恭喜！你已成为百万富翁！\n";
        return 1;
    }
    if (pl_money <= -1) {
        cout << "\n资金不足，游戏结束！\n";
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
};
bool compareGainers(const StockChange& a, const StockChange& b);
bool compareLosers(const StockChange& a, const StockChange& b);

void printHeader(const string& title) {
    cout << "\n╔════════════╤════════════════╤════════════╗\n"
         << "║ 排名       │ 股票名称         │ 涨跌幅     ║\n"
         << "╟────────────┼──────────────────┼────────────╢\n";
    cout << "║ " << title << " ║\n";
    cout << "╚════════════╧════════════════╧════════════╝\n\n";
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
            gainer_count++;
        } else if (change < 0) {
            losers[loser_count].name = share[i].name;
            losers[loser_count].change = -change;
            loser_count++;
        }
    }

    sort(gainers, gainers + gainer_count, compareGainers);
    sort(losers, losers + loser_count, compareLosers);

    cout << "\n【今日龙虎榜】\n";
    printHeader("涨幅前五");
    for (int i = 0; i < min(gainer_count, 5); i++) {
        printf("║ %-10d│ %-14s│ %+6.2f%% ║\n", 
              i+1, gainers[i].name.c_str(), gainers[i].change);
    }
    if (gainer_count == 0) cout << "║ 无数据                  ║\n";

    printHeader("跌幅前五");
    for (int i = 0; i < min(loser_count, 5); i++) {
        printf("║ %-10d│ %-14s│-%6.2f%%  ║\n", 
              i+1, losers[i].name.c_str(), losers[i].change);
    }
    if (loser_count == 0) cout << "║ 无数据                  ║\n";

    cout << "\n按任意键返回...";
    system("pause > nul");
}
bool compareGainers(const StockChange& a, const StockChange& b) {
    return a.change > b.change;
}

bool compareLosers(const StockChange& a, const StockChange& b) {
    return a.change > b.change;
}
void mystock() {
    int choice;
    while(true){
        
        cout << "\n==================================================== 自选股功能 =======================================================\n";
        cout << "[1] 添加自选股\t[2] 移除自选股\n";
        cout << "[3] 查看自选股\t[4] 返回主菜单\n";
        cout << "请输入操作编号: ";
        cin >> choice;

        if(choice == 1){
            if(my_stocks_count >= 20){
                error1("自选股已达上限(20支)！");
                continue;
            }
            
            int stock_idx;
            cout << "输入要添加的股票序号 (0-19): ";
            cin >> stock_idx;
            
            if(stock_idx < 0 || stock_idx >= 20){
                error1("无效股票序号！");
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
                error1("该股票已在自选列表中！");
            }
            else{
                my_stocks[my_stocks_count++] = stock_idx;
                cout << "成功添加 " << share[stock_idx].name << " 到自选股！\n";
            }
            system("pause");
        }
        
        else if(choice == 2){
            if(my_stocks_count == 0){
                error1("自选股列表为空！");
                continue;
            }
            
            cout << "\n当前自选股列表:\n";
            for(int i=0; i<my_stocks_count; i++){
                printf("%2d: %-8s (编号%d)\n", 
                      i+1, share[my_stocks[i]].name.c_str(), my_stocks[i]);
            }
            
            int del_idx;
            cout << "输入要移除的列表序号 (1-" << my_stocks_count << "): ";
            cin >> del_idx;
            
            if(del_idx < 1 || del_idx > my_stocks_count){
                error1("无效列表序号！");
            }
            else{
                cout << "已移除 " << share[my_stocks[del_idx-1]].name << endl;
                for(int i=del_idx-1; i<my_stocks_count-1; i++){
                    my_stocks[i] = my_stocks[i+1];
                }
                my_stocks_count--;
            }
            system("pause");
        }
        
        else if(choice == 3){
            if(my_stocks_count == 0){
                error1("自选股列表为空！");
                continue;
            }
            
            cout << "\n============= 自选股行情 =============\n";
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
            error1("无效操作编号！");
        }
    }
}

void save() {
    int slot;
    cout << "请选择存档位置（1-1000）: ";  // 修改提示为1-1000
    cin >> slot;
    if(slot < 1 || slot > 1000) {  // 修改检查范围为1-1000
        error1("无效存档位置！请输入1-1000之间的数字");
        return;
    }
    
    // 生成1-1000号存档文件名（game1.sav到game1000.sav）
    char filename[20];
    sprintf(filename, "game%d.sav", slot);  // 使用C风格字符串拼接
    
    ofstream file(filename, ios::binary);
    if (!file) {
        error1("无法创建存档文件！");
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

    cout << "存档" << slot << "已保存！" << endl;
    system("pause");
}

void read() {
    int slot;
    cout << "请选择加载的存档（1-1000）: ";  // 修改提示为1-1000
    cin >> slot;
    if(slot < 1 || slot > 1000) {  // 修改检查范围为1-1000
        error1("无效存档位置！请输入1-1000之间的数字");
        return;
    }
    
    // 生成1-1000号存档文件名（game1.sav到game1000.sav）
    char filename[20];
    sprintf(filename, "game%d.sav", slot);  //使用C风格字符串拼接
    
    ifstream file(filename, ios::binary);
    if (!file) {
        error1("该存档不存在！");
        return;
    }

    file.read(reinterpret_cast<char*>(&pl_money), sizeof(pl_money));
    file.read(reinterpret_cast<char*>(&day), sizeof(day));

    for (int i = 0; i < 20; ++i) {
        size_t name_len;
        file.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
        vector<char> name_buf(name_len + 1);
        file.read(name_buf.data(), name_len);
        share[i].name = string(name_buf.data(), name_len);

        size_t intro_len;
        file.read(reinterpret_cast<char*>(&intro_len), sizeof(intro_len));
        vector<char> intro_buf(intro_len + 1);
        file.read(intro_buf.data(), intro_len);
        share[i].introduction = string(intro_buf.data(), intro_len);

        file.read(reinterpret_cast<char*>(&share[i].kind), sizeof(share[i].kind));
        file.read(reinterpret_cast<char*>(&share[i].days), sizeof(share[i].days));
        file.read(reinterpret_cast<char*>(&share[i].price_itself), sizeof(share[i].price_itself));

        for (int j = 0; j < share[i].days; ++j) {
            file.read(reinterpret_cast<char*>(&share[i].now[j]), sizeof(double));
        }
        for (int j = share[i].days; j < 10000; ++j) {
            share[i].now[j] = 0.0;
        }
    }

    for (int i = 0; i < 20; ++i) {
        file.read(reinterpret_cast<char*>(&s_have[i].hand), sizeof(s_have[i].hand));
        file.read(reinterpret_cast<char*>(&s_have[i].cost), sizeof(s_have[i].cost));
    }

    file.read(reinterpret_cast<char*>(&my_stocks_count), sizeof(my_stocks_count));
    for (int i = 0; i < my_stocks_count; ++i) {
        file.read(reinterpret_cast<char*>(&my_stocks[i]), sizeof(int));
    }

    cout << "存档" << slot << "已加载！当前天数：" << day << endl;
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
        cout<<"已重置"<<endl;
        system("pause");
        system("cls"); 
    }
    else if(choice==4){ 
        save();
    }
    else if(choice==5){ 
        read();
    }
    else {
        error("未输入正确的指令!"); 
    }
}

int main() {
    srand(time(NULL));
    init_shares();
    start(); 
    pl_money = rand() % 100000 + 100000;
    cout << "欢迎来到股市游戏！\n目标：赚取100万元！\n";
    int day = 0;
    while (true) {
        cout << "\n=============================================== 第 " << ++day << " 天 =====================================================\n";
        while(true){
        	cout << "当前资金: " << pl_money << "元\n";
	        show_holdings();
	        show_market();
	        cout << "\n[1] 进行交易 [2] 跳过今日 [3]更多功能: ";
	        int choice;
	        flag=0;
	        cin >> choice;
	        
	        if (choice == 1) {
	            while(flag==0){
	            	trade_stock();
	            	cout<<"是否继续交易?[0]是[1]否:" ;
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
				cout<<"[1]龙虎榜[2]切换到自选股池[3]重玩游戏[4]存档[5]读档: ";
				cin>>choice;
				more(choice); 
			}
			
			else error("未输入正确的指令!");
			
		} 
		
        int status = check_game_status();
        if (status != 0) break;

        system("pause");
        system("cls");
    }
    return 0;
}


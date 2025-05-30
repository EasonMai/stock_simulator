#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<string>
using namespace std;
int pl_money;
int flag;
struct share_info {
    string name;
    string introduction;
    int kind; // 类型：0-高价高振幅,1-低价高振幅,2-高价低振幅,3-低价低振幅
    double now[1000];
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
    int hand = 0;    // 持有股数
    double cost = 0; // 总成本
};
struct pl_have s_have[20]; // 持仓记录

void show_holdings() {
    bool has_holdings = false;
    for (int i = 0; i < 20; i++) {
        if (s_have[i].hand > 0) {
            has_holdings = true;
            double current_price = share[i].now[share[i].days - 1];
            double avg_cost = s_have[i].cost / s_have[i].hand;
            double profit = ((current_price - avg_cost) / avg_cost) * 100;
            printf("%2d: %-8s 持有 %4d 股，成本价: %6.2f，当前价: %6.2f，盈亏率: %6.2f%%\n",
                   i, share[i].name.c_str(), s_have[i].hand, avg_cost, current_price, profit);
        }
    }
    if (!has_holdings) cout << "暂无持仓记录。\n";
}

void show_market() {
    for (int i = 0; i < 20; i++) {
        int current_day = share[i].days - 1;
        double current_price = share[i].now[current_day];
        double change_pct = 0.0;
        if (current_day > 0) { // 有前一天数据时计算涨跌幅
            double prev_price = share[i].now[current_day - 1];
            change_pct = (current_price - prev_price) / prev_price * 100;
        }
        printf("%2d: %-8s 当前价: %8.2f 元 涨跌幅: %6.2f%%，简介: %s\n",
               i, share[i].name.c_str(), current_price, change_pct, share[i].introduction.c_str());
    }
}

void simulate_day() {
    for (int i = 0; i < 20; i++) {
        // 每5天调整内在价值
        if (share[i].days % 5 == 0) {
            share[i].price_itself += rand() % 3 - 1; // -1, 0, +1
        }
        
        double last_price = share[i].now[share[i].days - 1];
        share[i].probability = share[i].price_itself / last_price;
        
        // 随机涨跌
        share[i].up_down = (rand() % 100) < (share[i].probability * 100);
        
        // 确定波动幅度
        switch (share[i].kind) {
            case 0: share[i].range = (rand() % 5000) / 10000.0; break; // 高价高振幅
            case 1: share[i].range = (rand() % 5000) / 10000.0; break; // 低价高振幅
            case 2: share[i].range = (rand() % 3000) / 10000.0; break; // 高价低振幅
            case 3: share[i].range = (rand() % 3000) / 10000.0; break; // 低价低振幅
        }
        
        // 限制涨跌幅
        if (share[i].up_down) {
            share[i].now[share[i].days] = last_price * (1 + share[i].range);
        } else {
            share[i].now[share[i].days] = last_price * (1 - share[i].range);
        }
        share[i].days++;
    }
}

void trade_stock() {
    int stock_idx, action, shares;
    cout << "输入股票序号 (0-19): ";
    cin >> stock_idx;
    if (stock_idx < 0 || stock_idx >= 20) {
        error1("无效股票序号！");
    }

    cout << "1. 买入  2. 卖出: ";
    cin >> action;
    double price = share[stock_idx].now[share[stock_idx].days - 1];

    if (action == 1) {
        cout << "买入股数: ";
        cin >> shares;
        double total = price * shares * 1.0003; // 0.03%手续费
        if (total > pl_money) {
            error1("资金不足！"); 
			 
        } else {
            pl_money -= total;
            s_have[stock_idx].hand += shares;
            s_have[stock_idx].cost += total;
            cout << "成功买入 " << shares << " 股。\n";
            
        }
    } else {
        cout << "卖出股数: ";
        cin >> shares;
        if (shares > s_have[stock_idx].hand) {
        	error1("持仓不足！"); 
        } else {
            double total = price * shares * 0.9997; // 0.03%手续费
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
    if (pl_money <= 50) {
        cout << "\n资金不足，游戏结束！\n";
        return -1;
    }
    return 0;
}

int main() {
    srand(time(NULL));
    init_shares();
    pl_money = rand() % 5000 + 1000;
    cout << "欢迎来到股市游戏！初始资金: " << pl_money << "元\n目标：赚取100万元！\n";

    int day = 0;
    while (true) {
        cout << "\n============== 第 " << ++day << " 天 ==============\n";
        cout << "当前资金: " << pl_money << "元\n";
        show_holdings();
        show_market();
        cout << "\n[1] 进行交易 [2] 跳过今日: ";
        int choice;
        flag=0;
        cin >> choice;
        
        if (choice == 1) {
            while(flag==0){
            	trade_stock();
            	cout<<"是否继续交易?[0]是[1]否:" ;
            	cin>>flag; 
			} 
        }

        // 模拟下一天
        simulate_day();

        // 检查游戏状态
        int status = check_game_status();
        if (status != 0) break;

        system("pause");
        system("cls");
    }
    return 0;
}

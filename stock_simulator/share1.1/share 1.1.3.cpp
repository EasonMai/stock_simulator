/*
	share v1.1.3更新公告： 
	增加龙虎榜&自选股功能 
	调整了main 
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
    int hand = 0;    // 持有股数
    double cost = 0; // 总成本
};
struct pl_have s_have[20]; // 持仓记录
vector<int> my_stocks; // 存储自选股索引的全局数组
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

        // 处理昨日价格显示
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
        // 每5天调整内在价值
        if (share[i].days % 5 == 0) {
            share[i].price_itself += rand() % 3 - 1; // -1, 0, +1
        }
        
        double last_price = share[i].now[share[i].days - 1];
        
        // 计算涨跌概率（基于内在价值与当前价格关系）
        double ratio = share[i].price_itself / last_price;
        double deviation = (ratio - 1.0) * 0.05; // 每20%价格偏差调整1%概率
        share[i].probability = 0.5 + deviation;
        // 限制概率在35%~65%之间
        if (share[i].probability < 0.35) share[i].probability = 0.35;
        else if (share[i].probability > 0.65) share[i].probability = 0.65;
        
        // 随机涨跌
        share[i].up_down = (rand() % 100) < (share[i].probability * 100);
        
        // 根据类型确定波动幅度
        switch (share[i].kind) {
            case 0: // 高价高振幅：0-20%
            case 1: // 低价高振幅：0-20%
                share[i].range = (rand() % 2000) / 10000.0; // 0%~20%
                break;
            case 2: // 高价低振幅：0-10%
            case 3: // 低价低振幅：0-10%
                share[i].range = (rand() % 1000) / 10000.0; // 0%~10%
                break;
        }
        
        // 计算新价格并限制精度
        double new_price;
        if (share[i].up_down) {
            new_price = last_price * (1 + share[i].range);
        } else {
            new_price = last_price * (1 - share[i].range);
        }
        new_price = floor(new_price * 100 + 0.5) / 100; // 保留两位小数
        
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

        // 处理昨日价格显示
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
    if (flag) { // 买入
        double price = share[stock_idx].now[share[stock_idx].days - 1]; // 获取当前价格
        if (price <= 0) {
            cout << "股票价格异常，无法买入。";
            return;
        }
        // 计算考虑手续费后的最大可买股数
        int max_shares = floor(pl_money / (price * 1.0003));
        cout << "至多可买" << max_shares << "股。" << endl;
    } else { // 卖出
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
    	show_max(true, stock_idx); // 显示买入限额
    	cout<<"现有资金："<<pl_money<<"元。"<<endl; 
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
    	show_max(false, stock_idx); // 显示卖出限额
        cout << "卖出股数: ";
        cin >> shares;
        if (shares > s_have[stock_idx].hand) {
        	error1("持仓不足！"); 
        } else {
            double total = price * shares * 0.995; // 0.5%手续费
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
void dt() {
    using namespace std;
    vector<pair<string, double>> topGainers;   // 涨幅榜（名称+涨幅）
    vector<pair<string, double>> topLosers;    // 跌幅榜（名称+跌幅）

    // 收集所有有历史数据的股票
    for (int i = 0; i < 20; i++) {
        if (share[i].days < 2) continue;  // 至少需要2天数据
        
        double prev = share[i].now[share[i].days - 2];
        double curr = share[i].now[share[i].days - 1];
        double change = (curr - prev) / prev * 100;  // 涨跌幅（%）

        if (change > 0) {
            topGainers.emplace_back(share[i].name, change);
        } else if (change < 0) {
            topLosers.emplace_back(share[i].name, -change);  // 存储正跌幅
        }
    }

    // 排序：涨幅从高到低
    sort(topGainers.begin(), topGainers.end(), 
         [](const auto& a, const auto& b) { return a.second > b.second; });

    // 排序：跌幅从高到低
    sort(topLosers.begin(), topLosers.end(), 
         [](const auto& a, const auto& b) { return a.second > b.second; });


    // 格式化输出
    auto printHeader = [](const string& title) {
        cout << "\n╔════════════╤════════════════╤════════════╗\n"
             << "║ 排名       │ 股票名称         │ 涨跌幅     ║\n"
             << "╟────────────┼──────────────────┼────────────╢\n";
        cout << "║ " << title << " ║\n";
        cout << "╚════════════╧════════════════╧════════════╝\n\n";
    };

    // 显示涨幅榜
    cout << "\n【今日龙虎榜】\n\n";
    printHeader("涨幅前五");
    for (int i = 0; i < min(5, (int)topGainers.size()); i++) {
        printf("║ %-10d│ %-14s│ %+6.2f%% ║\n", 
              i+1, topGainers[i].first.c_str(), topGainers[i].second);
    }
    if (topGainers.empty()) cout << "║ 无数据                  ║\n";


    // 显示跌幅榜
    printHeader("跌幅前五");
    for (int i = 0; i < min(5, (int)topLosers.size()); i++) {
        printf("║ %-10d│ %-14s│ %6.2f%%  ║\n", 
              i+1, topLosers[i].first.c_str(), topLosers[i].second);
    }
    if (topLosers.empty()) cout << "║ 无数据                  ║\n";


    cout << "\n按任意键返回...";
    system("pause > nul");  // 静默暂停
}
void mystock(){//自选股功能
    int choice;
    while(true){
        system("cls");
        cout << "\n=============== 自选股功能 ===============\n";
        cout << "[1] 添加自选股\t[2] 移除自选股\n";
        cout << "[3] 查看自选股\t[4] 返回主菜单\n";
        cout << "请输入操作编号: ";
        cin >> choice;

        // 添加自选股
        if(choice == 1){
            int stock_idx;
            cout << "输入要添加的股票序号 (0-19): ";
            cin >> stock_idx;
            
            // 有效性检查
            if(stock_idx < 0 || stock_idx >= 20){
                error1("无效股票序号！");
                continue;
            }
            
            // 查重检查
            if(find(my_stocks.begin(), my_stocks.end(), stock_idx) != my_stocks.end()){
                error1("该股票已在自选列表中！");
            }
            else{
                my_stocks.push_back(stock_idx);
                cout << "成功添加 " << share[stock_idx].name << " 到自选股！\n";
            }
            system("pause");
        }
        
        // 移除自选股
        else if(choice == 2){
            if(my_stocks.empty()){
                error1("自选股列表为空！");
                continue;
            }
            
            // 显示当前自选股
            cout << "\n当前自选股列表:\n";
            for(int i=0; i<my_stocks.size(); i++){
                printf("%2d: %-8s (编号%d)\n", 
                      i+1, share[my_stocks[i]].name.c_str(), my_stocks[i]);
            }
            
            int del_idx;
            cout << "输入要移除的列表序号 (1-" << my_stocks.size() << "): ";
            cin >> del_idx;
            
            // 有效性检查
            if(del_idx < 1 || del_idx > my_stocks.size()){
                error1("无效列表序号！");
            }
            else{
                cout << "已移除 " << share[my_stocks[del_idx-1]].name << endl;
                my_stocks.erase(my_stocks.begin() + del_idx-1);
            }
            system("pause");
        }
        
        // 查看自选股
        else if(choice == 3){
            if(my_stocks.empty()){
                error1("自选股列表为空！");
                continue;
            }
            
            cout << "\n============= 自选股行情 =============\n";
            for(int idx : my_stocks){
                show_point_stock(idx);  // 复用个股详情展示函数
                cout << endl;
            }
            system("pause");
        }
        
        // 返回主菜单
        else if(choice == 4){
            break;
        }
        
        else{
            error1("无效操作编号！");
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
	
	        // 模拟下一天
	        else if (choice ==2){
	        	simulate_day();
	        	break;
			}
			else if(choice == 3){
				cout<<"[1]龙虎榜[2]切换到自选股池";
				cin>>choice;
				more(choice); 
			}
			
			else error("未输入正确的指令!");
			
		} 
		
        // 检查游戏状态
        int status = check_game_status();
        if (status != 0) break;

        system("pause");
        system("cls");
    }
    return 0;
}

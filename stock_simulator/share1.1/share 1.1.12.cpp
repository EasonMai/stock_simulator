/*
	share v1.1.12更新公告： 
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
    int type;        // 1-买入 2-卖出
    int stock_idx;   // 股票索引（0-19）
    int shares;      // 交易数量
    double price;    // 交易价格（买入成本/卖出单价）
    int trade_day;   // 交易发生天数
    double profit_pct; // 盈亏百分比（卖出时有效）
    double profit;    // 盈亏金额（卖出时有效）
};

vector<TradeRecord> tradeHistory;  // 全局交易记录容器

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
            
            printf("%2d: %-8s 持有 %4d 股 | 成本价: %6.2f | 当前价: %6.2f | 盈亏: %+6.2f%% (%+8.2f元)\n",
                   i, share[i].name.c_str(), s_have[i].hand, avg_cost, current_price, 
                   profit_pct, profit);
        }
    }
    cout<<endl; 
    if (!has_holdings) cout << "暂无持仓记录。\n";
}

void show_market() {
    cout << "【操作提示】输入表格中的序号（0-19）即可进行买卖\n";
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

        printf("[%2d]: %-8s 昨日价: %s 当前价: %8.2f 元 涨跌幅: %6s，简介: %s\n",
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
    int current_day = day;

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

    cout << "\n【今日龙虎榜】（直接输入表格中的[序号]即可买卖对应股票）\n";
    
    cout << "╔═══════╤═══════╤════════════════╤════════════╗\n"
         << "║ 排名  │ 序号  │ 股票名称         │ 涨跌幅     ║\n"
         << "╟───────┼───────┼──────────────────┼────────────╢\n";
    if (gainer_count == 0) {
        cout << "║ 无符合条件数据（今日无上涨股票）                  ║\n";
    } else {
        for (int i = 0; i < min(gainer_count, 5); i++) {
            printf("║ %-4d│ [%2d] │ %-14s│ %+6.2f%% ║\n",
                  i+1, gainers[i].index, gainers[i].name.c_str(), gainers[i].change);
        }
    }
    cout << "╚═══════╧═══════╧════════════════╧════════════╝\n\n";

    cout << "╔═══════╤═══════╤════════════════╤════════════╗\n"
         << "║ 排名  │ 序号  │ 股票名称         │ 涨跌幅     ║\n"
         << "╟───────┼───────┼──────────────────┼────────────╢\n";
    if (loser_count == 0) {
        cout << "║ 无符合条件数据（今日无下跌股票）                  ║\n";
    } else {
        for (int i = 0; i < min(loser_count, 5); i++) {
            printf("║ %-4d│ [%2d] │ %-14s│ %-6.2f%%  ║\n",
                  i+1, losers[i].index, losers[i].name.c_str(), losers[i].change);
        }
    }
    cout << "╚═══════╧═══════╧════════════════╧════════════╝\n\n";

    cout << "按任意键返回...";
    system("pause > nul");
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

void showTradeHistory() {
    if (tradeHistory.empty()) {
        cout << "暂无交易记录。\n";
        system("pause");
        return;
    }

    cout << "\n【历史交易记录】（按时间倒序显示）\n";
    cout << "╔═══════╤═══════╤════════════╤══════════╤══════════╤═════════════╤════════════╗\n"
         << "║ 序号  │ 类型  │ 股票名称   │ 数量(股) │ 单价(元) │ 交易天数    │ 盈亏(元/%)  ║\n"
         << "╟───────┼───────┼────────────┼──────────┼──────────┼─────────────┼────────────╢\n";

    for (int i = tradeHistory.size() - 1; i >= 0; i--) {
        const TradeRecord& rec = tradeHistory[i];
        string type = (rec.type == 1) ? "买入" : "卖出";
        string stock_name = share[rec.stock_idx].name;
        char profit_str[30];

        if (rec.type == 1) {
            sprintf(profit_str, "无");
        } else {
            sprintf(profit_str, "%+8.2f元 (%+6.2f%%)", rec.profit, rec.profit_pct);
        }

        printf("║ %-4d│ %-4s│ %-10s│ %-8d│ %-8.2f│ 第%-4d天    │ %-12s║\n",
              tradeHistory.size() - i,
              type.c_str(),
              stock_name.c_str(),
              rec.shares,
              rec.price,
              rec.trade_day,
              profit_str);
    }
    cout << "╚═══════╧═══════╧════════════╧══════════╧══════════╧═════════════╧════════════╝\n";
    system("pause");
}

void save() {
    int slot;
    cout << "请选择存档位置（1-1000）: ";
    cin >> slot;
    if(slot < 1 || slot > 1000) {
        error1("无效存档位置！请输入1-1000之间的数字");
        return;
    }
    
    char filename[20];
    sprintf(filename, "game%d.sav", slot);
    
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

    cout << "存档" << slot << "已保存！" << endl;
    system("pause");
}

void read() {
    int slot;
    cout << "请选择加载的存档（1-1000）: ";
    cin >> slot;
    if (slot < 1 || slot > 1000) {
        error1("无效存档位置！请输入1-1000之间的数字");
        return;
    }

    char filename[20];
    sprintf(filename, "game%d.sav", slot);

    ifstream file(filename, ios::binary);
    if (!file) {
        error1("该存档不存在！");
        return;
    }

    try {
        // 基本数据读取校验
        if (!file.read(reinterpret_cast<char*>(&pl_money), sizeof(pl_money)) ||
            !file.read(reinterpret_cast<char*>(&day), sizeof(day))) {
            throw runtime_error("文件头损坏");
        }

        // 股票数据安全读取
        for (int i = 0; i < 20; ++i) {
            // 验证名称长度
            size_t name_len;
            if (!file.read(reinterpret_cast<char*>(&name_len), sizeof(name_len)) || name_len > 255) {
                throw runtime_error("股票名称长度异常");
            }
            
            // 预分配缓冲区
            vector<char> name_buf(name_len + 1);
            if (!file.read(name_buf.data(), name_len)) {
                throw runtime_error("股票名称读取失败");
            }
            share[i].name.assign(name_buf.data(), name_len);

            // 验证简介长度
            size_t intro_len;
            if (!file.read(reinterpret_cast<char*>(&intro_len), sizeof(intro_len)) || intro_len > 1024) {
                throw runtime_error("股票简介长度异常");
            }

            vector<char> intro_buf(intro_len + 1);
            if (!file.read(intro_buf.data(), intro_len)) {
                throw runtime_error("股票简介读取失败");
            }
            share[i].introduction.assign(intro_buf.data(), intro_len);

            // 验证核心数据
            if (!file.read(reinterpret_cast<char*>(&share[i].kind), sizeof(share[i].kind)) ||
                !file.read(reinterpret_cast<char*>(&share[i].days), sizeof(share[i].days)) ||
                !file.read(reinterpret_cast<char*>(&share[i].price_itself), sizeof(share[i].price_itself))) {
                throw runtime_error("股票核心数据损坏");
            }

            // 验证历史天数
            if (share[i].days < 0 || share[i].days > 10000) {
                throw runtime_error("历史数据天数异常");
            }

            // 读取价格历史
            for (int j = 0; j < share[i].days; ++j) {
                if (!file.read(reinterpret_cast<char*>(&share[i].now[j]), sizeof(double))) {
                    throw runtime_error("价格历史数据损坏");
                }
            }
        }

        // 持仓数据校验
        for (int i = 0; i < 20; ++i) {
            if (!file.read(reinterpret_cast<char*>(&s_have[i].hand), sizeof(s_have[i].hand)) ||
                !file.read(reinterpret_cast<char*>(&s_have[i].cost), sizeof(s_have[i].cost))) {
                throw runtime_error("持仓数据损坏");
            }
        }

        // 自选股数据校验
        if (!file.read(reinterpret_cast<char*>(&my_stocks_count), sizeof(my_stocks_count)) ||
            my_stocks_count < 0 || my_stocks_count > 20) {
            throw runtime_error("自选股数据异常");
        }

        for (int i = 0; i < my_stocks_count; ++i) {
            if (!file.read(reinterpret_cast<char*>(&my_stocks[i]), sizeof(int))) {
                throw runtime_error("自选股索引损坏");
            }
        }

        // 交易记录安全读取
        int history_size;
        if (!file.read(reinterpret_cast<char*>(&history_size), sizeof(history_size)) ||
            history_size < 0 || history_size > 1000000) {
            throw runtime_error("交易记录数量异常");
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
                throw runtime_error("交易记录损坏");
            }
            tradeHistory.push_back(rec);
        }

        cout << "存档" << slot << "已加载！当前天数：" << day << endl;
    }
    catch (const exception& e) {
        cerr << "\n存档加载失败：" << e.what() << endl;
        // 重置游戏状态
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
    else if(choice==6){ 
        showTradeHistory();
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
                cout<<"[1]龙虎榜[2]切换到自选股池[3]重玩游戏[4]存档[5]读档[6]查看交易记录: ";
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



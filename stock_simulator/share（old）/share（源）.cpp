#include<iostream>
#include<cstdio>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<deque>
#include<vector>
#include<queue>
#include<string>
#include<cstring>
#include<map>
#include<stack>
#include<set>
#include<time.h>
using namespace std;
int pl_money;

struct share_info {
    string name;
    string introduction;
    int kind;//类型
    double now[1000];
    int days;
    double price_itself;//内在价值
    bool up_down;
    float range;
    float probability;//概率
    //1.高价高振幅2. 低价高振幅3.高价低振幅4.低价低振幅
};
void error(string a) {
    cout << a;
    system("pause");
    system("cls");
}
struct share_info share[20];
struct share_info big_share[4];
string name1[20] = { "瑞恒科技","星耀文化","宏盛商贸","未来教育",
"丰泰环保","众合网络","新韵拍卖","金瑞峰建",
"逸韵设计", "通泰物流","康瑞医疗","锦程万里",
"恒信金融","瀚宇科技","新尚优品","嘉润农业",
"鑫润资本","慧聚信息","优逸家居","兴邦咨询",
};
string intro[20] = { "科技类企业，归属新能源板块","文化传媒，新媒体","外贸企业寡头之一","教育培训公司",
"国有股份,环保公司","网络攻防，技术保镖","拍卖物品，主营艺术品拍卖","基建公司，速度奇快",
"设计公司，拥有许多优秀设计师","物流公司，服务龙头","新科技医疗，掌握多项基因技术","旅游社，性价比之王",
"证券公司，波动大","电子科技，前沿技术","连锁优品店，蓝海公司","农业科技，已有多项杂交稻专利",
"拥有巨量资本，强大的分析团队，许多优秀公司的天使投资人","大数据分析企业，智慧城市的缔造者","家具制造，拥有许多工厂","咨询公司，服务行业"

};
int kind[20] = { 1,2,1,4,
                 3,2,3,2,
                 2,4,1,3,
                 2,1,3,3,
                 2,2,1,3,
};

void name_f() {
    for (int i = 1; i <= 20; i++) {
        share[i].name = name1[i];
        share[i].introduction = intro[i];
        share[i].kind = 0;
        share[i].days = 0;
        share[i].price_itself = 0.0;
        share[i].up_down = false;
        share[i].range = 0.0f;
        share[i].probability = 0.0f;
        for (int j = 0; j < 1000; j++) {
            share[i].now[j] = 0.0;
        }
    }
}
struct pl_have {
    int day;
    int hand;
    double got;
    double cost;
    int sale_money;
};
struct pl_have s_have[1000];
int have_num;
void show_s_data() {
    for (int i = 0; i <= have_num; i++) {
        cout<<i<<" : "<<share[i].name<<" , 持有"<<s_have[i].hand<<"股,成本："<<s_have[i].cost/s_have[i].hand<<"盈（亏）率："<<s_have[i].got<<"%";
    }
}
void now_s_got() {
    for (int i = 0; i <= have_num; i++) {
        s_have[i].got=s_have[i].cost/share[i].now[share[i].days];
        s_have[i].got*=100;
    }
}

void show_s_now() {
    for (int i = 0; i <= 20; i++) {
        cout << i << " : " << share[i].name <<"简介："<<intro[i]<< ",目前价格：" << share[i].now[share[i].days] << "元";

    }
}
void rand_share() {
    for (int i = 1; i <= 20; i++) {
        if (share[1].days % 5 == 0) {
            share[i].price_itself += rand() % 1 + 2;

        }
        for (int i = 1; i <= 20; i++) {
            share[i].probability = share[i].price_itself / int(share[i].now[share[i].days]);
            share[i].up_down = (rand() / (double)RAND_MAX) * (200 - 0) + 0;
            if (share[i].up_down >= share[i].probability)share[i].up_down = 1;
            else share[i].up_down = 0;
            if (share[i].kind == 1 || share[i].kind == 2) share[i].range = rand() % 1 + 5000;
            else share[i].range = rand() % 1 + 3000;
            share[i].range /= 10000;
            share[i].range *= share[i].probability / 100;

            if (big_share[share[i].kind].range >= share[i].range)share[i].range += (rand() / (double)RAND_MAX) * (big_share[share[i].kind].range - share[i].range) + share[i].range;
            else share[i].range += (rand() / (double)RAND_MAX) * (share[i].range - big_share[share[i].kind].range) + big_share[share[i].kind].range;
            if (share[i].range >= 0.2 && share[i].kind == 1 || share[i].kind == 2)share[i].range = 0.2;
            else if (share[i].range <= -0.2 && share[i].kind == 1 || share[i].kind == 2)share[i].range = -0.2;
            else if (share[i].range <= -0.1)share[i].range = -0.1;
            else if (share[i].range >= 0.1)share[i].range = 0.1;
            if (share[i].up_down == 0)share[i].now [share[i].days]*= (1 + share[i].range);
            else share[i].now [share[i].days]*= (1 - share[i].range);
        }

    }

}
void first() {
    srand(time(0));
    for (int i = 1; i <= 20; i++) {
        share[i].kind = rand() % 5;

        if (share[i].kind == 1) {
            share[i].price_itself = rand() % 150 + 50;
            share[i].price_itself *= 0.85;
            share[i].now[share[i].days] = rand() % int(share[i].price_itself*10) + 100;
            share[i].now[share[i].days]/=10;
            for (int j = 1; j <= 5; j++)rand_share();
        }
    }

}
void use_share() {
    int a, b,num;
    cout << "选择序号";
    cin >> b;
    cout << "1.买入2.卖出" << endl;
    cin >> a;
    if (a == 1) {
        cout << "买入数量：";
        cin >> a;
        if (pl_money >= 1.0003 * share[b].now[share[b].days] * a) {
            pl_money -= 1.0003 * share[b].now[share[b].days] * a;
            s_have[b].hand += a;
            s_have[b].cost += 1.0003 * share[b].now[share[b].days] * a;
            have_num++;
            cout << "finish" << endl;
            show_s_data();

        }
        else error("买入金额大于可用金额，操作失败！");
    }
    else {
        show_s_data();
        cout << "卖哪一笔？\n";
        cin >> a;
        cout << "数量：\n";
        cin >> num;

        if (s_have[b].hand >= num) {
            pl_money += s_have[b].hand * share[b].now[share[b].days] * 0.9997;
            s_have[b].hand -= num;
            s_have[b].cost -= 1.0003 * share[b].now[share[b].days] * num;
            cout << "finish" << endl;
            show_s_data();
        }
        else error("持有的股票数量不足，操作失败！");
    }

}
void share_main() {
    int a;
    rand_share();
    int x;
    cout << "是否进行买卖操作？1.是2.否";
    cin >> a;
    if (a == 1){
        while(true){
            cout<<"continue?1yes2no";
            cin>>x;
            if(x!=1)break;
            else use_share();

        }
    }

}
int game_end(){
    if(pl_money<=50){
        cout<<"you have "<<pl_money<<"now!"<<endl;
        cout<<"your money can't pay for your life cost.you lose!";
        return 1;
    }
    else if(pl_money>=1000000) {
        cout<<"you have "<<pl_money<<"now!"<<endl;
        cout<<"you win the game!!!";
        return -1;
    }
    return 0;
}
int main(){
    int a;
    string name;
    srand(time(0));
    cout<<"enter your name:";
    cin>>name;
    cout<<"to dear "<<name<<": welcome to the world of stock!!"<<endl<<"to win the game,you must save 1,000,000$!"<<endl<<"ready!here we go!"<<endl;
    pl_money = rand()%rand() % 5000 + 1000;
    cout<<"in now,you have "<<pl_money<<"$!"<<"good luck in the stock market!see you at the top!"<<endl;
    first();
    system("pause");
    system("cls");
    long long i=0;
    while(true){
        i++;
        int y;
        cout<<"--------------------------------the "<<i<<" day---------------------------------";
        cout<<"you have "<<pl_money<<"$!"<<endl;
        share_main();
        show_s_now();
        show_s_data();
        system("cls");
        cout<<"good night!";
        system("pause");
        if(game_end()!=0){
            break;
        }
    }
}

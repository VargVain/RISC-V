#include "decode.h"
#include "memory.h"
#include "execute.h"
#include "ROB.h"
#include "RS.h"
#include "SLB.h"
#include "counter.h"

//#pragma GCC optimize(2)

unsigned int mem[1000000];
int reg[32];
Instruction rpc;
int pc, pc_new;
int cnt_wrong = 0, cnt_all = 0;
bool b = false;

int clk = 0;
unsigned int mem_count = 0, ret = 0, dest = 0, pos = 0;
SLB_ele cur_slb_ele;

bool Stop = false;
std::queue<Instruction> IQueue, IQueue_new;
myROB ROB, ROB_new;
myRS RS, RS_new;
mySLB SLB, SLB_new;
myCounter Counter, Counter_new;
int RSS[40], RSS_new[40];

bool Jump = false, Jump_new = false;
bool flush = false;

void Run_Fetch() { //取指令并解码
    if (Stop) return;
    if (Jump) return;
    Instruction ins = Decode(mem[pc]);
    if (ins.op == JALR) Jump_new = true;
    if (ins.op == END) Stop = true;
    if (ins.isS || ins.isB) ins.rd = 0;
    if (ins.isU || ins.isJ) ins.rs1 = 0;
    if (ins.isU || ins.isJ || ins.isI || ins.isL) ins.rs2 = 0;
    if (ins.isJump) ins.tmp = pc;
    IQueue_new.push(ins);
    if (ins.op == JAL) pc_new = pc + ins.imm;
    else if (ins.isB) {
        if (Counter.jump()) pc_new = pc + ins.imm;
        else pc_new = pc + 4;
    }
    else pc_new = pc + 4;
}

void Run_Issue() {
    if (IQueue.empty()) return;
    Instruction ins = IQueue.front();
    int reOrder = ROB_new.insert({ins, not_ready, 0}); //reOrder即ROB内标号
    if (!reOrder) return;
    IQueue_new.pop();
    /*
     * 发射流程：
     * 计算指令与跳转指令进RS
     * SL指令进SLB，完全顺序执行（假设内存访问只能同时读或写一块区域）
     */
    if (ins.isCalc || ins.isJump) { //计算指令与跳转指令
        RS_ele rs_ele = {ins, not_ready, reOrder};
        if (ins.rs1 && RSS[ins.rs1]) rs_ele.Q1 = RSS[ins.rs1];
        if (ins.rs2 && RSS[ins.rs2]) rs_ele.Q2 = RSS[ins.rs2];
        /*
         * Q保存了指令发射时rs寄存器的依赖
         * RSS里保存的则是最后一个发射的依赖
         * 如果这个依赖（RSS里的值）被修改了，那说明是后序的指令改的，所以RSS不能再用
         */
        RS_new.insert(rs_ele);
        RSS_new[ins.rd] = reOrder;
    }
    else if (ins.isS || ins.isL) { //访存指令（单独写进SLB是因为要求顺序执行）
        SLB_ele slb_ele = {ins, not_ready, reOrder};
        if (ins.rs1 && RSS[ins.rs1]) slb_ele.QD = RSS[ins.rs1];
        if (ins.rs2 && RSS[ins.rs2]) slb_ele.QV = RSS[ins.rs2];
        if (!SLB_new.insert(slb_ele)) return;
        RSS_new[ins.rd] = reOrder;
    }
    else {
        std::cout << "Invalid opcode: " << name[ins.op];
        exit(0);
    }
}

void Run_ROB() {
    if (ROB.v[ROB.hd].status == write) {
        rpc = ROB.v[ROB.hd].ins;
        //std::cout << name[rpc.op] << '\n';
        if (ROB.v[ROB.hd].ins.op == END) {
            std::cout << ((unsigned int)reg[10] & 0xff) << "\n";
            //std::cout << clk << " clocks\n";
            //std::cout << cnt_wrong << "/" << cnt_all;
            exit(0);
        }
        ROB_new.v[ROB_new.hd].status = commit;
        if (ROB.v[ROB.hd].ins.isS) {
            mem[ROB.v[ROB.hd].ret2] = ROB.v[ROB.hd].ret;
        }
        if (ROB.v[ROB.hd].ins.rd) { //有rd的指令，需要真正写入寄存器
            reg[ROB.v[ROB.hd].ins.rd] = ROB.v[ROB.hd].ret;
        }
        if (RSS[ROB.v[ROB.hd].ins.rd] == ROB.hd && RSS_new[ROB.v[ROB.hd].ins.rd] == ROB.hd) {
            RSS_new[ROB.v[ROB.hd].ins.rd] = 0; //重置依赖
        }

        if (ROB.v[ROB.hd].ins.op == JALR) { //跳转指令-JALR
            pc_new = ROB.v[ROB.hd].ret2;
            Jump_new = false;
        }
        else if (ROB.v[ROB.hd].ins.isB) { //跳转指令-B
            cnt_all++;
            if (ROB.v[ROB.hd].ret) Counter_new.see(true);
            else Counter_new.see(false);
            if (ROB.v[ROB.hd].ret && !Counter.jump()) {
                pc_new = ROB.v[ROB.hd].ins.tmp + ROB.v[ROB.hd].ins.imm;
                cnt_wrong++;
                flush = true;
            }
            else if (!ROB.v[ROB.hd].ret && Counter.jump()) {
                pc_new = ROB.v[ROB.hd].ins.tmp + 4;
                cnt_wrong++;
                flush = true;
            }
        }
        ROB_new.hd = nxt(ROB.hd);

    }
}

void Run_RS() {
    for (int i = 0; i < RS.v.size(); ++i) {
        if (RS.v[i].status == commit) continue;
        if (RS.v[i].status == not_ready) {
            if (RS.v[i].Q1 > 0) {
                if (ROB.v[RS.v[i].Q1].status == write || ROB.v[RS.v[i].Q1].status == commit) {
                    RS_new.v[i].Q1 = -1;
                    RS_new.v[i].V1 = ROB.v[RS.v[i].Q1].ret;
                }
            }
            else if (RS.v[i].Q1 == 0) {
                RS_new.v[i].V1 = reg[RS.v[i].ins.rs1];
            }
            if (RS.v[i].Q2 > 0) {
                if (ROB.v[RS.v[i].Q2].status == write || ROB.v[RS.v[i].Q2].status == commit) {
                    RS_new.v[i].Q2 = -1;
                    RS_new.v[i].V2 = ROB.v[RS.v[i].Q2].ret;
                }
            }
            else if (RS.v[i].Q2 == 0) {
                RS_new.v[i].V2 = reg[RS.v[i].ins.rs2];
            }
            if (RS.v[i].Q1 <= 0 && RS.v[i].Q2 <= 0) {
                RS_new.v[i].status = execute;
            }
        }
        else if (RS.v[i].status == execute) { //执行
            RS_new.v[i].ret = Compute(RS.v[i].ins, RS.v[i].V1, RS.v[i].V2, pc);
            if (RS.v[i].ins.op == JALR) ROB_new.v[RS.v[i].dest].ret2 = (RS.v[i].V1 + RS.v[i].ins.imm) &~ 1;
            RS_new.v[i].status = write;
            ROB_new.v[RS.v[i].dest].status = execute;
        }
        else if (RS.v[i].status == write) { //准备写回
            RS_new.v[i].status = commit;
            ROB_new.v[RS.v[i].dest].ret = RS.v[i].ret;
            ROB_new.v[RS.v[i].dest].status = write;
        }
    }
}

void Run_SLB() {
    for (int i = 1; i < SLB.v.size(); ++i) { //完成取值操作后status为execute
        if (SLB.v[i].status == execute || SLB.v[i].status == commit) continue;
        if (SLB.v[i].status == not_ready) {
            if (SLB.v[i].QD > 0) {
                if (ROB.v[SLB.v[i].QD].status == write || ROB.v[SLB.v[i].QD].status == commit) {
                    SLB_new.v[i].QD = -1;
                    SLB_new.v[i].D = ROB.v[SLB.v[i].QD].ret + SLB.v[i].ins.imm;
                }
            }
            else if (SLB.v[i].QD == 0) {
                SLB_new.v[i].D = reg[SLB.v[i].ins.rs1] + SLB.v[i].ins.imm;
            }
            if (SLB.v[i].QV > 0) {
                if (ROB.v[SLB.v[i].QV].status == write || ROB.v[SLB.v[i].QV].status == commit) {
                    SLB_new.v[i].QV = -1;
                    SLB_new.v[i].V = ROB.v[SLB.v[i].QV].ret;
                }
            }
            else if (SLB.v[i].QV == 0) {
                SLB_new.v[i].V = reg[SLB.v[i].ins.rs2];
            }
            if (SLB.v[i].QD <= 0 && SLB.v[i].QV <= 0) {
                SLB_new.v[i].status = execute;
            }
        }
    }
    if (mem_count) { //模拟等待3周期
        mem_count--;
        if (!mem_count) {
            if (cur_slb_ele.ins.isS) {
                ROB_new.v[dest].ret = Store(cur_slb_ele.ins, cur_slb_ele.V);
                ROB_new.v[dest].ret2 = cur_slb_ele.D;
                SLB_new.v[pos].status = commit;
                ROB_new.v[dest].status = write;
            }
            else if (cur_slb_ele.ins.isL) {
                ROB_new.v[dest].ret = ret;
                SLB_new.v[pos].status = commit;
                ROB_new.v[dest].status = write;
            }
            else {
                std::cout << "Invalid opcode: not an SL opcode.";
                std::cout << cur_slb_ele.ins.op;
                exit(0);
            }
        }
        return;
    }
    if (SLB.v.empty()) return;
    if (SLB.v.size() <= SLB.hd) return;
    if (SLB.v[SLB.hd].status == execute) { //队首可以执行
        cur_slb_ele = SLB.v[SLB.hd];
        mem_count = 2;
        if (cur_slb_ele.ins.isL) ret = Load(SLB.v[SLB.hd].ins, SLB.v[SLB.hd].D);
        dest = SLB.v[SLB.hd].dest;
        ROB_new.v[dest].status = execute;
        pos = SLB.hd;
        SLB_new.hd = nxt(SLB.hd);
    }
}

void update() {
    if (flush) {
        std::queue<Instruction> IQueue_empty;
        swap(IQueue_empty, IQueue_new);
        ROB_new.clear();
        SLB_new.clear();
        RS_new.clear();
        memset(RSS_new, 0, sizeof(RSS_new));
        Jump_new = false;
        flush = false;
    }
    IQueue = IQueue_new;
    ROB = ROB_new;
    RS = RS_new;
    SLB = SLB_new;
    pc = pc_new;
    Jump = Jump_new;
    Counter = Counter_new;
    reg[0] = 0;
    RSS_new[0] = 0;
    memcpy(RSS, RSS_new, sizeof(RSS));
}

void Run() {
    while(true) {
        clk++;

        /*std::vector<int> r = { 1, 2, 3, 4, 5 };
        std::random_device rd;
        std::shuffle(r.begin(), r.end(), rd);
        for (auto it : r) {
            switch (it) {
                case 1 : {
                    Run_Fetch(); //1.从pc获得指令并decode
                    break;
                }
                case 2 : {
                    Run_Issue(); //2.发射给ROB和RS/SLB
                    break;
                }
                case 3 : {
                    Run_ROB(); //3.ROB检查头指令，检查能否commit
                    break;
                }
                case 4 : {
                    Run_RS(); //4.遍历RS，处理计算类的指令并广播
                    break;
                }
                case 5 : {
                    Run_SLB(); //5.SLB顺序执行
                    break;
                }
            }
        }*/

        Run_Fetch(); //1.从pc获得指令并decode
        Run_Issue(); //2.发射给ROB和RS/SLB
        Run_ROB(); //3.ROB检查头指令，检查能否commit
        Run_RS(); //4.遍历RS，处理计算类的指令并广播
        Run_SLB(); //5.SLB顺序执行

        update();
    }
}

int main() {
    //freopen("test.data", "r", stdin);
    //freopen("out.txt", "w", stdout);
    MemRead();
    Run();
    return 0;
}

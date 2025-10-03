#include <bits/stdc++.h>
using namespace std;

enum GateType { GT_UNDEF, GT_INPUT, GT_OUTPUT, GT_AND, GT_OR, GT_NOT, GT_BUF, GT_NAND, GT_NOR, GT_XOR };

struct Node {
    string name;
    GateType type = GT_UNDEF;
    vector<Node*> inputs;
    vector<Node*> outputs;
    int value = -1; // -1 unknown, 0, 1
    Node(const string &n): name(n) {}
};

static inline string trim(const string &s) {
    auto i = s.find_first_not_of(" \t\r\n");
    if (i==string::npos) return "";
    auto j = s.find_last_not_of(" \t\r\n");
    return s.substr(i, j-i+1);
}

static inline string up(const string &s) {
    string r=s; for(char &c:r) c = toupper((unsigned char)c); return r;
}

GateType parseGate(const string &g) {
    string G = up(g);
    if (G=="AND") return GT_AND;
    if (G=="OR")  return GT_OR;
    if (G=="NOT") return GT_NOT;
    if (G=="BUF" || G=="BUFF") return GT_BUF;
    if (G=="NAND") return GT_NAND;
    if (G=="NOR") return GT_NOR;
    if (G=="XOR") return GT_XOR;
    return GT_UNDEF;
}

vector<string> splitArgs(const string &s) {
    vector<string> out;
    string cur;
    for(char c: s){
        if (c==',') { out.push_back(trim(cur)); cur.clear(); }
        else cur.push_back(c);
    }
    if (!cur.empty()) out.push_back(trim(cur));
    return out;
}

int evalGate(GateType t, const vector<int>& vin) {
    if (t==GT_BUF) return vin[0];
    if (t==GT_NOT) return (vin[0]==-1 ? -1 : 1 - vin[0]);
    if (t==GT_AND || t==GT_NAND) {
        bool anyUnknown=false;
        for(int v:vin){ if (v==0) return (t==GT_AND?0:1); if (v==-1) anyUnknown=true; }
        int res = anyUnknown ? -1 : 1;
        if (t==GT_NAND && res!=-1) res = 1 - res;
        if (t==GT_NAND && res==-1) return -1;
        return res;
    }
    if (t==GT_OR || t==GT_NOR) {
        bool anyUnknown=false;
        for(int v:vin){ if (v==1) return (t==GT_OR?1:0); if (v==-1) anyUnknown=true; }
        int res = anyUnknown ? -1 : 0;
        if (t==GT_NOR && res!=-1) res = 1 - res;
        return res;
    }
    if (t==GT_XOR) {
        for(int v:vin) if (v==-1) return -1;
        int p=0; for(int v:vin) p ^= (v&1); return p;
    }
    return -1;
}

// ----------------- DOT export with color coding -----------------
void exportDOT(const vector<Node*>& all_nodes) {
    cout << "digraph circuit {\nrankdir=LR;\nnode [shape=rectangle, style=filled];\n";
    for (auto n : all_nodes) {
        string label = n->name;
        if (n->type != GT_INPUT) {
            string type_s = "GATE";
            if (n->type==GT_AND) type_s="AND";
            else if (n->type==GT_OR) type_s="OR";
            else if (n->type==GT_NOT) type_s="NOT";
            else if (n->type==GT_NAND) type_s="NAND";
            else if (n->type==GT_NOR) type_s="NOR";
            else if (n->type==GT_XOR) type_s="XOR";
            else if (n->type==GT_BUF) type_s="BUF";
            label += " = " + type_s;
        }
        label += "\\nVal=" + (n->value==-1?"X":to_string(n->value));

        string color;
        if (n->type == GT_INPUT) color = "lightblue";
        else if (n->value == 1) color = "lightgreen";
        else if (n->value == 0) color = "red";
        else color = "gray"; // unknown

        cout << "\"" << n->name << "\" [label=\"" << label << "\", fillcolor=" << color << "];\n";
    }

    // edges
    for (auto n : all_nodes) {
        for (auto out : n->outputs) {
            cout << "\"" << n->name << "\" -> \"" << out->name << "\";\n";
        }
    }
    cout << "}\n";
}
// ----------------- end DOT export -----------------

int main(int argc, char** argv){
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " netlist.bench [inputs.txt]\n";
        return 1;
    }

    // Open netlist file
    ifstream fin(argv[1]);
    if (!fin) { cerr << "Cannot open netlist file " << argv[1] << "\n"; return 1; }

    unordered_map<string, Node*> netmap;
    vector<Node*> all_nodes;
    vector<string> output_names;

    auto get_node = [&](const string &name)->Node*{
        string key = trim(name);
        auto it = netmap.find(key);
        if (it!=netmap.end()) return it->second;
        Node* n = new Node(key);
        netmap[key]=n;
        all_nodes.push_back(n);
        return n;
    };

    string line;
    while (getline(fin, line)) {
        line = trim(line);
        if (line.empty() || line[0]=='#') continue;

        // INPUT(name)
        if (line.rfind("INPUT(",0)==0) {
            auto inside = line.substr(6); if (!inside.empty() && inside.back()==')') inside.pop_back();
            string nm = trim(inside);
            Node* n = get_node(nm);
            n->type = GT_INPUT;
            continue;
        }

        // OUTPUT(name)
        if (line.rfind("OUTPUT(",0)==0) {
            auto inside = line.substr(7); if (!inside.empty() && inside.back()==')') inside.pop_back();
            string nm = trim(inside);
            output_names.push_back(nm);
            // ensure node exists
            get_node(nm);
            continue;
        }

        // Two possible gate syntaxes:
        // 1) lhs = GATETYPE(arg1,arg2,...)
        // 2) GATETYPE(out, in1, in2, ...)
        if (line.find('=') != string::npos) {
            // style 1: lhs = GATE(...)
            auto eqpos = line.find('=');
            string lhs = trim(line.substr(0, eqpos));
            string rhs = trim(line.substr(eqpos+1));
            auto p = rhs.find('(');
            if (p==string::npos) continue;
            string gname = trim(rhs.substr(0, p));
            size_t q = rhs.rfind(')');
            string args_str = (q==string::npos) ? rhs.substr(p+1) : rhs.substr(p+1, q-p-1);
            vector<string> args = splitArgs(args_str);
            Node* out = get_node(lhs);
            out->type = parseGate(gname);
            for (auto &a: args) {
                Node* in = get_node(a);
                out->inputs.push_back(in);
                in->outputs.push_back(out);
            }
        } else {
            // style 2: GATETYPE(out, in1, in2, ...)
            auto p = line.find('(');
            auto q = line.rfind(')');
            if (p==string::npos || q==string::npos || q <= p) continue;
            string gname = trim(line.substr(0, p));
            string args_str = line.substr(p+1, q-p-1);
            vector<string> args = splitArgs(args_str);
            if (args.empty()) continue;
            string lhs = args[0]; // first arg is destination
            Node* out = get_node(lhs);
            out->type = parseGate(gname);
            for (size_t i = 1; i < args.size(); ++i) {
                Node* in = get_node(args[i]);
                out->inputs.push_back(in);
                in->outputs.push_back(out);
            }
        }
    }
    fin.close();

    // Build indegree for topo sort
    unordered_map<Node*, int> indeg;
    for (auto &p : netmap) {
        Node* n = p.second;
        indeg[n] = (n->type==GT_INPUT ? 0 : (int)n->inputs.size());
    }

    queue<Node*> q;
    for (auto &kv : indeg) if (kv.second==0) q.push(kv.first);
    vector<Node*> topo;
    while (!q.empty()) {
        Node* u = q.front(); q.pop(); topo.push_back(u);
        for (Node* v : u->outputs) { indeg[v]--; if (indeg[v]==0) q.push(v); }
    }
    if (topo.size() != netmap.size()) {
        cerr << "Warning: graph may have cycles or disconnected components.\n";
    }

    // Read input values from file if provided
    unordered_map<string,int> input_values;
    if (argc >= 3) {
        ifstream fin_inputs(argv[2]);
        if (!fin_inputs) { cerr << "Cannot open input values file " << argv[2] << "\n"; return 1; }
        string n; int v;
        while (fin_inputs >> n >> v) input_values[n] = v;
        fin_inputs.close();
    }

    // ask user to set primary input values if not provided
    for (auto &p : netmap) if (p.second->type==GT_INPUT) {
        string name = p.first;
        if (input_values.count(name)) {
            p.second->value = input_values[name];
        } else {
            while (true) {
                cout << name << " (0/1): ";
                string s; if (!getline(cin,s)) s="0"; s=trim(s);
                if (s=="0" || s=="1") { p.second->value = stoi(s); break; }
                cout<<"Please type 0 or 1\n";
            }
        }
    }

    // evaluate in topo order
    for (Node* n : topo) {
        if (n->type == GT_INPUT) continue;
        vector<int> vin; vin.reserve(n->inputs.size());
        for (Node* in : n->inputs) vin.push_back(in->value);
        n->value = evalGate(n->type, vin);
    }

    // print results
    cout << "\nNode values:\n";
    for (Node* n : topo) {
        cout << n->name << " [" << (n->type==GT_INPUT?"IN":"GATE") << "] = "
             << (n->value==-1?"X":to_string(n->value)) << "\n";
    }

    // export DOT file to stdout
    cout << "\n=== DOT format (copy to .dot file or Graphviz) ===\n";
    exportDOT(all_nodes);

    // cleanup
    for (Node* n : all_nodes) delete n;
    netmap.clear(); all_nodes.clear();

    return 0;
}


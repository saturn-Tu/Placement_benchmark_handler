#include "legalizer.h" 

void Legalizer::ReadAux(string aux_file) {
    fstream fs;
    fs.open(aux_file, std::fstream::in);
    string s1, s2;
    // parse garbage
    fs >> s1 >> s2;
    int s_index = aux_file.find(".")+1;
    while (fs >> s1) {
        string type = s1.substr(s_index, s1.size()-s_index);
        if (type == "nodes"){
            this->ReadNode(s1); 
        } else if (type == "pl"){
            this->ReadPl(s1);
        } else if (type == "scl"){
            this->ReadScl(s1);
            //PrintLines();
        } 
    }
    fs.close();
}

void Legalizer::ReadNode(string node_file) {
    fstream fs;
    fs.open(node_file, std::fstream::in);
    string s1, s2, s3;
    // get garbage message 
    for (int n=0; n<3; n++)
        getline(fs, s1);
    // get useful data
    for (int n=0; n<2; n++) {
        fs >> s1 >> s2 >> s3;
        if (s1 == "NumNodes")
            this->node_num = stoi(s3);
        else if (s1 == "NumTerminals")
            this->terminal_num = stoi(s3);
    }
    cout << this->node_num << " " << this->terminal_num << endl; 
    for (; nodes.size()<this->node_num;) {
        getline(fs, s1); 
        stringstream ss; 
        ss.str("");
        ss.clear();
        ss << s1;
        int start_i = 0;
        int end_i=0, para_count, w, h, type;
        string name, message, last1, last2;
        for(para_count=0; ss.rdbuf()->in_avail(); para_count++) {
            ss >> message;
            last1 = last2;
            last2 = message;
            if (para_count == 0)
                name = message;
            else if (para_count == 1)
                w = stoi(message);
            else if (para_count == 2)
                h = stoi(message);
            else if (para_count == 3) 
                type = (message == "terminal") ? 1 : 2;
            start_i = end_i+1;   
        }
        // for avoid streamstring last error
        if (para_count >= 4 && last2 == last1)
            para_count--;
        this->node_idx[name] = nodes.size();
        if (para_count == 3) {
            this->nodes.emplace_back(name, w , h, 0);
        } else if (para_count == 4) {
            this->nodes.emplace_back(name, w , h, type);
        }
    }
    fs.close();
}

void Legalizer::ReadPl(string pl_file) {
    fstream fs;
    fs.open(pl_file, std::fstream::in);
    string s1, name, s_x, s_y;
    // get garbage message 
    for (int n=0; n<4; n++)
        getline(fs, s1);
    // get useful data
    for (int n=0; n<this->node_num; n++) {
        getline(fs, s1); 
        stringstream ss; 
        ss.str("");
        ss.clear();
        ss << s1;
        ss >> name >> s_x >> s_y;
        int& idx = node_idx[name];
        nodes[idx].x = stoi(s_x);
        nodes[idx].y = stoi(s_y);
    }
}

void Legalizer::ReadScl(string scl_file) {
    fstream fs;
    fs.open(scl_file, std::fstream::in);
    string s1, s2, s3;
    // get garbage message 
    for (int n=0; n<4; n++)
        getline(fs, s1);
    // get useful data
    fs >> s1 >> s2 >> s3;
    this->line_num = stoi(s3);
    for (int n=0; n<this->line_num; n++) {
        // read garbage CoreRow Horizontal
        this->lines.resize(lines.size()+1);
        auto& line = this->lines.back();
        line.resize(line.size()+1);
        auto& sub_line = line.back();
        int Sitespacing, NumSites;
        fs >> s1 >> s2;
        while(true) {
            fs >> s1;
            if (s1 == "End") break;
            fs >> s2 >> s3;
            if (s1 == "Coordinate") {
                sub_line.lb_y = stoi(s3);
            } else if (s1 == "Height") {
                sub_line.h = stoi(s3);
            } else if (s1 == "Sitespacing") {
                Sitespacing = stoi(s3);
            } else if (s1 == "SubrowOrigin") {
                sub_line.lb_x = stoi(s3);
            } else if (s1 == "NumSites") {
                NumSites = stoi(s3);
            }        
        }
        sub_line.rt_x = sub_line.lb_x + NumSites*Sitespacing;
    }
}

void Legalizer::PrintNodes() {
    for (auto& n : nodes)
        n.PrintNode();
    cout << this->node_num << "   " << nodes.size() << endl;
}

void Legalizer::PrintLines() {
    for (auto& l : this->lines)
        for (auto& sub_l : l)
            sub_l.PrintLine();
}

void Legalizer::DivideLines() {
    for (auto& node : nodes) {
        // find fix node
        if (node.type == 1) {
            node.legal_x = node.x;
            node.legal_y = node.y;
            // Divide line to subline
            for (vector<Line>& line : lines) {
                const auto line_rt_y = line[0].lb_y+line[0].h;
                if ( (node.y >= line[0].lb_y && node.y < line_rt_y) || 
                    (line[0].lb_y >= node.y && line_rt_y <= node.y+node.h) ) {
                //if ( (node.y >= line[0].lb_y && node.y < line_rt_y) ||
                //    (node.y+node.h > line[0].lb_y && node.y+node.h <= line_rt_y) )  {                  
                    // find overlap line
                    for (int n=0; n<line.size(); n++) {
                        Line& sub_line = line[n];
                        // find overLab subline
                        int delete_lx, delete_rx;
                        if (node.x >= sub_line.lb_x && node.x+node.w <= sub_line.rt_x) {
                            if (node.x==sub_line.lb_x) {
                                sub_line.lb_x = node.x+node.w;
                                break;
                            } 
                            if (node.x+node.w == sub_line.rt_x) {
                                sub_line.rt_x = node.x;
                                break;
                            }
                            // block in middle of subline
                            Line tmp = sub_line;
                            sub_line.rt_x = node.x;
                            tmp.lb_x = node.x+node.w;
                            line.insert(line.begin()+n+1, tmp);
                            break;                           
                        }                      
                    }                 
                }

            }
        }
    }
}

void Legalizer::Abacus() {
    sort(nodes.begin(), nodes.end());
    for (int n=0; n<this->nodes.size(); n++) {    
        Node& node = this->nodes[n];
        if (node.type != 0)
            continue;
        cout << "node " << n << endl;
        //cout << "ori Node:\t";
        node.PrintNode();
        int cost=INT32_MAX, tmp_cost, line_i=0, sub_line_i=0;
        // choose line
        for (int m=0; m<this->lines.size(); m++) {  
            for (int k=0; k<lines[m].size(); k++) {
                Line& sub_line = this->lines[m][k];
                // line y-coor is too far for node
                if (sub_line.InSearchRange(node, 100))
                    break;
                Line temp_line = sub_line;
                // choose subline              
                int tmp_cost = sub_line.PlaceRow(node, this->nodes, n);
                // recover
                sub_line = temp_line;
                if (tmp_cost < 0) {
                    continue;
                }
                // update best cost
                if ( cost > tmp_cost ) {
                    cost = tmp_cost;
                    line_i = m;
                    sub_line_i = k;
                }
            }
        }  
        if (cost == INT32_MAX) {
            cout << "no place!\n";
            node.PrintNode();
        }
        //final place row        
        cout << "line index (" << line_i << "," << sub_line_i << ")\n";
        //cout << "cost " << cost << endl;
        Line& sub_line = this->lines[line_i][sub_line_i];  
        sub_line.PlaceRow(node, this->nodes, n);
    }
}

void Legalizer::AbacusFast() {
    sort(nodes.begin(), nodes.end());
    for (int n=0; n<this->nodes.size(); n++) {    
        Node& node = this->nodes[n];
        cout << "node: " << n << endl;
        if (node.type != 0) {
            continue;
        }
        int cost=INT32_MAX, tmp_cost, line_i=0, sub_line_i=0;
        int m, best_abs = INT32_MAX;
        // find first y line
        queue<int> indexs_up, indexs_down;
        for (m=0; m<this->lines.size(); m++) { 
            int tmp = abs(node.y-this->lines[m][0].lb_y);
            if (best_abs <= tmp) 
                break;
            best_abs = tmp;  
        }
        if (m == 0)
            m = 1;
        indexs_up.push(m-1);
        indexs_down.push(m-1);
  
        while (true) {
            if (indexs_up.empty() && indexs_down.empty()) {
                cout << "empty error\n";
            }
            // place up
            int up_idx = indexs_up.front();
            if (!indexs_up.empty()) {
                indexs_up.pop();
                if ( up_idx+1 < this->lines.size() )
                    indexs_up.push(up_idx+1);
                tmp_cost = FastPlace(up_idx, sub_line_i, node, n, nodes);
                if (tmp_cost != INT32_MAX) {
                    cost = tmp_cost;
                    line_i = up_idx;
                    break;
                }
            }
            // place down
            int down_idx = indexs_down.front();
            if (!indexs_down.empty()) {
                indexs_down.pop();
                if (down_idx-1 >= 0)
                    indexs_down.push(down_idx-1);
                tmp_cost = FastPlace(down_idx, sub_line_i, node, n, nodes);
                if (tmp_cost != INT32_MAX) {
                    cost = tmp_cost;
                    line_i = down_idx;
                    break;
                }
            }
        }
        Line& sub_line = this->lines[line_i][sub_line_i];  
        sub_line.PlaceRow(node, this->nodes, n);
    }
}

int Legalizer::FastPlace(int l_index, int& sub_line_i, Node& node, int n_index, vector<Node>& nodes) {
    int cost=INT32_MAX;
    for (int k=0; k<lines[l_index].size(); k++) {
        Line& sub_line = this->lines[l_index][k];
        Line temp_line = sub_line;
        // choose subline              
        int tmp_cost = sub_line.PlaceRow(node, this->nodes, n_index);
        // recover
        sub_line = temp_line;
        if (tmp_cost < 0) {
            continue;
        }
        // update best cost
        if ( cost > tmp_cost ) {
            cost = tmp_cost;
            sub_line_i = k;
        }
    }
    return cost;
}

void Legalizer::OutputPl() {
    fstream fs;
    fs.open("output.pl", std::fstream::out);
    fs << "UCLA pl 1.0\n";
    fs << "# File header with version information, etc.\n";
    fs << "# Anything following “#” is a comment, and should be ignored\n\n";
    for (auto& node : nodes) {
        fs << node.name << " " << node.legal_x << " " << node.legal_y << " : ";
        if (node.type == 0) {
            fs << "N\n";
        } else if (node.type == 1) {
            fs << "N /FIXED\n";
        } else if (node.type == 2) {
            fs << "N /FIXED_NI\n";
        }
    }
    fs.close();
}
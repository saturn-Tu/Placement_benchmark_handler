#include "legalizer.h" 
using namespace std;

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

void Legalizer::PrintNodes() {
  for (auto& n : nodes)
    n.PrintNode();
  cout << this->node_num << "   " << nodes.size() << endl;
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
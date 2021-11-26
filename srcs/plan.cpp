#include "plan.h" 
using namespace std;

void Plan::readAux(string aux_file) {
  fstream fs;
  fs.open(aux_file, std::fstream::in);
  string s1, s2, node_filename, pl_filename;
  // parse garbage
  fs >> s1 >> s2;
  int s_index = aux_file.find(".")+1;
  while (fs >> s1) {
    string type = s1.substr(s_index, s1.size()-s_index);
    if (type == "node"){
      node_filename = s1;
    } else if (type == "pl"){
      pl_filename = s1;
    }
  }
  this->readNode(s1); 
  //this->readPl(s1);
  fs.close();
}

void Plan::readNode(string node_file) {
  fstream fs;
  fs.open(node_file, std::fstream::in);
  string s1, s2, s3;
  // get garbage message 
  for (int n=0; n<4; n++)
    getline(fs, s1);
  // get useful data
  for (int n=0; n<2; n++) {
    fs >> s1 >> s2 >> s3;
    if (s1 == "NumNodes") {
      node_num_ = stoi(s3);
      nodes_.resize(node_num_);
    }
    else if (s1 == "NumTerminals")
      terminal_num_ = stoi(s3);
  }
  cout << node_num_ << " " << terminal_num_ << endl;
  getline(fs, s1);
  for(int n=0; n<node_num_; n++) {
    auto& node = nodes_[n];
    getline(fs, s1);
    stringstream ss;
    ss << s1;
    while(ss >> message) {
      if (para_count == 0)
        node.name_ = message;
      else if (para_count == 1)
        node.w_ = stoi(message);
      else if (para_count == 2)
        node.h_ = stoi(message);
      else if (para_count == 3) 
        node.type_ = (message == "terminal") ? NodeType::kBlock : NodeType::kCore;
      para_count++;
    }
    // recode node name mapping to index
    this->node_idx_[name] = n;
    cout << node;
    int a; cin >> a;
  }
  fs.close();
}

void Plan::readPl(string pl_file) {
  fstream fs;
  fs.open(pl_file, std::fstream::in);
  string s1, name, s_x, s_y;
  // get garbage message 
  for (int n=0; n<4; n++)
    getline(fs, s1);
  // get useful data
  for (int n=0; n<this->node_num_; n++) {
    getline(fs, s1); 
    stringstream ss; 
    ss.str("");
    ss.clear();
    ss << s1;
    ss >> name >> s_x >> s_y;
    int& idx = node_idx_[name];
    nodes_[idx].x_ = stoi(s_x);
    nodes_[idx].y_ = stoi(s_y);
  }
}

void Plan::printNodes() {
  for (auto& n : nodes_)
    cout << n;
  cout << this->node_num_ << "   " << nodes_.size() << endl;
}

void Plan::outputPl() {
  fstream fs;
  fs.open("output.pl", std::fstream::out);
  fs << "UCLA pl 1.0\n";
  fs << "# File header with version information, etc.\n";
  fs << "# Anything following “#” is a comment, and should be ignored\n\n";
  for (auto& node : nodes_) {
    fs << node.name_ << " " << node.x_ << " " << node.y_ << " : ";
    if (node.type_ == NodeType::kCore) {
      fs << "N\n";
    } else if (node.type_ == NodeType::kBlock) {
      fs << "N /FIXED\n";
    }
  }
  fs.close();
}
#include "plan.h" 
using namespace std;

void Plan::readAux(string aux_file) {
  fstream fs;
  fs.open(aux_file, std::fstream::in);
  string s1, s2, prefix_filename, node_filename, pl_filename;
  prefix_filename = aux_file.substr(0, aux_file.find("."));
  // parse garbage
  fs >> s1 >> s2;
  while (fs >> s1) {
    int dot_pos = s1.find(".");
    string type = s1.substr(dot_pos+1, s1.size()-dot_pos);
    cout << type << endl;
    if (type == "nodes"){
      node_filename = prefix_filename+".nodes";
    } else if (type == "pl"){
      pl_filename = prefix_filename+".pl";
    }
  }
  this->readNode(node_filename); 
  //this->readPl(pl_filename);
  fs.close();
}

void Plan::readNode(string node_file) {
  fstream fs;
  fs.open(node_file, std::fstream::in);
  assert(fs);
  string s1, s2, s3;
  // get garbage message 
  for (int n=0; n<4; n++) {
    getline(fs, s1);
  }
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
  getline(fs, s1);
  for(int n=0; n<node_num_; n++) {
    auto& node = nodes_[n];
    getline(fs, s1);
    stringstream ss;
    ss << s1;
    int para_count = 0;
    while(ss >> s2) {
      if (para_count == 0)
        node.name_ = s2;
      else if (para_count == 1)
        node.w_ = stoi(s2);
      else if (para_count == 2)
        node.h_ = stoi(s2);
      else if (para_count == 3) 
        node.type_ = (s2 == "terminal") ? NodeType::kBlock : NodeType::kCore;
      para_count++;
    }
    // recode node name mapping to index
    cout << node;
    this->node_idx_[node.name_] = n;
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
#include "legalizer.h" 
using namespace std;

void Legalizer::readAux(string aux_file) {
  fstream fs;
  fs.open(aux_file, std::fstream::in);
  string s1, s2;
  // parse garbage
  fs >> s1 >> s2;
  int s_index = aux_file.find(".")+1;
  while (fs >> s1) {
    string type = s1.substr(s_index, s1.size()-s_index);
    if (type == "nodes_"){
      this->readNode(s1); 
    } else if (type == "pl"){
      this->readPl(s1);
    }
  }
  fs.close();
}

void Legalizer::readNode(string node_file) {
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
      this->node_num_ = stoi(s3);
    else if (s1 == "NumTerminals")
      this->terminal_num_ = stoi(s3);
  }
  cout << this->node_num_ << " " << this->terminal_num_ << endl; 
  for (; nodes_.size()<this->node_num_;) {
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
    this->node_idx_[name] = nodes_.size();
    if (para_count == 3) {
      this->nodes_.emplace_back(name, w , h, 0);
    } else if (para_count == 4) {
      this->nodes_.emplace_back(name, w , h, type);
    }
  }
  fs.close();
}

void Legalizer::readPl(string pl_file) {
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

void Legalizer::printNodes() {
  for (auto& n : nodes_)
    n.printNode();
  cout << this->node_num_ << "   " << nodes_.size() << endl;
}

void Legalizer::outputPl() {
  fstream fs;
  fs.open("output.pl", std::fstream::out);
  fs << "UCLA pl 1.0\n";
  fs << "# File header with version information, etc.\n";
  fs << "# Anything following “#” is a comment, and should be ignored\n\n";
  for (auto& node : nodes_) {
    fs << node.name_ << " " << node.x_ << " " << node.y_ << " : ";
    if (node.type_ == 0) {
      fs << "N\n";
    } else if (node.type_ == 1) {
      fs << "N /FIXED\n";
    } else if (node.type_ == 2) {
      fs << "N /FIXED_NI\n";
    }
  }
  fs.close();
}
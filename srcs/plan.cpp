#include "plan.h" 
using namespace std;

void Plan::readAux(string aux_file) {
  fstream fs;
  fs.open(aux_file, std::fstream::in);
  string s1, s2, prefix_filename;
  bool node_flg = 0, pl_flg = 0, partition_flg = 0;
  prefix_filename = aux_file.substr(0, aux_file.find("."));
  // parse garbage
  fs >> s1 >> s2;
  while (fs >> s1) {
    int dot_pos = s1.find(".");
    string type = s1.substr(dot_pos+1, s1.size()-dot_pos);
    cout << type << endl;
    if (type == "nodes"){
      node_flg = true;
    } else if (type == "pl"){
      pl_flg = true;
    } else if (type == "partition") {
      partition_flg = true;
    }
  }
  fs.close();
  if(node_flg)
    this->readNode(prefix_filename+".nodes");
  if(pl_flg)
    this->readPl(prefix_filename+".pl");
  if(partition_flg)
    this->readPartition(prefix_filename+".partition");
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
    this->node_idx_[node.name_] = n;
  }
  fs.close();
}

void Plan::readPl(string pl_file) {
  fstream fs;
  fs.open(pl_file, std::fstream::in);
  assert(fs);
  string s1, name, s_x, s_y;
  // get garbage message 
  for (int n=0; n<2; n++)
    getline(fs, s1);
  // get useful data
  for (int n=0; n<this->node_num_; n++) {
    getline(fs, s1); 
    stringstream ss;
    ss << s1;
    ss >> name >> s_x >> s_y;
    int& idx = node_idx_[name];
    nodes_[idx].x_ = stoi(s_x);
    nodes_[idx].y_ = stoi(s_y);
  }
}

void Plan::readPartition(string partition_file) {
  fstream fs;
  fs.open(partition_file, std::fstream::in);
  assert(fs);
  string s1, s2;
  fs >> s1;
  partition_num_ = stoi(s1);
  partitions_.resize(partition_num_);
  // parse garbage
  getline(fs, s1);
  for(int n=0; n<partition_num_; n++) {
    getline(fs, s1); 
    stringstream ss;
    ss << s1;
    while(ss >> s1 >> s2) {
      partitions_[n] << ClipperLib::IntPoint(stoi(s1), stoi(s2));
    }
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

void Plan::outputGDT(string gdt_file) {
  cout << "start output GDT file: " << gdt_file << endl;
  fstream fs;
  fs.open(gdt_file, std::fstream::out);
  fs << "gds2{600\n";
  fs << "m=201809-14 14:26:15 a=201809-14 14:26:15\n";
  fs << "lib \'asap7sc7p5t_24_SL\' 0.00025 2.5e-10\n";
  fs << "cell{c=201809-14 14:26:15 m=201809-14 14:26:15 \'AND2x2_ASAP7_75t_SL\'\n";
  for(auto& node:nodes_) {
    int layer;
    if(node.type_ == NodeType::kBlock) {
      layer = 2;
    } else if(node.type_ == NodeType::kCore) {
      layer = 1;
    }
    fs << "b{" << layer << " xy(" << node.x_ << " " << node.y_ << " " << node.x_+node.w_ << " " << node.y_ << 
      " " << node.x_+node.w_ << " " << node.y_+node.h_  << " " << node.x_ << " " << node.y_+node.h_ << ")}\n";
  }
  for(auto& shape:partitions_) {
    int layer = 3;
    fs << "b{" << layer << " xy(";
    for(auto& p:shape) {
      fs << p.X << " " << p.Y << " ";
    }
    fs << ")}\n";
  }
  fs << "}\n}";
  fs.close();
}
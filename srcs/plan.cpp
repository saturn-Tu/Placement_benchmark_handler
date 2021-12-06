#include "plan.h" 
using namespace std;

void Plan::readAux(string aux_file) {
  cout << "# Reading .aux file\n";
  fstream fs;
  fs.open(aux_file, std::fstream::in);
  string s1, s2, prefix_filename;
  bool node_flg = 0, pl_flg = 0, partition_flg = 0, net_flg = 0;
  prefix_filename = aux_file.substr(0, aux_file.find("."));
  // parse garbage
  fs >> s1 >> s2;
  while (fs >> s1) {
    int dot_pos = s1.find(".");
    string type = s1.substr(dot_pos+1, s1.size()-dot_pos);
    if (type == "nodes"){
      node_flg = true;
    } else if (type == "pl"){
      pl_flg = true;
    } else if (type == "partition") {
      partition_flg = true;
    } else if (type == "nets") {
      net_flg = true;
    }
  }
  fs.close();
  if(node_flg)
    this->readNode(prefix_filename+".nodes");
  if(pl_flg)
    this->readPl(prefix_filename+".pl");
  if(partition_flg) {
    this->readPartition(prefix_filename+".partition");
    this->checkPartitionsRectilinear();
  }
  if(net_flg) {
    this->readNet(prefix_filename+".nets");
  }
}

void Plan::readNode(string node_file) {
  cout << "# Reading .node file\n";
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
    Node& node = nodes_[n];
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
  cout << "# Reading .pl file\n";
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
  cout << "# Reading .partition file\n";
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
      partitions_[n].contour_ << ClipperLib::IntPoint(stoi(s1), stoi(s2));
    }
  }
}

void Plan::readNet(std::string net_file) {
  cout << "# Reading .nets file\n";
  fstream fs;
  fs.open(net_file, std::fstream::in);
  assert(fs);
  string s1, s2, s3, s4, s5;
  // get garbage message 
  for(int n=0; n<4; n++) {
    getline(fs, s1);
  }
  fs >> s1 >> s2 >> s3;
  nets_num_ = stoi(s3);
  fs >> s1 >> s2 >> s3;
  pins_num_ = stoi(s3);
  // get garbage message 
  getline(fs, s1);
  getline(fs, s1);
  nets_.resize(nets_num_);
  for(int n=0; n<nets_.size(); n++) {
    Net& net = nets_[n];
    getline(fs, s1);
    stringstream ss;
    ss << s1;
    ss >> s1 >> s2 >> s3 >> s4;
    int pin_num = stoi(s3);
    net.name_ = s4;
    // parse pin information
    for(int p=0; p<pin_num; p++) {
      getline(fs, s1);
      stringstream ss;
      ss << s1;
      ss >> s1 >> s2 >> s3 >> s4 >> s5;
      int cell_idx = stoi(s1.substr(1, s1.size()-1));
      net.terminals_idx_.insert(cell_idx);
      nodes_[cell_idx].nets_idx_.insert(n);
    }
  }
  fs.close();
}

void Plan::printNodes() {
  for (Node& n : nodes_)
    cout << n;
  cout << this->node_num_ << "   " << nodes_.size() << endl;
}

void Plan::outputPl() {
  fstream fs;
  fs.open("output.pl", std::fstream::out);
  fs << "UCLA pl 1.0\n";
  fs << "# File header with version information, etc.\n";
  fs << "# Anything following “#” is a comment, and should be ignored\n\n";
  for (Node& node : nodes_) {
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
  for(Node& node:nodes_) {
    int layer;
    if(node.type_ == NodeType::kBlock) {
      layer = 2;
    } else if(node.type_ == NodeType::kCore) {
      layer = 1;
    }
    fs << "b{" << layer << " xy(" << node.x_ << " " << node.y_ << " " << node.x_+node.w_ << " " << node.y_ << 
      " " << node.x_+node.w_ << " " << node.y_+node.h_  << " " << node.x_ << " " << node.y_+node.h_ << ")}\n";
  }
  for(Partition& shape:partitions_) {
    int layer = 3;
    fs << "b{" << layer << " xy(";
    for(ClipperLib::IntPoint& p:shape.contour_) {
      fs << p.X << " " << p.Y << " ";
    }
    fs << ")}\n";
  }
  fs << "}\n}";
  fs.close();
}

void Plan::checkPartitionsRectilinear() {
  for(Partition& pa:partitions_) {
    ClipperLib::Path& contour = pa.contour_;
    bool xy_type;
    if(contour[0].X == contour.back().X)
      xy_type = 0;
    else if(contour[0].Y == contour.back().Y)
      xy_type = 1;
    else 
      assert(false);
    for(int n=1; n<contour.size(); n++) {
      if(xy_type && contour[n].X == contour[n-1].X)
        xy_type = !xy_type;
      else if(!xy_type && contour[n].Y == contour[n-1].Y)
        xy_type = !xy_type;
      else
        assert(false);
    }
  }
}

void Plan::mapCellInPartition() {
  for(Node& node:nodes_) {
    for(int pa_idx=0; pa_idx<partitions_.size(); pa_idx++) {
      Partition& partition = partitions_[pa_idx];
      ClipperLib::IntPoint mid_p(node.x_+node.w_/2, node.y_+node.h_/2);
      if(ClipperLib::PointInPolygon(mid_p, partition.contour_)) {
        node.partition_idx_ = pa_idx;
        partition.cell_num_++;
      }
    }
  }
}

void Plan::mapNetInPartition() {
  for(int net_idx=0; net_idx<nets_.size(); net_idx++) {
    Net& net = nets_[net_idx];
    set<int> partition_set;
    for(const int& t_idx:net.terminals_idx_) {
      Node& terminal = nodes_[t_idx];
      partition_set.insert(terminal.partition_idx_);
    }
    // record inter_net
    if(partition_set.count(-1) == 0) {
      for(const int& pa_idx:partition_set) {
        Partition& partition = partitions_[pa_idx];
        partition.inter_nets_idx_.insert(net_idx);
      }
    }
  }
}
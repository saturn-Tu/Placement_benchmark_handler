#include "plan.h" 
using namespace std;

void Plan::readAux(string aux_file) {
  cout << "# Reading .aux file\n";
  fstream fs;
  fs.open(aux_file, std::fstream::in);
  string s1, s2;
  bool node_flg = 0, pl_flg = 0, partition_flg = 0, net_flg = 0;
  prefix_filename_ = aux_file.substr(0, aux_file.find("."));
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
  if(partition_flg) {
    this->readPartition(prefix_filename_+".partition");
    this->checkPartitionsRectilinear();
  }
  if(node_flg)
    this->readNode(prefix_filename_+".nodes");
  if(pl_flg)
    this->readPl(prefix_filename_+".pl");
  if(net_flg) {
    this->readNet(prefix_filename_+".nets");
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
  getline(fs, s1);
  getline(fs, s1);
  for(int node_idx=0; node_idx<node_num_; node_idx++) {
    Node& node = nodes_[node_idx];
    getline(fs, s1);
    stringstream ss;
    ss << s1;
    int para_count = 0;
    while(ss >> s2) {
      if (para_count == 0) {
        node.name_ = s2;
      }
      else if (para_count == 1)
        node.w_ = stoi(s2);
      else if (para_count == 2)
        node.h_ = stoi(s2);
      else if (para_count == 3) {
        // record macro id separately
        if (s2 == "terminal") {
          // hard macro node id start from #partitions
          node.type_ = NodeType::kBlock;
          node.id_ = macro_idx_2_node_idx_.size() + partition_num_;
          macro_idx_2_node_idx_.push_back(node_idx);
        } else {
          node.type_ = NodeType::kCore;
          node.id_ = node_idx;
        }
      }
      para_count++;
    }
    // recode node name mapping to index
    this->node_idx_[node.name_] = node_idx;
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
    partitions_[n].id_ = n;
    getline(fs, s1); 
    stringstream ss;
    ss << s1;
    while(ss >> s1 >> s2) {
      ClipperLib::IntPoint point(stoi(s1), stoi(s2));
      partitions_[n].contour_ << ClipperLib::IntPoint(point);
      partitions_[n].center_p_.X += point.X;
      partitions_[n].center_p_.Y += point.Y;
    }
    partitions_[n].center_p_.X /= partitions_[n].contour_.size();
    partitions_[n].center_p_.Y /= partitions_[n].contour_.size();
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
    net.id = n;
    // parse pin information
    for(int p=0; p<pin_num; p++) {
      getline(fs, s1);
      stringstream ss;
      ss << s1;
      ss >> s1 >> s2 >> s3 >> s4 >> s5;
      int cell_idx = stoi(s1.substr(1, s1.size()-1));
      Node& node = nodes_[cell_idx];
      int pin_id = 0;
      if(node.type_ == NodeType::kBlock) {
        int x_offset = stof(s4)+0.5;
        int y_offset = stof(s5)+0.5;
        auto& pin_map = node.pins_position_to_id_map;
        auto itr = pin_map.find({x_offset,y_offset});
        if(itr == pin_map.end()) {
          pin_id = pin_map.size();
          pin_map.insert({{x_offset, y_offset}, pin_id});
        } else {
          pin_id = itr->second;
        }
      }
      net.terminals_idx_pin_.insert({cell_idx, pin_id});
      node.nets_idx_.insert(net.id);
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
  cout << "Maping cell into partition" << endl;
  for(Node& node:nodes_) {
    if(node.type_ == NodeType::kBlock)
      continue;
    int min_distance = INT32_MAX;
    ClipperLib::IntPoint mid_p(node.x_+node.w_/2, node.y_+node.h_/2);
    for(int pa_idx=0; pa_idx<partitions_.size(); pa_idx++) {
      Partition& partition = partitions_[pa_idx];
      if(ClipperLib::PointInPolygon(mid_p, partition.contour_)) {
        node.partition_idx_ = pa_idx;
        break;
      } else {
        // find nearest partition by calculating distance to center of partition
        int distance = abs(mid_p.X-partition.center_p_.X) + abs(mid_p.Y-partition.center_p_.Y);
        if(distance < min_distance) {
          min_distance = distance;
          node.partition_idx_ = pa_idx;
        }
      }
    }
    // map cell to partition
    assert(node.partition_idx_ != -1);
    Partition& partition = partitions_[node.partition_idx_];
    partition.cell_num_++;
    partition.cells_idx_.insert(node.id_);
  }
}

void Plan::mapNetInPartition() {
  cout << "Maping net into partition" << endl;
  for(int net_idx=0; net_idx<nets_.size(); net_idx++) {
    Net& net = nets_[net_idx];
    // element: partition_id, pin_id
    unordered_set<pair<int, int>, boost::hash<pair<int,int>>> partition_pin_set;
    for(const pair<int,int>& node_pin:net.terminals_idx_pin_) {
      const int& t_idx = node_pin.first;
      if(nodes_[t_idx].type_ == NodeType::kBlock) {
        const int& pin_id = node_pin.second;
        partition_pin_set.insert({nodes_[t_idx].id_, pin_id});
      } else if(nodes_[t_idx].type_ == NodeType::kCore) {
        const int& partition_idx = nodes_[t_idx].partition_idx_;
        partition_pin_set.insert({partition_idx, 0});
      }
    }
    // record inter_net
    if(partition_pin_set.size() > 1) {
      inter_nets_.resize(inter_nets_.size()+1);
      inter_nets_.back().id = inter_nets_.size()-1;
      inter_nets_.back().terminals_idx_pin_ = partition_pin_set;
      for(const pair<int,int>& partition_pin:partition_pin_set) {
        const int& pa_idx = partition_pin.first;
        if(pa_idx < partition_num_ && pa_idx >= 0) { // this node are partition
          Partition& partition = partitions_.at(pa_idx);
          partition.inter_nets_idx_.insert(inter_nets_.back().id);
          partition.inter_cell_num_++;
        }
      }
    }
  }
  cout << "Net: " << inter_nets_.size() << " / " << nets_num_ << endl;
}

void Plan::outputPAFile2NCTUGR() {
  outputDesignFile(prefix_filename_+".design");
  outputPaNetFile(prefix_filename_+".pa_net");
}

void Plan::outputDesignFile(std::string design_file) {
  cout << "# output .design file\n";
  fstream fs;
  fs.open(design_file, std::fstream::out);
  assert(fs);
  // output partitions information
  fs << partition_num_ << endl;
  for(Partition& partition:partitions_) {
    fs << partition.id_ << endl;
    fs << "  " << partition.contour_.size() << "\n  ";
    for(ClipperLib::IntPoint& point:partition.contour_) {
      fs << point.X << " " << point.Y << " ";
    }
    fs << endl;
    fs << partition.inter_nets_idx_.size() << endl;
    fs << "  NumIntraCell " << partition.cell_num_ - partition.inter_cell_num_ << endl;
  }
  // output macros information
  fs << macro_idx_2_node_idx_.size() << endl;
  for(const int& idx:macro_idx_2_node_idx_) {
    Node& macro = nodes_.at(idx);
    fs << macro.name_ << endl;
    fs << "  " << macro.x_ << " " << macro.y_ << " " << macro.w_ << " " << macro.h_ << endl;
    fs << "  " << macro.pins_position_to_id_map.size() << endl;
    for(auto& pins_position:macro.pins_position_to_id_map) {
      fs << "    " << pins_position.first.first << " " << pins_position.first.second << endl;
    }
  }
  fs.close();
}

void Plan::outputPaNetFile(std::string pa_net_file) {
  cout << "# output .pa_net file\n";
  fstream fs;
  fs.open(pa_net_file, std::fstream::out);
  assert(fs);
  // output inter_net information
  fs << "NumNets : " << inter_nets_.size() << endl;
  vector<int> partition_pin_count(partitions_.size(), 0);
  for(Net& net:inter_nets_) {
    fs << "NetDegree : " << net.terminals_idx_pin_.size() << " " << net.name_ << endl;
    for(auto& terminal:net.terminals_idx_pin_) {
      if(terminal.first >= partitions_.size()) { // terminal is macro pin
        fs << "  m" << terminal.first << "_" << terminal.second << endl;
      } else {
        fs << "  " << terminal.first << "_" << partition_pin_count.at(terminal.first) << endl;
        partition_pin_count.at(terminal.first)++;
      }
    }
  }
  // output high-speed-net information
  fs.close();
}

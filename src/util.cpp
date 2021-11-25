#include "util.h"

void Line::PrintLine() {
    cout <<"("<< lb_x <<","<< lb_y <<") ("<< rt_x <<","<< lb_y+h << ")\n";
}

void Node::PrintNode() {
    cout << name <<" "<< w <<" "<< h <<" ("<< x <<","<< y <<") "<< "L: ("<< legal_x <<","<< legal_y <<") "<< type << endl;
}

int Node::NodeAbs() {
    return abs(legal_x-x) + abs(legal_y-y);
}

bool Node::operator < (const Node& n2) {
    return this->x < n2.x;
}

Cluster::Cluster() {
    this->q = 0;
    this->weight = 0;
    this->width = 0;
}

void Cluster::AddCell(Node& node, int n_index) {
    this->nodes_index.push_back(n_index);
    this->weight++;
    this->q += this->weight * (node.x - this->width);
    this->width += node.w;
}

void Line::AddCluster(Cluster& b_cluster, Cluster& cluster) {
    for(auto node : cluster.nodes_index)
        b_cluster.nodes_index.push_back(node);
    b_cluster.weight += cluster.weight;
    b_cluster.q += cluster.q - cluster.weight*cluster.width;
    b_cluster.width += cluster.width;
}

int Line::PlaceRow(Node& node, vector<Node>& nodes, int n_index) {
    Cluster& b_cluster = this->clusters.back();
    bool new_flag = 0;
    if (clusters.size()==0 || b_cluster.lb_x+b_cluster.width <= node.x) {
        new_flag = 1;
        //create new cluster
        //cout << "create\n";
        clusters.resize(clusters.size()+1);
        Cluster& new_cluster = clusters.back();
        new_cluster.lb_x = node.x;
        new_cluster.AddCell(node, n_index);
    }
    else {
        //cout << "else\n";
        b_cluster.AddCell(node, n_index);
        Collapse(b_cluster, clusters.size()-1);
    }
    // set new cluster lb_x
    if (new_flag && this->clusters.size() == 1) {
        SetLb_x(clusters.back());
        int c_rt_x = clusters.back().lb_x+clusters.back().width;
    }
    // place all node in all cluster
    int cost = 0;
    for (Cluster& cluster : this->clusters) {
        int c_rt_x = cluster.lb_x+cluster.width;
        if (cluster.lb_x < this->lb_x || c_rt_x > this->rt_x) {
            return -1;
        }
        int x = cluster.lb_x;
        for (int n : cluster.nodes_index) {
            Node& place_node = nodes[n];
            place_node.legal_x = x;
            place_node.legal_y = this->lb_y;
            x += place_node.w;
            cost += place_node.NodeAbs();
        }
    }
    return cost;
}

void Line::Collapse(Cluster& cluster, int c_index) {
    this->SetLb_x(cluster);
    if (c_index == 0) return;
    Cluster& b_cluster = this->clusters[c_index-1];
    if (b_cluster.lb_x + b_cluster.width > cluster.lb_x) {
        // merge cluster to b_cluster
        Cluster copy_cluster = cluster;
        this->clusters.erase(clusters.begin()+c_index);
        AddCluster(b_cluster, copy_cluster);      
    }
}

void Line::SetLb_x(Cluster& cluster) {
    cluster.lb_x = cluster.q/cluster.weight;
    //cout << "clu x: " << this->lb_x << " " << this->rt_x-cluster.width<< endl;
    if (cluster.lb_x < this->lb_x)
        cluster.lb_x = this->lb_x;
    if (cluster.lb_x > this->rt_x-cluster.width)
        cluster.lb_x = this->rt_x-cluster.width;
    //cout << "lb_x = " << cluster.lb_x << endl;
}   

bool Line::InSearchRange(const Node node, int range) {
    return node.y > this->lb_y-range && node.y < this->lb_y+range;
}
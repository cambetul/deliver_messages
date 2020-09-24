#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

struct node{
    node* host;
    node* brother;//next
    node* child;//sub
    node* parent;

    string  type;//mobile host or base station
    int id; // base station's or host's id
    int parent_id;
};
struct Network{
    node* root;//the root of tree
    void create(); //creates a root
    void traverse_preorder(node *,int);//travels up to the bs where the desired host is located in the tree, ND PRİNT EACH BS'S İD
    void insert(string, int, int);//adds nodes to my tree
    node *get_node(node *,int);//it is used in insert function to find appropriate node 
    bool isExist(node *,int);//it is used to know if the desired host is in the tree
    void shortest_path(int);//prints the shortest path to the desired host
    node * get_bs(node *, int);//Returns the pointer to the base station where the desired host is located
};

void Network::create(){// this function creates a root with type CC, id 0,  parent_id 0
    root = new node;
    root->type = "CC";
    root->id = 0;
    root->parent_id = 0;
    root->host=NULL;
    root->child = NULL;
    root->brother=NULL;
    root->parent = NULL;
}

node * Network::get_bs(node *n,int search_id){
    node *temp = n->host;
    node *found = NULL;
    while (temp){
        if(temp->id == search_id){
            return temp;
        }
        temp = temp->host;
    }
    if(n->child){
        found = get_bs(n->child, search_id);
        if(found){
            return found;
        }
    }
    if(n->brother){
        found = get_bs(n->brother, search_id);
        if(found){
            return found;
        }
    }
    return NULL;
}

void Network::shortest_path(int search_id){
    node *temp=get_bs(root,search_id);
    int array[25];
    int i = 0;
    while(temp->parent){
        temp = temp->parent;
        array[i]=temp->id;
        i++;
    }
    i--;
    while( i>=0){
        cout<< array[i]<<" ";
        i--;
    }
}

void Network::traverse_preorder(node *n, int search_id){
    node *temp = n->host;
    cout<<n->id<<" ";
    while(temp){
        if(temp->id==search_id){
            return;
        }
        temp = temp->host;
    }
    if(n->child){
        traverse_preorder(n->child,search_id);
    }
    if(n->brother){
        traverse_preorder(n->brother,search_id);
    }
}

node * Network::get_node(node *n, int search_id){
    node *found = NULL;
    if(n->id == search_id){
        return n;
    }
    if(n->child){
        found = get_node(n->child, search_id);
        if(found){
            return found;
        }
    }
    if(n->brother){
        found = get_node(n->brother, search_id);
        if(found){
            return found;
        }
    }
    return NULL;
}

void Network::insert(string type, int id, int parent_id){
    node *nn; //newnode
    nn = new node;
    nn->type = type;
    nn->id = id;
    nn->parent_id = parent_id;
    nn->brother = NULL;
    nn->child = NULL;
    nn->parent = NULL;
    nn->host = NULL; 

    node *parent_node = get_node(root, parent_id);
    if(nn->type=="BS"){
        if(parent_node == NULL){
            cout<<"Parent node does not exist."<<endl;
            return;
        }
        if(parent_node->child == NULL){
            parent_node->child = nn;
            nn->parent=parent_node;
            return;
        }
        else{
            node *c = parent_node->child; //c means child
            while(c->brother){
                c = c->brother;  //finds nn's brother
            }
            c->brother = nn;
            nn->parent=parent_node;
            return;
        }
    }
    else if(nn->type == "MH"){
        if(parent_node == NULL){
            cout<<"Parent node does not exist."<<endl;
            return;
        }
        if(parent_node->host == NULL){
            parent_node->host = nn;
            nn->parent = parent_node;
        }
        else{
            node *h = parent_node->host; //h means host
            while(h->host){
                h = h->host;  //finds nn's brother
            }
            h->host = nn;
            nn->parent = parent_node;
            return;
        }
    }
}

bool Network::isExist(node *n, int search_id){
    node *temp = n->host;
    while(temp){
        if(temp->id == search_id){
            return true;
        }
        temp=temp->host;
    }
    if(n->child){
        if(isExist(n->child,search_id)){
            return true;
        }
    }
    if(n->brother){
        if(isExist(n->brother,search_id)){
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv){
    ifstream input(argv[1]);
	if(!input.is_open()){
		cerr<<"ERROR: File could not been open"<<endl;
	}
    Network myNetwork;
    myNetwork.create();

    int id,parent_id;
    string type; //mh or bs
    while(input>>type && input>>id && input>>parent_id){
        myNetwork.insert(type,id,parent_id);
    }
    input.close();

    ifstream in(argv[2]); //get messages now
    if(!in.is_open()){
		cout<<"ERROR: File could not been open"<<endl;
    }
    while(in.peek() != EOF ){		
		string message, target;
		getline(in,message,'>');
		getline(in,target,'\n');

        stringstream geek(target); // converting target to int
        int target_int = 0; 
        geek >> target_int; //x has the same value with target but its int version

        bool exist = false;
        exist = myNetwork.isExist(myNetwork.root, target_int);

        if(exist){
            cout<<"Traversing:";
            myNetwork.traverse_preorder(myNetwork.root, target_int);
            cout<<endl<<"Message:"<<message<<" To:";
            myNetwork.shortest_path(target_int);
            cout<<"mh_"<<target_int<<endl;
        }
        else{
            cout<<"Traversing:";
            myNetwork.traverse_preorder(myNetwork.root, target_int);
            cout<<endl<<"Can not be reached the mobile host mh_" <<target_int <<" at the moment"<<endl;
        }
	}
	in.close();

    return EXIT_SUCCESS;
}
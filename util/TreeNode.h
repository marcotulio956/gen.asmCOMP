#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>

template<class T> class TreeNode {
public:
    TreeNode(const T& data);
    //virtual ~TreeNode();

    // const T& getData() const {return(m_data);}
    // const std::vector<TreeNode<T>*> getChildren() {return(m_children);}
    // void setData(const T& data) {m_data = data;}

    void addChild(TreeNode<T>* node);
    void addChild(T child_name);

    void pretty_print(std::string tab="") {
        printf("%s%s\n", tab.c_str(), m_data.c_str()); 
        for (long unsigned int i=0; i<m_children.size(); i++){
            m_children[i]->pretty_print("\t"+tab);
        }
    }

private:
    T m_data;
    std::vector<TreeNode<T>*> m_children;
};

#endif
#include <memory>
#include <string>
#include <vector>
#include <map>

// Generic Decision Node
template <typename T, typename S, typename U>
class DecisionNode {
public:
    T m_result;
    S(*m_condition)(U);
    std::shared_ptr<DecisionNode> m_true;
    std::shared_ptr<DecisionNode> m_false;

    DecisionNode(S(*condition)(U), T result)
        : m_condition(condition), m_result(result), m_true(nullptr), m_false(nullptr) {
    }
};

// Decision Tree Class
template <typename T, typename S, typename U>
class DecisionTree
{
public:
    DecisionTree(std::shared_ptr<DecisionNode<T, S, U>> root)
        : m_root(root) {}

    std::shared_ptr<DecisionNode<T, S, U>> GetRoot() { return m_root; }

    void AddBranch(std::string id, std::shared_ptr<DecisionNode<T, S, U>> root)
    {
        m_branches.emplace_back(id, root);
    }

    std::shared_ptr<DecisionNode<T, S, U>> GetBranchNode(std::string id, std::shared_ptr<DecisionNode<T, S, U>> parent, S(*condition)(U), T result, bool branch)
    {
        auto it = m_branches.find(id);
        if (it != nodeMap.end())
            return it;

        return nullptr;
    }

    // Function to dynamically add a node
    std::shared_ptr<DecisionNode<T, S, U>> AddNode(std::shared_ptr<DecisionNode<T, S, U>> parent, S(*condition)(U), T result, bool branch)
    {
        auto newNode = std::make_shared<DecisionNode<T, S, U>>(condition, result);
        if (branch)
        {
            parent->m_true = newNode;
        }
        else
        {
            parent->m_false = newNode;
        }

        return newNode;
    }

    void Evaluate(U input)
    {
        T result = evaluateTree(root, input);
        results.push_back(result);

        // Execute additional trees if present
        for (auto& branchTree : m_branches)
        {
            branchTree->evaluate(input);
            results.insert(results.end(), branchTree->results.begin(), branchTree->results.end());
        }
    }

    std::vector<T> GetResults() { return m_results; }

private:

    T EvaluateTree(std::shared_ptr<DecisionNode<T, S, U>> node, U input)
    {
        auto current = node;
        while (current)
        {
            if (current->m_condition)
            {
                if (current->m_condition(input))
                {
                    if (!current->m_true)
                        return current->m_result;
                    current = current->m_true;
                }
                else
                {
                    if (!current->m_false)
                        return current->m_result;
                    current = current->m_false;
                }
            }
            else {
                return current->m_result;
            }
        }
        throw std::runtime_error("Decision tree evaluation failed");
    }

    std::shared_ptr<DecisionNode<T, S, U>> root; // Root of the tree
    std::map<std::string id, std::shared_ptr<DecisionNode<T, S, U>>> m_branches;
    std::vector<T> m_results;
};
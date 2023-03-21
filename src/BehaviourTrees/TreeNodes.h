//
// Created by MLG Erwich on 19/11/2022.
//

#ifndef PACMANSFML_TREENODES_H
#define PACMANSFML_TREENODES_H

#include <iostream>
#include <vector>
#include "../GameObjects/Entity.h"
#include "BlackBoard.h"

enum class ReturnType
{
  NONE = 0,
  SUCCESS = 1,
  FAILURE = 2,
  RUNNING = 3
};

enum class NodeType
{
  NODE = 0,
  SELECTOR = 1,
  SEQUENCE = 2,
  DECORATOR = 3,
  LEAF = 4
};

class Node
{
 public:
  Node() { node_type = NodeType::NODE; }
  virtual ~Node() = default;
  virtual void ready() = 0;
  virtual ReturnType tick(Entity &actor, BlackBoard& blackboard) = 0;
  NodeType getNodeType() { return node_type; }
  const std::vector<std::unique_ptr<Node>>& getChildren() const { return children; }

  void addChild(Node* child) { children.emplace_back(child); }

 protected:
  NodeType node_type;
  std::vector<std::unique_ptr<Node>> children;
};

class Composite : public Node
{
  void ready() override
  {
    if (children.empty()) std::cout << "Composite should have at least 1 child." << std::endl;

    for (auto& child : children) { child->ready(); }
  }
  //ReturnType tick() override { return ReturnType::SUCCESS; }
};

class Sequence : public Composite
{
 public:
  Sequence() { node_type = NodeType::SEQUENCE; }
  ReturnType tick(Entity &actor, BlackBoard& blackboard) override
  {
    for (auto& child : children)
    {
      ReturnType response = child->tick(actor, blackboard);
      if (response != ReturnType::SUCCESS) return response;
    }
    return ReturnType::SUCCESS;
  }
};

class Selector : public Composite
{
 public:
  Selector() { node_type = NodeType::SELECTOR; }
  ReturnType tick(Entity &actor, BlackBoard& blackboard) override
  {
    for (auto& child : children)
    {
      ReturnType response = child->tick(actor, blackboard);
      if (response != ReturnType::FAILURE) return response;
    }
    return ReturnType::FAILURE;
  }
};

class Decorator : public Node
{
 public:
  Decorator() { node_type = NodeType::DECORATOR; }
  void ready() override
  {
    if (children.size() != 1) { std::cout << "Decorator should only have 1 child" << std::endl; }
    children[0]->ready();
  }
};

class Inverter : public Decorator
{
 public:
  ReturnType tick(Entity &actor, BlackBoard& blackboard) override
  {
    ReturnType response = children[0]->tick(actor, blackboard);
    if (response == ReturnType::SUCCESS) return ReturnType::FAILURE;
    if (response == ReturnType::FAILURE) return ReturnType::SUCCESS;
    return ReturnType::NONE;
  }
};

class Leaf : public Node
{
 public:
  void ready() override
  {
    if (!children.empty()) { std::cout << "Leaf shouldn't have any children" << std::endl; }
  }
};

#endif // PACMANSFML_TREENODES_H

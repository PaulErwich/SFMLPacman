//
// Created by MLG Erwich on 19/11/2022.
//

#ifndef PACMANSFML_BEHAVIOURTREE_H
#define PACMANSFML_BEHAVIOURTREE_H

#include "TreeNodes.h"
#include "GhostNodes.h"
#include <iostream>
#include <list>
#include <memory>
#include <vector>

class RootNode
{
 public:
  bool ready()
  {
    child->ready();
    return true;
  }
  bool getEnabled() { return enabled; }
  void enable() { enabled = true; }
  void disable() { enabled = false; }

  void set_child(NodeType node_type)
  {
    switch(node_type)
    {
      case NodeType::SELECTOR:
        child = std::make_unique<Selector>();
        break;
      case NodeType::SEQUENCE:
        child = std::make_unique<Sequence>();
        break;
      default:
        break;
    }
  }
  ReturnType tick(Entity& actor, BlackBoard& blackboard) { return child->tick(actor, blackboard); }
  const std::unique_ptr<Node>& getChild() const { return child; }

 private:
  bool enabled = true;

  std::unique_ptr<Node> child;
};

class BehaviourTree
{
 public:
  BehaviourTree() : root(new RootNode()) { };
  void run(Entity& actor, BlackBoard& blackboard) { root->tick(actor, blackboard); }
  virtual void buildTree() {};
 protected:
  std::unique_ptr<RootNode> root;
};

class BlinkyTree : public BehaviourTree
{
 public:
  void buildTree() override
  {
    root->set_child(NodeType::SELECTOR);

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[0]->addChild(new GhostDead());
    root->getChild()->getChildren()[0]->addChild(new Selector());

    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new AtSpawnPoint());
    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new PathToSpawnPoint());

    root->getChild()->addChild(new CheckDestination());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[2]->addChild(new InScatterMode());
    root->getChild()->getChildren()[2]->addChild(new PathToScatterPoint());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[3]->addChild(new PathToPacman());
  }
};

class PinkyTree : public BehaviourTree
{
 public:
  void buildTree() override
  {
    root->set_child(NodeType::SELECTOR);

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[0]->addChild(new GhostDead());
    root->getChild()->getChildren()[0]->addChild(new Selector());

    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new AtSpawnPoint());
    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new PathToSpawnPoint());

    root->getChild()->addChild(new CheckDestination());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[2]->addChild(new InScatterMode());
    root->getChild()->getChildren()[2]->addChild(new PathToScatterPoint());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[3]->addChild(new CheckFutureCost());
    root->getChild()->getChildren()[3]->addChild(new PathToFuturePacman());

    root->getChild()->addChild(new PathToPacman());

    root->ready();
  }
};

class InkyTree : public BehaviourTree
{
 public:
  void buildTree() override
  {
    root->set_child(NodeType::SELECTOR);

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[0]->addChild(new GhostDead());
    root->getChild()->getChildren()[0]->addChild(new Selector());

    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new AtSpawnPoint());
    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new PathToSpawnPoint());

    root->getChild()->addChild(new CheckDestination());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[2]->addChild(new InScatterMode());
    root->getChild()->getChildren()[2]->addChild(new PathToScatterPoint());

    root->getChild()->addChild(new Selector());

    root->getChild()->getChildren()[3]->addChild(new Sequence());

    root->getChild()->getChildren()[3]->getChildren()[0]->addChild(new FindDistance());
    root->getChild()->getChildren()[3]->getChildren()[0]->addChild(new InkyPath());

    root->getChild()->getChildren()[3]->addChild(new Sequence());
//
    root->getChild()->getChildren()[3]->getChildren()[1]->addChild(new MoveEndPointInMap());
    root->getChild()->getChildren()[3]->getChildren()[1]->addChild(new MakeEndPointValid());
    root->getChild()->getChildren()[3]->getChildren()[1]->addChild(new InkyPath());

    root->ready();
  }
};

class ClydeTree : public BehaviourTree
{
 public:
  void buildTree() override
  {
    root->set_child(NodeType::SELECTOR);

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[0]->addChild(new GhostDead());
    root->getChild()->getChildren()[0]->addChild(new Selector());

    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new AtSpawnPoint());
    root->getChild()->getChildren()[0]->getChildren()[1]->addChild(new PathToSpawnPoint());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[1]->addChild(new InScatterMode());
    root->getChild()->getChildren()[1]->addChild(new PathToScatterPoint());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[2]->addChild(new WithinRange());
    root->getChild()->getChildren()[2]->addChild(new PathToScatterPoint());

    root->getChild()->addChild(new Sequence());

    root->getChild()->getChildren()[3]->addChild(new FarAway());
    root->getChild()->getChildren()[3]->addChild(new PathToPacman());

    root->ready();
  }
};


#endif // PACMANSFML_BEHAVIOURTREE_H

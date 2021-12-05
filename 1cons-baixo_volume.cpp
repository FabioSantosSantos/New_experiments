/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "helloworld-consumer-app.hpp"
#include "helloworld-producer-app.hpp"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

int
main(int argc, char* argv[])
{
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("10p"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(10);

  // Connecting nodes (Links)
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));// node A(cons) to node B
  p2p.Install(nodes.Get(1), nodes.Get(2));// node B to node C
  p2p.Install(nodes.Get(2), nodes.Get(3));// node C to node D
  p2p.Install(nodes.Get(3), nodes.Get(4));// node D to node E
  p2p.Install(nodes.Get(4), nodes.Get(5));// node E to node F(prod)
  p2p.Install(nodes.Get(1), nodes.Get(6));// node B to node G
  p2p.Install(nodes.Get(6), nodes.Get(3));// node G to node D
  p2p.Install(nodes.Get(6), nodes.Get(4));// node G to node E
  p2p.Install(nodes.Get(6), nodes.Get(9));// node G to node K
  p2p.Install(nodes.Get(1), nodes.Get(7));// node G to node H
  p2p.Install(nodes.Get(1), nodes.Get(8));// node B to node I
  p2p.Install(nodes.Get(7), nodes.Get(8));// node H to node I
  p2p.Install(nodes.Get(9), nodes.Get(4));// node K to node E


  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/multicast");

  // Set BestRoute strategy
  //ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  //ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  //ndnGlobalRoutingHelper.InstallAll();

  // Getting containers for the consumer/producer
  //Ptr<Node> producer = grid.GetNode(0);
  //NodeContainer consumerNodes;
  //consumerNodes.Add(grid.GetNode(5));  


  // Installing applications

  std::string prefix = "/prefix";

  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetPrefix(prefix);
  consumerHelper.SetAttribute("Frequency", StringValue("10")); // 10 interests a second
  consumerHelper.Install(nodes.Get(0));
  
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix(prefix);
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(5));
  
  Simulator::Stop(Seconds(20.0));
  
  ndn::L3RateTracer::InstallAll("rate-trace-1cons-baixo_volume.txt", Seconds(0.5));
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}

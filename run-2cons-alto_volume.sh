#!/bin/bash

docker exec -it -e NS_LOG=ndn.Consumer:ndn.Producer ndn-tutorial-e1 ./waf --run "2cons-alto_volume" $@

#!/bin/bash

# this file is used to control different configurations, usage: source configure.sh
# comment out an entry to get the described default behaviour

# reset existing configurations so that a default can be applied by commenting out a value
unset THRILL_WORKERS_PER_HOST THRILL_RAM THRILL_NET THRILL_LOCAL THRILL_LOG

# number of workers per host, default: number of cores detected
export THRILL_WORKERS_PER_HOST="4"

# total amount of memory used by thrill, default: all available memory
export THRILL_RAM="4GiB"

# networking implementation used, default: local
# mock - mock networking using shared memory
# local - loopback sockets
# tcp - tcp sockets
# mpi - message passing inteface (mpi)
export THRILL_NET="local"

# the amount of hosts simulated on the local machine, only usable with mock and local network, default: 2
export THRILL_LOCAL="1"

# the location of the log files, thrill will append "-host-{host-id}.json" to this, default: no logs
# export THRILL_LOG="/data/log"

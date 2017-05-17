# TODO

## General
- [ ] Job tracker: parse commands; schedule tasks; rewire nodes;...
- [ ] Supervisor: run cmd until removed; rewire when told so via pipe/signal 
- [ ] Nodes keep running even after the input stream has stopped, stopping only when explicitly removed
- [ ] Concurrency in writes to the stdin of nodes
- [ ] Default output for nodes with no output specified
- [ ] Dynamic network 
- [ ] Nodes should be able to change their task during run-time

## Components
- [x] const <value>
- [x] filter <column> <operator> <operand> 
- [x] window <column> <operation> <lines>
- [x] spawn <cmd> <args...>

## Controler
- [x] node <id> <cmd> <args...>
- [x] connect <id> <ids...>
- [ ] disconnect <id1> <id2>
- [ ] inject <id> <cmd> <args..>
- [ ] remove <id>

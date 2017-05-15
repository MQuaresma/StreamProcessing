# TODO

## General
- [ ] Job tracker: parse commands; schedule tasks; rewire nodes;...
- [ ] Supervisor: run cmd until removed; rewire when told so via pipe/signal 

## Components
- [x] const <value>
- [x] filter <column> <operator> <operand> 
- [x] window <column> <operation> <lines>
- [x] spawn <cmd> <args...>

## Controler
- [ ] node <id> <cmd> <args...>
- [ ] connect <id> <ids...>
- [ ] disconnect <id1> <id2>
- [ ] inject <id> <cmd> <args..>
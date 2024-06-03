#!/bin/bash
client_amount=$1

gnome-terminal --command="./server" 2>/dev/null
for (( counter=0; counter<client_amount; counter++ ))
do
 gnome-terminal -- sh -c "bash -c \"./client ; exec bash\""
done

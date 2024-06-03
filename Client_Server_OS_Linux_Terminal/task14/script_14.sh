num_clients=$1
# Запуск читателей

PORT=8888;
IP="127.0.0.1"
gnome-terminal --command="./server" 2>/dev/null
for ((i=1; i<=$num_clients; i++))
do
    gnome-terminal --command="./client $IP $PORT $i" 2>/dev/null
done

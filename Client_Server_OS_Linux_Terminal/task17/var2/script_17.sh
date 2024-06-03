num_readers=$1
num_writers=$2
# Запуск читателей
for ((i=1; i<=$num_readers; i++)); 
do
    gnome-terminal --command="./server $i" 2>/dev/null
done
# Запуск писателей
for ((i=1; i<=$num_writers; i++)); 
do
    gnome-terminal --command="./client $i" 2>/dev/null
done

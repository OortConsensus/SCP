
trap killgroup SIGINT

killgroup(){
    echo killing...

    rm FIFO1
    rm FIFO2
    kill 0
}
CHAN=70b049f0
NAME=53849b03fb
net=undernet

rm FIFO1
rm FIFO2
mkfifo FIFO1
mkfifo FIFO2
echo "connecting on " $net "channel" $CHAN "with name" $NAME
./main FIFO1 FIFO2 >&1 &
python bot.py $net $CHAN $NAME   FIFO2 FIFO1 >&1 &
wait




# Cpp Compile trigger program

Compile trigger program: (signal)
gcc -o signal signal.cpp -l wiringPi

#####################################

To enable auto run, open bashrc file using:

---
nano ~/.bashrc
---

uncomment the following commands:

cd Desktop/cam_python 
sleep 10
./signal &
python3 piv.py &
## Open the Laser

```
sudo screen /dev/nACM0
```

## PIV Program
g++ -o piv piv.cpp -l wiringPi

## Shut off signal
g++ -o shut_off_signal shut_off_signal.cpp -l wiringPi

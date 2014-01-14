#sleep 3
#reboot
ifconfig eth0 up 
ifconfig eth0 172.6.109.100 netmask 255.255.255.0
#cp /dav/app.tar.gz /home
cd /dav
#cp davinci cmemk.ko  dm365mmap.ko  irqk.ko  edmak.ko loadmodules_264.sh NetVideoOCX.cab index.htm pppoed pppd t1 /home
cp davinci cmemk.ko  dm365mmap.ko  irqk.ko  edmak.ko loadmodules_264.sh /home
cd /home
rm -f ./pidfile
./loadmodules_264.sh

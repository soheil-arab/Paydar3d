#!/bin/bash
red='\e[0;31m'
RED='\e[1;31m'
blue='\e[0;34m'
BLUE='\e[1;34m'
cyan='\e[0;36m'
CYAN='\e[1;36m'
NC='\e[0m'
prog="./Robotoos3D"
teamname="PaYdar3D"
killall -9 "Robotoos3D"
host=$1

if test -z  $host
then
	host="localhost"
fi

name[1]="Mammad"
name[2]="Haamed"
name[3]="Ghaader"
name[4]="Jaaber"
name[5]="Abbaas"
name[6]="Baagher"
echo
echo
echo -e "${cyan} Starting Robotoos3D Soccer Simulation Team On $host ..."
echo ""
echo -e "${blue} Member:"
echo -e "${red}           <<  Mohammad Razeghi >> "
echo -e "${cyan}"
for i in `seq 1 11`
do
echo "Starting Number $i --> ${name[$i]}" 
$prog --num $i --ip $host --name $teamname > /dev/null&
sleep 1
done
echo -ne "${NC}"


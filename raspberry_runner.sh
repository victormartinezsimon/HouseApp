#!/bin/bash

cd "/home/pi/Desktop/HouseAppRunner/"

echo "update database"
cd ../HouseAppCode
git pull

echo "making things"
cd build
make

echo "move back"
cd ../../HouseAppRunner
echo "copy program"
cp ../HouseAppCode/build/houseApp ./houseApp

echo "copy updated file"
cp ../HouseAppCode/config/web_data.json ./config/web_data.json

echo "running houseApp"
./houseApp

echo "End"
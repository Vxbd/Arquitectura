#!/bin/bash

# Script pruebas de Arquitectura de Computadores

# Caches
s1=32768 # Empleamos solo la parte de datos

s2=262144

s3=6291456

NLC=($(echo 0.5*$s1 | bc) $(echo 1.5*$s1 | bc) $(echo "0.5*$s2" | bc) \
$(echo "1.5*$s2" | bc) $(echo "0.5*$s3" | bc) $(echo "0.75*$s3" | bc) \
$(echo "2*$s3" | bc) $(echo "4*$s3" | bc) $(echo "8*$s3" | bc))
D=(1 3 15 40 80)

echo "${D[@]}"

for i in "${NLC[@]}";do
	for j in "${D[@]}";do
		echo $i
		R=$(echo "$i/(8+(8*$j))" | bc)
		echo "Prueba -> ./arcompRandom $j $R"
		./arcomp $j $R
	done
done

### Ejercicio 1 ###
cat /proc/cpuinfo | grep "model name" -m 1

### Ejercicio 2 ###
cat /proc/cpuinfo | grep "model name" | printf "CPU Cores: %d\n" "$(wc -l)"

### Ejercicio 3 ###
wget -O - https://www.gutenberg.org/files/11/11-0.txt | sed 's/Alice/Santi/g' > Archivos\ de\ datos/Santi_in_wonderland.txt && printf "Santi_in_wonderland.txt created successfully\n"

### Ejercicio 4 ###
# Date of Max maximum temperature
sort -k 5 -n Archivos\ de\ datos/weather_cordoba.in | awk '{print $1" "$2" "$3}' | printf "Date of Max maximum temperature: $(tail -n 1)\n"

# Date of Min maximum temperature
sort -k 5 -n Archivos\ de\ datos/weather_cordoba.in | awk '{print $1" "$2" "$3}' | printf "Date of Min maximum temperature: $(head -n 1)\n" 

### Ejercicio 5 ###
sort -k 3 -n Archivos\ de\ datos/atpplayers.in > Archivos\ de\ datos/atpplayers_sorted.in && printf "atpplayers_sorted.in created successfully\n"

### Ejercicio 6 ###
awk '{print $0" "$7-$8}' Archivos\ de\ datos/superliga.in | sort -k2,2 -k9,9 -n -r > Archivos\ de\ datos/superliga_sorted.in && printf "superliga_sorted.in created successfully\n"

### Ejercicio 7 ###
ip link | grep 'link/ether ..:..:..:..:..:..' | grep  '..:..:..:..:..:..' -o

### Ejercicio 8 ###
# Creo Carpeta y archivos
mkdir "The Big Bang Theory" && for i in {1..10}; do touch The\ Big\ Bang\ Theory/TBBT_S01E"$i"_es.srt ; done && printf "The Big Bang Theory files created successfully\n"

# Modificar nombre de archivos
cd The\ Big\ Bang\ Theory/ && for i in *_es.srt; do mv $i ${i%_es.srt}.srt ; done && cd .. && printf "Files rename successfully\n"
### Ejercicio 1 ###
cat /proc/cpuinfo | grep "model name" -m 1

### Ejercicio 2 ###
cat /proc/cpuinfo | grep "model name" | wc -l

### Ejercicio 3 ###
wget -O - https://www.gutenberg.org/files/11/11-0.txt | sed 's/Alice/Santi/g' > Archivos\ de\ datos/Santi_in_wonderland.txt

### Ejercicio 4 ###
# Fecha de la maxima temperatura maxima
sort -k 5 -n Archivos\ de\ datos/weather_cordoba.in | awk '{print $1" "$2" "$3}' | tail -n 1

# Fecha de la minima temperatura minima
sort -k 6 -n Archivos\ de\ datos/weather_cordoba.in | awk '{print $1" "$2" "$3}' | head -n 1

# Ejercicio 5

# Ejercicio 6

# Ejercicio 7

# Ejercicio 8

# Ejercicio 9


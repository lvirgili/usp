set terminal png size 640,480 font cour 8
set output "plot5_6.png"
set xlabel "Metodo"
set ylabel "Tamanho da regex"
set style line 1 lc rgb 'grey30' ps 0 lt 1 lw 2
set style line 2 lc rgb 'grey70' lt 1 lw 2
set style fill solid 1.0 border rgb 'grey30'
set label '*' at 3,0.8 center
set label '*' at 4,0.8 center
set border 3
set xtics nomirror scale 0
set ytics nomirror out scale 0
plot '5_6.dat' u 0:2:(0.7):xtic(1) w boxes ls 2 notitle
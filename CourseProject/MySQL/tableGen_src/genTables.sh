DATA="../data/3.txt"
awk -f videos.awk $DATA > videos.txt
awk -f related.awk $DATA > related.txt

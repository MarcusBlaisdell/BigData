SRC="tableGen_src"
cd $SRC
pwd
echo "Generating formatted table text files from text files"
./genTables.sh
echo "Loading tables into mysql"
sudo mysql -u root -p < loadTables.sql
echo "Table load complete!"
current_time=$(date +"%Y%m%d")

cd ../data

tar -czvf "../download/data${current_time}.tar.gz" -c ../data



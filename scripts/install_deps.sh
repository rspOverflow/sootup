sudo apt-get update
grep -v '^#' deps.txt | xargs sudo apt-get install -y

make
echo 

bin/main.out -d test/nestest.nes

echo 
echo "program has returned"
echo "checking differences"

log/diffs.sh
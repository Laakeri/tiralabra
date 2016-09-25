make
for f in testdata/*.in; do
	./main < $f > out
	diff out ${f%.in}.ans
	if [ $? -ne 0 ]
	then
		echo "Wrong answer in test $f"
	else
		echo "Correct answer in test $f"
	fi
done
rm out
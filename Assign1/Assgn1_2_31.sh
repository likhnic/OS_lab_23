fruits=`cat fruits.txt`
for line in `cat $1`;
do 
    [[ $line =~ ^[a-zA-Z][a-zA-Z0-9]{4,18}[0-9]+[a-zA-Z0-9]*$ ]]&&echo $line|grep -ivqE "${fruits[*]}"&&echo YES||echo NO
done > validation_results.txt
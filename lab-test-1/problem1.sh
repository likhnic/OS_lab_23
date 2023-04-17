read -r x
read -r y
read -r m
if ((x>0 && y>0 && m>0))
then
    echo $(((x**((y*y)%m))%m))
else
    echo invalid input
fi
